using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel
{
    public class TelemetryData
    {
        public UInt16 BatteryVoltage = 0;
        public int[] Motor = new int[4];

        public int[] DataArray = new int[9];
        public Int16 sp;
        public Int16 sq;
        public Int16 sr;

        public Int16 phi;
        public Int16 theta;
        public Int16 psi;

        public UInt16[] P = new UInt16[3];
        public UInt16 P_H = 1;
        public byte droneMessageCode = 0;

        public UInt32 pressure = 0;

        public Modes.DroneStates currentDroneMode;
    }
}
