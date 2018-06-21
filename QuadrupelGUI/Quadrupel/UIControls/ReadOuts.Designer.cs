using System;

namespace Quadrupel.Controls
{
    partial class ReadOuts
    {
        private const string Format = "Phi:{0}\nTheta:{1}\nPsi:{2}\nsp:{3}\nsq:{4}\nsr:{5}\nP:{6}\nP1:{7}\nP2:{8}\n";


        private int[] _Values = new int[9];

        /// <summary>
        /// Propert to set the Values
        /// </summary>
        //Arun Geo Thomas
        public int[] Values
        {
            get
            {
                return _Values;
            }
            set
            {
                value.CopyTo(_Values, 0);

                lblDisplay.Text = string.Format(Format, _Values[0], _Values[1], _Values[2], _Values[3], _Values[4], _Values[5], _Values[6], _Values[7], _Values[8]);
            }

        }
        private System.UInt32 _Pressure;
        //Arun Geo Thomas
        public UInt32 SetPressure {
            get
            { return _Pressure; }
            set
            {
               _Pressure = value;
                lblDisplay.Text += "Pressure:" + _Pressure.ToString() + "\n";

            }
        }

        private System.UInt32 _BatVolt;
        //Arun Geo Thomas
        public UInt32 BatteryVoltage
        {
            get
            { return _BatVolt; }
            set
            {
                _BatVolt = value;
                lblDisplay.Text += "BatVolt:" + _BatVolt.ToString() + "\n";

            }
        }


        private System.UInt32 _P_H;
        //Arun Geo Thomas
        public UInt32 P_H
        {
            get
            { return _P_H; }
            set
            {
                _P_H = value;
                lblDisplay.Text += "P_H:" + _P_H.ToString() + "\n";

            }
        }



        private string _AdditionalMessage = "";

        //Arun Geo Thomas
        public string AdditionalMessage {
            get
            {
                return _AdditionalMessage;
            } set
            {
                _AdditionalMessage = value;
                lblAdMessages.Text = _AdditionalMessage;
            }
        }

        /// <summary>
        /// Clear the Control
        /// </summary>
        //Arun Geo Thomas
        public void Clear()
        {
            lblDisplay.Text = "";
            lblAdMessages.Text = "";
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
            this.gbReadOutValues = new System.Windows.Forms.GroupBox();
            this.lblDisplay = new System.Windows.Forms.Label();
            this.gbAdditionalMessages = new System.Windows.Forms.GroupBox();
            this.lblAdMessages = new System.Windows.Forms.Label();
            this.gbReadOutValues.SuspendLayout();
            this.gbAdditionalMessages.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbReadOutValues
            // 
            this.gbReadOutValues.Controls.Add(this.lblDisplay);
            this.gbReadOutValues.Location = new System.Drawing.Point(3, 3);
            this.gbReadOutValues.Name = "gbReadOutValues";
            this.gbReadOutValues.Size = new System.Drawing.Size(139, 178);
            this.gbReadOutValues.TabIndex = 10;
            this.gbReadOutValues.TabStop = false;
            this.gbReadOutValues.Text = "Control Values";
            // 
            // lblDisplay
            // 
            this.lblDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblDisplay.Location = new System.Drawing.Point(6, 16);
            this.lblDisplay.Name = "lblDisplay";
            this.lblDisplay.Size = new System.Drawing.Size(127, 159);
            this.lblDisplay.TabIndex = 4;
            this.lblDisplay.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // gbAdditionalMessages
            // 
            this.gbAdditionalMessages.Controls.Add(this.lblAdMessages);
            this.gbAdditionalMessages.Location = new System.Drawing.Point(3, 181);
            this.gbAdditionalMessages.Name = "gbAdditionalMessages";
            this.gbAdditionalMessages.Size = new System.Drawing.Size(139, 80);
            this.gbAdditionalMessages.TabIndex = 11;
            this.gbAdditionalMessages.TabStop = false;
            this.gbAdditionalMessages.Text = "Debug Messages";
            // 
            // lblAdMessages
            // 
            this.lblAdMessages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblAdMessages.Location = new System.Drawing.Point(6, 16);
            this.lblAdMessages.Name = "lblAdMessages";
            this.lblAdMessages.Size = new System.Drawing.Size(127, 61);
            this.lblAdMessages.TabIndex = 4;
            this.lblAdMessages.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ReadOuts
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbAdditionalMessages);
            this.Controls.Add(this.gbReadOutValues);
            this.Name = "ReadOuts";
            this.Size = new System.Drawing.Size(145, 270);
            this.gbReadOutValues.ResumeLayout(false);
            this.gbAdditionalMessages.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.GroupBox gbReadOutValues;
        private System.Windows.Forms.Label lblDisplay;
        private System.Windows.Forms.GroupBox gbAdditionalMessages;
        private System.Windows.Forms.Label lblAdMessages;
    }
}
