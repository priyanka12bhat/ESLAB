namespace Quadrupel.Controls.MotorSpeed
{
    partial class MotorSpeed
    {
        private int _motorCount = 0;
        /// <summary>
        /// Property to set Motor Number
        /// </summary>
        //Arun Geo Thomas
        public int MotorCount
        {
            get
            {
                return _motorCount;
            }
            set
            {

                _motorCount = value;
                lblMotor.Text = System.String.Format("Motor[{0}]", _motorCount);
            }
        }

        /// <summary>
        /// Property to set values in the control
        /// </summary>
        //Arun Geo Thomas
        public int Value { get
            { return progressBar.Value;
            }
            set
            {
                progressBar.Value = value > 1000 ? 1000:value;
                lblValue.Text = value.ToString();
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.lblMotor = new System.Windows.Forms.Label();
            this.lblValue = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // progressBar
            // 
            this.progressBar.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.progressBar.Location = new System.Drawing.Point(66, 3);
            this.progressBar.Maximum = 1000;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(176, 22);
            this.progressBar.TabIndex = 0;
            // 
            // lblMotor
            // 
            this.lblMotor.Location = new System.Drawing.Point(3, 3);
            this.lblMotor.Name = "lblMotor";
            this.lblMotor.Size = new System.Drawing.Size(57, 22);
            this.lblMotor.TabIndex = 1;
            this.lblMotor.Text = "Motor[0]";
            this.lblMotor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblValue
            // 
            this.lblValue.Location = new System.Drawing.Point(245, 4);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(34, 22);
            this.lblValue.TabIndex = 1;
            this.lblValue.Text = "0";
            this.lblValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // MotorSpeed
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblValue);
            this.Controls.Add(this.lblMotor);
            this.Controls.Add(this.progressBar);
            this.Name = "MotorSpeed";
            this.Size = new System.Drawing.Size(283, 29);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Label lblMotor;
        private System.Windows.Forms.Label lblValue;
    }
}
