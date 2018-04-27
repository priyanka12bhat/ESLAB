#include "packet.h"	
#include "crc16.h"


Packet Create_Packet(char type,char length, char *value)
{
	Packet *pkt = (Packet *)malloc(sizeof(Packet));
	pkt->startByte = START_BYTE;
	pkt->value = value;
	pkt->type = type;
	pkt->dataLength=Get_DataLength(length);

	pkt->CRC=Get_CRCValue(*pkt);

	return *pkt;
}

char Get_DataLength(char length)
{
	return length+1;
}
char *Get_CRCValue(Packet *p)
{
	char *DataCRC = (char *)malloc(sizeof(char)*pkt->dataLength);
	DataCRC[0]=pkt->type;
	for(char i =0;i<dataLength-1;i++)
	{
		DataCRC[i+1]=pkt->value[i];
	}
	uint16_t CRC = crc16_compute(DataCRC, uint32_t dataLength, NULL);
	char* CRCARRAY = (char *)malloc(sizeof(char)*2);
	CRCARRAY[0]=(char)CRC & 0x00FF;
	CRCARRAY[1]=(char)(CRC & 0xFF00>>8);

	return CRCARRAY;

}