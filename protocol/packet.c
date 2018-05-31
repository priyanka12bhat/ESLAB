#include <stdlib.h>

#include "packet.h"	
#include "crc16.h"

Packet pktObj;
Packet TpktObj;

unsigned char serialPacket[LP_SIZE+10];

Packet *Create_Packet(unsigned char type,unsigned char length, unsigned char *value)
{

	
	pktObj.startByte = START_BYTE;
	pktObj.type = type;
	for(int i = 0;i<length;i++)
	{
		pktObj.value[i] = value[i];
	}

	pktObj.valueLength = length;
	pktObj.dataLength=Get_DataLength(length);
	pktObj.packetLength=1+1+pktObj.dataLength+(CRC_BYTE_SIZE);
	Set_CRCValue(&pktObj);
	return &pktObj;
}




unsigned char Get_DataLength(unsigned char length) //length is the length of value array
{
	return length+1;
}

void Set_CRCValue(Packet *pkt)
{

	uint8_t DataCRC[LP_SIZE+5];
	DataCRC[0]=pkt->type;
	for(int i =0;i<pkt->valueLength;i++)
	{
		DataCRC[i+1]=pkt->value[i];
	}
	uint16_t CRC = crc16_compute(DataCRC, pkt->dataLength, NULL);
	pkt->CRC[1]=(unsigned char)CRC & 0x00FF;
	pkt->CRC[0]=(unsigned char)((CRC & 0xFF00)>>8);

}


void Destroy_Packet(Packet *pkt)
{
}

unsigned char *Get_Byte_Stream(Packet *pkt)
{
	serialPacket[0]=pkt->startByte;
	serialPacket[1]=pkt->dataLength;
	serialPacket[2]=pkt->type;
	for(int i =0;i<pkt->valueLength;i++)
	{
		serialPacket[3+i]=pkt->value[i];
	}
	serialPacket[pkt->packetLength-1]=pkt->CRC[1];
	serialPacket[pkt->packetLength-2]=pkt->CRC[0];
	serialPacket[pkt->packetLength]='\n';
	return serialPacket;
}


Packet *Create_Telemetery_Packet(uint16_t bat_volt, int16_t *MotorValues, int16_t phi, int16_t theta, int16_t psi, int16_t sp, int16_t sq, int16_t sr, unsigned char _msgCode, int16_t PArray[])
{
	TpktObj.startByte = START_BYTE;
	TpktObj.type = T_DATA;
	for(uint8_t i =0;i<4;i++)
	{
		TpktObj.value[(i<<1)]=MotorValues[i]>>8;
		TpktObj.value[(i<<1)+1]=MotorValues[i]&0x00FF;
	}

	TpktObj.value[8]=bat_volt>>8;
	TpktObj.value[9]=bat_volt&0x00FF;

	TpktObj.value[10]=phi>>8;
	TpktObj.value[11]=phi&0x00FF;
	TpktObj.value[12]=theta>>8;
	TpktObj.value[13]=theta&0x00FF;
	TpktObj.value[14]=psi>>8;
	TpktObj.value[15]=psi&0x00FF;
	TpktObj.value[16]=sp>>8;
	TpktObj.value[17]=sp&0x00FF;
	TpktObj.value[18]=sq>>8;
	TpktObj.value[19]=sq&0x00FF;
	TpktObj.value[20]=sr>>8;
	TpktObj.value[21]=sr&0x00FF;
	TpktObj.value[22]=_msgCode;
	TpktObj.value[23]=PArray[0]>>8;
	TpktObj.value[24]=PArray[0]&0x00FF;
	TpktObj.value[25]=PArray[1]>>8;
	TpktObj.value[26]=PArray[1]&0x00FF;
	TpktObj.value[27]=PArray[2]>>8;
	TpktObj.value[28]=PArray[2]&0x00FF;	


	TpktObj.valueLength = 29;
	TpktObj.dataLength=Get_DataLength(TpktObj.valueLength );
	TpktObj.packetLength=1+1+TpktObj.dataLength+(CRC_BYTE_SIZE);
	Set_CRCValue(&TpktObj);
	return &TpktObj;
}

Packet *Create_MSG_Packet(char code)
{
	pktObj.startByte = START_BYTE;
	pktObj.type = T_MSG;
	pktObj.value[0] = code;
	pktObj.value[1] = code;
	pktObj.valueLength = 2;
	pktObj.dataLength=Get_DataLength(pktObj.valueLength);
	pktObj.packetLength=1+1+pktObj.dataLength+(CRC_BYTE_SIZE);
	Set_CRCValue(&pktObj);
	return &pktObj;

}

Packet *Create_HeartBeatPacket()
{
	pktObj.startByte = START_BYTE;
	pktObj.type = T_HEARTBEAT;
	pktObj.value[0] = T_HEARTBEAT;
	pktObj.valueLength = 1;
	pktObj.dataLength=Get_DataLength(pktObj.valueLength);
	pktObj.packetLength=1+1+pktObj.dataLength+(CRC_BYTE_SIZE);
	Set_CRCValue(&pktObj);
	return &pktObj;
}
