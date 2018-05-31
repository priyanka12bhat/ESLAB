#include "modes.h"
#include "../messages/messages.h"


#include "in4073.h"


//Moments
uint32_t Z=0;
int32_t L=0;
int32_t M=0;
int32_t N=0;

int32_t JS_N = 0;
int32_t JS_L = 0;
int32_t JS_M = 0;
int32_t JS_Z = 0;


//Yaw Controlled Mode
int32_t yawSetPoint =0;
int16_t yawSetPoint_K = 0;
int16_t yawSetPoint_J = 0;

//Full Control Mode
int32_t pitchSetPoint =0;
int16_t pitchSetPoint_K = 0;
int16_t pitchSetPoint_J = 0;
int32_t rollSetPoint =0;
int16_t rollSetPoint_K = 0;
int16_t rollSetPoint_J = 0;
int16_t P[3] = {100,1000,100};

//Sensor Handling
// MPU wrapper
int16_t phi_offset=0, theta_offset=0, psi_offset=0;
int16_t sp_offset=0, sq_offset=0, sr_offset=0;
int16_t sax_offset=0, say_offset=0, saz_offset=0;


void Modes_Initialize()
{
	Modes[M_SAFE-1].state=Safe;
	Modes[M_SAFE-1].Mode_Initialize=&Safe_Mode_Initialize;
	Modes[M_SAFE-1].Mode_Execute=&Safe_Mode_Execute;

	Modes[M_PANIC-1].state=Panic;
	Modes[M_PANIC-1].Mode_Initialize=&Panic_Mode_Initialize;
	Modes[M_PANIC-1].Mode_Execute=&Panic_Mode_Execute;

	Modes[M_MANUAL-1].state=Manual;
	Modes[M_MANUAL-1].Mode_Initialize=&Manual_Mode_Initialize;
	Modes[M_MANUAL-1].Mode_Execute=&Manual_Mode_Execute;
	Modes[M_MANUAL-1].Input_Handler=&Manual_Mode_Input_Handler;

	Modes[M_CALIBRATION-1].state=Callibration;
	Modes[M_CALIBRATION-1].Mode_Initialize=&Callibration_Mode_Initialize;
	Modes[M_CALIBRATION-1].Mode_Execute=&Callibration_Mode_Execute;

	Modes[M_YAWCONTROL-1].state=YawControlled;
	Modes[M_YAWCONTROL-1].Mode_Initialize=&Yaw_Control_Mode_Initialize;
	Modes[M_YAWCONTROL-1].Mode_Execute=&Yaw_Control_Mode_Execute;
	Modes[M_YAWCONTROL-1].Input_Handler=&Yaw_Controlled_Mode_Input_Handler;

	Modes[M_FULLCONTROL-1].state=FullControl;
	Modes[M_FULLCONTROL-1].Mode_Initialize=&Full_Control_Mode_Initialize;
	Modes[M_FULLCONTROL-1].Mode_Execute=&Full_Control_Mode_Execute;
	Modes[M_FULLCONTROL-1].Input_Handler=&Full_Control_Mode_Input_Handler;

	Modes[M_RAWMODE-1].state=RawMode;
	Modes[M_RAWMODE-1].Mode_Initialize=&Raw_Mode_Initialize;
	Modes[M_RAWMODE-1].Mode_Execute=&Raw_Mode_Execute;



	CurrentMode = GetMode(M_SAFE);

}

/***********************************************************************

MODE SPECIFIC INTILIALIZATION FUNCTION GOES HERE
************************************************************************/

void EnterSafeMode();
void update_motors();
void SetMessage(unsigned char _msgCode);

void Safe_Mode_Initialize()
{

	EnterSafeMode();
	update_motors();
}
void Panic_Mode_Initialize(){
	SetMessage(MSG_ENTERING_PANIC_MODE);	
	int16_t ae_avg = (ae[0]+ae[1]+ae[2]+ae[3])>>2;
			ae[0] = ae[1] = ae[2] = ae[3] = ae_avg;			

}




inline void clearControlVariables();

