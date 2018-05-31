#ifndef packet__
#define packet__

#include <stdint.h>

#define START_BYTE 255

//packet Tag Names
#define T_MODE 1
#define T_CONTROL 2
#define T_DATA 3
#define T_CONFIG 4
#define T_EXIT 5
#define T_MSG 6
#define T_HEARTBEAT 7 

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

//Control Joystick Types
//#define CJ_LIFT 24

//#define CJ_ROLL 26

//#define CJ_PITCH 28

//#define CJ_YAW 30
#define C_JOYSTICK 24

#define JSSCALEMAX 100


#define CRC_BIT_SIZE 16
#define CRC_BYTE_SIZE CRC_BIT_SIZE/8

#define MAX_MSG_SIZE 8
#define P_SIZE MAX_MSG_SIZE
#define LP_SIZE 40
#define JS_READ_GAP 40
#define DISCONNECTED_GAP_US JS_READ_GAP*1000*100
#define TELE_SEND_GAP_MS 100
#define TELE_SEND_GAP_US TELE_SEND_GAP_MS*1000

typedef struct packet
{
	unsigned char startByte;
	unsigned char packetLength;
	unsigned char dataLength;
	unsigned char type;
	unsigned char value[LP_SIZE];
	unsigned char valueLength;
	unsigned char CRC[CRC_BYTE_SIZE];
}Packet;

unsigned char Get_DataLength(unsigned char length);
//unsigned char *Get_CRCValue(Packet *pkt);
unsigned char *Get_Byte_Stream(Packet *pkt);
Packet *Create_Packet(unsigned char type,unsigned char length, unsigned char *value);
void Destroy_Packet(Packet *pkt);
void Set_CRCValue(Packet *pkt);

Packet *Create_Telemetery_Packet(uint16_t bat_volt, int16_t *MotorValues, int16_t phi, int16_t theta, int16_t psi, int16_t sp, int16_t sq, int16_t sr, unsigned char _msgCode, int16_t PArray[]);

Packet *Create_MSG_Packet(char code);

Packet *Create_HeartBeatPacket();



#endif
