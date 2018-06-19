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
#include <stdarg.h>

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

//ONLY FOR DEBUGGING!. Remove instances after use.
void SendAdditionalMessage( char* msgfmt, ...)
{   
	char additionalMessage2[15];
    va_list vl;
    va_start(vl, msgfmt);

    vsnprintf( additionalMessage2, 15, msgfmt, vl);

    va_end(vl);
    SendPacket(Create_adMSG_Packet(additionalMessage2));


}

void SetAdditionalMessage( char* msgfmt, ...)
{   
    va_list vl;
    va_start(vl, msgfmt);

    vsnprintf( additionalMessage, 15, msgfmt, vl);

    va_end(vl);
    


}



//Sending
Packet *pkt_S=NULL;

void SendPacket(Packet *packetToSend)
{

	if(packetToSend!=NULL)
	{
		unsigned char *dataToSend = Get_Byte_Stream(packetToSend);
			for(int i =0;i<packetToSend->packetLength+1;i++)
			{
				uart_put(dataToSend[i]);
			}
	}

}


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
uint32_t lastBlueLEDBlinkTime = 0;
uint32_t lastBaroReadTime =0;





/*------------------------------------------------------------------
 * process_packet -- process command keys
 *------------------------------------------------------------------
 */
void process_packet(Packet *pkt_R)
{

	lastPacketTime = get_time_us();
	//printf("PacketRecived@%ld\n",lastPacketTime);
	if(CurrentMode.state!=Panic){
		switch (pkt_R->type)
		{
			case T_EXIT:
						msgCode=MSG_EXITING;
						if(CurrentMode.state != Safe)
							{
								PrevMode = CurrentMode;
								CurrentMode = GetMode(M_PANIC);
								(*CurrentMode.Mode_Initialize)();
							}
						else 
							{
								demo_done=0;
							}
						
						
				break;
			case T_MODE:
				nrf_gpio_pin_toggle(YELLOW);

				if(pkt_R->value[0]==M_SAFE || pkt_R->value[0]==M_PANIC || (CurrentMode.state==Safe && pkt_R->value[0]!=M_HEIGHTCONTROL)  || (CurrentMode.state>Callibration && CurrentMode.state!=HeightControl   && pkt_R->value[0]==M_HEIGHTCONTROL)  )
				{
					PrevMode = CurrentMode;
					CurrentMode=GetMode(pkt_R->value[0]);
					(*CurrentMode.Mode_Initialize)();
				}else if(CurrentMode.state==HeightControl && ((PrevMode.state==(pkt_R->value[0]-1))||(pkt_R->value[0]==M_HEIGHTCONTROL)))
				{
					Mode temp = PrevMode;
					PrevMode = CurrentMode;
					CurrentMode=temp;
				}
				else
				{
					SetMessage(MSG_BANNED_MODE_CHANGE);
				}

				break;
			case T_CONTROL:
				nrf_gpio_pin_toggle(GREEN);
				if(CurrentMode.Input_Handler!=NULL)
					(*CurrentMode.Input_Handler)(pkt_R->value);

				break;
			case T_CONFIG:
				nrf_gpio_pin_toggle(GREEN);
				Modes_ToggleLogging();
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
	ble_init();


	Modes_Initialize();


	demo_done = true;
	currentStateR=checkStartByte;

	int readCounter = 10;

	while(demo_done) {



		//Receive Packets
		if (checkCount()){  //continuously check for new elements in the UART queue

			unsigned char currentByte = readData();

			stateHandler(currentByte);
		}



		//Execute Mode Specific Function
		(*CurrentMode.Mode_Execute)();

		

		//Check for Disconnection and Battery ADC Invoke
		if (readCounter%100 == 0){
			adc_request_sample();
			//printf("%ld\n",currentTime);

			if(checkGap(lastPacketTime,DISCONNECTED_GAP_US))
			{
				SetMessage(MSG_DISCONNECTED);
				PrevMode = CurrentMode;
				CurrentMode=GetMode(M_PANIC);
				(*CurrentMode.Mode_Initialize)();					

			}
			readCounter=0;
		}

		if(bat_volt <= 1050 && bat_volt>0 )
		{	
			//PrevMode = CurrentMode;
			//CurrentMode=GetMode(M_PANIC);
			//(*CurrentMode.Mode_Initialize)();	
		}
		
		readCounter++;


		//Send Telemetry Packets
		if(checkGap(lastTelePacketSendTime, TELE_SEND_GAP_US))
		{

			SendPacket(Create_Telemetery_Packet(bat_volt, ae, phi, theta, psi, sp, sq, sr,msgCode,GetPArray(),pressure,CurrentMode.state));
			//SendAdditionalMessage(additionalMessage);
			lastTelePacketSendTime=currentTime;
		}


		//Blue LED Blink
				//Send Telemetry Packets
		if(checkGap(lastBlueLEDBlinkTime, 1000000))
		{

			nrf_gpio_pin_toggle(BLUE);
			lastBlueLEDBlinkTime=currentTime;
		}


		if(checkGap(lastBaroReadTime, 50))
		{
			read_baro();
			lastBaroReadTime = currentTime;
		}
		
	}

	nrf_delay_ms(100);

	NVIC_SystemReset();

}

bool checkGap(uint32_t lastTime, uint32_t readGap)
{
	currentTime = get_time_us();
	return ((currentTime==lastTime)?0:((currentTime>lastTime)?((currentTime-lastTime)>=readGap): (((lastTime-currentTime)<400)?0:((UINT32_MAX-lastTime+currentTime)>=readGap))));


}

void SendBluetoothPacket(Packet *packetToSend)
{

	if(packetToSend!=NULL)
	{
		unsigned char *dataToSend = Get_Byte_Stream(packetToSend);
			for(int i =0;i<packetToSend->packetLength+1;i++)
			{
				enqueue(&ble_tx_queue, (char)dataToSend[i]);
			}
	}

}

