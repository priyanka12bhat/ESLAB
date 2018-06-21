namespace Quadrupel.Controls.MotorSpeed
{
    partial class MotorSpeeds
    {
        private int[] _MotorValues = new int[4];
        /// <summary>
        /// Property to set all Motor Values
        /// </summary>
        //Arun Geo Thomas
        public int[] Values {
            get {
                return _MotorValues;
            }
            set
            {
                value.CopyTo(_MotorValues,0);
                motorSpeed0.Value = _MotorValues[0];
                motorSpeed1.Value = _MotorValues[1];
                motorSpeed2.Value = _MotorValues[2];
                motorSpeed3.Value = _MotorValues[3];
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
            this.gbMotorSpeeds = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.motorSpeed0 = new Quadrupel.Controls.MotorSpeed.MotorSpeed();
            this.motorSpeed1 = new Quadrupel.Controls.MotorSpeed.MotorSpeed();
            this.motorSpeed2 = new Quadrupel.Controls.MotorSpeed.MotorSpeed();
            this.motorSpeed3 = new Quadrupel.Controls.MotorSpeed.MotorSpeed();
            this.gbMotorSpeeds.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbMotorSpeeds
            // 
            this.gbMotorSpeeds.Controls.Add(this.tableLayoutPanel1);
            this.gbMotorSpeeds.Location = new System.Drawing.Point(3, 3);
            this.gbMotorSpeeds.Name = "gbMotorSpeeds";
            this.gbMotorSpeeds.Size = new System.Drawing.Size(298, 165);
            this.gbMotorSpeeds.TabIndex = 1;
            this.gbMotorSpeeds.TabStop = false;
            this.gbMotorSpeeds.Text = "Motor Speeds";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Controls.Add(this.motorSpeed0, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.motorSpeed1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.motorSpeed2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.motorSpeed3, 0, 3);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(6, 19);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(285, 134);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // motorSpeed0
            // 
            this.motorSpeed0.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.motorSpeed0.Location = new System.Drawing.Point(3, 3);
            this.motorSpeed0.MotorCount = 0;
            this.motorSpeed0.Name = "motorSpeed0";
            this.motorSpeed0.Size = new System.Drawing.Size(279, 27);
            this.motorSpeed0.TabIndex = 0;
            this.motorSpeed0.Value = 0;
            // 
            // motorSpeed1
            // 
            this.motorSpeed1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.motorSpeed1.Location = new System.Drawing.Point(3, 36);
            this.motorSpeed1.MotorCount = 1;
            this.motorSpeed1.Name = "motorSpeed1";
            this.motorSpeed1.Size = new System.Drawing.Size(279, 27);
            this.motorSpeed1.TabIndex = 1;
            this.motorSpeed1.Value = 0;
            // 
            // motorSpeed2
            // 
            this.motorSpeed2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.motorSpeed2.Location = new System.Drawing.Point(3, 69);
            this.motorSpeed2.MotorCount = 2;
            this.motorSpeed2.Name = "motorSpeed2";
            this.motorSpeed2.Size = new System.Drawing.Size(279, 27);
            this.motorSpeed2.TabIndex = 1;
            this.motorSpeed2.Value = 0;
            // 
            // motorSpeed3
            // 
            this.motorSpeed3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.motorSpeed3.Location = new System.Drawing.Point(3, 102);
            this.motorSpeed3.MotorCount = 3;
            this.motorSpeed3.Name = "motorSpeed3";
            this.motorSpeed3.Size = new System.Drawing.Size(279, 29);
            this.motorSpeed3.TabIndex = 1;
            this.motorSpeed3.Value = 0;
            // 
            // MotorSpeeds
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbMotorSpeeds);
            this.Name = "MotorSpeeds";
            this.Size = new System.Drawing.Size(304, 171);
            this.gbMotorSpeeds.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbMotorSpeeds;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private MotorSpeed motorSpeed0;
        private MotorSpeed motorSpeed1;
        private MotorSpeed motorSpeed2;
        private MotorSpeed motorSpeed3;
    }
}
