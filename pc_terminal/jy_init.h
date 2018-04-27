#ifndef _JY_INIT
#define _JY_INIT

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "joystick.h"

#define NAME_LENGTH 128
#define JS_DEV	"/dev/input/js0"

void jyInit(int* fd)
#endif /* _JY_INIT */