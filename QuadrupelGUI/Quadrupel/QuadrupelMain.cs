using Quadrupel.Communication;
using Quadrupel.Drone;
using Quadrupel.Input;
using System;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace Quadrupel
{
    internal partial class QuadrupelControlForm : Form,IPacketReceiver
    {
        private Thread serialPortReadThread;
        private Boolean readSerialPortData = false;
        private PacketReceiver packetReciever;
        private SerialPortManager serialPortManager;
        private string logFileName = Environment.CurrentDirectory + @"\log.txt";

        //private Device joystickDevice;
        //Arun Geo Thomas
        public QuadrupelControlForm()
        {

            InitializeComponent();
            packetReciever = new PacketReceiver(this);
            //InitJoyStick();
            

        }
        //Arun Geo Thomas
        private void QuadrupelControlForm_Load(object sender, EventArgs e)
        {
            ClearUI();
            string[] args = Environment.GetCommandLineArgs();
            if (args.Length >= 2)
            {
                cbCOMPortsSelector.Items.Add(args[1]);
                cbCOMPortsSelector.SelectedIndex = 0;
                cbCOMPortsSelector.Text = args[1];
                btnStartOperation_Click(this,new EventArgs());


            }
        }


        /// <summary>
        /// Method to Update UI
        /// </summary>
        /// <param name="data">Telemetry Data to be updated in UI</param>
        //Arun Geo Thomas
        private void UpdateUI(TelemetryData data)
        {
            //lblOutput.Text = data.BatteryVoltage.ToString();
            motorSpeeds1.Values = data.Motor;
            dronesModes1.ActiveMode = data.currentDroneMode;
            //lblP.Text = "ABCD\nabcd";
            //lblP1Value.Text = data.P[1].ToString();
            //lblP2Value.Text = data.P[2].ToString();
            readOuts1.Values = data.DataArray;
            readOuts1.SetPressure = data.pressure;
            readOuts1.P_H = data.P_H;
            readOuts1.BatteryVoltage = data.BatteryVoltage;
            lblDroneMessageText.Text = Messages.MsgTable[data.droneMessageCode];

        }
        //Arun Geo Thomas
        private void UpdateUI(string additionalMessage)
        {
            readOuts1.AdditionalMessage = additionalMessage;
        }

        /// <summary>
        /// Clear UI
        /// </summary>
        //Arun Geo Thomas
        private void ClearUI()
        {
            //lblOutput.Text = data.BatteryVoltage.ToString();
            motorSpeeds1.Values = new int[4];
            readOuts1.Clear();
            dronesModes1.ClearFormating();
            controlInputs1.ClearUI();
            btnStartOperation.Enabled = true;
            cbCOMPortsSelector.Enabled = true;
        }
        //Arun Geo Thomas
        private void btnStartOperation_Click(object sender, EventArgs e)
        {
            if (File.Exists(logFileName)) {
                File.Delete(logFileName);
            }
            
            try
            {
                serialPortManager = new SerialPortManager(cbCOMPortsSelector.SelectedItem.ToString());


                if (serialPortManager.Connect())
                {
                    lblSerialportMessages.Text = "Connected";
                    readSerialPortData = true;
                    serialPortReadThread = new Thread(new ThreadStart(BackendThreadExecute));
                    serialPortReadThread.Start();
                    cbCOMPortsSelector.Enabled = false;
                    btnStartOperation.Enabled = false;
                    JSTimer.Start();
                }
                else
                {
                    lblSerialportMessages.Text = "Already Connected";
                }               
            }
            catch (Exception ex)
            {
                lblSerialportMessages.Text = "Error Cant Connect: " + ex.Message;

            }


        }
        //Arun Geo Thomas
        private void btnStopOpertion_Click(object sender, EventArgs e)
        {

            if (serialPortManager!=null)
            {
                Object thisLock = new Object();
                lock (thisLock)
                {
                    readSerialPortData = false;
                }

                if (serialPortManager.CloseConnection())
                {
                    JSTimer.Stop();


                    lblSerialportMessages.Text = "Disconnected";
                    cbCOMPortsSelector.Enabled = true;
                    btnStartOperation.Enabled = true;
                    serialPortManager = null;

                }
                else
                {
                    lblSerialportMessages.Text = "Already Disconnected";
                }
            }
            else
            {
                lblSerialportMessages.Text = "No valid connection";
            }
            ClearUI();
        }


        /// <summary>
        /// Function to send Packet through serial port to drone
        /// </summary>
        /// <param name="pkt">Packet to be send</param>
        //Arun Geo Thomas
        void Send_Packet(Packet pkt)
        {
            try
            {
                if (serialPortManager.IsConnected)
                {

                    byte[] packetByteStream = Packet.Get_Byte_Stream(ref pkt);


                    serialPortManager.WriteToSerialPort(packetByteStream);
                }
            }
            catch (Exception e)
            {
                this.Invoke((MethodInvoker)(() => ClearUI()));
            }


        }
        /// <summary>
        /// Function to handle keyboard Input
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="keyData"></param>
        /// <returns></returns>
        //Arun Geo Thomas
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            Packet pkt = KBHandler.kb_CMDKey_handler(keyData);
            if (pkt != null && (serialPortManager?.IsConnected??false))
            {
                Send_Packet(pkt);
                pkt = null;
            }
            this.lblPCMessageText.Text = KBHandler.Message;
            return base.ProcessCmdKey(ref msg, keyData);
        }


        /// <summary>
        /// 
        /// </summary>
        private void InitJoyStick()
        {
           
        }


        /// <summary>
        /// Check there are Bytes to be read in Serial port 
        /// </summary>
        /// <returns>yes if there is Bytes to be read in serial port</returns>
        //Arun Geo Thomas
        public bool CheckInputQueueCount()
        {
            return (serialPortManager.BytesToRead > 0);
        }

        /// <summary>
        /// Gets the next byte from the serial port
        /// </summary>
        /// <returns>data byte from serail port</returns>
        //Arun Geo Thomas
        public byte GetElementFromInputQueue()
        {
            return serialPortManager.ReadCharFromSerialPort();
        }

        /// <summary>
        /// Function to process the received packet
        /// </summary>
        /// <param name="packet">Receieved Packet</param>
        //Arun Geo Thomas
        public void ProcessPacket(Packet packet)
        {

            if (packet.type == Packet.T_DATA)
            {
                TelemetryData newData = new TelemetryData();

                newData.BatteryVoltage = (UInt16)((((UInt16)packet.value[9]) << 8) | packet.value[8]);
                for (int i = 0; i < 4; i++)
                {
                    newData.Motor[i] = (int)((((int)packet.value[2 * i + 1]) << 8) | packet.value[2 * i]);
                }

                newData.currentDroneMode = (Modes.DroneStates)((int)packet.value[34]);


                //Pvalues 
                newData.P[0] = (UInt16)((((UInt16)packet.value[24]) << 8) | packet.value[23]);
                newData.P[1] = (UInt16)((((UInt16)packet.value[26]) << 8) | packet.value[25]);
                newData.P[2] = (UInt16)((((UInt16)packet.value[28]) << 8) | packet.value[27]);
                newData.P_H = (UInt16)((((UInt16)packet.value[30]) << 8) | packet.value[29]);
                newData.DataArray[0] = newData.phi = (Int16)((((UInt16)packet.value[11]) << 8) | packet.value[10]);
                newData.DataArray[1] = newData.theta = (Int16)((((UInt16)packet.value[13]) << 8) | packet.value[12]);
                newData.DataArray[2] = newData.psi = (Int16)((((UInt16)packet.value[15]) << 8) | packet.value[14]);

                newData.DataArray[3] = newData.sp = (Int16)((((UInt16)packet.value[17]) << 8) | packet.value[16]);
                newData.DataArray[4] = newData.sq = (Int16)((((UInt16)packet.value[19]) << 8) | packet.value[18]);
                newData.DataArray[5] = newData.sr = (Int16)((((UInt16)packet.value[21]) << 8) | packet.value[20]);
                newData.DataArray[6] = (Int16)newData.P[0];
                newData.DataArray[7] = (Int16)newData.P[1];
                newData.DataArray[8] = (Int16)newData.P[2];
                newData.pressure = (((UInt32)packet.value[31]) << 16) | (((UInt32)packet.value[32]) << 8) | (packet.value[33]);
                newData.droneMessageCode = packet.value[22];

                newData.BatteryVoltage = (UInt16)((((UInt16)packet.value[9]) << 8) | packet.value[8]);

                tData = newData;

            }
            else if (packet.type == Packet.T_adMSG)
            {
                AdditionalMessage = System.Text.Encoding.GetEncoding(28591).GetString(packet.value, 0, packet.valueLength);

            } else if (packet.type == Packet.T_FLASHMEM)
            {
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(Environment.CurrentDirectory+"\\log.txt", true))
                {
                    
                    file.Write("{0}\t", (((UInt32)packet.value[9]) << 24 | ((UInt32)packet.value[8]) << 16 | ((UInt32)packet.value[7]) << 8 | (UInt32)packet.value[6]).ToString());
                    file.Write("\t{0}\t", ((Int16)(((UInt16)packet.value[0]) << 8 | packet.value[1])).ToString());
                    file.Write("\t{0}\t", ((Int16)(((UInt16)packet.value[2]) << 8 | packet.value[3])).ToString());
                    file.Write("\t{0}\t", ((Int16)(((UInt16)packet.value[4]) << 8 | packet.value[5])).ToString());
                    file.Write("\t{0}\t", ((Int16)(((UInt16)packet.value[10]) << 8 | packet.value[11])).ToString());
                    file.Write("\t{0}\t", ((Int16)(((UInt16)packet.value[12]) << 8 | packet.value[13])).ToString());
                    file.Write("\t{0}\n", ((Int16)(((UInt16)packet.value[14]) << 8 | packet.value[15])).ToString());

                }

               
            }





        }
        //Arun Geo Thomas
        private void QuadrupelControlForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPortManager != null && serialPortManager.IsConnected)
            {
                


                readSerialPortData = false;


                serialPortReadThread.Abort();

                serialPortManager.CloseConnection();
                serialPortManager = null;
            }
        }

        /// <summary>
        /// Execution Function of Serial Port Read Thread
        /// </summary>
        //Arun Geo Thomas
        void BackendThreadExecute()
        {

            try
            {
                while (readSerialPortData)
                {
                    if (packetReciever.checkCount())
                    {
                        packetReciever.stateHandler();
                    }





                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                this.Invoke((MethodInvoker)(() => ClearUI()));
            }



        }
        //Arun Geo Thomas
        private void cbCOMPortsSelector_DropDown(object sender, EventArgs e)
        {
            cbCOMPortsSelector.Items.Clear();
            foreach (var ports in SerialPort.GetPortNames())
            {
                cbCOMPortsSelector.Items.Add(ports);
            }

        }

        private DateTime lastSendTime = DateTime.Now;
        private DateTime lastHBSendTime = DateTime.Now;
        //Arun Geo Thomas
        private void JSTimer_Tick(object sender, EventArgs e)
        {

            Packet pkt = JSWrapper.GetJSPacket();
            if (pkt != null)
            {
                if (serialPortManager.IsConnected)
                {
                    Send_Packet(pkt);
                    lastSendTime = DateTime.Now;
                }
            }

            if ((DateTime.Now - lastSendTime).TotalMilliseconds > 100 && (DateTime.Now - lastHBSendTime).TotalMilliseconds > 100)
            {
                Send_Packet(Packet.Create_HeartBeatPacket());
                lastHBSendTime = DateTime.Now;
            }
            

            this.controlInputs1.UpdateValues = true;

        }
        //Arun Geo Thomas
        private void button1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// Method to Update stored message for user
        /// </summary>
        /// <param name="msg">message that to be shown to user</param>
        //Arun Geo Thomas
        public void showPCMessage(String msg)
        {
            this.lblPCMessageText.Text = msg;
        }


    }


    
}
