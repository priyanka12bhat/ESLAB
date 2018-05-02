#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "packet.h"
#include "keyboard.h"

struct termios 	savetty;

int run = 1;
char* value_tag =NULL; 
char type_tag = 0;
Packet *pkt=NULL;

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

int term_getchar_nb()
{
        static unsigned char c;

        if (read(0,&c,1)) // note: destructive read
        		return (int) c;

        return -1;
}

void ExitSafe(void)
{
	//implement 
	run = 0;
}

void kb_input_handler(char c)
{

			switch(c)
			{

				case ZERO:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_SAFE;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);

					term_puts("changed mode to safe mode\n");
					break;

				case ONE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_PANIC;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case TWO:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_MANUAL;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case THREE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_CALIBRATION;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case FOUR:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_YAWCONTROL;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case FIVE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_FULLCONTROL;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case SIX:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_RAWMODE;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case SEVEN:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_HEIGHTCONTROL;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case EIGHT:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_WIRELESS;
					type_tag = T_MODE;
					pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case 'a'://lift up
					//Create Packet Tag:Control+Value
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_LIFTUP;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("lift up\n");
				break;

				case 'z'://lift down
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_LIFTDOWN;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("lift down\n");

				break;
				case 'q'://Yaw down
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_YAWDOWN;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("Yaw down\n");

				break;
				case 'w'://Yaw up
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_YAWUP;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("Yaw up\n");

				break;

				case 'u'://Yaw control P up

					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_PUP;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("Yaw control P up\n");

				break;

				case 'j'://Yaw control P down
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_PDOWN;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("Yaw control P down\n");
				break;

				case 'i':// P1 up
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_P1UP;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("P1 up\n");
				break;

				case 'k':// P1 down
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_P1DOWN;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("P1 down\n");
				break;
				case 'o':// P2 up
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_P2UP;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("P2 up\n");

				break;
				case 'l':// P2 down
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = C_P2DOWN;
					type_tag = T_CONTROL;
					pkt = Create_Packet(type_tag,1,value_tag);
					printf("P2 down\n");
				break;




				case '\033':
					if ((c = term_getchar_nb())!=-1)
					{
							switch(c = term_getchar_nb()){  
								case 'D'://Left Arrow - RollUp
									value_tag = (char *)malloc(sizeof(char)*1);
									*value_tag = C_ROLLUP;
									type_tag = T_CONTROL;
									pkt = Create_Packet(type_tag,1,value_tag);
									printf("RollUp\n");
								break;
								case 'C'://Right Arrow - RollDown
									value_tag = (char *)malloc(sizeof(char)*1);
									*value_tag = C_ROLLDOWN;
									type_tag = T_CONTROL;
									pkt = Create_Packet(type_tag,1,value_tag);								
									printf("RollDown\n");
								break;
								case 'A'://Up Arrow - PitchDown
									value_tag = (char *)malloc(sizeof(char)*1);
									*value_tag = C_PITCHDOWN;
									type_tag = T_CONTROL;
									pkt = Create_Packet(type_tag,1,value_tag);		
									printf("PitchDown\n");
								break;
								case 'B'://Arrow Down - PitchUP
									value_tag = (char *)malloc(sizeof(char)*1);
									*value_tag = C_PITCHUP;
									type_tag = T_CONTROL;
									pkt = Create_Packet(type_tag,1,value_tag);									
									printf("PitchUP\n");
								break;
							}

					}
					else
					{
						printf("Exiting....");
						value_tag = (char *)malloc(sizeof(char)*1);
						*value_tag = M_PANIC;
						type_tag = T_MODE;
						pkt = Create_Packet(type_tag,1,value_tag);

						ExitSafe();


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


int main(void)
{
	term_puts("\nTerminal program - Embedded Real-Time Systems\n");

	term_initio();
	printf("Start\n");
	int c  = 0;

	while(run)
	{
	//printf("Loop\n");
		if((c=term_getchar_nb())!=-1)
		{
			kb_input_handler(c);

			if(pkt!=NULL)
			{

					char *packetByteStream = Get_Byte_Stream(pkt);
					//Send Packet bytes through RS232
					printf("Testing- Type:%d\n",pkt->type);
					printf("Testing- startbyte:%d\n",pkt->startByte);
					printf("Testing- datalength:%d\n",pkt->dataLength);
					printf("Testing- value length:%d\n",pkt->valueLength);
					printf("Testing- value:%d\n",*(pkt->value));
					printf("Testing- CRC:%d\n",*(pkt->CRC));
					Destroy_Packet(pkt);
					free(packetByteStream);
					pkt=NULL;			


			}
		}

	}


	term_exitio();
	return 0;

}
