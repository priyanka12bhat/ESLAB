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
#include "math.h"
//#include "control.c"

#define B_DASH 1
#define D_DASH 4


#define MAX_SPEED 400
#define SPEED_SCALE 1
#define MAX_SPEED_SCALED MAX_SPEED/SPEED_SCALE

#define MAX_Z 4*MAX_SPEED_SCALED*MAX_SPEED_SCALED
#define MAX_L MAX_SPEED_SCALED*MAX_SPEED_SCALED
#define MAX_M MAX_SPEED_SCALED*MAX_SPEED_SCALED
#define MAX_N MAX_SPEED_SCALED*MAX_SPEED_SCALED*2

#define INCREMENT_PERCENT 10

#define INC_Z MAX_Z/100*INCREMENT_PERCENT
#define INC_L MAX_L/100*INCREMENT_PERCENT
#define INC_M MAX_M/100*INCREMENT_PERCENT
#define INC_N MAX_N/100*INCREMENT_PERCENT


//#include "keyboard.h"
//General
void update_motors(void);
bool operation = 1;
bool ExitFlag = 0;
void DoAction();
enum DroneState {Safe,Panic,Manual,Callibration, YawControlled, FullControl, RawMode, HeightControl,Wireless,Exiting} droneState;
void GetMotorValues();
void SetMotorValues_Manual();


//Sensor Handling
// MPU wrapper
int16_t phi_offset=0, theta_offset=0, psi_offset=0;
int16_t sp_offset=0, sq_offset=0, sr_offset=0;
int16_t sax_offset=0, say_offset=0, saz_offset=0;


//Manual Mode
int16_t aek[4];

//Yaw Controlled Mode
int16_t yawSetPoint =0;
int16_t yawSetPoint_K = 0;
int8_t yawSetPoint_J = 0;
int16_t P = 1000;


//Moments
uint32_t Z=0;
int32_t L=0;
int32_t M=0;
int32_t N=0;

int32_t JS_N = 0;
int32_t JS_L = 0;
int32_t JS_M = 0;
int32_t JS_Z = 0;

//Packet Reception
unsigned char currentByte;
unsigned char msgSize;
unsigned char msgType;
unsigned char *ptr;
unsigned counter = 0;
Packet *pkt_R=NULL;
int maxMsgSize = 12;
uint32_t lastPacketTime = 0;

void process_packet(Packet *pkt_R);


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


//Enums with all the possible states and events

enum stateListR	{checkStartByte, getMsgSize, checkMessageType,setupMsg, getMsg, checkCRC0, checkCRC1}	currentStateR, nextState;

enum eventListR	{noEvent, byteReceived, error, timeout} currentEvent;
	
//Event handling functions

void changeEvent(enum eventListR newEventType){
	currentEvent = newEventType;
}

