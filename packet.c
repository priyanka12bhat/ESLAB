#include <stdlib.h>

#include "packet.h"	
#include "crc16.h"


Packet *Create_Packet(char type,char length, char *value)
{
	Packet *pkt = (Packet *)malloc(sizeof(Packet));
	pkt->startByte = START_BYTE;
	pkt->type = type;
	pkt->value = value;
	pkt->valueLength = length;
	pkt->dataLength=Get_DataLength(length);
	pkt->CRC=Get_CRCValue(pkt);

	return pkt;
}

char Get_DataLength(char length) //length is the length of value array
{
	return length+1;
}
char *Get_CRCValue(Packet *pkt)
{
	char *DataCRC = (char *)malloc(sizeof(char)*(pkt->dataLength));
	DataCRC[0]=pkt->type;
	for(char i =0;i<pkt->valueLength;i++)
	{
		DataCRC[i+1]=pkt->value[i];
	}

	uint16_t CRC = crc16_compute(DataCRC, pkt->dataLength, NULL);
	char* CRCARRAY = (char *)malloc(sizeof(char)*(CRC_BIT_SIZE/8));
	CRCARRAY[1]=(char)CRC & 0x00FF;
	CRCARRAY[0]=(char)((CRC & 0xFF00)>>8);
	free(DataCRC);
	return CRCARRAY;

}

void Destroy_Packet(Packet *pkt)
{
	free(pkt->value);
	free(pkt->CRC);
	free(pkt);
}

char *Get_Byte_Stream(Packet *pkt)
{
	char packetLength = 1+1+pkt->dataLength+(CRC_BIT_SIZE/8);
	char *serialPacket = (char *)malloc(sizeof(char)*packetLength);
	serialPacket[0]=pkt->startByte;
	serialPacket[1]=pkt->dataLength;
	serialPacket[2]=pkt->type;
	for(char i =0;i<pkt->valueLength;i++)
	{
		serialPacket[3+i]=pkt->value[i];
	}
	serialPacket[packetLength-1]=pkt->CRC[1];
	serialPacket[packetLength-2]=pkt->CRC[0];
	return serialPacket;
}
