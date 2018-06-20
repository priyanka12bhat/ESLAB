using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quadrupel.Communication
{
    internal interface IPacketReceiver
    {
        Boolean CheckInputQueueCount();
        byte GetElementFromInputQueue();
        void ProcessPacket(Packet packet);
    }
}
