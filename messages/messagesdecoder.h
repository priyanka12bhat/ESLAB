#ifndef messages_decoder
#define messages_decoder
#include "messages.h"

//Msgs
const char MsgTable[255][255]=
{
	"Hi, Whatup?", //dont use this
	"I Quit!(Exit)",//1
	"Saving Myself!(Safe Mode)",//2
	"OH MY GOD! Am Panicy(Panic Mode)",//3
	"Hmmm.. So Drive Me (Manual Mode)",//4
	"K!.. Let us try Calibrating (Calibration Mode)",//5
	"Dude, Am done with calibration",//6
	"K..Let's Try Yaw Control(Yaw Controlled Mode)",//7
	"DISCONNECTED",//8
	"F**k You.. Dont over do!..I know it (Invalid Change)"//9

};


#define DecodeMessage(x) MsgTable[x]




#endif
