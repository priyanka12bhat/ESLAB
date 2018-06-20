using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel.Input
{

    public class JSWrapper
    {
        /// <summary>
        /// Returns an Array of 6 elements
        /// </summary>
        /// <returns>a[0]=ROLL, a[1] = PITCH, a[2]=YAW, a[3]=LIFT, a[4]=KILLBUTTON, a[5]=VALIDREADING </returns>
        [DllImport("JoyStick.dll")]
        private static extern IntPtr ReadJoystick();

        [DllImport("JoyStick.dll")]
        public static extern void array_release(IntPtr ptr);

        public static JSData ReadJS()
        {
            JSData data = null;
            IntPtr aP = ReadJoystick();
            int[] a = new int[6];
            Marshal.Copy(aP, a, 0, 6);
            //array_release(aP);
            if (a[5] == 1)
            {
                JSData.VALIDREAD = true;
                data = new JSData();
                JSData.ROLL = a[0] - 32767;
                JSData.PITCH = a[1] - 32767;
                JSData.YAW = a[2] - 32767;
                JSData.LIFT = a[3]-32767;
                JSData.KILL = (a[4] == 1) ? true : false;

            }
            else
            {
                JSData.VALIDREAD = false;
            }

            return data;

        }

        public static Packet GetJSPacket()
        {
            Packet pkt = null;
            ReadJS();
            if (JSData.VALIDREAD)
            {
                if (JSData.KILL == true)
                {
                    pkt = Packet.Create_Packet(Packet.T_EXIT, 1, new byte[] { Packet.M_SAFE });
                }
                else
                {
                    byte[] values = new byte[] { Packet.C_JOYSTICK, ((byte)((JSData.ROLL* Packet.JSSCALEMAX)/32767)), ((byte)((JSData.PITCH * Packet.JSSCALEMAX) / 32767)), ((byte)((-JSData.YAW * Packet.JSSCALEMAX) / 32767)), ((byte)((JSData.LIFT * -Packet.JSSCALEMAX) / 32767))};
                    pkt = Packet.Create_Packet(Packet.T_CONTROL, 5, values);
                }

            }
            return pkt;
        }

    }
}
