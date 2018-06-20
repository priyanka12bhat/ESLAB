using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel.Communication
{
    class SerialPortManager
    {
        public static Int32 baudRate = 115200;
        public static Parity parity = Parity.None;
        public static Int32 dataBits = 8;
        public static StopBits stopBits = StopBits.One;
        private SerialPort serialPort;
        
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="portName">serial port name</param>
        public SerialPortManager(String portName)
        {
            serialPort = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
            serialPort.RtsEnable = true;
            serialPort.DtrEnable = true;
            serialPort.Encoding = System.Text.Encoding.GetEncoding(28591);
        }

        public Boolean Connect()
        {
            if (serialPort != null && !serialPort.IsOpen)
            {      



                serialPort.Open();
                return true;

            }

            return false;
        }

        /// <summary>
        /// Bytes to be read from serial port
        /// </summary>
        public int BytesToRead
        {
            get
            {
                return IsConnected?serialPort.BytesToRead:0;

            }
        }

        /// <summary>
        /// Is there a valid connection
        /// </summary>
        public bool IsConnected
        { get
            {
                return (serialPort != null && serialPort.IsOpen);
            }
        }
        /// <summary>
        /// write bytes to serial port
        /// </summary>
        /// <param name="bytesToSend">bytes to sebd</param>
        public void WriteToSerialPort(byte[] bytesToSend)
        {
            if (IsConnected)
            {
                string s = System.Text.Encoding.GetEncoding(28591).GetString(bytesToSend, 0, bytesToSend.Length);
                serialPort.Write(s);
            }
        }

        /// <summary>
        /// REad next character
        /// </summary>
        /// <returns></returns>
        public byte ReadCharFromSerialPort()
        {
            return (byte)serialPort.ReadChar();
        }


        public bool CloseConnection()
        {
            if (IsConnected)
            {


                serialPort.Close();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