void Manual_Mode_Initialize()
{
	SetMessage(MSG_ENTERING_MANUAL_MODE);
	if(CurrentMode.state!=Manual){

		JS_N = 0;
		JS_L = 0;
		JS_M = 0;
		JS_Z = 0;
		clearControlVariables();
	}
	

}


#define MAX_SAMPLES 128



	int32_t phi_offset_sum=0;
	int32_t theta_offset_sum=0;
	int32_t psi_offset_sum=0;
	int32_t sp_offset_sum=0;
	int32_t sq_offset_sum=0;
	int32_t sr_offset_sum=0;

void Callibration_Mode_Initialize()
{

	phi_offset_sum=0;
	theta_offset_sum=0;
	psi_offset_sum=0;
	sp_offset_sum=0;
	sq_offset_sum=0;
	sr_offset_sum=0;

	SetMessage(MSG_ENTERING_CALIBRATION_MODE);

	char count=0;
	while(check_sensor_int_flag() && count< MAX_SAMPLES) 
	{

		get_dmp_data();

		phi_offset_sum+=phi;
		theta_offset_sum+= theta;
		psi_offset_sum = psi;
		sp_offset_sum = sp;
		sq_offset_sum = sq;
		sr_offset_sum = sr;

	count++;
	}

	phi_offset=phi_offset_sum>>7;
	theta_offset=theta_offset_sum>>7;
	psi_offset=psi_offset_sum>>7;
	sp_offset=sp_offset_sum>>7;	
	sq_offset=sq_offset_sum>>7;
	sr_offset=sr_offset_sum>>7;


	SetMessage(MSG_EXITING_CALIBRATION_MODE);
	CurrentMode = GetMode(M_SAFE);

}

void update_offsets()
{
	//call only after get_dmp_data()
	phi_offset_sum = phi_offset_sum-phi_offset+phi;
	theta_offset_sum = theta_offset_sum-theta_offset+theta;
	psi_offset_sum = psi_offset_sum-psi_offset+psi;
	sp_offset_sum = sp_offset_sum-sp_offset+sp;
	sq_offset_sum = sq_offset_sum-sq_offset+sq;
	sr_offset_sum = sr_offset_sum-sr_offset+sr;

	phi_offset=phi_offset_sum>>7;
	theta_offset=theta_offset_sum>>7;
	psi_offset=psi_offset_sum>>7;
	sp_offset=sp_offset_sum>>7;	
	sq_offset=sq_offset_sum>>7;
	sr_offset=sr_offset_sum>>7;

}




void Yaw_Control_Mode_Initialize()
{
	yawSetPoint =0;
	yawSetPoint_K = 0;
	yawSetPoint_J = 0;
	clearControlVariables();
	SetMessage(MSG_ENTERING_YAWCONTROL_MODE);
}
void Full_Control_Mode_Initialize(){
	yawSetPoint = 0;
	yawSetPoint_K = 0;
	yawSetPoint_J = 0;
	pitchSetPoint = 0;
	pitchSetPoint_K = 0;
	pitchSetPoint_J = 0;
	rollSetPoint = 0;
	rollSetPoint_K = 0;
	rollSetPoint_J = 0;
	clearControlVariables();
	SetMessage(MSG_ENTERING_FULLCONTROL_MODE);
}
void Raw_Mode_Initialize(){}
void Height_Control_Mode_Initialize(){}
void Wireless_Control_Mode_Initialize(){}



/***********************************************************************

MODE SPECIFIC EXECUTION FUNCTION GOES HERE
************************************************************************/


void Safe_Mode_Execute(){}




void Panic_Mode_Execute()
{
	static 	uint32_t us_TimeStamp = 0;
	
			uint32_t us_currentTime = get_time_us();

			if(us_currentTime>us_TimeStamp?us_currentTime-us_TimeStamp>500000: (UINT32_MAX-us_TimeStamp+us_currentTime)>500000){
			//printf("%10ld | ", us_currentTime);
			//printf("%10ld | \n", us_TimeStamp);
			
			if(ae[0]>10)
			{
				ae[0]-=10;
			}
			if(ae[1]>10)
			{
				ae[1]-=10;
			}
			if(ae[2]>10)
			{
				ae[2]-=10;
			}
			if(ae[3]>10)
			{
				ae[3]-=10;
			}

			us_TimeStamp = us_currentTime;
			if(ae[0]<=10 && ae[1]<=10 && ae[2]<=10 && ae[3]<=10)
			{
				
				EnterSafeMode();
				CurrentMode = GetMode(M_SAFE);		

			}

			update_motors();
			}

}

