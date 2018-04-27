#include keyboard.h
#include "../packet.h"

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