/*
Steps:
	1. Initialize joystick and communication
	2. Read inputs from joystick and keyboard
	3. Create packet
	4. Send packet
*/
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

//#include "joystick.h"

uint32_t packetCount=0;
/*------------------------------------------------------------
 * console I/O
 *------------------------------------------------------------
 */
struct termios 	savetty;

void	term_initio()
{
	struct termios tty;

	tcgetattr(0, &savetty);
	tcgetattr(0, &tty);

	tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;

	tcsetattr(0, TCSADRAIN, &tty);
}

void	term_exitio()
{
	tcsetattr(0, TCSADRAIN, &savetty);
}

void	term_puts(char *s)
{
	fprintf(stderr,"%s",s);
}

void	term_putchar(char c)
{
	putc(c,stderr);
}

int	term_getchar_nb()
{
        static unsigned char 	line [2];

        if (read(0,line,1)) // note: destructive read
        		return (int) line[0];

        return -1;
}

int	term_getchar()
{
        int    c;

        while ((c = term_getchar_nb()) == -1)
                ;
        return c;
}



/*------------------------------------------------------------
 * Serial I/O
 * 8 bits, 1 stopbit, no parity,
 * 115,200 baud
 *------------------------------------------------------------
 */
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "../protocol/packet.h"
#include "keyboard.h"
#include "joystick.h"
#include "../protocol/receivepacket.h"
#include "../messages/messagesdecoder.h"

#define clearUI() printf("\033[H\033[J")
#define setCursor(x, y) printf("\033[%d;%dH", x, y)
int run = 1;
//unsigned char* value_tag = NULL;
//unsigned char longValueArray[10];
//unsigned char shortValueArray[1];

unsigned char type_tag = 0;
FILE *f = NULL;
Packet *pkt = NULL;
Packet *pkt_K = NULL;
int axis[6];
int button[12];

unsigned int KBLastReadTimeStamp = 0;
unsigned int  HBLastSendTimeStamp = 0;
int CheckReadGap(unsigned int lastSendTime, char times);
int serial_device = 0;
int fd_RS232;

char msgToPrint[512];
char additionalMessage[16+5];
char msgCode = 0;
void storeUIMessage(const char msg[]);
void printUIMessage();

void rs232_open(void)
{
  	char 		*name;
  	int 		result;
  	struct termios	tty;
       	fd_RS232 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY); // Hardcode your serial port here, or request it as an argument at runtime

	assert(fd_RS232>=0);

  	result = isatty(fd_RS232);
  	assert(result == 1);

  	name = ttyname(fd_RS232);
  	assert(name != 0);

  	result = tcgetattr(fd_RS232, &tty);
	assert(result == 0);

	tty.c_iflag = IGNBRK; /* ignore break condition */
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 0; // added timeout

	tty.c_iflag &= ~(IXON|IXOFF|IXANY);

	result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */

	tcflush(fd_RS232, TCIOFLUSH); /* flush I/O buffer */
}


void 	rs232_close(void)
{
  	int 	result;

  	result = close(fd_RS232);
  	assert (result==0);
}


int	rs232_getchar_nb()
{
	int 		result;
	unsigned char 	c;

	result = read(fd_RS232, &c, 1);

	if (result == 0)
		return -1;

	else
	{
		assert(result == 1);
		return (int) c;
	}
}


int 	rs232_getchar()
{
	int 	c;
	while ((c = rs232_getchar_nb()) == -1)
		;
	return c;
}


int 	rs232_putchar(char c)
{
	int result;

	do {
		result = (int) write(fd_RS232, &c, 1);
	} while (result == 0);

	assert(result == 1);
	return result;
}

void ExitSafe(void)
{
	//implement 
	run = 0;


	
}