void SetMotorValues_Manual();

void Manual_Mode_Execute()
{
	SetMotorValues_Manual();

	update_motors();

}
void Callibration_Mode_Execute(){}

void SetMotorValues();


void Yaw_Control_Mode_Execute()
{
	if (check_sensor_int_flag()) 
	{
		get_dmp_data();
		//update_offsets();		
		N = P[0]* (yawSetPoint - sr + sr_offset);
			//printf("Z:%ld|L:%ld|M:%ld|N:%ld|",Z,L,M,N);
		SetMotorValues();
		update_motors();
			//printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
	}
}
void Full_Control_Mode_Execute()
{
	if (check_sensor_int_flag()) 
		{
			get_dmp_data();	
			//update_offsets();	
			N = P[0]* (yawSetPoint - sr + sr_offset); //Yaw
			M = P[1]* (pitchSetPoint - phi) - P[2]*(sp + sp_offset); //Pitch
			L = P[1]* (rollSetPoint - phi) - P[2]*(sq + sq_offset); //Roll
				//printf("Z:%ld|L:%ld|M:%ld|N:%ld|",Z,L,M,N);
			SetMotorValues();
			update_motors();
				//printf("Motor[0]:%d,Motor[1]:%d,Motor[2]:%d,Motor[3]:%d\n",ae[0],ae[1],ae[2],ae[3]);
		}
}
void Raw_Mode_Execute(){}
void Height_Control_Mode_Execute(){}
void Wireless_Control_Mode_Execute(){}



/***********************************************************************

MODE SPECIFIC INPUT HANDLER FUNCTION GOES HERE
************************************************************************/



void Manual_Mode_Input_Handler(unsigned char *Input)
{
	switch(Input[0]){
						

						case C_LIFTUP:

							if((Z+INC_Z)<=MAX_Z){
								Z+=INC_Z;
							}						

							
						break;
						case C_LIFTDOWN:
							if(Z>=INC_Z)
							{
								Z-=INC_Z;
							}
								

						break;

						case C_ROLLUP:
							if((L+INC_L)<=MAX_L){
								L+=INC_L;
							}						

						break;

						case C_ROLLDOWN:
							if((L-INC_L)>=(-1*MAX_L))
							{
								
								L-=INC_L;
							}
								
						break;

						case C_PITCHUP:
							if((M+INC_M)<=MAX_M){
								M+=INC_M;
							}
						break;

						case C_PITCHDOWN:
							if((M-INC_M)>=(-1*MAX_M))
							{
								M-=INC_M;
							}
						break;

						case C_YAWUP:
							if((N+INC_N)<=MAX_N){
								N+=INC_N;
							}		
						break;

						case C_YAWDOWN:
							if((N-INC_N)>=(-1*MAX_N))
							{
								N-=INC_N;
							}
						break;						

						case C_JOYSTICK:

						JS_L = ((int32_t)MAX_L)*((int8_t)Input[1])/JSSCALEMAX;
						JS_M = ((int32_t)MAX_M)*((int8_t)Input[2])/JSSCALEMAX;
						JS_N = ((int32_t)MAX_N)*((int8_t)Input[3])/JSSCALEMAX;
						JS_Z = ((int32_t)MAX_Z)*(JSSCALEMAX+(int8_t)Input[4])/JSSCALEMAX/2;

						break;
						}

}

