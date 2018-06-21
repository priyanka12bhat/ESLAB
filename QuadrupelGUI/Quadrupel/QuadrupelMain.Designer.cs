using System;
using System.Windows.Forms;

namespace Quadrupel
{
    partial class QuadrupelControlForm
    {
        private TelemetryData _tData;
        /// <summary>
        /// Telemetery Data 
        /// </summary>
        //Arun Geo Thomas
        public TelemetryData tData
        {
            get
            {
                return _tData;
            }
            set
            {
                _tData = value;
                this.Invoke((MethodInvoker)(() => UpdateUI(value)));
            }
        }
        //Arun Geo Thomas
        public string AdditionalMessage {

            set {

                this.BeginInvoke((MethodInvoker)(() => UpdateUI(value)));
            }
        }




        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(QuadrupelControlForm));
            this.lblProgramTitle = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.cbCOMPortsSelector = new System.Windows.Forms.ComboBox();
            this.btnStartOperation = new System.Windows.Forms.Button();
            this.lblSelectCOMPort = new System.Windows.Forms.Label();
            this.btnStopOpertion = new System.Windows.Forms.Button();
            this.lblSerialportMessages = new System.Windows.Forms.Label();
            this.lblOutput = new System.Windows.Forms.Label();
            this.JSTimer = new System.Windows.Forms.Timer(this.components);
            this.btnClose = new System.Windows.Forms.Button();
            this.controlInputs1 = new Quadrupel.UIControls.ControlInputs();
            this.readOuts1 = new Quadrupel.Controls.ReadOuts();
            this.dronesModes1 = new Quadrupel.Controls.MotorSpeed.DronesModes();
            this.motorSpeeds1 = new Quadrupel.Controls.MotorSpeed.MotorSpeeds();
            this.gbMessages = new System.Windows.Forms.GroupBox();
            this.lblDroneMessageText = new System.Windows.Forms.Label();
            this.lblPCMessageText = new System.Windows.Forms.Label();
            this.lblDroneMessage = new System.Windows.Forms.Label();
            this.lblPCMessages = new System.Windows.Forms.Label();
            this.tableLayoutPanel1.SuspendLayout();
            this.gbMessages.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblProgramTitle
            // 
            this.lblProgramTitle.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblProgramTitle.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan(this.lblProgramTitle, 8);
            this.lblProgramTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProgramTitle.ForeColor = System.Drawing.Color.Navy;
            this.lblProgramTitle.Location = new System.Drawing.Point(3, 0);
            this.lblProgramTitle.Name = "lblProgramTitle";
            this.lblProgramTitle.Size = new System.Drawing.Size(842, 30);
            this.lblProgramTitle.TabIndex = 0;
            this.lblProgramTitle.Text = "Quadrupel Control";
            this.lblProgramTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 8;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12.5F));
            this.tableLayoutPanel1.Controls.Add(this.cbCOMPortsSelector, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.btnStartOperation, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblProgramTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblSelectCOMPort, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.btnStopOpertion, 7, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblSerialportMessages, 3, 1);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 12);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(848, 60);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // cbCOMPortsSelector
            // 
            this.cbCOMPortsSelector.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cbCOMPortsSelector.FormattingEnabled = true;
            this.cbCOMPortsSelector.Location = new System.Drawing.Point(109, 33);
            this.cbCOMPortsSelector.Name = "cbCOMPortsSelector";
            this.cbCOMPortsSelector.Size = new System.Drawing.Size(100, 21);
            this.cbCOMPortsSelector.TabIndex = 3;
            this.cbCOMPortsSelector.DropDown += new System.EventHandler(this.cbCOMPortsSelector_DropDown);
            // 
            // btnStartOperation
            // 
            this.btnStartOperation.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStartOperation.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnStartOperation.Location = new System.Drawing.Point(215, 33);
            this.btnStartOperation.Name = "btnStartOperation";
            this.btnStartOperation.Size = new System.Drawing.Size(100, 24);
            this.btnStartOperation.TabIndex = 3;
            this.btnStartOperation.Text = "Start Operation";
            this.btnStartOperation.UseVisualStyleBackColor = true;
            this.btnStartOperation.Click += new System.EventHandler(this.btnStartOperation_Click);
            // 
            // lblSelectCOMPort
            // 
            this.lblSelectCOMPort.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblSelectCOMPort.AutoSize = true;
            this.lblSelectCOMPort.Location = new System.Drawing.Point(3, 30);
            this.lblSelectCOMPort.Name = "lblSelectCOMPort";
            this.lblSelectCOMPort.Size = new System.Drawing.Size(100, 30);
            this.lblSelectCOMPort.TabIndex = 3;
            this.lblSelectCOMPort.Text = "Select COM Port";
            this.lblSelectCOMPort.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnStopOpertion
            // 
            this.btnStopOpertion.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStopOpertion.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnStopOpertion.Location = new System.Drawing.Point(745, 33);
            this.btnStopOpertion.Name = "btnStopOpertion";
            this.btnStopOpertion.Size = new System.Drawing.Size(100, 24);
            this.btnStopOpertion.TabIndex = 3;
            this.btnStopOpertion.Text = "Stop Operation";
            this.btnStopOpertion.UseVisualStyleBackColor = true;
            this.btnStopOpertion.Click += new System.EventHandler(this.btnStopOpertion_Click);
            // 
            // lblSerialportMessages
            // 
            this.lblSerialportMessages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblSerialportMessages.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan(this.lblSerialportMessages, 4);
            this.lblSerialportMessages.Location = new System.Drawing.Point(321, 30);
            this.lblSerialportMessages.Name = "lblSerialportMessages";
            this.lblSerialportMessages.Size = new System.Drawing.Size(418, 30);
            this.lblSerialportMessages.TabIndex = 3;
            this.lblSerialportMessages.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblOutput
            // 
            this.lblOutput.AutoSize = true;
            this.lblOutput.Location = new System.Drawing.Point(437, 213);
            this.lblOutput.Name = "lblOutput";
            this.lblOutput.Size = new System.Drawing.Size(0, 13);
            this.lblOutput.TabIndex = 3;
            // 
            // JSTimer
            // 
            this.JSTimer.Interval = 40;
            this.JSTimer.Tick += new System.EventHandler(this.JSTimer_Tick);
            // 
            // btnClose
            // 
            this.btnClose.AutoSize = true;
            this.btnClose.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnClose.Location = new System.Drawing.Point(838, 5);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(28, 23);
            this.btnClose.TabIndex = 9;
            this.btnClose.Text = "X";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.button1_Click);
            // 
            // controlInputs1
            // 
            this.controlInputs1.Location = new System.Drawing.Point(8, 96);
            this.controlInputs1.Name = "controlInputs1";
            this.controlInputs1.Size = new System.Drawing.Size(401, 170);
            this.controlInputs1.TabIndex = 10;
            // 
            // readOuts1
            // 
            this.readOuts1.AdditionalMessage = "";
            this.readOuts1.BatteryVoltage = ((uint)(0u));
            this.readOuts1.Location = new System.Drawing.Point(720, 94);
            this.readOuts1.Name = "readOuts1";
            this.readOuts1.P_H = ((uint)(0u));
            this.readOuts1.SetPressure = ((uint)(0u));
            this.readOuts1.Size = new System.Drawing.Size(145, 270);
            this.readOuts1.TabIndex = 6;
            this.readOuts1.Values = new int[] {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0};
            // 
            // dronesModes1
            // 
            this.dronesModes1.ActiveMode = Quadrupel.Modes.DroneStates.Safe;
            this.dronesModes1.BackColor = System.Drawing.Color.Transparent;
            this.dronesModes1.Location = new System.Drawing.Point(8, 271);
            this.dronesModes1.Name = "dronesModes1";
            this.dronesModes1.Size = new System.Drawing.Size(707, 88);
            this.dronesModes1.TabIndex = 5;
            // 
            // motorSpeeds1
            // 
            this.motorSpeeds1.BackColor = System.Drawing.Color.Transparent;
            this.motorSpeeds1.Location = new System.Drawing.Point(411, 94);
            this.motorSpeeds1.Name = "motorSpeeds1";
            this.motorSpeeds1.Size = new System.Drawing.Size(306, 172);
            this.motorSpeeds1.TabIndex = 4;
            this.motorSpeeds1.Values = new int[] {
        0,
        0,
        0,
        0};
            // 
            // gbMessages
            // 
            this.gbMessages.Controls.Add(this.lblDroneMessageText);
            this.gbMessages.Controls.Add(this.lblPCMessageText);
            this.gbMessages.Controls.Add(this.lblDroneMessage);
            this.gbMessages.Controls.Add(this.lblPCMessages);
            this.gbMessages.Location = new System.Drawing.Point(12, 359);
            this.gbMessages.Name = "gbMessages";
            this.gbMessages.Size = new System.Drawing.Size(853, 42);
            this.gbMessages.TabIndex = 11;
            this.gbMessages.TabStop = false;
            this.gbMessages.Text = "Messages";
            // 
            // lblDroneMessageText
            // 
            this.lblDroneMessageText.AutoSize = true;
            this.lblDroneMessageText.Location = new System.Drawing.Point(444, 18);
            this.lblDroneMessageText.Name = "lblDroneMessageText";
            this.lblDroneMessageText.Size = new System.Drawing.Size(0, 13);
            this.lblDroneMessageText.TabIndex = 0;
            // 
            // lblPCMessageText
            // 
            this.lblPCMessageText.AutoSize = true;
            this.lblPCMessageText.Location = new System.Drawing.Point(36, 18);
            this.lblPCMessageText.Name = "lblPCMessageText";
            this.lblPCMessageText.Size = new System.Drawing.Size(0, 13);
            this.lblPCMessageText.TabIndex = 0;
            // 
            // lblDroneMessage
            // 
            this.lblDroneMessage.AutoSize = true;
            this.lblDroneMessage.Location = new System.Drawing.Point(401, 16);
            this.lblDroneMessage.Name = "lblDroneMessage";
            this.lblDroneMessage.Size = new System.Drawing.Size(39, 13);
            this.lblDroneMessage.TabIndex = 0;
            this.lblDroneMessage.Text = "Drone:";
            // 
            // lblPCMessages
            // 
            this.lblPCMessages.AutoSize = true;
            this.lblPCMessages.Location = new System.Drawing.Point(6, 18);
            this.lblPCMessages.Name = "lblPCMessages";
            this.lblPCMessages.Size = new System.Drawing.Size(24, 13);
            this.lblPCMessages.TabIndex = 0;
            this.lblPCMessages.Text = "PC:";
            // 
            // QuadrupelControlForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(872, 413);
            this.Controls.Add(this.gbMessages);
            this.Controls.Add(this.controlInputs1);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.readOuts1);
            this.Controls.Add(this.dronesModes1);
            this.Controls.Add(this.motorSpeeds1);
            this.Controls.Add(this.lblOutput);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MaximizeBox = false;
            this.Name = "QuadrupelControlForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Quadrupel Control";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.QuadrupelControlForm_FormClosing);
            this.Load += new System.EventHandler(this.QuadrupelControlForm_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.gbMessages.ResumeLayout(false);
            this.gbMessages.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblProgramTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Button btnStartOperation;
        private System.Windows.Forms.Button btnStopOpertion;
        private System.Windows.Forms.ComboBox cbCOMPortsSelector;
        private System.Windows.Forms.Label lblSelectCOMPort;
        private System.Windows.Forms.Label lblOutput;
        private Controls.MotorSpeed.MotorSpeeds motorSpeeds1;
        private System.Windows.Forms.Label lblSerialportMessages;
        private Controls.MotorSpeed.DronesModes dronesModes1;
        private Controls.ReadOuts readOuts1;
        private Timer JSTimer;
        private Button btnClose;
        private UIControls.ControlInputs controlInputs1;
        private GroupBox gbMessages;
        private Label lblDroneMessageText;
        private Label lblPCMessageText;
        private Label lblDroneMessage;
        private Label lblPCMessages;
    }
}

