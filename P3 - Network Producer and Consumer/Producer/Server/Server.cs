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
            IPAddress localIp = ipHost.AddressList.FirstOrDefault(ip => ip.AddressFamily == AddressFamily.InterNetwork);

            IPEndPoint localEndPoint = new IPEndPoint(localIp, 1023); // Bind to all interfaces
            producer.LogMessage("[SERVER]: Preparing the local end point on...");

            Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); // Ensure IPv4
            producer.LogMessage("[SERVER]: Setting up socket and TCP connection...");

            listener.Bind(localEndPoint);
            producer.LogMessage($"[SERVER]: Connected on IPv4 Connection - ({localIp})");

            listener.Listen(5);
            producer.LogMessage("[SERVER]: Preparing for client connection...");

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
                        //producer.LogMessage("[SYSTEM]: Uploading " + filePath + "...");
                        ClientSettings.selectedSocket.Send(fileData);
                        producer.LogMessage("[SYSTEM]: Video has been successfully uploaded!");
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

            /*
            // Receive message from Client
            byte[] buffer = new byte[4096];
            int bytesReceived = clientSocket.Receive(buffer, SocketFlags.None);
            string clientSentData = System.Text.Encoding.UTF8.GetString(buffer, 0, bytesReceived);

            if (clientSentData == "exit")
            {
                clientSocket.Close();
            }
            */

            // Once communication is done, you can close the socket.
            //clientSocket.Shutdown(SocketShutdown.Both);
            //clientSocket.Close();
            //producer.LogMessage("[SERVER]: Client connection closed.");

            Thread thread = new Thread(() => ReceiveQueueFullMsg(clientSocket, producer));
            thread.IsBackground = true;
            thread.Start();
        }

        private static void SendUniqueClientSocket(Socket clientSocket, Producer producer)
        {
            string clientEndPoint = clientSocket.RemoteEndPoint.ToString();
            producer.LogMessage("[SERVER]: Sending Unique Identifier...");

            byte[] endPointBytes = System.Text.Encoding.UTF8.GetBytes(clientEndPoint);
            clientSocket.Send(endPointBytes);
            producer.LogMessage("[SERVER]: Unique identifier sent!");
        }

        private static void ReceiveQueueFullMsg(Socket clientSocket, Producer producer)
        {
            producer.LogMessage("[SERVER]: Message Receiver prepared...");
            while (true)
            {
                try
                {
                    byte[] buffer = new byte[1024];
                    int bytesReceived = clientSocket.Receive(buffer, SocketFlags.None);
                    string serverSentData = System.Text.Encoding.UTF8.GetString(buffer, 0, bytesReceived);
                    producer.LogMessage(serverSentData);
                } catch (SocketException ex)
                {
                    producer.LogMessage($"[SERVER ERROR]: {ex}");
                    break;
                }
            }
        }
    }
}
