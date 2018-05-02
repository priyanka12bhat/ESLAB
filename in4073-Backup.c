/*------------------------------------------------------------------
 *  in4073.c -- test QR engines and sensors
 *
 *  reads ae[0-3] uart rx queue
 *  (q,w,e,r increment, a,s,d,f decrement)
 *
 *  prints timestamp, ae[0-3], sensors to uart tx queue
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  June 2016
 *------------------------------------------------------------------
 */

#include <stdlib.h>
#include "in4073.h"
#include "protocol/packet.h"


//#include "keyboard.h"

int mode = 0;
unsigned char currentByte;
unsigned char msgSize;
unsigned char msgType;
bool operation = 1;
unsigned char *ptr;
unsigned counter = 0;
Packet *pkt_R=NULL;

int maxMsgSize = 5;

void DoAction();



typedef struct Node{
	unsigned char data;
	struct Node *next;
}node;

node *InputDataBuffer = NULL;


node *addNode(node* firstNode, unsigned char newData){
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->data = newData;
	if (firstNode != NULL){
		temp->next = firstNode;
	}else {
		temp->next = NULL;
	}
	return temp;
}


unsigned char readData()
{
	if(InputDataBuffer==NULL)
		return dequeue(&rx_queue);
	else
	{

		unsigned char dataToReturn = InputDataBuffer->data;
		node *temp = InputDataBuffer->next;
		free(InputDataBuffer);
		InputDataBuffer = temp;
		return dataToReturn;

	}
}

uint16_t checkCount()
{
	return InputDataBuffer!=NULL?1:rx_queue.count;
}

void addData(unsigned char *datas, unsigned char length)
{
	for(int i =length-1;i>=0;i--)
	{
		InputDataBuffer = addNode(InputDataBuffer, datas[i]);
	}

}




/*------------------------------------------------------------------
 * process_key -- process command keys
 *------------------------------------------------------------------
 */
void process_packet(Packet *pkt_R)
{
	switch (pkt_R->type)
	{
		case T_MODE:
			nrf_gpio_pin_toggle(YELLOW);
			break;
		case T_CONTROL:
			nrf_gpio_pin_toggle(BLUE);
			break;
		default:
			nrf_gpio_pin_toggle(RED);

	}
	//printf("FCB\n");

	Destroy_Packet(pkt_R);
}




//Enums with all the possible states and events

enum stateList	{checkStartByte, getMsgSize, checkMessageType,setupMsg, getMsg, checkCRC0, checkCRC1}	currentState, nextState;

enum eventList	{noEvent, byteReceived, error, timeout} currentEvent;


//Event handling functions

void changeEvent(enum eventList newEventType){
	currentEvent = newEventType;
}

enum eventList getEvent(void){
	return currentEvent;
}

void clearEvents(void){
	currentEvent = noEvent;
}

void SearchforStartByte(unsigned char CRCPos)
{
	unsigned char *serialPacket = Get_Byte_Stream(pkt_R);
	unsigned char byteLength=pkt_R->packetLength-(1-CRCPos);

	for(int i =1;i<byteLength;i++)
	{
		if(serialPacket[i]==START_BYTE)
		{
			addData(&serialPacket[i],byteLength-i);
			break;
		}
	}
	Destroy_Packet(pkt_R);
	free(serialPacket);
	nextState=checkStartByte;
}


void storeValues()
{
			ptr[counter] = currentByte;


			counter++;
			if (counter >= msgSize-1) {
				nextState = checkCRC0;
			} else {
				nextState = getMsg;
			}

}
/*------------------------------------------------------------------
 * State Machine 
 *------------------------------------------------------------------
 */


