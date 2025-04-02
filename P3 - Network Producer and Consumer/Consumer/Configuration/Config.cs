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
        public static int nConsumerThreads;
        public static int nMaxQueueLength;
        public static string ipAddress;
    }

    public struct Video
    {
        public string videoFilePath;
        public string thumbnailFileName;
        public string md5Hash;
    }

    public struct CollectionVideoList
    {
        public static List<Video> collectionVideoList = new List<Video>();
    }

    public struct Queue
    {
        public static Queue<byte[]> videoQueue = new Queue<byte[]>();
    }
}