void CreatePacketWithSafetyCheck(const char* switchMessage,unsigned char type_tag,unsigned char length, unsigned char *value_tag);
void kb_input_handler(unsigned char c)
{
	unsigned char value_tag[1];
	switch (c)
	{
	case ZERO:
		

		*value_tag = M_SAFE;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);

		storeUIMessage("Switching mode to safe mode\n\0");
		break;

	case ONE:
		
		*value_tag = M_PANIC;
		type_tag = T_MODE;

		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Switching mode to panic mode\n\0");
		break;

	case TWO:
		
		*value_tag = M_MANUAL;
		type_tag = T_MODE;		

		CreatePacketWithSafetyCheck("Switching mode to manual mode\n\0",type_tag,1,value_tag);
		break;

	case THREE:
		
		*value_tag = M_CALIBRATION;
		type_tag = T_MODE;

		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Requesting calibration mode\n\0");
		break;

	case FOUR:
		
		
		*value_tag = M_YAWCONTROL;
		type_tag = T_MODE;



		CreatePacketWithSafetyCheck("Switching to Yaw Conrolled mode\n\0",type_tag,1,value_tag);




		break;

	case FIVE:
		
		*value_tag = M_FULLCONTROL;
		type_tag = T_MODE;

		CreatePacketWithSafetyCheck("Switching to Full Conrolled mode\n\0",type_tag,1,value_tag);
		
		break;

	case SIX:
		
		*value_tag = M_RAWMODE;
		type_tag = T_MODE;
		CreatePacketWithSafetyCheck("Switching to RAW mode\n\0",type_tag,1,value_tag);
		break;

	case SEVEN:
		
		*value_tag = M_HEIGHTCONTROL;
		type_tag = T_MODE;
		CreatePacketWithSafetyCheck("Toggling Height Controlled mode\n\0",type_tag,1,value_tag);
		break;

	case EIGHT:
		
		*value_tag = M_WIRELESS;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case 'a'://lift up
			 //Create Packet Tag:Control+Value
		
		*value_tag = C_LIFTUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("lift up\n\0");
		break;

	case 'z'://lift down
		
		*value_tag = C_LIFTDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("lift down\n\0");

		break;
	case 'w'://Yaw down
		
		*value_tag = C_YAWDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Yaw down\n\0");

		break;
	case 'q'://Yaw up
		
		*value_tag = C_YAWUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Yaw up\n\0");

		break;

	case 'u'://Yaw control P up

		
		*value_tag = C_PUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Yaw control P up\n\0");

		break;

	case 'j'://Yaw control P down
		
		*value_tag = C_PDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("Yaw control P down\n\0");
		break;

	case 'i':// P1 up
		
		*value_tag = C_P1UP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P1 up\n\0");
		break;

	case 'k':// P1 down
		
		*value_tag = C_P1DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P1 down\n\0");
		break;
	case 'o':// P2 up
		
		*value_tag = C_P2UP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P2 up\n\0");

		break;
	case 'l':// P2 down
		
		*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P2 down\n\0");

		break;

	case 'm': //logging enable

		*value_tag = C_LOGGING;
		type_tag = T_CONFIG;
		CreatePacketWithSafetyCheck("Toggling Logging\n\0",type_tag,1,value_tag);
		break;
		/*case 't':// P2 down
				*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P2 down\n");

		unsigned char value_tag[5];
		*value_tag = C_JOYSTICK;
		value_tag[1] = -100;
		value_tag[2] = 12;
		value_tag[3] = 80;
		value_tag[4] = -52;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 5, value_tag);
		storeUIMessage("JS SIMULATION TESTING\n");
		break;*/

		/*case 'y':// P2 down
		
		*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		storeUIMessage("P2 down\n");

		unsigned char value_tag[5];
		*value_tag = C_JOYSTICK;
		value_tag[1] = 100;
		value_tag[2] = -12;
		value_tag[3] = -80;
		value_tag[4] = 52;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 5, value_tag);
		storeUIMessage("JS SIMULATION TESTING\n");
		break;*/
	case 27:
	//storeUIMessage("%i\n",c );
		if ((c = term_getchar_nb()) != -1)
		{
			switch (c = term_getchar_nb()) {
			case 'C'://Right Arrow - RollDown
				
				*value_tag = C_ROLLUP;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				storeUIMessage("RollUp\n\0");
				break;
			case 'D'://Left Arrow - RollUp
				
				*value_tag = C_ROLLDOWN;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				storeUIMessage("RollDown\n\0");
				break;
			case 'B'://Arrow Down - PitchUP
				
				*value_tag = C_PITCHDOWN;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				storeUIMessage("PitchDown\n\0");
				break;
			case 'A'://Up Arrow - PitchDown
				
				*value_tag = C_PITCHUP;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				storeUIMessage("PitchUP\n\0");



				break;
			default:

				storeUIMessage("Exiting....\n\0");
				
				*value_tag = M_SAFE;
				type_tag = T_EXIT;
				pkt = Create_Packet(type_tag, 1, value_tag);

				ExitSafe();
				break;

			}
		}
		break;

		//case 27:
		//break;
	default:
		storeUIMessage("Invalid Control Input\n\0");
		//invalid_input=1;
		break;
	}
}

