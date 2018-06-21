namespace Quadrupel.UIControls
{
    partial class ControlInputs
    {
        //Arun Geo Thomas
        public bool UpdateValues {
            
            set {

                this.PITCHMeter.Level = JSData.PITCH;
                this.ROLLMeter.Level = JSData.ROLL;
                this.YAWMeter.Level = JSData.YAW;
                this.LIFTMeter.Level = -JSData.LIFT;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        //Arun Geo Thomas
        public void ClearUI()
        {
            this.PITCHMeter.Level = 0;
            this.ROLLMeter.Level = 0;
            this.YAWMeter.Level = 0;
            this.LIFTMeter.Level = -32767;
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.LIFTMeter = new VU_MeterLibrary.VuMeter();
            this.gbControlInput = new System.Windows.Forms.GroupBox();
            this.PITCHMeter = new VU_MeterLibrary.VuMeter();
            this.YAWMeter = new VU_MeterLibrary.VuMeter();
            this.ROLLMeter = new VU_MeterLibrary.VuMeter();
            this.groupBox1.SuspendLayout();
            this.gbControlInput.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.LIFTMeter);
            this.groupBox1.Location = new System.Drawing.Point(359, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(39, 163);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Lift";
            // 
            // LIFTMeter
            // 
            this.LIFTMeter.AnalogMeter = false;
            this.LIFTMeter.DialBackground = System.Drawing.Color.White;
            this.LIFTMeter.DialTextNegative = System.Drawing.Color.Red;
            this.LIFTMeter.DialTextPositive = System.Drawing.Color.Black;
            this.LIFTMeter.DialTextZero = System.Drawing.Color.DarkGreen;
            this.LIFTMeter.Led1ColorOff = System.Drawing.Color.DarkGreen;
            this.LIFTMeter.Led1ColorOn = System.Drawing.Color.LimeGreen;
            this.LIFTMeter.Led1Count = 11;
            this.LIFTMeter.Led2ColorOff = System.Drawing.Color.Olive;
            this.LIFTMeter.Led2ColorOn = System.Drawing.Color.Yellow;
            this.LIFTMeter.Led2Count = 8;
            this.LIFTMeter.Led3ColorOff = System.Drawing.Color.Maroon;
            this.LIFTMeter.Led3ColorOn = System.Drawing.Color.Red;
            this.LIFTMeter.Led3Count = 4;
            this.LIFTMeter.LedSize = new System.Drawing.Size(6, 5);
            this.LIFTMeter.LedSpace = 1;
            this.LIFTMeter.Level = -32767;
            this.LIFTMeter.LevelMax = 32767;
            this.LIFTMeter.LevelMin = -32767;
            this.LIFTMeter.Location = new System.Drawing.Point(14, 16);
            this.LIFTMeter.MeterScale = VU_MeterLibrary.MeterScale.Analog;
            this.LIFTMeter.Name = "LIFTMeter";
            this.LIFTMeter.NeedleColor = System.Drawing.Color.Black;
            this.LIFTMeter.PeakHold = false;
            this.LIFTMeter.Peakms = 1000;
            this.LIFTMeter.PeakNeedleColor = System.Drawing.Color.Red;
            this.LIFTMeter.ShowDialOnly = false;
            this.LIFTMeter.ShowLedPeak = false;
            this.LIFTMeter.ShowTextInDial = false;
            this.LIFTMeter.Size = new System.Drawing.Size(8, 139);
            this.LIFTMeter.TabIndex = 8;
            this.LIFTMeter.TextInDial = new string[] {
        "-40",
        "-20",
        "-10",
        "-5",
        "0",
        "+6"};
            this.LIFTMeter.UseLedLight = false;
            this.LIFTMeter.VerticalBar = true;
            this.LIFTMeter.VuText = "LIFT";
            // 
            // gbControlInput
            // 
            this.gbControlInput.Controls.Add(this.PITCHMeter);
            this.gbControlInput.Controls.Add(this.YAWMeter);
            this.gbControlInput.Controls.Add(this.ROLLMeter);
            this.gbControlInput.Location = new System.Drawing.Point(3, 2);
            this.gbControlInput.Name = "gbControlInput";
            this.gbControlInput.Size = new System.Drawing.Size(348, 165);
            this.gbControlInput.TabIndex = 11;
            this.gbControlInput.TabStop = false;
            this.gbControlInput.Text = "Control Input";
            // 
            // PITCHMeter
            // 
            this.PITCHMeter.AnalogMeter = true;
            this.PITCHMeter.BackColor = System.Drawing.Color.White;
            this.PITCHMeter.DialBackground = System.Drawing.Color.White;
            this.PITCHMeter.DialTextNegative = System.Drawing.Color.Red;
            this.PITCHMeter.DialTextPositive = System.Drawing.Color.Black;
            this.PITCHMeter.DialTextZero = System.Drawing.Color.DarkGreen;
            this.PITCHMeter.Font = new System.Drawing.Font("Microsoft Sans Serif", 4F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PITCHMeter.Led1ColorOff = System.Drawing.Color.DarkGreen;
            this.PITCHMeter.Led1ColorOn = System.Drawing.Color.RoyalBlue;
            this.PITCHMeter.Led1Count = 6;
            this.PITCHMeter.Led2ColorOff = System.Drawing.Color.Olive;
            this.PITCHMeter.Led2ColorOn = System.Drawing.Color.Yellow;
            this.PITCHMeter.Led2Count = 6;
            this.PITCHMeter.Led3ColorOff = System.Drawing.Color.Maroon;
            this.PITCHMeter.Led3ColorOn = System.Drawing.Color.RoyalBlue;
            this.PITCHMeter.Led3Count = 6;
            this.PITCHMeter.LedSize = new System.Drawing.Size(4, 14);
            this.PITCHMeter.LedSpace = 3;
            this.PITCHMeter.Level = -32767;
            this.PITCHMeter.LevelMax = 32767;
            this.PITCHMeter.LevelMin = -32767;
            this.PITCHMeter.Location = new System.Drawing.Point(201, 26);
            this.PITCHMeter.MeterScale = VU_MeterLibrary.MeterScale.Analog;
            this.PITCHMeter.Name = "PITCHMeter";
            this.PITCHMeter.NeedleColor = System.Drawing.Color.Black;
            this.PITCHMeter.PeakHold = false;
            this.PITCHMeter.Peakms = 1000;
            this.PITCHMeter.PeakNeedleColor = System.Drawing.Color.Red;
            this.PITCHMeter.ShowDialOnly = true;
            this.PITCHMeter.ShowLedPeak = false;
            this.PITCHMeter.ShowTextInDial = false;
            this.PITCHMeter.Size = new System.Drawing.Size(147, 117);
            this.PITCHMeter.TabIndex = 7;
            this.PITCHMeter.TextInDial = new string[] {
        "-40",
        "-20",
        "-10",
        "-5",
        "0",
        "+6"};
            this.PITCHMeter.UseLedLight = false;
            this.PITCHMeter.VerticalBar = false;
            this.PITCHMeter.VuText = "PITCH";
            // 
            // YAWMeter
            // 
            this.YAWMeter.AnalogMeter = true;
            this.YAWMeter.BackColor = System.Drawing.Color.White;
            this.YAWMeter.DialBackground = System.Drawing.Color.White;
            this.YAWMeter.DialTextNegative = System.Drawing.Color.Red;
            this.YAWMeter.DialTextPositive = System.Drawing.Color.Black;
            this.YAWMeter.DialTextZero = System.Drawing.Color.DarkGreen;
            this.YAWMeter.Font = new System.Drawing.Font("Microsoft Sans Serif", 4F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.YAWMeter.Led1ColorOff = System.Drawing.Color.DarkGreen;
            this.YAWMeter.Led1ColorOn = System.Drawing.Color.DarkGray;
            this.YAWMeter.Led1Count = 6;
            this.YAWMeter.Led2ColorOff = System.Drawing.Color.Olive;
            this.YAWMeter.Led2ColorOn = System.Drawing.Color.Yellow;
            this.YAWMeter.Led2Count = 6;
            this.YAWMeter.Led3ColorOff = System.Drawing.Color.Maroon;
            this.YAWMeter.Led3ColorOn = System.Drawing.Color.DarkGray;
            this.YAWMeter.Led3Count = 6;
            this.YAWMeter.LedSize = new System.Drawing.Size(4, 14);
            this.YAWMeter.LedSpace = 3;
            this.YAWMeter.Level = -32767;
            this.YAWMeter.LevelMax = 32767;
            this.YAWMeter.LevelMin = -32767;
            this.YAWMeter.Location = new System.Drawing.Point(99, 78);
            this.YAWMeter.MeterScale = VU_MeterLibrary.MeterScale.Analog;
            this.YAWMeter.Name = "YAWMeter";
            this.YAWMeter.NeedleColor = System.Drawing.Color.Black;
            this.YAWMeter.PeakHold = false;
            this.YAWMeter.Peakms = 1000;
            this.YAWMeter.PeakNeedleColor = System.Drawing.Color.Red;
            this.YAWMeter.ShowDialOnly = true;
            this.YAWMeter.ShowLedPeak = false;
            this.YAWMeter.ShowTextInDial = false;
            this.YAWMeter.Size = new System.Drawing.Size(147, 117);
            this.YAWMeter.TabIndex = 7;
            this.YAWMeter.TextInDial = new string[] {
        "-40",
        "-20",
        "-10",
        "-5",
        "0",
        "+6"};
            this.YAWMeter.UseLedLight = false;
            this.YAWMeter.VerticalBar = false;
            this.YAWMeter.VuText = "YAW RATE";
            // 
            // ROLLMeter
            // 
            this.ROLLMeter.AnalogMeter = true;
            this.ROLLMeter.BackColor = System.Drawing.Color.White;
            this.ROLLMeter.DialBackground = System.Drawing.Color.White;
            this.ROLLMeter.DialTextNegative = System.Drawing.Color.Red;
            this.ROLLMeter.DialTextPositive = System.Drawing.Color.Black;
            this.ROLLMeter.DialTextZero = System.Drawing.Color.DarkGreen;
            this.ROLLMeter.Font = new System.Drawing.Font("Microsoft Sans Serif", 4F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ROLLMeter.Led1ColorOff = System.Drawing.Color.DarkGreen;
            this.ROLLMeter.Led1ColorOn = System.Drawing.Color.Red;
            this.ROLLMeter.Led1Count = 6;
            this.ROLLMeter.Led2ColorOff = System.Drawing.Color.Olive;
            this.ROLLMeter.Led2ColorOn = System.Drawing.Color.Yellow;
            this.ROLLMeter.Led2Count = 6;
            this.ROLLMeter.Led3ColorOff = System.Drawing.Color.Maroon;
            this.ROLLMeter.Led3ColorOn = System.Drawing.Color.Red;
            this.ROLLMeter.Led3Count = 6;
            this.ROLLMeter.LedSize = new System.Drawing.Size(4, 14);
            this.ROLLMeter.LedSpace = 3;
            this.ROLLMeter.Level = -32767;
            this.ROLLMeter.LevelMax = 32767;
            this.ROLLMeter.LevelMin = -32767;
            this.ROLLMeter.Location = new System.Drawing.Point(-2, 19);
            this.ROLLMeter.MeterScale = VU_MeterLibrary.MeterScale.Analog;
            this.ROLLMeter.Name = "ROLLMeter";
            this.ROLLMeter.NeedleColor = System.Drawing.Color.Black;
            this.ROLLMeter.PeakHold = false;
            this.ROLLMeter.Peakms = 1000;
            this.ROLLMeter.PeakNeedleColor = System.Drawing.Color.Red;
            this.ROLLMeter.ShowDialOnly = true;
            this.ROLLMeter.ShowLedPeak = false;
            this.ROLLMeter.ShowTextInDial = false;
            this.ROLLMeter.Size = new System.Drawing.Size(147, 117);
            this.ROLLMeter.TabIndex = 7;
            this.ROLLMeter.TextInDial = new string[] {
        "-40",
        "-20",
        "-10",
        "-5",
        "0",
        "+6"};
            this.ROLLMeter.UseLedLight = false;
            this.ROLLMeter.VerticalBar = false;
            this.ROLLMeter.VuText = "ROLL";
            // 
            // ControlInputs
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.gbControlInput);
            this.Name = "ControlInputs";
            this.Size = new System.Drawing.Size(401, 170);
            this.groupBox1.ResumeLayout(false);
            this.gbControlInput.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private VU_MeterLibrary.VuMeter LIFTMeter;
        private System.Windows.Forms.GroupBox gbControlInput;
        private VU_MeterLibrary.VuMeter PITCHMeter;
        private VU_MeterLibrary.VuMeter YAWMeter;
        private VU_MeterLibrary.VuMeter ROLLMeter;
    }
}
