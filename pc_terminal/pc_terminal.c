/*
Steps:
	1. Initialize joystick and communication
	2. Read inputs from joystick and keyboard
	3. Create packet (todo)
	4. Send packet (todo)
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "../protocol/packet.h"
#include "keyboard.h"
#include "joystick.h"
#include "Variable.h"

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

int serial_device = 0;
int fd_RS232;

void rs232_open(void)
{
  	char 		*name;
  	int 		result;
  	struct termios	tty;
       	fd_RS232 = open("/dev/ttyS2", O_RDWR | O_NOCTTY);  // Hardcode your serial port here, or request it as an argument at runtime

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


void kb_input_handler(char pressed_key)
{
	switch (pressed_key)
	{
	case ESC:
		mode = M_PANIC;
		break;

	case ZERO:
		mode = M_SAFE;
		term_puts("changed mode to safe mode\n");
		break;

	case ONE:
		mode = M_PANIC;
		break;

	case TWO:
		mode = M_MANUAL;
		break;

	case THREE:
		mode = M_CALIBRATION;
		break;

	case FOUR:
		mode = M_YAWCONTROL;
		break;

	case FIVE:
		mode = M_FULLCONTROL;
		break;

	case SIX:
		mode = M_RAWMODE;
		break;

	case SEVEN:
		mode = M_HEIGHTCONTROL;
		break;

	case EIGHT:
		mode = M_WIRELESS;
		break;
	}
}


/*----------------------------------------------------------------
 * main -- execute terminal
 *----------------------------------------------------------------
 */
int main(int argc, char **argv)
{
	/* communication initialization
	*/
	char	c;

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
	js_init(&fd);

	/* send & receive
	 */
	for (;;)
	{
		/* read joystick
		*/
		read_js(&fd, &axis, &button);

		/* read keyboard
		*/
		if ((c = term_getchar_nb()) != -1)
			kb_input_handler(c);

		/* create packet and send packet to the board
		*/

	}

	term_exitio();
	rs232_close();
	term_puts("\n<exit>\n");

	return 0;
}