enum eventListR getEvent(void){
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
	switch(currentStateR){
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
			if((currentByte==T_MODE) || (currentByte==T_CONTROL) || (currentByte==T_DATA)||(currentByte==T_EXIT))
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
	currentStateR = nextState;

}



void DroneStateHandler()
{

}

void EnterSafeMode()
{
	printf("Entered Safe Mode\n");
	droneState = Safe;
	ae[0] = 0;
	ae[1] = 0;
	ae[2] = 0;
	ae[3] = 0;
	aej[0] = 0;
	aej[1] = 0;
	aej[2] = 0;
	aej[3] = 0;


}


/*------------------------------------------------------------------
 * process_packet -- process command keys
 *------------------------------------------------------------------
 */
void process_packet(Packet *pkt_R)
{
	lastPacketTime = get_time_us();
	if(droneState!=Panic){
		switch (pkt_R->type)
		{
			case T_EXIT:
						printf("Drone Exiting..");
						ExitFlag = 1;
						if(droneState != Safe)
							{
								droneState = Panic;
							}
						else 
							{
								operation=0;
							}
						
						
				break;
			case T_MODE:
				nrf_gpio_pin_toggle(YELLOW);
				switch(pkt_R->value[0])
				{
					case M_SAFE:

						EnterSafeMode();
						update_motors();
					break;
					case M_PANIC:
						printf("Mode Switched to Panic mode\n");
						droneState = Panic;
						


					break;
					
					case M_MANUAL:
						if(droneState!=Manual){
							Z=0;
							L=0;
							M=0;
							N=0;


							JS_N = 0;
							JS_L = 0;
							JS_M = 0;
							JS_Z = 0;
						}
						if(droneState==Safe){
							printf("Mode Switched to Manual mode\n");
							droneState = Manual;
						}

					break;

					case M_CALIBRATION:
						
						if(droneState==Safe){
							printf("Calibrating..\n");

							get_dmp_data();

							phi_offset=phi;
							theta_offset= theta;
							psi_offset = psi;
							sp_offset = sp;
							sq_offset = sq;
							sr_offset = sr;
							sax_offset = sax;
							say_offset = say;
							saz_offset = saz;
							printf("phi_offset:%6d theta_offset:%6d psi_offset:%6d\n", phi_offset, theta_offset, psi_offset);
							printf("sp_offset:%6d sq_offset:%6d sr_offset:%6d\n", sp_offset, sq_offset, sr_offset);
							printf("sax_offset:%6d say_offset:%6d saz_offset:%6d\n", sax_offset, say_offset, saz_offset);
							printf("Calibration Done\n");
						}

					break;

					case M_YAWCONTROL:
						//Z=64000*5;
						if(droneState==Safe){
							droneState=YawControlled;
							yawSetPoint =0;
							yawSetPoint_K = 0;
							yawSetPoint_J = 0;
							printf("Switched to Yaw Controlled Mode..\n");							
						}

					break;



				}
				break;
			case T_CONTROL:
				nrf_gpio_pin_toggle(BLUE);
				if(droneState==Manual){					
					nrf_gpio_pin_toggle(RED);
					
					switch(pkt_R->value[0])
					{
						

						case C_LIFTUP:

							if((Z+INC_Z)<=MAX_Z){
								Z+=INC_Z;
							}						

							
						break;
						case C_LIFTDOWN:
							if(Z>=INC_Z)
							{
								Z-=INC_Z;
							}
								

						break;

						case C_ROLLUP:
							if((L+INC_L)<=MAX_L){
								L+=INC_L;
							}						

						break;

						case C_ROLLDOWN:
							if((L-INC_L)>=(-1*MAX_L))
							{
								
								L-=INC_L;
							}
								
						break;

						case C_PITCHUP:
							if((M+INC_M)<=MAX_M){
								M+=INC_M;
							}
						break;

						case C_PITCHDOWN:
							if((M-INC_M)>=(-1*MAX_M))
							{
								M-=INC_M;
							}
						break;

						case C_YAWUP:
							if((N+INC_N)<=MAX_N){
								N+=INC_N;
							}		
						break;

						case C_YAWDOWN:
							if((N-INC_N)>=(-1*MAX_N))
							{
								N-=INC_N;
							}
						break;						

						case C_JOYSTICK:

						JS_L = ((int32_t)MAX_L)*((int8_t)pkt_R->value[1])/JSSCALEMAX;
						JS_M = ((int32_t)MAX_M)*((int8_t)pkt_R->value[2])/JSSCALEMAX;
						JS_N = ((int32_t)MAX_N)*((int8_t)pkt_R->value[3])/JSSCALEMAX;
						JS_Z = ((int32_t)MAX_Z)*(JSSCALEMAX+(int8_t)pkt_R->value[4])/JSSCALEMAX;
						printf("JS_R:%ld|",JS_L);
						printf("JS_P:%ld|",JS_M);
						printf("JS_Y:%ld|",JS_N);
						printf("JS_L:%ld|",JS_Z);


						break;
						}
					printf("Z:%ld|",Z);
					printf("L:%ld|",L);
					printf("M:%ld|",M);
					printf("N:%ld|",N);
				SetMotorValues_Manual();
				printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
				update_motors();
				}
				if(droneState==YawControlled)
				{	
					switch(pkt_R->value[0]){

						case C_LIFTUP:

							if((Z+INC_Z)<=MAX_Z){
								Z+=INC_Z;
							}						

							
						break;
						case C_LIFTDOWN:
							if(Z>=INC_Z)
							{
								Z-=INC_Z;
							}
								

						break;

						case C_YAWUP:

							yawSetPoint_K+=10;

						break;

						case C_YAWDOWN:
							yawSetPoint_K-=10;
						break;						

						case C_JOYSTICK:
							yawSetPoint_J = ((int8_t)pkt_R->value[3])*10/JSSCALEMAX;
							JS_Z = ((int32_t)INC_Z)*2*((int8_t)pkt_R->value[4])/JSSCALEMAX;

						break;

						case C_PUP:
							P+=10;
						break;
						case C_PDOWN:
							if(P-10>=10){
								P-=10;
							}
						break;


						
					}
					printf("P:%d\n",P);
					yawSetPoint = yawSetPoint_K + yawSetPoint_J;


				}


				
				break;
			default:
				nrf_gpio_pin_toggle(RED);

		}
	}
	//printf("FCB\n");

	Destroy_Packet(pkt_R);
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
	currentStateR=checkStartByte;
	droneState = Safe;
	uint32_t us_TimeStamp = 0;
	int readCounter = 10;

	while(operation) {


		/*if (rx_queue.count) {process_Test( dequeue(&rx_queue) );
			printf("T\n");


		}*/

		if (checkCount()){  //continuously check for new elements in the UART queue
			currentByte = readData();
			//printf("State:%d\n",(int)currentStateR);
			//printf("FCB: %d\n",currentByte);
			stateHandler();
			//printf("State:%d\n",(int)currentStateR);

		}

		nrf_delay_ms(1);

		
		if (readCounter == 10){
			adc_request_sample();

			readCounter = 0;
			uint32_t currentTime = get_time_us();
			//printf("%ld\n",currentTime);

			if(currentTime>lastPacketTime?currentTime-lastPacketTime>DISCONNECTED_GAP_US: (UINT32_MAX-lastPacketTime+currentTime)>DISCONNECTED_GAP_US)
			{
							nrf_gpio_pin_set(RED);
					nrf_gpio_pin_set(BLUE);
					nrf_gpio_pin_set(GREEN);
				
				
				if(!ExitFlag)
					{
				
				//printf("%ld\n",currentTime);
					if(droneState!=Safe){
					droneState=Panic;
					printf("Disconnected\n");
				//	nrf_gpio_pin_set(RED);
				//	nrf_gpio_pin_set(BLUE);
				//	nrf_gpio_pin_set(GREEN);
				}

			}
					

			}
			}
			//printf("%4d\n",bat_volt);
		readCounter++;

		if(bat_volt <= 1050 && bat_volt>0 )
		{	
			droneState = Panic;
			printf("BAT VOLT:%d\n",bat_volt);
		}

		


		/*if (checkCount()){  //continuously check for new elements in the UART queue
			currentByte = readData();
			printf("FCB: %d",currentByte);
			stateHandler();
		}*/

		if(droneState==YawControlled)
		{

			if (check_sensor_int_flag()) 
			{
			get_dmp_data();		
			N = P* (yawSetPoint - sr + sr_offset);
			printf("Z:%ld|L:%ld|M:%ld|N:%ld|",Z,L,M,N);
			GetMotorValues();
			update_motors();
			printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
			}


		}

		if(droneState==Panic)
		{
			int16_t ae_avg = (ae[0]+ae[1]+ae[2]+ae[3])>>2;
			ae[0] = ae[1] = ae[2] = ae[3] = ae_avg;
			uint32_t us_currentTime = get_time_us();

			if(us_currentTime>us_TimeStamp?us_currentTime-us_TimeStamp>500000: (UINT32_MAX-us_TimeStamp+us_currentTime)>500000){
			//printf("%10ld | ", us_currentTime);
			//printf("%10ld | \n", us_TimeStamp);
			
			if(ae[0]>10)
			{
				ae[0]-=10;
			}
			if(ae[1]>10)
			{
				ae[1]-=10;
			}
			if(ae[2]>10)
			{
				ae[2]-=10;
			}
			if(ae[3]>10)
			{
				ae[3]-=10;
			}

			printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
			us_TimeStamp = us_currentTime;
			if(ae[0]<=10 && ae[1]<=10 && ae[2]<=10 && ae[3]<=10)
			{
				
				if(!ExitFlag)
					{
						EnterSafeMode();
					}
				else
				{
					operation = 0;
				}


			}

			update_motors();
			}
		}

		
	}

	
	char junk = 27;

	printf("%c%c%c%c%c\n",junk,junk,junk,junk,junk);

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


void SetMotorValues_Manual()
{
	int32_t z = (Z+JS_Z)*1/B_DASH;
	int32_t l = (L+JS_L)*1/B_DASH;
	int32_t m = (M+JS_M)*1/B_DASH;
	int32_t n = (N+JS_N)*1*D_DASH;
	int32_t ae0_2 = (m-(n>>1)+(z>>1))>>1;
	int32_t ae1_2 = ((n>>1)-l+(z>>1))>>1;
	int32_t ae2_2 = ((z>>1)-(n>>1)-m)>>1;
	int32_t ae3_2 = (l+(n>>1)+(z>>1))>>1;
	ae[0]=(uint16_t)sqrt(ae0_2<0?0:ae0_2);
	ae[1]=(uint16_t)sqrt(ae1_2<0?0:ae1_2);
	ae[2]=(uint16_t)sqrt(ae2_2<0?0:ae2_2);
	ae[3]=(uint16_t)sqrt(ae3_2<0?0:ae3_2);

}

//State machine for all the states

void GetMotorValues()
{
	int32_t z = Z*1/B_DASH;
	int32_t l = L*1/B_DASH;
	int32_t m = M*1/B_DASH;
	int32_t n = N*1/D_DASH;

	int32_t ae0_2 = (m-(n>>1)+(z>>1))>>1;
	int32_t ae1_2 = ((n>>1)-l+(z>>1))>>1;
	int32_t ae2_2 = ((z>>1)-(n>>1)-m)>>1;
	int32_t ae3_2 = (l+(n>>1)+(z>>1))>>1;
	ae[0]=(uint16_t)sqrt(ae0_2<0?0:ae0_2);
	ae[1]=(uint16_t)sqrt(ae1_2<0?0:ae1_2);
	ae[2]=(uint16_t)sqrt(ae2_2<0?0:ae2_2);
	ae[3]=(uint16_t)sqrt(ae3_2<0?0:ae3_2);



}




