namespace Quadrupel.Controls.MotorSpeed
{
    partial class DronesModes
    {
        private Modes.DroneStates _ActiveMode;
        /// <summary>
        /// Property to set ActiveMode of the Control
        /// </summary>
        public Modes.DroneStates ActiveMode
        { get
            {
                return _ActiveMode;
            }
          set
            {
                value = (((int)value > 9) ? 0 : value);
                this.lblModes[(int)_ActiveMode].BackColor = inActiveBackColor;
                _ActiveMode = value;
                this.lblModes[(int)_ActiveMode].BackColor = activeBackColor;
            }
        }

        /// <summary>
        /// Clear Active Coloration
        /// </summary>
        public void ClearFormating()
        {
            this.lblModes[(int)_ActiveMode].BackColor = inActiveBackColor;
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
            this.gpDoneModes = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lblModes = new System.Windows.Forms.Label[10];
            this.lblModes[0] = new System.Windows.Forms.Label();
            this.lblModes[3] = new System.Windows.Forms.Label();
            this.lblModes[1] = new System.Windows.Forms.Label();
            this.lblModes[2] = new System.Windows.Forms.Label();
            this.lblModes[8] = new System.Windows.Forms.Label();
            this.lblModes[7] = new System.Windows.Forms.Label();
            this.lblModes[6] = new System.Windows.Forms.Label();
            this.lblModes[5] = new System.Windows.Forms.Label();
            this.lblModes[4] = new System.Windows.Forms.Label();
            this.gpDoneModes.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // gpDoneModes
            // 
            this.gpDoneModes.Controls.Add(this.tableLayoutPanel1);
            this.gpDoneModes.Location = new System.Drawing.Point(4, 4);
            this.gpDoneModes.Name = "gpDoneModes";
            this.gpDoneModes.Size = new System.Drawing.Size(700, 81);
            this.gpDoneModes.TabIndex = 0;
            this.gpDoneModes.TabStop = false;
            this.gpDoneModes.Text = "Drone Mode";
            this.gpDoneModes.Enter += new System.EventHandler(this.gpDoneModes_Enter);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 9;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.11F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 11.12F));

            this.tableLayoutPanel1.Controls.Add(this.lblModes[0], 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[3], 3, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[1], 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[2], 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[8], 8, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[7], 7, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[6], 6, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[5], 5, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblModes[4], 4, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(7, 20);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 51F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(687, 51);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // lblSafe
            // 
            this.lblModes[0].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[0].AutoSize = true;
            this.lblModes[0].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[0].ForeColor = System.Drawing.Color.Black;
            this.lblModes[0].Location = new System.Drawing.Point(3, 0);
            this.lblModes[0].Name = "lblSafe";
            this.lblModes[0].Size = new System.Drawing.Size(70, 51);
            this.lblModes[0].TabIndex = 0;
            this.lblModes[0].Text = "Safe";
            this.lblModes[0].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label8
            // 
            this.lblModes[3].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[3].AutoSize = true;
            this.lblModes[3].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[3].ForeColor = System.Drawing.Color.Black;
            this.lblModes[3].Location = new System.Drawing.Point(231, 0);
            this.lblModes[3].Name = "label8";
            this.lblModes[3].Size = new System.Drawing.Size(70, 51);
            this.lblModes[3].TabIndex = 0;
            this.lblModes[3].Text = "Callibration";
            this.lblModes[3].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.lblModes[1].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[1].AutoSize = true;
            this.lblModes[1].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[1].ForeColor = System.Drawing.Color.Black;
            this.lblModes[1].Location = new System.Drawing.Point(79, 0);
            this.lblModes[1].Name = "label1";
            this.lblModes[1].Size = new System.Drawing.Size(70, 51);
            this.lblModes[1].TabIndex = 0;
            this.lblModes[1].Text = "Panic";
            this.lblModes[1].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.lblModes[2].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[2].AutoSize = true;
            this.lblModes[2].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[2].ForeColor = System.Drawing.Color.Black;
            this.lblModes[2].Location = new System.Drawing.Point(155, 0);
            this.lblModes[2].Name = "label2";
            this.lblModes[2].Size = new System.Drawing.Size(70, 51);
            this.lblModes[2].TabIndex = 0;
            this.lblModes[2].Text = "Manual";
            this.lblModes[2].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.lblModes[8].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[8].AutoSize = true;
            this.lblModes[8].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[8].ForeColor = System.Drawing.Color.Black;
            this.lblModes[8].Location = new System.Drawing.Point(611, 0);
            this.lblModes[8].Name = "label7";
            this.lblModes[8].Size = new System.Drawing.Size(73, 51);
            this.lblModes[8].TabIndex = 0;
            this.lblModes[8].Text = "Wireless";
            this.lblModes[8].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.lblModes[7].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[7].AutoSize = true;
            this.lblModes[7].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[7].ForeColor = System.Drawing.Color.Black;
            this.lblModes[7].Location = new System.Drawing.Point(535, 0);
            this.lblModes[7].Name = "label6";
            this.lblModes[7].Size = new System.Drawing.Size(70, 51);
            this.lblModes[7].TabIndex = 0;
            this.lblModes[7].Text = "Height Controlled";
            this.lblModes[7].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.lblModes[6].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[6].AutoSize = true;
            this.lblModes[6].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[6].ForeColor = System.Drawing.Color.Black;
            this.lblModes[6].Location = new System.Drawing.Point(459, 0);
            this.lblModes[6].Name = "label5";
            this.lblModes[6].Size = new System.Drawing.Size(70, 51);
            this.lblModes[6].TabIndex = 0;
            this.lblModes[6].Text = "Raw Mode";
            this.lblModes[6].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.lblModes[5].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[5].AutoSize = true;
            this.lblModes[5].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[5].ForeColor = System.Drawing.Color.Black;
            this.lblModes[5].Location = new System.Drawing.Point(383, 0);
            this.lblModes[5].Name = "label4";
            this.lblModes[5].Size = new System.Drawing.Size(70, 51);
            this.lblModes[5].TabIndex = 0;
            this.lblModes[5].Text = "Full Control";
            this.lblModes[5].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.lblModes[4].Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblModes[4].AutoSize = true;
            this.lblModes[4].BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.lblModes[4].ForeColor = System.Drawing.Color.Black;
            this.lblModes[4].Location = new System.Drawing.Point(307, 0);
            this.lblModes[4].Name = "label3";
            this.lblModes[4].Size = new System.Drawing.Size(70, 51);
            this.lblModes[4].TabIndex = 0;
            this.lblModes[4].Text = "Yaw Controlled";
            this.lblModes[4].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Modes
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gpDoneModes);
            this.Name = "Modes";
            this.Size = new System.Drawing.Size(707, 88);
            this.gpDoneModes.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gpDoneModes;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label[] lblModes;

    }
}