void Full_Control_Mode_Input_Handler(unsigned char *Input)
{				
					
	switch(Input[0]){
						case C_LIFTUP:

							if((Z+INC_Z)<=MAX_Z){
								Z+=INC_Z;
							}						

							
						break;
						case C_LIFTDOWN:
							if(Z>=INC_Z)
							{
								Z-=INC_Z;
							}
								
						break;

						case C_YAWUP:
							yawSetPoint_K+=10;
						break;

						case C_YAWDOWN:
							yawSetPoint_K-=10;
						break;	

						case C_PITCHUP:
							pitchSetPoint_K+=10;
						break;

						case C_PITCHDOWN:
							pitchSetPoint_K-=10;
						break;

						case C_ROLLUP:
							rollSetPoint_K+=10;
						break;

						case C_ROLLDOWN:
							rollSetPoint_K-=10;
						break;					

						case C_JOYSTICK:
							yawSetPoint_J = ((int16_t)Input[3])*100/JSSCALEMAX;
							pitchSetPoint_J = ((int16_t)Input[2])*100/JSSCALEMAX;
							rollSetPoint_J = ((int16_t)Input[1])*100/JSSCALEMAX;
							JS_Z = ((int32_t)INC_Z)*2*((int8_t)Input[4])/JSSCALEMAX;
						break;

						case C_PUP:
							P[0]+=10;
						break;

						case C_PDOWN:
							if(P[0]-10>=10){
								P[0]-=10;
							}
						break;

						case C_P1UP:
							P[1]+=10;
						break;

						case C_P1DOWN:
							if(P[1]-10>=10){
								P[1]-=10;
							}
						break;

						case C_P2UP:
							P[2]+=10;
						break;

						case C_P2DOWN:
							if(P[2]-10>=10){
								P[2]-=10;
							}
						break;
						
					}

					yawSetPoint = yawSetPoint_K + yawSetPoint_J;
					pitchSetPoint = pitchSetPoint_K + pitchSetPoint_J;
					rollSetPoint = rollSetPoint_K + rollSetPoint_J;
}


void Yaw_Controlled_Mode_Input_Handler(unsigned char *Input)
{

					
					
	switch(Input[0]){

						case C_LIFTUP:

							if((Z+INC_Z)<=MAX_Z){
								Z+=INC_Z;
							}						

							
						break;
						case C_LIFTDOWN:
							if(Z>=INC_Z)
							{
								Z-=INC_Z;
							}
								

						break;

						case C_YAWUP:

							yawSetPoint_K+=10;

						break;

						case C_YAWDOWN:
							yawSetPoint_K-=10;
						break;						

						case C_JOYSTICK:
							yawSetPoint_J = ((int16_t)Input[3])*100/JSSCALEMAX;
							JS_Z = ((int32_t)INC_Z)*2*((int8_t)Input[4])/JSSCALEMAX;

						break;

						case C_PUP:
							P[0]+=10;
						break;
						case C_PDOWN:
							if(P[0]-10>=10){
								P[0]-=10;
							}
						break;


						
					}
					yawSetPoint = yawSetPoint_K + yawSetPoint_J;

}



void SetMotorValues_Manual()
{

	if(Z+JS_Z==0)
	{
		//setting mottor values to zero in case of zero lift
		L=0;
		M=0;
		N=0;
		ae[0]=0;
		ae[1]=0;
		ae[2]=0;
		ae[3]=0;
	}
	else{

	/*
	Solving System of Equation

	Z=B_DASH(ae0^2+ae1^2+ae2^2+ae3^2)
	L=B_DASH(ae0^2-ae2^2)
	L=B_DASH(ae1^2-ae3^2)
	N*D_DASH=(ae0^2-ae1^2+ae2^2-ae3^2)
	*/
	int32_t z = (Z+JS_Z)/B_DASH;
	int32_t l = (L+JS_L)/B_DASH;
	int32_t m = (M+JS_M)/B_DASH;
	int32_t n = (N+JS_N)*D_DASH;

	int32_t ae0_2 = (m-(n>>1)+(z>>1))>>1;
	int32_t ae1_2 = ((n>>1)-l+(z>>1))>>1;
	int32_t ae2_2 = ((z>>1)-(n>>1)-m)>>1;
	int32_t ae3_2 = (l+(n>>1)+(z>>1))>>1;

	//For negative values, seting minimum as zero
	ae[0]=(uint16_t)sqrt(ae0_2<0?0:ae0_2);
	ae[1]=(uint16_t)sqrt(ae1_2<0?0:ae1_2);
	ae[2]=(uint16_t)sqrt(ae2_2<0?0:ae2_2);
	ae[3]=(uint16_t)sqrt(ae3_2<0?0:ae3_2);

	//Calculating the speed for current lift applied alone
	uint16_t minSpeed = (uint16_t)sqrt(z>>2);
	//Ensuring lift wont go below MIN_SPEED_ONFLY limit, for YAWING, ROLLING, PITCHING with higher lifts
	minSpeed = (minSpeed<MIN_SPEED_ONFLY)?minSpeed:MIN_SPEED_ONFLY;

	//Confirm Motor Values considering all limits
	ae[0]=(ae[0]<MIN_SPEED_ONFLY)?minSpeed:ae[0];
	ae[1]=(ae[1]<MIN_SPEED_ONFLY)?minSpeed:ae[1];
	ae[2]=(ae[2]<MIN_SPEED_ONFLY)?minSpeed:ae[2];
	ae[3]=(ae[3]<MIN_SPEED_ONFLY)?minSpeed:ae[3];
	}

}

