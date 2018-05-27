

#include "receivepacket.h"

//Packet Reception
unsigned char currentByte;
unsigned char msgSize;
unsigned char msgType;

unsigned counter = 0;
Packet *pkt_R=NULL;

#define BUFFER_QUEUE_SIZE 255+50
typedef struct {
	uint8_t Data[BUFFER_QUEUE_SIZE];
  	uint16_t count; 
} Bqueue;



Bqueue InputBuffer;
char maxMsgSize = MAX_MSG_SIZE;

void Reception_Init(char _maxMsgSize)
{
	maxMsgSize=_maxMsgSize;
	InputBuffer.count=0;
}



void addNode(unsigned char newData)
{
	if(InputBuffer.count>=BUFFER_QUEUE_SIZE)
		return;
	InputBuffer.Data[InputBuffer.count]=newData;
	InputBuffer.count++;
}


unsigned char readData()
{
	if(InputBuffer.count==0)
		currentByte =  getElementFromInputQueue();
	else
	{
		InputBuffer.count--;
		currentByte = InputBuffer.Data[InputBuffer.count];

	}
	return currentByte;


}

uint16_t checkCount()
{
	return InputBuffer.count>0?1:getInputQueueCount();
}

void addData(unsigned char *datas, unsigned char length)
{
	for(int i =length-1;i>=0;i--)
	{
		addNode(datas[i]);
	}

}



	
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
	nextState=checkStartByte;
}


void storeValues(unsigned char ptr[])
{

			ptr[counter] = currentByte;


			counter++;
			if (counter >= msgSize-1) {
				nextState = checkCRC0;
			} else {
				nextState = getMsg;
			}

}

void stateHandler(){
	static unsigned char ptr[LP_SIZE+1];

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
			if(msgSize>=maxMsgSize)
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
			counter=0;

			storeValues(ptr);

		break;

		case getMsg:

			storeValues(ptr);

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