using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Project.Client
{
    internal class Client
    {
        public static async void ConnectToServer(Consumer consumer)
        {
            IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHost.AddressList[0];
            consumer.LogMessage("[SERVER]: Retrieving Local IP Address...");

            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 1023);
            consumer.LogMessage("[SERVER]: Preparing the local end point on...");

            Socket sender = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            consumer.LogMessage("[SERVER]: Setting up socket and TCP connection...");

            // Start listening for connections
            await Task.Run(() => ServerListener(consumer, sender, localEndPoint));
        }
        private static async Task ServerListener(Consumer consumer, Socket sender, IPEndPoint localEndPoint)
        {
            try
            {
                //while (true)
                //{
                    sender.Connect(localEndPoint);
                    consumer.LogMessage("[SERVER]: Successfully connected to the server!");
                //}
            }
            catch (Exception ex)
            {
                consumer.LogMessage("[SERVER]: Exception occurred: " + ex.Message);
            }
        }
    }
}
