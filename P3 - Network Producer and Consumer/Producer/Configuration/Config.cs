using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Producer.Configuration
{
    // This is simply just a separate file for configuration parameters for cleanliness.
    // https://www.programiz.com/csharp-programming/struct
    public struct ConfigParameter
    {
        public static int nProducerThreads;
    }

    // Class for storing folder related information
    public class ConfigFolders
    {
        // Maximum of 10 Folders
        public static List<string> foldersFilePath = new List<string>();
    }

    public struct ClientSettings
    {
        public static List<int> clientID = new List<int>();
        public static List<Socket> clientSocket = new List<Socket>();
        public static Socket selectedSocket;
    }
}