void SetMotorValues()
{
	if(Z==0)
	{
		//setting mottor values to zero in case of zero lift
		L=0;
		M=0;
		N=0;
		ae[0]=0;
		ae[1]=0;
		ae[2]=0;
		ae[3]=0;
	}
	else{

	/*
	Solving System of Equation

	Z=B_DASH(ae0^2+ae1^2+ae2^2+ae3^2)
	L=B_DASH(ae0^2-ae2^2)
	L=B_DASH(ae1^2-ae3^2)
	N*D_DASH=(ae0^2-ae1^2+ae2^2-ae3^2)
	*/
	int32_t z = Z/B_DASH;
	int32_t l = L/B_DASH;
	int32_t m = M/B_DASH;
	int32_t n = N*D_DASH;

	int32_t ae0_2 = (m-(n>>1)+(z>>1))>>1;
	int32_t ae1_2 = ((n>>1)-l+(z>>1))>>1;
	int32_t ae2_2 = ((z>>1)-(n>>1)-m)>>1;
	int32_t ae3_2 = (l+(n>>1)+(z>>1))>>1;

	//For negative values, seting minimum as zero
	ae[0]=(uint16_t)sqrt(ae0_2<0?0:ae0_2);
	ae[1]=(uint16_t)sqrt(ae1_2<0?0:ae1_2);
	ae[2]=(uint16_t)sqrt(ae2_2<0?0:ae2_2);
	ae[3]=(uint16_t)sqrt(ae3_2<0?0:ae3_2);

	//Calculating the speed for current lift applied alone
	uint16_t minSpeed = (uint16_t)sqrt(z>>2);
	//Ensuring lift wont go below MIN_SPEED_ONFLY limit, for YAWING, ROLLING, PITCHING with higher lifts
	minSpeed = (minSpeed<MIN_SPEED_ONFLY)?minSpeed:MIN_SPEED_ONFLY;

	//Confirm Motor Values considering all limits
	ae[0]=(ae[0]<MIN_SPEED_ONFLY)?minSpeed:ae[0];
	ae[1]=(ae[1]<MIN_SPEED_ONFLY)?minSpeed:ae[1];
	ae[2]=(ae[2]<MIN_SPEED_ONFLY)?minSpeed:ae[2];
	ae[3]=(ae[3]<MIN_SPEED_ONFLY)?minSpeed:ae[3];
	}



}


inline void EnterSafeMode()
{
	SetMessage(MSG_ENTERING_SAFE_MODE);
	ae[0] = 0;
	ae[1] = 0;
	ae[2] = 0;
	ae[3] = 0;

}


inline void clearControlVariables()
{
	Z=0;
	L=0;
	M=0;
	N=0;
	ae[0]=0;
	ae[1]=0;
	ae[2]=0;
	ae[3]=0;
}

int16_t* GetPArray()
{
	return P;
}