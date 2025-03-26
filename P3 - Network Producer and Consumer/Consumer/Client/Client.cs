using System;
using System.Diagnostics;
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
            await Task.Run(() => ClientListener(consumer, sender, localEndPoint));
        }
        private static async Task ClientListener(Consumer consumer, Socket sender, IPEndPoint localEndPoint)
        {
            try
            {
                //while (true)
                //{

                //}

                // Connect to the server
                sender.Connect(localEndPoint);
                consumer.LogMessage("[SERVER]: Successfully connected to the server!");

                // Get unique identifier
                await GetUIDAsync(sender, consumer);

                // Get video files
                while ( sender.Available == 0)
                {
                    await Task.Delay(100); //Wait for 100 millisecond before checking for any new video file again.
                }
                string saveFolderDirectory = @"C:\Users\mauri\Desktop\Received\";
                await ReceiveVideoAsync(sender, consumer, saveFolderDirectory);
            }
            catch (Exception ex)
            {
                consumer.LogMessage("[SERVER]: Exception occurred: " + ex.Message);
            }
        }

        private static async Task GetUIDAsync(Socket sender, Consumer consumer)
        {
            byte[] buffer = new byte[1024];
            int bytesReceived = await sender.ReceiveAsync(buffer, SocketFlags.None);
            string serverSentData = System.Text.Encoding.UTF8.GetString(buffer, 0, bytesReceived);
            consumer.LogMessage("[SYSTEM]: Unique Identifier received!");
            consumer.AddUIDTitle(serverSentData);
        }

        private static async Task ReceiveVideoAsync(Socket sender, Consumer consumer, string saveDirectory)
        {
            try
            {
                // Ensure the directory exists, if not, create it
                if (!Directory.Exists(saveDirectory))
                {
                    Directory.CreateDirectory(saveDirectory);
                    consumer.LogMessage("[SERVER]: Directory created at " + saveDirectory);
                }

                while (true) // Loop to continually receive files
                {
                    // Receive the video file size
                    byte[] videoFileSize = new byte[sizeof(long)]; // Assumes long to represent large file sizes
                    int sizeBytesReceived = await sender.ReceiveAsync(videoFileSize, SocketFlags.None);

                    // If sizeBytesReceived is 0, connection has been closed
                    if (sizeBytesReceived == 0)
                    {
                        consumer.LogMessage("[SERVER]: Connection closed by client.");
                        break;
                    }

                    if (sizeBytesReceived != sizeof(long))
                    {
                        consumer.LogMessage("[SERVER]: Invalid file size received.");
                        continue; // Skip to next file
                    }

                    // Convert the byte array to long for actual file size
                    long fileSize = BitConverter.ToInt64(videoFileSize, 0);
                    consumer.LogMessage("[SERVER]: Video file size: " + fileSize + " bytes");

                    // Allocate buffer for the actual file
                    byte[] videoFileBuffer = new byte[fileSize];
                    long totalBytesReceived = 0;

                    // Receive the video file in chunks until all bytes are received
                    while (totalBytesReceived < fileSize)
                    {
                        int bytesReceived = await sender.ReceiveAsync(videoFileBuffer.AsMemory((int)totalBytesReceived), SocketFlags.None);
                        if (bytesReceived == 0)
                        {
                            consumer.LogMessage("[SERVER]: Connection closed unexpectedly while receiving file.");
                            break;
                        }
                        totalBytesReceived += bytesReceived;
                        consumer.LogMessage($"[SERVER]: Received {bytesReceived} bytes. Total so far: {totalBytesReceived}/{fileSize}");
                    }

                    if (totalBytesReceived == fileSize)
                    {
                        consumer.LogMessage("[SERVER]: Video file received successfully.");

                        // Save the received video file
                        string fileName = "received_video_" + DateTime.Now.Ticks + ".mp4"; // Use timestamp to ensure unique file names
                        string filePath = Path.Combine(saveDirectory, fileName);
                        await File.WriteAllBytesAsync(filePath, videoFileBuffer);
                        consumer.LogMessage("[SERVER]: Video file saved at " + filePath);
                    }
                    else
                    {
                        consumer.LogMessage("[SERVER]: Error: File size mismatch. Received: " + totalBytesReceived + " bytes, Expected: " + fileSize + " bytes.");
                    }
                }
            }
            catch (Exception ex)
            {
                consumer.LogMessage("[SERVER]: Error receiving video: " + ex.Message);
            }
        }
    }
}
