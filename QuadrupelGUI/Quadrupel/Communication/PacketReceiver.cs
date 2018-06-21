using Quadrupel.Communication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel
{
    /// <summary>
    /// Helper class to Receive Packets from drone - State Machine
    /// </summary>
    internal class PacketReceiver
    {
        private PacketReceiver() { }
        private IPacketReceiver callBackObject;

        //Arun Geo Thomas
        public PacketReceiver(IPacketReceiver callBackObject)
        {
            this.callBackObject = callBackObject;
        }

        

        //States of the state machine
        private enum States { checkStartByte, getMsgSize, checkMessageType, setupMsg, getMsg, checkCRC0, checkCRC1 };

        //State Machine configurations
        private byte maxMsgSize = (byte)Packet.LP_SIZE;
        private static ushort BUFFER_QUEUE_SIZE = 512;
        private byte[] Data = new byte[BUFFER_QUEUE_SIZE];
        private States currentStateR = States.checkStartByte;



        private List<byte> InputBuffer = new List<byte>();
        int counter = 0;
        private byte currentByte;
        private byte msgSize;
        private byte msgType;
        private States nextState;
        private static byte[] ptr = new byte[Packet.LP_SIZE + 20];
        private Packet pkt_R;


        /// <summary>
        /// State Handler for the state machine
        /// </summary>
        //Arun Geo Thomas
        public void stateHandler()
        {
            ReadData();
            switch (currentStateR)
            {
                case States.checkStartByte:
                    if (currentByte != Packet.START_BYTE)
                    {


                        nextState = States.checkStartByte;
                    }
                    else
                    {
                        nextState = States.getMsgSize;

                    }
                    break;
                case States.getMsgSize:

                    msgSize = currentByte;
                    nextState = States.checkMessageType;
                    if ((msgSize >= maxMsgSize)|msgSize<=0)
                    {
                        if (currentByte == Packet.START_BYTE)
                        {
                            nextState = States.getMsgSize;
                        }
                        else
                        {
                            nextState = States.checkStartByte;
                        }

                    }


                    break;
                case States.checkMessageType:

                    if ((currentByte == Packet.T_MODE) || (currentByte == Packet.T_CONTROL) || (currentByte == Packet.T_DATA) || (currentByte == Packet.T_EXIT) || (currentByte == Packet.T_adMSG) || (currentByte == Packet.T_HEARTBEAT)|| (currentByte == Packet.T_FLASHMEM))
                    {
                        msgType = currentByte;
                        nextState = States.setupMsg;


                    }
                    else
                    {
                        if (msgSize == Packet.START_BYTE)
                        {
                            msgSize = currentByte;
                            nextState = States.checkMessageType;
                        }
                        else if (currentByte == Packet.START_BYTE)
                        {
                            nextState = States.getMsgSize;
                        }
                        else
                        {

                            nextState = States.checkStartByte;
                        }

                    }

                    break;

                case States.setupMsg:
                    counter = 0;

                    storeValues();

                    break;

                case States.getMsg:

                    storeValues();

                    break;
                case States.checkCRC0:
                    counter = 0;

                    pkt_R = Packet.Create_Packet(msgType, (byte)(msgSize - 1), ptr);



                    if (pkt_R.CRC[0] == (byte)currentByte)
                    {
                        nextState = States.checkCRC1;
                    }
                    else
                    {
                        pkt_R.CRC[0] = currentByte;
                        SearchforStartByte(0);
                    }


                    break;
                case States.checkCRC1:

                    if (pkt_R.CRC[1] == currentByte)
                    {
                        nextState = States.checkStartByte;
                        callBackObject.ProcessPacket(pkt_R);

                    }
                    else
                    {
                        pkt_R.CRC[1] = currentByte;
                        SearchforStartByte(1);
                    }

                    break;

            }
            currentStateR = nextState;

        }

        //store read values
        //Arun Geo Thomas
        private void storeValues()
        {

            ptr[counter] = currentByte;


            counter++;
            if (counter >= msgSize - 1)
            {
                nextState = States.checkCRC0;
            }
            else
            {
                nextState = States.getMsg;
            }
        }

        //Search for start byte again
        //Arun Geo Thomas
        private void SearchforStartByte(byte CRCPos)
        {
            byte[] serialPacket = Packet.Get_Byte_Stream(ref pkt_R);
            byte byteLength = (byte)(pkt_R.packetLength - (1 - CRCPos));

            for (int i = 1; i < byteLength; i++)
            {
                if (serialPacket[i] == Packet.START_BYTE)
                {
                    byte[] dataToStore = new byte[serialPacket.Length];
                    Array.Copy(serialPacket, i, dataToStore, 0, byteLength-i);
                    addData(dataToStore, (byte)(byteLength - i));
                    break;
                }
            }
            Packet.Destroy_Packet(ref pkt_R);
            nextState = States.checkStartByte;
        }

        /// <summary>
        /// REad data from where ever it is available
        /// </summary>
        /// <returns>Data byte</returns>
        //Arun Geo Thomas
        private byte ReadData()
        {
            if (InputBuffer.Count > 0)
            {

                currentByte = InputBuffer.Last();
                InputBuffer.RemoveAt(InputBuffer.Count-1);
            }

            else
            {
                currentByte = callBackObject.GetElementFromInputQueue();
            }
            return currentByte;


        }



        /// <summary>
        /// Check if theres a data availableto be read
        /// </summary>
        /// <returns></returns>
        //Arun Geo Thomas
        public Boolean checkCount()
        {
            return InputBuffer.Count > 0 ? true : callBackObject.CheckInputQueueCount();
        }

        /// <summary>
        /// Add read data to Input Buffer
        /// </summary>
        /// <param name="datas">datas to be fed back</param>
        /// <param name="length">length of array</param>
        //Arun Geo Thomas
        private void addData(byte[] datas, byte length)
        {
            for (int i = length - 1; i >= 0; i--)
            {
                InputBuffer.Add(datas[i]);
            }

        }



    }



}
