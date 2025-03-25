using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Producer.Configuration
{
    // This is simply just a separate file for configuration parameters for cleanliness.
    // https://www.programiz.com/csharp-programming/struct
    public struct ConfigParameter
    {
        public int nConsumerThreads;
        public int nMaxQueueLength;
    }
    
}
