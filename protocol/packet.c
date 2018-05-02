#include <stdlib.h>

#include "packet.h"	
#include "crc16.h"


Packet *Create_Packet(unsigned char type,unsigned char length, unsigned char *value)
{
	Packet *pkt = (Packet *)malloc(sizeof(Packet));
	
	pkt->startByte = START_BYTE;
	pkt->type = type;
	pkt->value = value;
	pkt->valueLength = length;
	pkt->dataLength=Get_DataLength(length);
	pkt->packetLength=1+1+pkt->dataLength+(CRC_BIT_SIZE/8);
	pkt->CRC=Get_CRCValue(pkt);
	return pkt;
}

unsigned char Get_DataLength(unsigned char length) //length is the length of value array
{
	return length+1;
}
unsigned char *Get_CRCValue(Packet *pkt)
{
	uint8_t *DataCRC = (uint8_t *)malloc(sizeof(uint8_t)*(pkt->dataLength));
	DataCRC[0]=pkt->type;
	for(int i =0;i<pkt->valueLength;i++)
	{
		DataCRC[i+1]=pkt->value[i];
	}

	uint16_t CRC = crc16_compute(DataCRC, pkt->dataLength, NULL);
	unsigned char* CRCARRAY = (unsigned char *)malloc(sizeof(unsigned char)*(CRC_BIT_SIZE/8));
	CRCARRAY[1]=(unsigned char)CRC & 0x00FF;
	CRCARRAY[0]=(unsigned char)((CRC & 0xFF00)>>8);
	free(DataCRC);
	return CRCARRAY;

}

void Destroy_Packet(Packet *pkt)
{
	free(pkt->value);
	free(pkt->CRC);
	free(pkt);
}

unsigned char *Get_Byte_Stream(Packet *pkt)
{
	unsigned char *serialPacket = (unsigned char *)malloc(sizeof(unsigned char)*(pkt->packetLength));
	serialPacket[0]=pkt->startByte;
	serialPacket[1]=pkt->dataLength;
	serialPacket[2]=pkt->type;
	for(int i =0;i<pkt->valueLength;i++)
	{
		serialPacket[3+i]=pkt->value[i];
	}
	serialPacket[pkt->packetLength-1]=pkt->CRC[1];
	serialPacket[pkt->packetLength-2]=pkt->CRC[0];
	return serialPacket;
}
