#ifndef _packet
#define _packet

#define START_BYTE 255

//Tag Names
#define T_MODE 1
#define T_CONTROL 2
#define T_DATA 3

//Mode Types
#define M_SAFE 1
#define M_PANIC 2
#define M_MANUAL 3
#define M_CALIBRATION 4
#define M_YAWCONTROL 5
#define M_FULLCONTROL 6
#define M_RAWMODE 7
#define M_HEIGHTCONTROL 8
#define M_WIRELESS 9

//Control Types
#define C_LIFTUP 10
#define C_LIFTDOWN 11
#define C_ROLLUP 12
#define C_ROLLDOWN 13
#define C_PITCHUP 14
#define C_PITCHDOWN 15
#define C_YAWUP 16
#define C_YAWDOWN 17
#define C_PUP 18
#define C_PDOWN 19
#define C_P1UP 20
#define C_P1DOWN 21
#define C_P2UP 22
#define C_P2DOWN 23

#define CRC_BIT_SIZE 16

typedef struct packet
{
	char startByte;
	char dataLength;
	char type;
	char *value;
	char valueLength;
	char *CRC;
}Packet;

char Get_DataLength(char length);
char *Get_CRCValue(Packet *pkt);
char *Get_Byte_Stream(Packet *pkt);
Packet *Create_Packet(char type,char length, char *value);
void Destroy_Packet(Packet *pkt);

#endif
