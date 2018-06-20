using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel.Drone
{
    public class Messages
    {

        public static readonly string[] MsgTable = {
    "Hi, Whatup?", //dont use this
	"I Quit!(Exit)",//1
	"Saving Myself!(Safe Mode)",//2
	"OH MY GOD! Am Panicy(Panic Mode)",//3
	"Hmmm.. K you can maneuver now (Manual Mode)",//4
	"K!.. Let us try Calibrating (Calibration Mode)",//5
	"Dude, Am done with calibration(Safe Mode)",//6
	"K..Let's Try Yaw Control(Yaw Controlled Mode)",//7
	"DISCONNECTED",//8
	"Dont over do!..I know it (Invalid Change)",//9
	"Full Control Mode", //10
	"Height Control Mode",
    "Entering Raw Mode",
    "Logging Mode Enabled",
    "Logging Mode Disabled",
    "Wireless Mode"
                };

    }
}
