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


//#include "joystick.h"


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

int run = 1;
unsigned char* value_tag = NULL;
unsigned char type_tag = 0;
Packet *pkt = NULL;

uint32_t JSLastReadTimeStamp = 0;
int serial_device = 0;
int fd_RS232;

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
	tty.c_cc[VTIME] = 1; // added timeout

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

void kb_input_handler(unsigned char c)
{
	switch (c)
	{
	case ZERO:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_SAFE;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);

		term_puts("Switching mode to safe mode\n");
		break;

	case ONE:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_PANIC;
		type_tag = T_MODE;

		pkt = Create_Packet(type_tag, 1, value_tag);
		term_puts("Switching mode to panic mode\n");
		break;

	case TWO:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_MANUAL;
		type_tag = T_MODE;
		term_puts("Switching mode to manual mode\n");
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case THREE:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_CALIBRATION;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case FOUR:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_YAWCONTROL;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case FIVE:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_FULLCONTROL;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case SIX:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_RAWMODE;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case SEVEN:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_HEIGHTCONTROL;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case EIGHT:
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = M_WIRELESS;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		break;

	case 'a'://lift up
			 //Create Packet Tag:Control+Value
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_LIFTUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("lift up\n");
		break;

	case 'z'://lift down
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_LIFTDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("lift down\n");

		break;
	case 'q'://Yaw down
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_YAWDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("Yaw down\n");

		break;
	case 'w'://Yaw up
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_YAWUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("Yaw up\n");

		break;

	case 'u'://Yaw control P up

		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_PUP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("Yaw control P up\n");

		break;

	case 'j'://Yaw control P down
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_PDOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("Yaw control P down\n");
		break;

	case 'i':// P1 up
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P1UP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P1 up\n");
		break;

	case 'k':// P1 down
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P1DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P1 down\n");
		break;
	case 'o':// P2 up
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P2UP;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P2 up\n");

		break;
	case 'l':// P2 down
		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P2 down\n");

		break;

		case 't':// P2 down
		/*value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P2 down\n");*/

		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 9);
		*value_tag = C_JOYSTICK;
		value_tag[1] = -100;
		value_tag[2] = 12;
		value_tag[3] = 80;
		value_tag[4] = -52;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 5, value_tag);
		printf("JS SIMULATION TESTING\n");
		break;

		case 'y':// P2 down
		/*value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		*value_tag = C_P2DOWN;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 1, value_tag);
		printf("P2 down\n");*/

		value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 9);
		*value_tag = C_JOYSTICK;
		value_tag[1] = 100;
		value_tag[2] = -12;
		value_tag[3] = -80;
		value_tag[4] = 52;
		type_tag = T_CONTROL;
		pkt = Create_Packet(type_tag, 5, value_tag);
		printf("JS SIMULATION TESTING\n");
		break;
	case 27:
	printf("%i\n",c );
		if ((c = term_getchar_nb()) != -1)
		{
			switch (c = term_getchar_nb()) {
			case 'D'://Left Arrow - RollUp
				value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
				*value_tag = C_ROLLUP;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				printf("RollUp\n");
				break;
			case 'C'://Right Arrow - RollDown
				value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
				*value_tag = C_ROLLDOWN;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				printf("RollDown\n");
				break;
			case 'A'://Up Arrow - PitchDown
				value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
				*value_tag = C_PITCHDOWN;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				printf("PitchDown\n");
				break;
			case 'B'://Arrow Down - PitchUP
				value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
				*value_tag = C_PITCHUP;
				type_tag = T_CONTROL;
				pkt = Create_Packet(type_tag, 1, value_tag);
				printf("PitchUP\n");



				break;
			default:
				printf("Exiting....\n");
				value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
				*value_tag = M_PANIC;
				type_tag = T_MODE;
				pkt = Create_Packet(type_tag, 1, value_tag);
				ExitSafe();
				break;

			}
		}
		break;

		//case 27:
		//break;
	default:
		printf("Invalid Control Input\n");
		//invalid_input=1;
		break;
	}
}

void Send_Packet(Packet *pkt)
{
	unsigned char *packetByteStream = Get_Byte_Stream(pkt);
	/*printf("Packet1\n");

	rs232_putchar(1);
	rs232_putchar(1);
	rs232_putchar(255);
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==0 || i==1)
			continue;
		rs232_putchar(packetByteStream[i]);
		printf("Sending:%d\n",packetByteStream[i]);
	}


	printf("Packet2\n");
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==2 || i==1)
			continue;
		rs232_putchar(packetByteStream[i]);
		printf("Sending:%d\n",packetByteStream[i]);
	}

	printf("Packet3\n");
	for(int i=0;i<pkt->packetLength;i++)
	{
		if(i==3 || i==4 || i ==5)
			continue;
		rs232_putchar(packetByteStream[i]);
		printf("Sending:%d\n",packetByteStream[i]);
	}

	printf("Packet4\n");

	rs232_putchar(255);*/

	for(int i=0;i<pkt->packetLength;i++)
	{
		rs232_putchar(packetByteStream[i]);
		//printf("Sending:%d\n",packetByteStream[i]);
	}






	free(packetByteStream);
}