void CreatePacketWithSafetyCheck(const char* switchMessage,unsigned char type_tag,unsigned char length, unsigned char *value_tag)
{

		if(((axis[0]==0) && (axis[1]==0) && (axis[2]==0) &&(axis[3]==32767)) || 1 ){
		pkt = Create_Packet(type_tag, 1, value_tag);

		storeUIMessage(switchMessage);
		}
		else
		{
			storeUIMessage("Zero Joystick\n\0");
			pkt=NULL;
			//free(value_tag);
		}

}

void Send_Packet(Packet *pkt)
{
	unsigned char *packetByteStream = Get_Byte_Stream(pkt);
	//printf("Packet1\n");

	/*rs232_putchar(1);
	rs232_putchar(1);
	rs232_putchar(255);
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==0 || i==1)
			continue;
		rs232_putchar(packetByteStream[i]);
		//printf("Sending:%d\n",packetByteStream[i]);
	}


	//printf("Packet2\n");
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==2 || i==1)
			continue;
		rs232_putchar(packetByteStream[i]);
		//printf("Sending:%d\n",packetByteStream[i]);
	}

	//printf("Packet3\n");
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==3 || i==4 || i ==5)
			continue;
		rs232_putchar(packetByteStream[i]);
		//printf("Sending:%d\n",packetByteStream[i]);
	}

	//printf("Packet4\n");

	rs232_putchar(255);*/

	for(int i=0;i<pkt->packetLength;i++)
	{
		rs232_putchar(packetByteStream[i]);
		//printf("Sending:%d\n",packetByteStream[i]);
	}
	packetCount++;
	//printf("PacketSend:%d\n",packetCount);

}

/* Joystick Initialization
*/
//Mingxi LI
void joystick_init(int *fd)
{

    if ((*fd = open(JS_DEV, O_RDONLY)) < 0)
    {
        perror("jstest");
        exit(1);
    }

    	unsigned char axes = 2;

    unsigned char buttons = 2;
	int version = 0x000800;
	char name[NAME_LENGTH] = "Unknown";

    ioctl(*fd, JSIOCGVERSION, &version);
	ioctl(*fd, JSIOCGAXES, &axes);
	ioctl(*fd, JSIOCGBUTTONS, &buttons);
	ioctl(*fd, JSIOCGNAME(NAME_LENGTH), name);

	printf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
		name, axes, buttons, version >> 16, (version >> 8) & 0xff, version & 0xff);
	


    // non-blocking mode
    fcntl(*fd, F_SETFL, O_NONBLOCK);



}


/* time
 */
unsigned int    mon_time_ms(void)
{
        unsigned int    ms;
        struct timeval  tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        ms = 1000 * (tv.tv_sec % 65); // 65 sec wrap around
        ms = ms + tv.tv_usec / 1000;
        return ms;
}

void    mon_delay_ms(unsigned int ms)
{
        struct timespec req, rem;

        req.tv_sec = ms / 1000;
        req.tv_nsec = 1000000 * (ms % 1000);
        assert(nanosleep(&req,&rem) == 0);
}