void stateHandler(){
	switch(currentState){
		case checkStartByte:
			if(currentByte != START_BYTE){


				nextState = checkStartByte;
			}else{
				nextState = getMsgSize;
			}
			break;
		case getMsgSize:
			
			msgSize = currentByte;
			nextState = checkMessageType;
			if(msgSize>maxMsgSize)
			{
				if(currentByte==START_BYTE)
				{
					nextState=getMsgSize;
				}
				else
				{
				nextState=checkStartByte;
				}

			}


			break;
		case checkMessageType:
			if((currentByte==T_MODE) || (currentByte==T_CONTROL) || (currentByte==T_DATA))
			{
				msgType = currentByte;
				nextState = setupMsg;
				counter = 0;

			}
			else
			{
				if(msgSize==START_BYTE)
				{
					msgSize=currentByte;
					nextState = checkMessageType;
				}
				else if(currentByte==START_BYTE)
				{
					nextState=getMsgSize;
				}else
				{

					nextState=checkStartByte;
				}

			}

			break;

		case setupMsg:

			if(ptr!=NULL)
			{
				free(ptr);
			}

			ptr = (unsigned char *)malloc(sizeof(unsigned char)*(msgSize-1));

			storeValues();

		break;

		case getMsg:

			storeValues();

			break;
		case checkCRC0:
			counter=0;

			pkt_R = Create_Packet(msgType,msgSize-1, ptr);

				//printf("TestingF- Type:%d\n", pkt_R->type);
				//printf("TestingF- startbyte:%d\n", pkt_R->startByte);
				//printf("TestingF- datalength:%d\n", pkt_R->dataLength);
				//printf("TestingF- value length:%d\n", pkt_R->valueLength);
				//printf("TestingF- value:%d\n", *(pkt_R->value));
				//printf("TestingF- CRC0:%d\n", *(pkt_R->CRC));
				//printf("TestingF- CRC1:%d\n", pkt_R->CRC[1]);


			if(pkt_R->CRC[0]==(unsigned char)currentByte){
				nextState = checkCRC1;
			}
			else
			{
				pkt_R->CRC[0]=currentByte;
				SearchforStartByte(0);
			}


			break;
		case checkCRC1:

			if(pkt_R->CRC[1]==currentByte){
				nextState=checkStartByte;
				DoAction(); //todo
				process_packet(pkt_R);

			}
			else{
				pkt_R->CRC[1]=currentByte;
				SearchforStartByte(1);
			}

		break;		

	}
	currentState = nextState;

}




/*------------------------------------------------------------------
 * main -- everything you need is here :)
 *------------------------------------------------------------------
 */
int main(void)
{
	uart_init();
	gpio_init();
	timers_init();
	adc_init();
	twi_init();
	imu_init(true, 100);	
	baro_init();
	spi_flash_init();

	//ble_init();

	demo_done = true;
	currentState=checkStartByte;


	while(operation) {


		/*if (rx_queue.count) {process_Test( dequeue(&rx_queue) );
			printf("T\n");


		}*/

		if (checkCount()){  //continuously check for new elements in the UART queue
			currentByte = readData();
			//printf("State:%d\n",(int)currentState);
			//printf("FCB: %d\n",currentByte);
			stateHandler();
			//printf("State:%d\n",(int)currentState);

		}

		nrf_delay_ms(1);

		/*if (checkCount()){  //continuously check for new elements in the UART queue
			currentByte = readData();
			printf("FCB: %d",currentByte);
			stateHandler();
		}*/
	}
	
	

	/*while (!demo_done)
	{
		if (rx_queue.count) process_key( dequeue(&rx_queue) );

		if (check_timer_flag()) 
		{
			if (counter++%20 == 0) nrf_gpio_pin_toggle(BLUE);

			adc_request_sample();
			read_baro();

			printf("%10ld | ", get_time_us());
			printf("%3d %3d %3d %3d | ",ae[0],ae[1],ae[2],ae[3]);
			printf("%6d %6d %6d | ", phi, theta, psi);
			printf("%6d %6d %6d | ", sp, sq, sr);
			printf("%4d | %4ld | %6ld \n", bat_volt, temperature, pressure);

			clear_timer_flag();
		}

		if (check_sensor_int_flag()) 
		{
			get_dmp_data();
			run_filters_and_control();
		}\
	}*/	

	//printf("\n\t Goodbye \n\n");
	nrf_delay_ms(100);

	NVIC_SystemReset();
}

void DoAction(){}

//State machine for all the states