/* Joystick Initialization
*/
void joystick_init(int *fd)
{

    if ((*fd = open(JS_DEV, O_RDONLY)) < 0)
    {
        perror("jstest");
    }
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
js_command *read_js(int* fd, int axis[], int button[])
{
	struct js_event js;
	js_command *js_c;
	/* check up on JS
	*/
	while (read(*fd, &js, sizeof(struct js_event)) ==
		sizeof(struct js_event)) {

		/* register data
		*/
		// fprintf(stderr,".");
		switch (js.type & ~JS_EVENT_INIT) {
		case JS_EVENT_BUTTON:
			button[js.number] = js.value;
			break;
		case JS_EVENT_AXIS:
			axis[js.number] = js.value;
			break;
		}
	
		if((js.time - JSLastReadTimeStamp) <JS_READ_GAP)
		{
			return NULL;
		}
		JSLastReadTimeStamp=js.time;
	
		js_c= (js_command *)malloc(sizeof(js_command));


		if (errno != EAGAIN)
		{
			perror("\njs: error reading (EAGAIN)");
			exit(1);
		}

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
		if (axis[0])
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
		}
	}

	return js_c;
}


void Create_jsPacket(js_command* js_comm)
{

	if(js_comm->Type == T_MODE)
	{
			
		//value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 1);
		//*value_tag = js_comm->Mode;
		//type_tag = T_MODE;

		//pkt = Create_Packet(type_tag, 1, value_tag);
	}
	else
	{
		if(js_comm->Mode == T_CONTROL)
		{
			value_tag = (unsigned char *)malloc(sizeof(unsigned char) * 5);
			*value_tag = C_JOYSTICK;

			value_tag[1] = (char)((int32_t)js_comm->Roll*100/32767);
			value_tag[2] = (char)((int32_t)js_comm->Pitch*100/32767);
			value_tag[3] = (char)((int32_t)js_comm->Yaw*100/32767);
			value_tag[4] = (char)((int32_t)js_comm->Lift*100/32767);			
			type_tag = T_CONTROL;
			pkt = Create_Packet(type_tag, 5, value_tag);	
		}
	}
	free(js_comm);
	js_comm = NULL;
}






/*----------------------------------------------------------------
 * main -- execute terminal
 *----------------------------------------------------------------
 */
int main(int argc, char **argv)
{
	int axis[6];
	int button[12];
	int fd = 0;

	js_command *js_comm;
		
	/* communication initialization
	*/
	int c = 0;

	term_puts("\nTerminal program - Embedded Real-Time Systems\n");

	term_initio();
	rs232_open();

	term_puts("Type ^C to exit\n");

	/* discard any incoming text
	*/
	while ((c = rs232_getchar_nb()) != -1)
		fputc(c, stderr);

	/* joystick initialization
	*/
	//joystick_init(&fd);

	/* send & receive
	 */
	while (run)
	{
		/* read joystick inputs
		*/
		/*		
		js_comm = read_js(&fd, axis, button);
		if (js_comm != NULL){

			 Create_jsPacket(js_comm);
		
		
			if (pkt != NULL)
			{
				//Send Packet bytes through RS232
				Send_Packet(pkt);
				Destroy_Packet(pkt);
				pkt = NULL;
			}
		}
		*/
		if ((c = term_getchar_nb()) != -1)
		{
			//rs232_putchar(c);
			kb_input_handler(c);
			if (pkt != NULL)
			{
				//Send Packet bytes through RS232
				Send_Packet(pkt);
				/*printf("Testing- Type:%d\n", pkt->type);
				printf("Testing- startbyte:%d\n", pkt->startByte);
				printf("Testing- datalength:%d\n", pkt->dataLength);
				printf("Testing- value length:%d\n", pkt->valueLength);
				printf("Testing- value:%d\n", *(pkt->value));
				printf("Testing- CRC0:%d\n", *(pkt->CRC));
				printf("Testing- CRC1:%d\n", pkt->CRC[1]);*/
				Destroy_Packet(pkt);
				pkt = NULL;
			}
		}

		if((c = rs232_getchar_nb()) != -1)
			term_putchar(c);

	}
	term_exitio();
	rs232_close();
	term_puts("\n<exit>\n");
	return 0;
}
