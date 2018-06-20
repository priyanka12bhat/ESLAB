using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel
{
    /// <summary>
    /// Class to store information about Drone Modes.
    /// </summary>
    public class Modes
    {
        /// <summary>
        /// Represents various states drones can be in
        /// </summary>
        public enum DroneStates
        { Safe = 0, Panic = 1, Manual = 2, Callibration = 3, YawControlled = 4, FullControl = 5, RawMode = 6, HeightControl = 7, Wireless = 8, Exiting = 9 }



    }
}
