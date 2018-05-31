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
#include "messages/messages.h"
#include "modes/modes.h"


//#include "keyboard.h"
//General
void update_motors(void);
bool ExitFlag = 0;

void GetMotorValues();
void SetMotorValues_Manual();
void SendPacket(Packet *packetToSend);
char msgCode = 0;

void SetMessage(unsigned char _msgCode)
{
	msgCode=_msgCode;
}

//Sending
Packet *pkt_S=NULL;

//Logging to flash memory
int flashCounter = 0x000000;


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
uint32_t lastTelePacketSendTime = 0;
bool checkGap(uint32_t lastTime, uint32_t readGap);


/*------------------------------------------------------------------
 * process_packet -- process command keys
 *------------------------------------------------------------------
 */
void process_packet(Packet *pkt_R)
{
	lastPacketTime = get_time_us();

	if(CurrentMode.state!=Panic){
		switch (pkt_R->type)
		{
			case T_EXIT:
						msgCode=MSG_EXITING;
						if(CurrentMode.state != Safe)
							{
								CurrentMode = GetMode(M_PANIC);
							}
						else 
							{
								demo_done=0;
							}
						
						
				break;
			case T_MODE:
				nrf_gpio_pin_toggle(YELLOW);

				if(pkt_R->value[0]==M_SAFE || pkt_R->value[0]==M_PANIC || CurrentMode.state==Safe)
				{
					CurrentMode=GetMode(pkt_R->value[0]);
					(*CurrentMode.Mode_Initialize)();
				}
				else
				{
					SetMessage(MSG_BANNED_MODE_CHANGE);
				}

				break;
			case T_CONTROL:
				nrf_gpio_pin_toggle(BLUE);
				if(CurrentMode.Input_Handler!=NULL)

					(*CurrentMode.Input_Handler)(pkt_R->value);

				
				break;
			default:
				nrf_gpio_pin_toggle(RED);
				break;

		}
	}
	//printf("FCB\n");

	for (uint32_t j=0x000000; j<0x100000; j++){

	flash_write_byte(j,pkt_R->value[8]);
	flash_write_byte(j+1,pkt_R->value[9]); //batt volt
	for(int i=0;i<4;i++){
		flash_write_byte(j+2,pkt_R->value[2*i]);
		flash_write_byte(j+3,pkt_R->value[2*i+1]); //motors
	}
	flash_write_byte(j+4,pkt_R->value[10]); 
	flash_write_byte(j+5,pkt_R->value[11]); //phi
	flash_write_byte(j+6,pkt_R->value[12]); 
	flash_write_byte(j+7,pkt_R->value[13]); //theta 
	flash_write_byte(j+8,pkt_R->value[14]); 
	flash_write_byte(j+9,pkt_R->value[15]); //psi
	flash_write_byte(j+10,pkt_R->value[16]); 
	flash_write_byte(j+11,pkt_R->value[17]); //sp
	flash_write_byte(j+12,pkt_R->value[18]); 
	flash_write_byte(j+13,pkt_R->value[19]); //sq
	flash_write_byte(j+14,pkt_R->value[20]); 
	flash_write_byte(j+15,pkt_R->value[21]); //sq
	flash_write_byte(j+16,pkt_R->value[22]); //drone msg

	}


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


	Modes_Initialize();


	demo_done = true;
	currentStateR=checkStartByte;

	int readCounter = 10;

	while(demo_done) {

		//Execute Mode Specific Function

		(*CurrentMode.Mode_Execute)();

		//Recieve Packets
		if (checkCount()){  //continuously check for new elements in the UART queue

			readData();

			stateHandler();
		}

		

		//Check for Disconnection and Battery ADC Invoke
		if (readCounter%10 == 0){
			adc_request_sample();
			//printf("%ld\n",currentTime);

			if(checkGap(lastPacketTime,DISCONNECTED_GAP_US))
			{
				nrf_gpio_pin_set(RED);
				nrf_gpio_pin_set(BLUE);
				nrf_gpio_pin_set(GREEN);
				SetMessage(MSG_DISCONNECTED);	
				CurrentMode=GetMode(M_PANIC);	
			
					

			}
			readCounter=0;
		}

		if(bat_volt <= 1050 && bat_volt>0 )
		{	
			//CurrentMode=GetMode(M_PANIC);
		}
		
		readCounter++;


		//Send Telemetry Packets
		if(checkGap(lastTelePacketSendTime, TELE_SEND_GAP_MS))
		{

			SendPacket(Create_Telemetery_Packet(bat_volt, ae, phi, theta, psi, sp, sq, sr,msgCode));
			lastTelePacketSendTime=get_time_us();
		}
		
	}

	nrf_delay_ms(100);

	NVIC_SystemReset();

}

bool checkGap(uint32_t lastTime, uint32_t readGap)
{
	uint32_t currentTime = get_time_us();

	return (currentTime>lastTime?currentTime-lastTime>readGap: (UINT32_MAX-lastTime+currentTime)>readGap);


}


//State machine for all the states




void SendPacket(Packet *packetToSend)
{

	if(packetToSend!=NULL)
	{
		unsigned char *dataToSend = Get_Byte_Stream(packetToSend);
			for(int i =0;i<packetToSend->packetLength+1;i++)
			{
				printf("%c",dataToSend[i]);
			}
	}

}

