#include joystick.h
#include "../packet.h"

void js_init(int* fd)
{
	unsigned char axes = 2;
	unsigned char buttons = 2;
	int version = 0x000800;
	char name[NAME_LENGTH] = "Unknown";

	if ((*fd = open(JS_DEV, O_RDONLY)) < 0)
	{
		perror("jyinit");
		exit(1);
	}

	ioctl(*fd, JSIOCGVERSION, &version);
	ioctl(*fd, JSIOCGAXES, &axes);
	ioctl(*fd, JSIOCGBUTTONS, &buttons);
	ioctl(*fd, JSIOCGNAME(NAME_LENGTH), name);

	printf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
		name, axes, buttons, version >> 16, (version >> 8) & 0xff, version & 0xff);
	printf("Testing ... (interrupt to exit)\n");

	/* non-blocking mode
	*/
	fcntl(*fd, F_SETFL, O_NONBLOCK);
}

/*
Description:
If we get some inputs from the joystick, read the command.
*/
void read_js(int* fd)
{
	struct js_event js;

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
	}

	if (errno != EAGAIN)
	{
		perror("\njs: error reading (EAGAIN)");
		exit(1);
	}

	// Switch the mode
	if (button[0])
		mode = M_SAFE;
	if (button[1])
		mode = M_PANIC;
	if (button[2])
		mode = M_MANUAL;
	if (button[3])
		mode = M_CALIBRATION;
	if (button[4])
		mode = M_YAWCONTROL;
	if (button[5])
		mode = M_FULLCONTROL;
	if (button[6])
		mode = M_RAWMODE;
	if (button[7])
		mode = M_HEIGHTCONTROL;
	if (button[8])
		mode = M_WIRELESS;

	// Actions


}