/*
	Description: 
		Read inputs from joystick
	Inputs:
		* fd, axis, button
	Outputs:
		a js_command
*/


//Mingxi LI
void read_values(int* fd, int axis[], int button[])
{
	struct js_event js;
	while (read(*fd, &js, sizeof(struct js_event)) == sizeof(struct js_event)) {


		//printf("%d\n",js.time);
		//printf("%d\n",js.value);
		//printf("%d\n",js.type);
		//printf("%d\n",js.number);

		/* register data
		*/
		// fprintf(stderr,".");
		switch (js.type & ~JS_EVENT_INIT) {

		case JS_EVENT_BUTTON:
			//printf("%d\n",js.value);
			button[js.number] = js.value;
			break;
		case JS_EVENT_AXIS:
			//printf("%d\n",js.value);
			axis[js.number] = js.value;
			break;
		}
	}
}

js_command *read_js_simulator()
{
	js_command *js_c = NULL;



		js_c= (js_command *)malloc(sizeof(js_command));
		js_c->Type = T_CONTROL;
		js_c->Roll = 0;
		js_c->Pitch = 0;
		js_c->Yaw = 0;
		js_c->Lift = 22767;
	return js_c;
}

js_command js_command_obj;

//Mingxi LI
js_command *read_js(int* fd, int axis[], int button[])
{
	js_command *js_c = NULL;
	/* check up on JS

	*/


		read_values(fd,axis,button);
		//printf("%d\n",js.time);
		/*
		if((js.time - JSLastReadTimeStamp) <JS_READ_GAP)
		{
			return NULL;
		}
		JSLastReadTimeStamp=js.time;
		*/
		


		// Switch the mode
		/*if (button[0])
			js_c->Type = T_MODE;
			js_c->Mode = M_SAFE;
		if (button[1])
			js_c->Type = T_MODE;
			js_c->Mode = M_PANIC;
		if (button[2])
			js_c->Type = T_MODE;
			js_c->Mode = M_MANUAL;
		if (button[3])
			js_c->Type = T_MODE;
			js_c->Mode = M_CALIBRATION;
		if (button[4])
			js_c->Type = T_MODE;
			js_c->Mode = M_YAWCONTROL;
		if (button[5])
			js_c->Type = T_MODE;
			js_c->Mode = M_FULLCONTROL;
		if (button[6])
			js_c->Type = T_MODE;
			js_c->Mode = M_RAWMODE;
		if (button[7])
			js_c->Type = T_MODE;
			js_c->Mode = M_HEIGHTCONTROL;
		if (button[8])
			js_c->Type = T_MODE;
			js_c->Mode = M_WIRELESS;*/						

		// roll
		/*if (axis[0])
		{
			js_c->Type = T_CONTROL;
			js_c->Roll = axis[0];
		}

		// pitch
		if (axis[1])
		{
			js_c->Type = T_CONTROL;
			js_c->Pitch = axis[1];
		}

		// yaw
		if (axis[2])
		{
			js_c->Type = T_CONTROL;
			js_c->Yaw = axis[2];
		}

		// lift
		if (axis[3])
		{
			js_c->Type = T_CONTROL;
			js_c->Lift = axis[3];
		}*/

	
		js_c= &js_command_obj;
		js_c->Type = T_CONTROL;
		js_c->Roll = axis[0];
		js_c->Pitch = axis[1];
		js_c->Yaw = axis[2];
		js_c->Lift = axis[3];


		if (button[0]){
			js_c->Type = T_EXIT;
			js_c->Mode = M_SAFE;
		}
		//printf("%d\n",js_c->Type);
		//printf("%d\n",js_c->Roll);
		//printf("%d\n",js_c->Pitch);
		//printf("%d\n",js_c->Yaw);	
		//printf("%d\n",js_c->Lift);


	

	if ((errno != EAGAIN) && (errno != 0 ))
		{
			//printf("abcd%d\n",errno);
			perror("\njs: error reading (EAGAIN)");
			exit(1);
		}

	return js_c;
}

