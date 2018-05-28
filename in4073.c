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
#include "protocol/receivepacket.h"


#define B_DASH 1
#define D_DASH 4


#define MAX_SPEED 400
#define MIN_SPEED_ONFLY 200
#define SPEED_SCALE 1
#define MAX_SPEED_SCALED MAX_SPEED/SPEED_SCALE
unsigned char *ptr;
unsigned counter = 0;
Packet *pkt_R=NULL;
uint8_t *flashBuffer;

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
bool ExitFlag = 0;

enum DroneState {Safe,Panic,Manual,Callibration, YawControlled, FullControl, RawMode, HeightControl,Wireless,Exiting} droneState;
void GetMotorValues();
void SetMotorValues_Manual();


//Sensor Handling
// MPU wrapper
int16_t phi_offset=0, theta_offset=0, psi_offset=0;
int16_t sp_offset=0, sq_offset=0, sr_offset=0;
int16_t sax_offset=0, say_offset=0, saz_offset=0;


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

//Sending
Packet *pkt_S=NULL;


//Functions for Package Reception
void process_packet(Packet *pkt_R);
char getElementFromInputQueue()
{
	return dequeue(&rx_queue);
}

uint16_t getInputQueueCount()
{
	return rx_queue.count;
}


uint32_t lastPacketTime = 0;
uint32_t PacketReceptionStartTime = 0;
uint32_t PacketCount = 0;

/*------------------------------------------------------------------
 * State Machine 
 *------------------------------------------------------------------
 */






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

}


/*------------------------------------------------------------------
 * process_packet -- process command keys
 *------------------------------------------------------------------
 */
void process_packet(Packet *pkt_R)
{
	lastPacketTime = get_time_us();
	//printf("RT:%ld\n",lastPacketTime-PacketReceptionStartTime);
	//PacketReceptionStartTime = 0;

	PacketCount++;
	printf("PacketRece:%ld\n",PacketCount);
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
								demo_done=0;
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
						JS_Z = ((int32_t)MAX_Z)*(JSSCALEMAX+(int8_t)pkt_R->value[4])/JSSCALEMAX/2;
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
				break;

		}
	}
	//printf("FCB\n");

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
	Reception_Init(MAX_MSG_SIZE);
	//ble_init();



	demo_done = true;
	currentStateR=checkStartByte;
	droneState = Safe;
	uint32_t us_TimeStamp = 0;
	int readCounter = 100;

	while(demo_done) {
		//uint32_t LoopTime =  get_time_us();

		/*if (rx_queue.count) {process_Test( dequeue(&rx_queue) );
			printf("T\n");


		}*/

		if (checkCount()){  //continuously check for new elements in the UART queue
			if(PacketReceptionStartTime==0)
				PacketReceptionStartTime = get_time_us();
			readData();
			//printf("State:%d\n",(int)currentStateR);
			//printf("FCB: %d\n",currentByte);
			stateHandler();
			//printf("State:%d\n",(int)currentStateR);

		}

		//nrf_delay_ms(1);

		

		


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
			//printf("Z:%ld|L:%ld|M:%ld|N:%ld|",Z,L,M,N);
			GetMotorValues();
			update_motors();
			//printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
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
			flash_read_bytes(0x000000, flashBuffer, 8*(flashCounter+1));
			for (int i = 0; i < 8*(flashCounter+1); i++){
				printf("%c\n",*flashBuffer+i);
}
			if(ae[0]<=10 && ae[1]<=10 && ae[2]<=10 && ae[3]<=10)
			{
				
				if(!ExitFlag)
					{
						EnterSafeMode();
					}
				else
				{
					demo_done = 0;
				}


			}

			update_motors();
			}
		}
		//printf("LoopTime:%ld\n",get_time_us()-LoopTime);


		if (readCounter%10 == 0){
			adc_request_sample();

			uint32_t currentTime = get_time_us();
			//printf("%ld\n",currentTime);

			if(currentTime>lastPacketTime?currentTime-lastPacketTime>DISCONNECTED_GAP_US: (UINT32_MAX-lastPacketTime+currentTime)>DISCONNECTED_GAP_US)
			{
							nrf_gpio_pin_set(RED);
					nrf_gpio_pin_set(BLUE);
					nrf_gpio_pin_set(GREEN);
				
				
				if(!ExitFlag){
				
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

		if(readCounter%99==0)
		{
			readCounter=0;
			Packet *pkt_S = Create_Telemetery_Packet(bat_volt, ae, phi, theta, psi, sp, sq, sr);
			unsigned char *dataToSend = Get_Byte_Stream(pkt_S);
			for(int i =0;i<pkt_S->packetLength;i++)
			{
				printf("%c",dataToSend[i]);
			}
		}



			//printf("%4d\n",bat_volt);
		readCounter++;

		if(bat_volt <= 1050 && bat_volt>0 )
		{	
			//droneState = Panic;
			//printf("BAT VOLT:%d\n",bat_volt);
		}

		
	}

	
	char junk = 27;

	printf("%c%c%c%c%c\n",junk,junk,junk,junk,junk);

	nrf_delay_ms(100);

	NVIC_SystemReset();

}

void SetMotorValues_Manual()
{

	if(Z+JS_Z==0)
	{
		//setting mottor values to zero in case of zero lift
		L=0;
		M=0;
		N=0;
		ae[0]=0;
		ae[1]=0;
		ae[2]=0;
		ae[3]=0;
	}
	else{

	/*
	Solving System of Equation

	Z=B_DASH(ae0^2+ae1^2+ae2^2+ae3^2)
	L=B_DASH(ae0^2-ae2^2)
	L=B_DASH(ae1^2-ae3^2)
	N*D_DASH=(ae0^2-ae1^2+ae2^2-ae3^2)
	*/
	int32_t z = (Z+JS_Z)/B_DASH;
	int32_t l = (L+JS_L)/B_DASH;
	int32_t m = (M+JS_M)/B_DASH;
	int32_t n = (N+JS_N)*D_DASH;

	int32_t ae0_2 = (m-(n>>1)+(z>>1))>>1;
	int32_t ae1_2 = ((n>>1)-l+(z>>1))>>1;
	int32_t ae2_2 = ((z>>1)-(n>>1)-m)>>1;
	int32_t ae3_2 = (l+(n>>1)+(z>>1))>>1;

	//For negative values, seting minimum as zero
	ae[0]=(uint16_t)sqrt(ae0_2<0?0:ae0_2);
	ae[1]=(uint16_t)sqrt(ae1_2<0?0:ae1_2);
	ae[2]=(uint16_t)sqrt(ae2_2<0?0:ae2_2);
	ae[3]=(uint16_t)sqrt(ae3_2<0?0:ae3_2);

	//Calculating the speed for current lift applied alone
	uint16_t minSpeed = (uint16_t)sqrt(z>>2);
	//Ensuring lift wont go below MIN_SPEED_ONFLY limit, for YAWING, ROLLING, PITCHING with higher lifts
	minSpeed = (minSpeed<MIN_SPEED_ONFLY)?minSpeed:MIN_SPEED_ONFLY;

	//Confirm Motor Values considering all limits
	ae[0]=(ae[0]<MIN_SPEED_ONFLY)?minSpeed:ae[0];
	ae[1]=(ae[1]<MIN_SPEED_ONFLY)?minSpeed:ae[1];
	ae[2]=(ae[2]<MIN_SPEED_ONFLY)?minSpeed:ae[2];
	ae[3]=(ae[3]<MIN_SPEED_ONFLY)?minSpeed:ae[3];
	}

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




