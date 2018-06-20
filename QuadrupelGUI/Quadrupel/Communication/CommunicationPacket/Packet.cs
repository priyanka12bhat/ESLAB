using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel
{
    /// <summary>
    /// Helper class for packet creation - Tobe maintained in sync with the Drone Side C Code
    /// </summary>
    public class Packet
    {
        //Constants
        //Types of Packets supported 
        public static readonly byte START_BYTE = 255;
        public static readonly byte T_MODE = 1;
        public static readonly byte T_CONTROL = 2;
        public static readonly byte T_DATA = 3;
        public static readonly byte T_CONFIG = 4;
        public static readonly byte T_EXIT = 5;
        public static readonly byte T_adMSG = 6;
        public static readonly byte T_HEARTBEAT = 7;
        public static readonly byte T_FLASHMEM = 8;
        public static readonly ushort LP_SIZE=40;


        //Mode Types
        public static readonly byte M_SAFE = 1;
        public static readonly byte M_PANIC = 2;
        public static readonly byte M_MANUAL = 3;
        public static readonly byte M_CALIBRATION = 4;
        public static readonly byte M_YAWCONTROL = 5;
        public static readonly byte M_FULLCONTROL = 6;
        public static readonly byte M_RAWMODE = 7;
        public static readonly byte M_HEIGHTCONTROL = 8;
        public static readonly byte M_WIRELESS = 9;

        //Control Types
        public static readonly byte C_LIFTUP = 10;
        public static readonly byte C_LIFTDOWN = 11;
        public static readonly byte C_ROLLUP = 12;
        public static readonly byte C_ROLLDOWN = 13;
        public static readonly byte C_PITCHUP = 14;
        public static readonly byte C_PITCHDOWN = 15;
        public static readonly byte C_YAWUP = 16;
        public static readonly byte C_YAWDOWN = 17;
        public static readonly byte C_PUP = 18;
        public static readonly byte C_PDOWN = 19;
        public static readonly byte C_P1UP = 20;
        public static readonly byte C_P1DOWN = 21;
        public static readonly byte C_P2UP = 22;
        public static readonly byte C_P2DOWN = 23;
        public static readonly byte C_JOYSTICK = 24;
        public static readonly byte C_PHDOWN = 26;
        public static readonly byte C_PHUP = 25;
        public static readonly byte C_LOGGING = 27;
        public static readonly byte JSSCALEMAX = 100;


        /// <summary>
        /// Packet Structure - Members
        /// </summary>
        public byte startByte;
        public byte packetLength;
        public byte dataLength;
        public byte type;
        public byte[] value= new byte[Packet.LP_SIZE];
        public byte valueLength;
        public byte[] CRC = new byte[2];

        

        /// <summary>
        /// Method to Get object in the Packet Structure
        /// </summary>
        /// <param name="type">Type of Packet</param>
        /// <param name="length">length of elements in the value array</param>
        /// <param name="value">value array</param>
        /// <returns>Supplied data as an object of packet</returns>
        internal static Packet Create_Packet(byte type, byte length, byte[] value)
        {
            Packet pktObj = new Packet();

            pktObj.startByte = START_BYTE;
            pktObj.type = type;
            for (int i = 0; i < length; i++)
            {
                pktObj.value[i] = value[i];
            }

            pktObj.valueLength = length;
            pktObj.dataLength = Get_DataLength(length);
            pktObj.packetLength = (byte)(1 + 1 + pktObj.dataLength + 2);
            Set_CRCValue(ref pktObj);
            return pktObj;
        }



        /// <summary>
        /// Get the total length of data that is transported
        /// </summary>
        /// <param name="length"></param>
        /// <returns>data length of the packet</returns>
        internal static byte Get_DataLength(byte length) //length is the length of value array
        {
            return (byte)(length+1);
        }

        /// <summary>
        /// Set CRC Bits of a supplied packet
        /// </summary>
        /// <param name="pkt">Packet for which CRC Values Should be set</param>
        private static void Set_CRCValue(ref Packet pkt)
        {

            byte[] DataCRC = new byte[Packet.LP_SIZE + 5];
            DataCRC[0] = pkt.type;
            for (int i = 0; i < pkt.valueLength; i++)
            {
                DataCRC[i + 1] = pkt.value[i];
            }
            UInt16 CRC = crc16_compute(DataCRC, pkt.dataLength);
            pkt.CRC[1] = (byte)(CRC & 0x00FF);
            pkt.CRC[0] = (byte)((CRC & 0xFF00) >> 8);

        }

        /// <summary>
        /// Function to compute CRC values for a data array
        /// </summary>
        /// <param name="p_data">data array</param>
        /// <param name="size">length of the array</param>
        /// <returns>CRC Value</returns>
        private static UInt16 crc16_compute(byte[] p_data, byte size)
        {
            UInt32 i;
            UInt16 crc = 0xffff;

            for (i = 0; i < size; i++)
            {
                crc = (UInt16)((byte)(crc >> 8) | (crc << 8));
                crc ^= p_data[i];
                crc ^= (UInt16)((byte)(crc & 0xff) >> 4);
                crc ^= (UInt16)((crc << 8) << 4);
                crc ^= (UInt16)(((crc & 0xff) << 4) << 1);
            }


            return crc;
        }

        /// <summary>
        /// Method to destory Packets if its needed
        /// </summary>
        /// <param name="pkt"></param>
        static internal void Destroy_Packet(ref Packet pkt)
        {
        }

        /// <summary>
        /// Get serialized byte stream of a packet to send
        /// </summary>
        /// <param name="pkt">Packet</param>
        /// <returns>serialized packet</returns>
        static internal byte[] Get_Byte_Stream(ref Packet pkt)
        {
            byte[] serialPacket = new byte[Packet.LP_SIZE + 10];
            serialPacket[0] = pkt.startByte;
            serialPacket[1] = pkt.dataLength;
            serialPacket[2] = pkt.type;
            for (int i = 0; i < pkt.valueLength; i++)
            {
                serialPacket[3 + i] = pkt.value[i];
            }
            serialPacket[pkt.packetLength - 1] = pkt.CRC[1];
            serialPacket[pkt.packetLength - 2] = pkt.CRC[0];
            serialPacket[pkt.packetLength] = (byte)'\n';
            return serialPacket;
        }


        public static Packet Create_HeartBeatPacket()
        {
            Packet pktObj = new Packet();
            pktObj.startByte = START_BYTE;
            pktObj.type = T_HEARTBEAT;
            pktObj.value[0] = T_HEARTBEAT;
            pktObj.valueLength = 1;
            pktObj.dataLength = Get_DataLength(pktObj.valueLength);
            pktObj.packetLength = (byte)(1 + 1 + pktObj.dataLength + 2);
            Set_CRCValue(ref pktObj);
            return pktObj;
        }



    }
}