void js_safety_check(int* fd, int axis[], int button[])
{
	for(int i = 0;i<20;i++)
	{
		read_values(fd,axis,button);
	}
	
	if((axis[0]!=0 )|(axis[1]!=0)|(axis[2]!=0)|(axis[3]!=0))
	{
		printf("Place jostick in neutral position and Try Again\n");
	}
	exit(0);
}

//Mingxi LI
void Create_jsPacket(js_command* js_comm)
{

	//printf("%d\n",js_comm->Type);
	//printf("%d\n",js_comm->Roll);
	//printf("%d\n",js_comm->Pitch);
	//printf("%d\n",js_comm->Yaw);	
	//printf("%d\n",js_comm->Lift);

	if(js_comm->Type == T_EXIT)
	{
			
				printf("Exiting....\n");
				unsigned char value_tag[1];
				*value_tag = M_SAFE;
				type_tag = T_EXIT;
				pkt = Create_Packet(type_tag, 1, value_tag);

				ExitSafe();
	}
	else
	{
		if(js_comm->Type == T_CONTROL)
		{

			unsigned char value_tag[5];
			*value_tag = C_JOYSTICK;

			value_tag[1] = (char)((int32_t)js_comm->Roll*100/32767);
			value_tag[2] = (char)((int32_t)js_comm->Pitch*100/32767);
			value_tag[3] = (char)((int32_t)js_comm->Yaw*100/32767);
			value_tag[4] = (char)((int32_t)js_comm->Lift*-100/32767);			
			type_tag = T_CONTROL;
			pkt = Create_Packet(type_tag, 5, value_tag);	
		}
	}
}






/*----------------------------------------------------------------
 * main -- execute terminal
 *----------------------------------------------------------------
 */
int main(int argc, char **argv)
{
	
	int fd = 0;
	int c = 0;
	unsigned int lastJSSendTime = 0;

	js_command *js_comm;
		
	/* communication initialization
	*/


	term_puts("\nTerminal program - Embedded Real-Time Systems\n");

	term_initio();

	Reception_Init(LP_SIZE);
	rs232_open();

	term_puts("Type ^C to exit\n");

	/* discard any incoming text
	*/
	while ((c = rs232_getchar_nb()) != -1)
		fputc(c, stderr);

	/* joystick initialization
	*/
	//joystick_init(&fd);

	//js_safety_check(&fd,axis,button);

	/* send & receive
	 */
	while (run)
	{
		/* read joystick inputs
		*/
		//printf("T1:%d\n",mon_time_ms());
		//js_comm = read_js(&fd, axis, button);
		js_comm = read_js_simulator();
		
		if (js_comm != NULL) {
			if(CheckReadGap(lastJSSendTime,1)){
			//printf("js_comm not null\n");

				 Create_jsPacket(js_comm);
				 //printf("packet created\n");
			
				if (pkt != NULL)
				{
					//printf("pkt not null\n");
					//Send Packet bytes through RS232
					Send_Packet(pkt);
					//printf("pkt send\n");
					//Destroy_Packet(pkt);
					//printf("pkt destroyed\n");
					//free(value_tag);
					pkt = NULL;
				}
				lastJSSendTime = mon_time_ms();
				//printf("%d\n",lastJSSendTime);
			}
			//free(js_comm);
		}
		//printf("T2:%d\n",mon_time_ms());
		if ((c = term_getchar_nb()) != -1)
		{
			//rs232_putchar(c);
			kb_input_handler(c);
			if (pkt != NULL)
			{

				if(CheckReadGap(KBLastReadTimeStamp,1)){
					//Send Packet bytes through RS232
					Send_Packet(pkt);
					/*printf("Testing- Type:%d\n", pkt->type);
					printf("Testing- lastJSSendTimebyte:%d\n", pkt->lastJSSendTimeByte);
					printf("Testing- datalength:%d\n", pkt->dataLength);
					printf("Testing- value length:%d\n", pkt->valueLength);
					printf("Testing- value:%d\n", *(pkt->value));
					printf("Testing- CRC0:%d\n", *(pkt->CRC));
					printf("Testing- CRC1:%d\n", pkt->CRC[1]);*/
					//Destroy_Packet(pkt);
					//free(value_tag);
					
					KBLastReadTimeStamp = mon_time_ms();

				}
				
				pkt = NULL;
			}
		}
		//printf("T3:%d\n",mon_time_ms());

		if (checkCount()){  //continuously check for new elements in the UART queue

			unsigned char currentByte = readData();

			stateHandler(currentByte);
		}
		//printf("T4:%d\n",mon_time_ms());

		//if ((c = rs232_getchar_nb()) != -1)
			//term_putchar(c);

		if((CheckReadGap(lastJSSendTime,3) && CheckReadGap(KBLastReadTimeStamp,1))?CheckReadGap(HBLastSendTimeStamp,2):0)
		{
			pkt=Create_HeartBeatPacket();
			Send_Packet(pkt);
			pkt=NULL;
			HBLastSendTimeStamp=mon_time_ms();


		}

		printUIMessage();

	}


	/*for (;;)
	{

		if ((c = rs232_getchar_nb()) != -1)
			term_putchar(c);
		if(c==27)
		{
			break;
		}

	}*/

	//storeUIMessage("\nFCB Exited\n\0");



	term_exitio();
	rs232_close();

	storeUIMessage("Exiting Host Program\n\0");
	return 0;
}

