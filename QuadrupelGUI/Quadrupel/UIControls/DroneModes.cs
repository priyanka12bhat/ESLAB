using System;
using System.Drawing;
using System.Windows.Forms;

namespace Quadrupel.Controls.MotorSpeed
{
    public partial class DronesModes : UserControl
    {
        private Color inActiveBackColor = Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
        private Color activeBackColor = Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(255)))), ((int)(((byte)(0)))));
        public DronesModes()
        {
            InitializeComponent();
        }

        private void gpDoneModes_Enter(object sender, EventArgs e)
        {

        }
    }
}
