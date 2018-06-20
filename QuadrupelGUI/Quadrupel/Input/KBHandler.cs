using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Quadrupel
{
    /// <summary>
    ///  Keyboard Inputs Handler
    /// </summary>
    class KBHandler
    {
        private static Stopwatch stopwatch = new Stopwatch();
        public static string Message = "";
        /// <summary>
        /// Handler to handle Key Presses
        /// </summary>
        /// <param name="keyData"> Pressed Key Data </param>
        /// <returns>Packet to Send to Drone</returns>
        public static Packet kb_CMDKey_handler(Keys keyData) {
            byte[] value_tag = new byte[1];
            byte type_tag;
            Packet pkt = null;
            if (stopwatch.IsRunning && (stopwatch.ElapsedMilliseconds < 40))
            {
                return null;
            }
            stopwatch.Restart();

            switch (keyData)
            {
                case Keys.Right:
                    value_tag[0] = Packet.C_ROLLUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("RollUp\n\0");
                    break;

                case Keys.Left:
                    value_tag[0] = Packet.C_ROLLDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("RollDown\n\0");
                    break;
                case Keys.Down:
                    value_tag[0] = Packet.C_PITCHDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("PitchDown\n\0");
                    break;

                case Keys.Up:
                    value_tag[0] = Packet.C_PITCHUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("PitchUP\n\0");
                    break;
                case Keys.D0:
                case Keys.NumPad0:


                    value_tag[0] = Packet.M_SAFE;
                    type_tag = Packet.T_MODE;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                    storeUIMessage("Switching mode to safe mode\n\0");
                    break;

                case Keys.D1:
                case Keys.NumPad1:

                    value_tag[0] = Packet.M_PANIC;
                    type_tag = Packet.T_MODE;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Switching mode to panic mode\n\0");
                    break;


                case Keys.D2:
                case Keys.NumPad2:

                    value_tag[0] = Packet.M_MANUAL;
                    type_tag = Packet.T_MODE;
                    pkt= CreatePacketWithSafetyCheck("Switching mode to manual mode\n\0", type_tag, 1, value_tag);                   
                    break;

                case Keys.D3:
                case Keys.NumPad3:

                    value_tag[0] = Packet.M_CALIBRATION;
                    type_tag = Packet.T_MODE;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Requesting calibration mode\n\0");
                    break;

                case Keys.D4:
                case Keys.NumPad4:


                    value_tag[0] = Packet.M_YAWCONTROL;
                    type_tag = Packet.T_MODE;



                    pkt = CreatePacketWithSafetyCheck("Switching to Yaw Conrolled mode\n\0", type_tag, 1, value_tag);




                    break;
                case Keys.D5:
                case Keys.NumPad5:

                    value_tag[0] = Packet.M_FULLCONTROL;
                    type_tag = Packet.T_MODE;

                    pkt = CreatePacketWithSafetyCheck("Switching to Full Conrolled mode\n\0", type_tag, 1, value_tag);

                    break;

                case Keys.D6:
                case Keys.NumPad6:

                    value_tag[0] = Packet.M_RAWMODE;
                    type_tag = Packet.T_MODE;
                    pkt = CreatePacketWithSafetyCheck("Switching to RAW mode\n\0", type_tag, 1, value_tag);
                    break;

                case Keys.D7:
                case Keys.NumPad7:

                    value_tag[0] = Packet.M_HEIGHTCONTROL;
                    type_tag = Packet.T_MODE;
                    pkt = CreatePacketWithSafetyCheck("Toggling Height Controlled mode\n\0", type_tag, 1, value_tag);
                    break;

                case Keys.D8:
                case Keys.NumPad8:

                    value_tag[0] = Packet.M_WIRELESS;
                    type_tag = Packet.T_MODE;
                    pkt = pkt = CreatePacketWithSafetyCheck("Switching to Wireless mode\n\0", type_tag, 1, value_tag);
                    break;

                case Keys.A://lift up
                            //Create Packet Tag:Control+Value

                    value_tag[0] = Packet.C_LIFTUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("lift up\n\0");
                    break;

                case Keys.Z://lift down

                    value_tag[0] = Packet.C_LIFTDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("lift down\n\0");

                    break;


                case Keys.W://Yaw down

                    value_tag[0] = Packet.C_YAWDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw down\n\0");

                    break;
                case Keys.Q://Yaw up

                    value_tag[0] = Packet.C_YAWUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw up\n\0");

                    break;

                case Keys.U://Yaw control P up


                    value_tag[0] = Packet.C_PUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw control P up\n\0");

                    break;

                case Keys.J://Yaw control P down

                    value_tag[0] = Packet.C_PDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw control P down\n\0");
                    break;

                case Keys.I:// P1 up

                    value_tag[0] = Packet.C_P1UP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P1 up\n\0");
                    break;

                case Keys.K:// P1 down

                    value_tag[0] = Packet.C_P1DOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P1 down\n\0");
                    break;
                case Keys.O:// P2 up

                    value_tag[0] = Packet.C_P2UP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P2 up\n\0");

                    break;
                case Keys.L:// P2 down

                    value_tag[0] = Packet.C_P2DOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P2 down\n\0");

                    break;

                case Keys.V:

                    value_tag[0] = Packet.C_PHUP;
                    type_tag = Packet.T_CONTROL;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Switching mode to panic mode\n\0");
                    break;

                case Keys.B:

                    value_tag[0] = Packet.C_PHDOWN;
                    type_tag = Packet.T_CONTROL;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Switching mode to panic mode\n\0");
                    break;
                case Keys.M: //logging enable

                    value_tag[0] = Packet.C_LOGGING;
                    type_tag = Packet.T_CONFIG;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Toggling Logging");
                    

                    break;

                case Keys.Escape:


                    storeUIMessage("Exiting....\n\0");

                    value_tag[0] = Packet.M_SAFE;
                    type_tag = Packet.T_EXIT;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                    //ExitSafe();



                    break;

                //case 27:
                //break;
                default:
                    storeUIMessage("Invalid Control Input\n\0");
                    //invalid_input=1;
                    break;

            }


            /*if ((c = terPacket.M_getchar_nb()) != -1)
                  {
                      switch (c = terPacket.M_getchar_nb())
                      {
                          case 'D'://Left Arrow - RollUp

                             
                              break;
                          case 'C'://Right Arrow - RollDown

                              
                              break;
                          case 'A'://Up Arrow - PitchDown

                              
                          case 'B'://Arrow Down - PitchUP

                              



                              break;
                          default:
      break;*/

            return pkt;
        }




        /*public static Packet kb_ASCIIKey_Handler(char c)
        {
            byte[] value_tag = new byte[1];
            byte type_tag;
            Packet pkt = null;

            if (stopwatch.IsRunning && (stopwatch.ElapsedMilliseconds < 40))
            {
                return null;
            }
            stopwatch.Restart();


            switch (c)
            {
                case '0':


                    value_tag[0] = Packet.M_SAFE;
                    type_tag = Packet.T_MODE;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                    storeUIMessage("Switching mode to safe mode\n\0");
                    break;

                case '1':

                    value_tag[0] = Packet.M_PANIC;
                    type_tag = Packet.T_MODE;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Switching mode to panic mode\n\0");
                    break;

                case '2':

                    value_tag[0] = Packet.M_MANUAL;
                    type_tag = Packet.T_MODE;

                    if (((JSData.axis[0] == 0) && (JSData.axis[1] == 0) && (JSData.axis[2] == 0) && (JSData.axis[3] == 32767)) || true)
                    {
                        pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                        storeUIMessage("Switching mode to manual mode\n\0");
                    }
                    else
                    {
                        storeUIMessage("Zero Joystick\n\0");
                        pkt = null;
                        //free(value_tag);
                    }
                    break;

                case '3':

                    value_tag[0] = Packet.M_CALIBRATION;
                    type_tag = Packet.T_MODE;

                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Requesting calibration mode\n\0");
                    break;

                case '4':


                    value_tag[0] = Packet.M_YAWCONTROL;
                    type_tag = Packet.T_MODE;

                    if (((JSData.axis[0] == 0) && (JSData.axis[1] == 0) && (JSData.axis[2] == 0) && (JSData.axis[3] == 32767)) || true)
                    {
                        pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                        storeUIMessage("Switching to Yaw Conrolled mode\n\0");
                    }
                    else
                    {
                        storeUIMessage("Zero Joystick\n\0");
                        pkt = null;
                        //free(value_tag);
                    }




                    break;

                case '5':

                    value_tag[0] = Packet.M_FULLCONTROL;
                    type_tag = Packet.T_MODE;


                    if (((JSData.axis[0] == 0) && (JSData.axis[1] == 0) && (JSData.axis[2] == 0) && (JSData.axis[3] == 32767)) || true)
                    {
                        pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                        storeUIMessage("Switching to Full Conrolled mode\n\0");
                    }
                    else
                    {
                        storeUIMessage("Zero Joystick\n\0");
                        pkt = null;
                        //free(value_tag);
                    }
                    break;

                case '6':

                    value_tag[0] = Packet.M_RAWMODE;
                    type_tag = Packet.T_MODE;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    break;

                case '7':

                    value_tag[0] = Packet.M_HEIGHTCONTROL;
                    type_tag = Packet.T_MODE;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    break;

                case '8':

                    value_tag[0] = Packet.M_WIRELESS;
                    type_tag = Packet.T_MODE;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    break;

                case 'a'://lift up
                         //Create Packet Tag:Control+Value

                    value_tag[0] = Packet.C_LIFTUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("lift up\n\0");
                    break;

                case 'z'://lift down

                    value_tag[0] = Packet.C_LIFTDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("lift down\n\0");

                    break;
                case 'q'://Yaw down

                    value_tag[0] = Packet.C_YAWDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw down\n\0");

                    break;
                case 'w'://Yaw up

                    value_tag[0] = Packet.C_YAWUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw up\n\0");

                    break;

                case 'u'://Yaw control P up


                    value_tag[0] = Packet.C_PUP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw control P up\n\0");

                    break;

                case 'j'://Yaw control P down

                    value_tag[0] = Packet.C_PDOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("Yaw control P down\n\0");
                    break;

                case 'i':// P1 up

                    value_tag[0] = Packet.C_P1UP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P1 up\n\0");
                    break;

                case 'k':// P1 down

                    value_tag[0] = Packet.C_P1DOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P1 down\n\0");
                    break;
                case 'o':// P2 up

                    value_tag[0] = Packet.C_P2UP;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P2 up\n\0");

                    break;
                case 'l':// P2 down

                    value_tag[0] = Packet.C_P2DOWN;
                    type_tag = Packet.T_CONTROL;
                    pkt = Packet.Create_Packet(type_tag, 1, value_tag);
                    storeUIMessage("P2 down\n\0");

                    break;

                
                case (char)27:
                   

                                storeUIMessage("Exiting....\n\0");

                                value_tag[0] = Packet.M_SAFE;
                                type_tag = Packet.T_EXIT;
                                pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                                //ExitSafe();


                       
                    break;

                //case 27:
                //break;
                default:
                    storeUIMessage("Invalid Control Input\n\0");
                    //invalid_input=1;
                    break;
            }

            return pkt;
        }*/







        public static Packet CreatePacketWithSafetyCheck(string switchMessage, byte type_tag, byte length, byte[] value_tag)
        {
            Packet pkt = null;
            if (((JSData.ROLL== 0) && (JSData.PITCH == 0) && (JSData.YAW == 0) && (JSData.LIFT >= 32766)) || true)
            {
                pkt = Packet.Create_Packet(type_tag, 1, value_tag);

                storeUIMessage(switchMessage);
            }
            else
            {
                storeUIMessage("Zero Joystick\n\0");
                pkt = null;
                //free(value_tag);
            }

            return pkt;
        }

        static void storeUIMessage(String msg) {
            KBHandler.Message=msg;
        }

    }
    

   

}