int CheckReadGap(unsigned int lastSendTime,char times)
{
	unsigned int currentTime = mon_time_ms();

	return ((currentTime==lastSendTime)?0:((currentTime>lastSendTime)?((currentTime - lastSendTime) >= JS_READ_GAP*times):((UINT_MAX-lastSendTime+currentTime)>=JS_READ_GAP*times)));

}

char getElementFromInputQueue()
{
	//term_putchar(c);
	return 0;
}

uint16_t getInputQueueCount()
{
	int c;
	if(((c = rs232_getchar_nb()) != -1))
	{


		addNode(c);
		return 1;

	}
	return 0;
}


void storeUIMessage(const char msg[])
{
	strcpy(msgToPrint,msg);

}

void printUIMessage()
{
	setCursor(15, 0);
	printf("LastAction:%s\n",msgToPrint);
	printf("Drone Say's:%s\n",DecodeMessage(msgCode));
	printf("\nAdditional Debug Messages:%s\n",additionalMessage);
}


void process_packet(Packet *pkt_R)
{	
	if (pkt_R->type == T_FLASHMEM){
	clearUI();
	printf("Saving flash memory to file, goodbye");
	if (f == NULL) {
		f = fopen("file.txt", "a");
	}
	    fprintf(f,"\t%ld\t",(((uint32_t)pkt_R->value[9])<<24|((uint32_t)pkt_R->value[8])<<16|((uint32_t)pkt_R->value[7])<<8|(uint32_t)pkt_R->value[6]));
	    fprintf(f,"\t%d\t",(int16_t)(((uint16_t)pkt_R->value[0])<<8|pkt_R->value[1]));
	    fprintf(f,"\t%d\t",(int16_t)(((uint16_t)pkt_R->value[2])<<8|pkt_R->value[3]));
	    fprintf(f,"\t%d\t",(int16_t)(((uint16_t)pkt_R->value[4])<<8|pkt_R->value[5]));
	    fprintf(f,"\t%d\t",(int16_t)(((uint16_t)pkt_R->value[10])<<8|pkt_R->value[11]));
	    fprintf(f,"\t%d\t",(int16_t)(((uint16_t)pkt_R->value[12])<<8|pkt_R->value[13]));
	    fprintf(f,"\t%d\n",(int16_t)(((uint16_t)pkt_R->value[14])<<8|pkt_R->value[15]));
	    if (pkt_R->value[22]==11){
	    	fclose(f);
	    	f = NULL;
	    }
	}

	if(pkt_R->type==T_DATA){
		clearUI();
		printf("=========================================================================================================\n");
		setCursor(2, 45);
		printf("Quadruple Control\n");
		printf("=========================================================================================================\n");
		uint16_t * uint16Values =  (uint16_t *)pkt_R->value;

		printf("Battery Voltage:\t%d\n",uint16Values[4]);
		
		

		for(int i=0;i<4;i++){
			printf("Motor[%d]:\t%d\t",i,uint16Values[i]);
		}
	    puts("\n");
	    if (pkt_R->value[22] == MSG_ENTERING_RAWCONTROL_MODE){
			printf("ax:\t%d\t",(((uint16_t)pkt_R->value[0])<<8|pkt_R->value[1]));
	    	printf("ay:\t%d\t",(((uint16_t)pkt_R->value[2])<<8|pkt_R->value[3]));
	    	printf("az:\t%d\t",(((uint16_t)pkt_R->value[4])<<8|pkt_R->value[5]));
	    }
	    else {
	    	printf("phi:\t%d\t",(int16_t)uint16Values[5]);
	   		printf("theta:\t%d\t",(int16_t)uint16Values[6]);
	    	printf("psi:\t%d\n",(int16_t)uint16Values[7]);
	    }
	    
	    printf("sp:\t%d\t",(int16_t)uint16Values[8]);
	    printf("sq:\t%d\t",(int16_t)uint16Values[9]);
	    printf("sr:\t%d\n",(int16_t)uint16Values[10]);
	    uint16Values =  (uint16_t *)&(pkt_R->value[23]);
	    printf("P:\t%d\t",uint16Values[0]);
	    printf("P1:\t%d\t",uint16Values[1]);
	    printf("P2:\t%d\n",uint16Values[2]);

	    uint32_t pressure = (((uint32_t)pkt_R->value[31])<<16)|(((uint32_t)pkt_R->value[32])<<8)|(pkt_R->value[33]);
	    printf("Pressure: \t%d\n",pressure);
	    printf("Current Drone Mode: \t%d\n",pkt_R->value[34]);
	  
	   
	   	/*for(int i=0;i<4;i++){
			printf("Motor[%d]:\t%d\t",i,((uint16_t)pkt_R->value[2*i])<<8|pkt_R->value[2*i+1]);
		}
	    puts("\n");
	    printf("phi:\t%d\t",(int16_t)(((uint16_t)pkt_R->value[10])<<8|pkt_R->value[11]));
	    printf("theta:\t%d\t",(int16_t)(((uint16_t)pkt_R->value[12])<<8|pkt_R->value[13]));
	    printf("psi:\t%d\n",(int16_t)(((uint16_t)pkt_R->value[14])<<8|pkt_R->value[15]));

	    printf("sp:\t%d\t",(int16_t)(((uint16_t)pkt_R->value[16])<<8|pkt_R->value[17]));
	    printf("sq:\t%d\t",(int16_t)(((uint16_t)pkt_R->value[18])<<8|pkt_R->value[19]));
	    printf("sr:\t%d\n",(int16_t)(((uint16_t)pkt_R->value[20])<<8|pkt_R->value[21]));
	    printf("P:\t%d\t",(((uint16_t)pkt_R->value[23])<<8|pkt_R->value[24]));
	    printf("P1:\t%d\t",(((uint16_t)pkt_R->value[25])<<8|pkt_R->value[26]));
	    printf("P2:\t%d\t",(((uint16_t)pkt_R->value[27])<<8|pkt_R->value[28]));*/
		//mvprintw (0, 0, "%d", pkt_R->value[0]);     
		//refresh ();

		msgCode = pkt_R->value[22]; 

		printUIMessage();


	}
	else if(pkt_R->type==T_adMSG)
	{
		 strncpy(additionalMessage,pkt_R->value,15);
		 additionalMessage[15]='\0';
	}
	
}
