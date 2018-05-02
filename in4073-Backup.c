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
char currentByte;
char msgSize;
char msgType;
bool operation = 0;
char *ptr;
unsigned counter = 0;
Packet *pkt_R=NULL;

void DoAction();


char CRC0;
char CRC1;


typedef struct LinkedList{
	char data;
	struct LinkedList *next;
}node;


node *MsgBuffer = NULL;

node *addNode(node* firstNode, char newData){
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

void freeList(node *list)
{
	if(list!=NULL){
	node *temp = list;
	list = list->next;
	free(temp);
	freeList(list);
	}

}

/*------------------------------------------------------------------
 * process_key -- process command keys
 *------------------------------------------------------------------
 */
/*void process_key(uint8_t c)
{
	switch (c)
	{
		case ONE:
			nrf_gpio_pin_toggle(YELLOW)
			break;
		case TWO:
			nrf_gpio_pin_toggle(BLUE)
			break;
		default:
			nrf_gpio_pin_toggle(RED);
	}
}*/


//Enums with all the possible states and events

enum stateList	{checkStartByte, getMsgSize, checkMessageType, setupMsg, getMsg, checkCRC0, checkCRC1}	currentState, nextState;

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

void SearchforStartByte(int CRCByte)
{
	char *serialPacket = Get_Byte_Stream(pkt_R);
	int i = 0;
	for(i=pkt->packetLength-1;i>0;i--)
	{
		if(serialPacket[i]==START_BYTE)
		{
			break;
		}
	}

	if(i!=0)
	{
		for(j=1;j+i<packetLength-1;j++)
		{
			serialPacket[i+j]
		}
	}



				node *searchNode=MsgBuffer;
				int count = 0;
				while(searchNode!=NULL)
				{
					if(searchNode->data!=START_BYTE)
					{
						searchNode=searchNode->next;
						count++;
					}
					else
					{
						switch(count)
						{
							case 0:
							nextState=getMsgSize;
							break;
							
							case 1:
							msgSize=MsgBuffer->data;
							nextState=checkMessageType;
							break;

							case 2:
							msgType=MsgBuffer->data;
							msgSize=MsgBuffer->next->data;
							nextState=setupMsg;
							counter=0;
							break;

							default:
							nextState=checkStartByte;
							break;
						}
						
						break;


					}
				}
				if(searchNode==NULL)
				{
					nextState=checkStartByte;
				}else
				{
					if(searchNode->next!=NULL)
					{
						freeList(searchNode->next);
						searchNode->next=NULL;
					}

				}



				nextState=checkStartByte;

}

/*------------------------------------------------------------------
 * State Machine 
 *------------------------------------------------------------------
 */


void stateHandler(){
	switch(currentState){
		case checkStartByte:
			if(currentByte != START_BYTE){
				freeList(MsgBuffer);
				MsgBuffer=NULL;
				nextState = checkStartByte;
			}
			nextState = getMsgSize;
			break;
		case getMsgSize:
			nextState = checkMessageType;
			msgSize = currentByte;
			break;
		case checkMessageType:
			if((currentByte==T_MODE) || (currentByte==T_CONTROL) || (currentByte==T_DATA))
			{
				msgType = currentByte;
				nextState = setupMsg;

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
				freeList(MsgBuffer);
				MsgBuffer=NULL;
			}

			break;
		case setupMsg:
			ptr = (char *)malloc(sizeof(char)*(msgSize-1));
			ptr[0]=currentByte;
			counter = 1;
			nextState = getMsg;
			break;
		case getMsg:
			ptr[counter] = currentByte;
			counter++;
			if (counter > msgSize-1) {
				nextState = checkCRC0;
			} else {
				nextState = getMsg;
			}
			break;
		case checkCRC0:
			pkt_R = Create_Packet(msgType,msgSize-1, ptr);

			if(pkt_R->CRC[0]==currentByte){
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
				freeList(MsgBuffer);
				MsgBuffer = NULL;
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
		if (rx_queue.count){  //continuously check for new elements in the UART queue
			currentByte = dequeue(&rx_queue);
			MsgBuffer = addNode(MsgBuffer, currentByte);
			changeEvent(byteReceived);
			stateHandler();
		}
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




