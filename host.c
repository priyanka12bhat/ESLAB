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
}


int main(void)
{
	term_puts("\nTerminal program - Embedded Real-Time Systems\n");

	term_initio();
	printf("Start\n");
	int c  = 0;
	char* value_tag =NULL; 
	char type_tag = 0;;
	char invalid_input=0;
	while(run)
	{
	//printf("Loop\n");
		if((c=term_getchar_nb())!=-1)
		{

			switch(c)
			{

				case ZERO:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_SAFE;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);

					term_puts("changed mode to safe mode\n");
					break;

				case ONE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_PANIC;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case TWO:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_MANUAL;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case THREE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_CALIBRATION;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case FOUR:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_YAWCONTROL;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case FIVE:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_FULLCONTROL;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case SIX:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_RAWMODE;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case SEVEN:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_HEIGHTCONTROL;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case EIGHT:
					value_tag = (char *)malloc(sizeof(char)*1);
					*value_tag = M_WIRELESS;
					type_tag = T_MODE;
					Packet *pkt = Create_Packet(type_tag,1,value_tag);
					break;

				case 'a'://lift up
					//Create Packet Tag:Control+Value
					printf("a\n");
				break;

				case 'z'://lift down

				break;
				case 'q'://Yaw down

				break;
				case 'w'://Yaw up

				break;

				case 'u'://Yaw control P up

				break;
				case 'j'://Yaw control P down
				break;
				case 'i':// P1 up

				break;
				case 'k':// P1 down
				break;
				case 'o':// P2 up

				break;
				case 'l':// P2 down
				break;




				case '\033':
					if ((c = term_getchar_nb())!=-1)
					{
							switch(c = term_getchar_nb()){  
								case 'D'://Left Arrow - RollUp
									printf("RollUp\n");
								break;
								case 'C'://Right Arrow - RollDown
									printf("RollDown\n");
								break;
								case 'A'://Up Arrow - PitchDown
									printf("PitchDown\n");
								break;
								case 'B'://Arrow Down - PitchUP
									printf("PitchUP\n");
								break;
							}

					}
					else
					{
						printf("Exiting....");
						ExitSafe();
						run = 0;
					}
				break;


				//case 27:

				//break;

				default:
				printf("Invalid Control Input\n");
				invalid_input=1;
				break;
			}
			if(!invalid_input)
			{

					char *packetByteStream = Get_Byte_Stream(pkt);
					//Send Packet bytes through RS232
					printf("Testing:%c",pkt->type);
					Destroy_Packet(pkt);
					free(packetByteStream);			


			}
			else
			{
				invalid_input =0;
			}
		}

	}


	term_exitio();
	return 0;

}
