#ifndef modes_drones_
#define modes_drones_

#include "../protocol/packet.h"
#define GetMode(x) Modes[x-1]


typedef enum _DroneState {Safe=M_SAFE-1,Panic=M_PANIC-1,Manual=M_MANUAL-1,Callibration=M_CALIBRATION-1, YawControlled=M_YAWCONTROL-1, FullControl=M_FULLCONTROL-1, RawMode=M_RAWMODE-1, HeightControl=M_HEIGHTCONTROL-1,Wireless=M_WIRELESS-1,Exiting=10-1} DroneState;

typedef struct droneMode
{
	DroneState state;
	void (*Mode_Initialize)();
	void (*Mode_Execute)();
	void (*Input_Handler)(unsigned char *Input);

}Mode;

Mode Modes[9];

Mode CurrentMode;



void Modes_Initialize();

void Safe_Mode_Initialize();
void Panic_Mode_Initialize();
void Manual_Mode_Initialize();
void Callibration_Mode_Initialize();
void Yaw_Control_Mode_Initialize();
void Full_Control_Mode_Initialize();
void Raw_Mode_Initialize();
void Height_Control_Mode_Initialize();
void Wireless_Control_Mode_Initialize();


void Safe_Mode_Execute();
void Panic_Mode_Execute();
void Manual_Mode_Execute();
void Callibration_Mode_Execute();
void Yaw_Control_Mode_Execute();
void Full_Control_Mode_Execute();
void Raw_Mode_Execute();
void Height_Control_Mode_Execute();
void Wireless_Control_Mode_Execute();




void Manual_Mode_Input_Handler(unsigned char *Input);
void Yaw_Controlled_Mode_Input_Handler(unsigned char *Input);
void Full_Control_Mode_Input_Handler(unsigned char *Input);
void Raw_Control_Mode_Input_Handler(unsigned char *Input);

int16_t* GetPArray();
void SendAdditionalMessage( char* msgfmt, ...);


#endif
