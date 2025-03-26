using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Producer.Configuration;

namespace Project.Server
{

    internal class Server
    {
        // https://www.geeksforgeeks.org/socket-programming-in-c-sharp/
        // We are going to implement a TCP connection since we will be uploading a video file, and we don't want to use UDP for the connection because
        // if the packets are not received by the consumer, they will be dropped (packet loss), which will result in missing parts of the video file.
        // We want to ensure a stable internet connection and if it fails, we will just cut the upload.

        // The way the algorithm works is like a PEER-TO-PEER Connection
        // Producer = this is the SERVER/HOST, this will be the FIRST peer who will open the application. (The connection establishment will be done at the background on PRODUCER SIDE)
        // Consumer = 

        private static int clientID = 0;
        public static async void EstablishConnection(Producer producer)
        {
            IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHost.AddressList[0];
            producer.LogMessage("[SERVER]: Retrieving Local IP Address...");

            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 1023);
            producer.LogMessage("[SERVER]: Preparing the local end point on...");

            Socket listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            producer.LogMessage("[SERVER]: Setting up socket and TCP connection...");

            listener.Bind(localEndPoint);
            producer.LogMessage("[SERVER]: Binding socket to local end point...");

            listener.Listen(5);
            producer.LogMessage("[SERVER]: Preparing for client connection (max of 5)...");

            // Start listening for connections
            await Task.Run(() => ServerListener(producer, listener));
        }

        private static readonly object socketLock = new object();

        public static void SendVideoToClient(string filePath, Producer producer)
        {
            lock (socketLock)  // Locking the socket communication
            {
                try
                {
                    // 1. Send the file size
                    FileInfo fileInfo = new FileInfo(filePath);
                    long fileSize = fileInfo.Length;
                    byte[] fileSizeBytes = BitConverter.GetBytes(fileSize);
                    ClientSettings.selectedSocket.Send(fileSizeBytes);

                    // 2. Send the video file
                    byte[] fileData = File.ReadAllBytes(filePath);
                    if (ClientSettings.selectedSocket != null && ClientSettings.selectedSocket.Connected)
                    {
                        ClientSettings.selectedSocket.Send(fileData);
                        producer.LogMessage("[SYSTEM]: Video file (" + filePath + ") is being received by the consumer...");
                    }
                    else
                    {
                        producer.LogMessage("[ERROR]: Socket is not connected.");
                    }
                }
                catch (Exception ex)
                {
                    producer.LogMessage($"[ERROR]: Exception occurred: {ex.Message}");
                }
            }
        }


        private static async Task ServerListener(Producer producer, Socket listener)
        {
            try
            {
                while (true)
                {
                    producer.LogMessage("[SERVER]: Awaiting connection from clients...");

                    // Accept a client connection asynchronously
                    Socket clientSocket = await listener.AcceptAsync();
                    ClientSettings.clientID.Add(clientID);
                    ClientSettings.clientSocket.Add(clientSocket);
                    clientID++;
                    producer.LogMessage("[SERVER]: Client connected.");
                    producer.LogConnectedClient(clientSocket);

                    // Now you can handle the client communication here (e.g., receive/send data)
                    // You might spawn a separate thread/task to handle communication with this client.
                    await Task.Run(() => HandleClient(clientSocket, producer));
                }
            }
            catch (Exception ex)
            {
                producer.LogMessage("[SERVER]: Exception occurred: " + ex.Message);
            }
        }

        private static void HandleClient(Socket clientSocket, Producer producer)
        {
            producer.LogMessage("[SERVER]: Handling client communication...");

            SendUniqueClientSocket(clientSocket, producer);
            // You can add logic here for reading/writing data to the client.
            // For example: clientSocket.Receive(), clientSocket.Send() etc.

            // Once communication is done, you can close the socket.
            //clientSocket.Shutdown(SocketShutdown.Both);
            //clientSocket.Close();
            //producer.LogMessage("[SERVER]: Client connection closed.");
        }

        private static void SendUniqueClientSocket(Socket clientSocket, Producer producer)
        {
            string clientEndPoint = clientSocket.RemoteEndPoint.ToString();
            producer.LogMessage("[SERVER]: Sending Unique Identifier...");

            byte[] endPointBytes = System.Text.Encoding.UTF8.GetBytes(clientEndPoint);
            clientSocket.Send(endPointBytes);
            producer.LogMessage("[SERVER]: Unique identifier sent!");
        }
    }
}
