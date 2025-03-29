using System;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Producer.Configuration;

namespace Project.Client
{
    internal class Client
    {
        public static async void ConnectToServer(Consumer consumer)
        {
            string serverIpAddress = consumer.RetrieveIPAddress(); // Replace with the actual IPv4 address of HOST (cmd -> ipconfig -> copy ipv4 address)
            int serverPort = 1023;

            try
            {
                consumer.LogMessage($"[SYSTEM]: Attempting to connect to {serverIpAddress}:{serverPort}...");

                IPAddress ipAddress = IPAddress.Parse(serverIpAddress);
                IPEndPoint remoteEndPoint = new IPEndPoint(ipAddress, serverPort);

                Socket sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                await sender.ConnectAsync(remoteEndPoint);
                consumer.LogMessage($"[SYSTEM]: Connected to {serverIpAddress}:{serverPort}.");

                // Start listening for data from the server
                await Task.Run(() => ClientListener(consumer, sender)); // Create ClientListener Method

            }
            catch (SocketException ex)
            {
                consumer.LogMessage($"[SYSTEM]: Connection failed: {ex.Message}");
            }
            catch (Exception ex)
            {
                consumer.LogMessage($"[SYSTEM]: An error occurred: {ex.Message}");
            }
        }
        private static async Task ClientListener(Consumer consumer, Socket sender)
        {
            try
            {
                // Get unique identifier
                await GetUIDAsync(sender, consumer);

                // Get video files
                while ( sender.Available == 0)
                {
                    await Task.Delay(100); //Wait for 100 millisecond before checking for any new video file again.
                }

                // Get the path to the user's Desktop directory
                string desktopDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                // Specify the folder or file path on the Desktop
                string saveFolderDirectory = System.IO.Path.Combine(desktopDirectory, "Received");

                // Start receiving the videos and queue them accordingly (this is in a different detached thread) so it can keep on receive and do queueing.
                StartReceivingInBackground(sender, consumer, saveFolderDirectory);

                // Start processing the queue
                StartVideoProcessingTask(consumer, saveFolderDirectory);
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

        private static void StartReceivingInBackground(Socket sender, Consumer consumer, string saveDirectory)
        {
            // Create a new thread and start it
            Thread thread = new Thread(() => KeepReceivingVideo(sender, consumer, saveDirectory));
            thread.IsBackground = true; // Make it a background thread
            thread.Start();
        }

        public static void StartVideoProcessingTask(Consumer consumer, string saveDirectory)
        {
            // Set the maximum number of threads in the thread pool to the value defined in the configuration
            //ThreadPool.SetMaxThreads(ConfigParameter.nConsumerThreads, ConfigParameter.nConsumerThreads);

            for (int i = 0; i < ConfigParameter.nConsumerThreads; i++)
            {
                /*
                // For each thread, run the StartProcessingVideo method
                Task.Run(() =>
                {
                    StartProcessingVideo(consumer, saveDirectory, i);
                });
                */

                Thread thread = new Thread(() => StartProcessingVideo(consumer, saveDirectory, i));
                thread.IsBackground = true; // Make it a background thread
                thread.Start();
            }
        }

        private static object fileWriteLock = new object();
        private static void StartProcessingVideo(Consumer consumer, string saveDirectory, int threadID)
        {
            while (true)
            {
                Video? completedVideo = null;

                // Lock to safely dequeue the video
                lock (Queue.videoQueue)
                {
                    // Check if there's any video to dequeue
                    if (Queue.videoQueue.Count > 0)
                    {
                        completedVideo = Queue.videoQueue.Dequeue(); // Dequeue the completed video
                        //consumer.LogMessage($"[SERVER]: Video dequeued. Queue size: {Queue.videoQueue.Count}");
                    }
                }

                // If a video was dequeued, process it outside the lock
                if (completedVideo.HasValue)
                {
                    // Process the dequeued video (e.g., save it to a file)
                    string fileName = $"video_{DateTime.Now.Ticks}.mp4";
                    string filePath = Path.Combine(saveDirectory, fileName);

                    lock (fileWriteLock)
                    {
                        consumer.LogMessage($"[SYSTEM - THREAD#{threadID}]: Downloading received video...");
                        File.WriteAllBytes(filePath, completedVideo.Value.videosByte); // Save video to disk
                        consumer.LogMessage($"[SYSTEM - THREAD#{threadID}]: Video successfully saved at {filePath}");
                    }
                }
            }
        }

        // This function is ran on a different single thread, its main functionality is to just keep on receiving video and queue them.
        private static void KeepReceivingVideo(Socket sender, Consumer consumer, string saveDirectory)
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
                    // Receive the video file size (first step)
                    byte[] videoFileSize = new byte[sizeof(long)]; // 8 bytes for file size
                    int sizeBytesReceived = sender.Receive(videoFileSize, SocketFlags.None);

                    // If sizeBytesReceived contains a value, it means there's a video being received.
                    if (sizeBytesReceived > 0)
                    {
                        // Retrieve the video file size
                        long fileSize = BitConverter.ToInt64(videoFileSize, 0);
                        byte[] videoFileBuffer = new byte[fileSize];
                        long totalBytesReceived = 0;

                        // Create video object
                        Video videoFile = new Video();
                        videoFile.videosByte = new byte[fileSize]; // This will create the buffer for the video.

                        bool isVideoQueued = false;

                        while (totalBytesReceived < fileSize)
                        {
                            int bytesReceived = sender.Receive(videoFileBuffer, (int)totalBytesReceived, (int)(fileSize - totalBytesReceived), SocketFlags.None); // Synchronous receive

                            if (bytesReceived == 0)
                            {
                                consumer.LogMessage("[SERVER]: Connection closed unexpectedly while receiving the file.");
                                break;
                            }

                            // Update the total bytes received
                            totalBytesReceived += bytesReceived;

                            // Copy the received bytes into the videoFile's videosByte array
                            Array.Copy(videoFileBuffer, 0, videoFile.videosByte, totalBytesReceived - bytesReceived, bytesReceived);

                            //consumer.LogMessage($"[SERVER]: Received {bytesReceived} bytes. Total bytes received: {totalBytesReceived} of {fileSize}");

                            // If part of the video is uploaded and it hasn't been queued yet
                            if (!isVideoQueued && totalBytesReceived == fileSize)
                            {
                                lock (Queue.videoQueue) // Ensure only one thread can enqueue at a time
                                {
                                    if (Queue.videoQueue.Count >= ConfigParameter.nMaxQueueLength)
                                    {
                                        consumer.LogMessage("[SERVER]: Queue full! Video skipped...");
                                    }
                                    else
                                    {
                                        Queue.videoQueue.Enqueue(videoFile); // Enqueue the video object
                                        //consumer.LogMessage($"[SERVER]: Video enqueued. Queue size: {Queue.videoQueue.Count}");
                                    }
                                }
                                isVideoQueued = true; // Prevent requeuing the same video file
                            }
                        }
                    }
                    else
                    {
                        consumer.LogMessage("[SERVER]: Connection closed by client.");
                        break;
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
