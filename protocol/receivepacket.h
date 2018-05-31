#ifndef _RECEIVE_PACKET
#define _RECEIVE_PACKET


#include "packet.h"
#include <stdlib.h>
#include <stdint.h>

//Enums with all the possible states and events

enum stateListR	{checkStartByte, getMsgSize, checkMessageType,setupMsg, getMsg, checkCRC0, checkCRC1}	currentStateR, nextState;

enum eventListR	{noEvent, byteReceived, error, timeout} currentEvent;






char getElementFromInputQueue();
uint16_t getInputQueueCount();

void process_packet(Packet *pkt_R);





void addNode(unsigned char newData);
unsigned char readData();


uint16_t checkCount();

void addData(unsigned char *datas, unsigned char length);



	
//Event handling functions

void changeEvent(enum eventListR newEventType);
enum eventListR getEvent(void);

void clearEvents(void);

void SearchforStartByte(unsigned char CRCPos);

void storeValues();
void stateHandler();
void Reception_Init(char _maxMsgSize);


#endif 
