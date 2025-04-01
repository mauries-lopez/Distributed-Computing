using System.Diagnostics.Eventing.Reader;
using System.Net;
using System.Net.Sockets;
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
                StartVideoProcessingTask(sender, consumer, saveFolderDirectory);
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
            
            //Task.Run(() => KeepReceivingVideo(sender, consumer, saveDirectory)); // Uses ThreadPool
        }

        public static void StartVideoProcessingTask(Socket sender, Consumer consumer, string saveDirectory)
        {
            
            for (int i = 0; i < ConfigParameter.nConsumerThreads; i++)
            {
                int threadIndex = i; // Capture loop variable
                ThreadPool.QueueUserWorkItem(state =>
                {
                    StartProcessingVideo(sender, consumer, saveDirectory, threadIndex);
                });
            }
        }

        private static object fileWriteLock = new object();
        private static void StartProcessingVideo(Socket sender, Consumer consumer, string saveDirectory, int threadID)
        {
            while (true)
            {
                byte[] videoFileBytes = null;

                // Lock to safely dequeue the video
                lock (Queue.videoQueue)
                {
                    // Check if there's any video to dequeue
                    if (Queue.videoQueue.Count > 0)
                    {
                        videoFileBytes = Queue.videoQueue.Dequeue(); // Dequeue the completed video
                        //consumer.LogMessage($"[SERVER]: Video dequeued. Queue size: {Queue.videoQueue.Count}");
                    }
                }

                // If a video was dequeued, process it outside the lock
                if (videoFileBytes != null)
                {
                    // Process the dequeued video (e.g., save it to a file)
                    string fileName = $"video_{DateTime.Now.Ticks}.mp4";
                    string filePath = Path.Combine(saveDirectory, fileName);

                    lock (fileWriteLock)
                    {
                        Video video = new Video();
                        
                        consumer.LogMessage($"[SYSTEM - THREAD#{threadID}]: Downloading received video...");
                        File.WriteAllBytes(filePath, videoFileBytes); // Save video to disk

                        video.videoFilePath = filePath;

                        // Ensure the 'thumbnails' directory exists inside saveDirectory
                        string thumbnailsDir = Path.Combine(saveDirectory, "thumbnails");
                        if (!Directory.Exists(thumbnailsDir))
                        {
                            Directory.CreateDirectory(thumbnailsDir);
                        }

                        // Generate thumbnail inside the 'thumbnails' directory
                        string thumbnailPath = Path.Combine(thumbnailsDir, $"{Path.GetFileNameWithoutExtension(fileName)}.jpg");
                        consumer.GenerateThumbnail(filePath, thumbnailPath);

                        video.thumbnailFileName = $"{Path.GetFileNameWithoutExtension(fileName)}.jpg";

                        CollectionVideoList.collectionVideoList.Add(video);

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
                        // Retrieve the video file
                        long fileSize = BitConverter.ToInt64(videoFileSize, 0);
                        byte[] videoFileBuffer = new byte[fileSize];
                        long totalBytesReceived = 0;

                        // Receive the video file in chunks until all bytes are received
                        while (totalBytesReceived < fileSize)
                        {
                            int bytesReceived = sender.Receive(videoFileBuffer, (int)totalBytesReceived, (int)(fileSize - totalBytesReceived), SocketFlags.None);
                            if (bytesReceived == 0)
                            {
                                consumer.LogMessage("[SERVER]: Connection closed unexpectedly while receiving file.");
                                break;
                            }
                            totalBytesReceived += bytesReceived;
                            //consumer.LogMessage($"[SERVER]: Received {bytesReceived} bytes. Total so far: {totalBytesReceived}/{fileSize}");
                        }

                        if (totalBytesReceived == fileSize)
                        {
                            lock (Queue.videoQueue)
                            {
                                if (Queue.videoQueue.Count >= ConfigParameter.nMaxQueueLength)
                                {
                                    consumer.LogMessage("[SERVER]: Queue full! Video skipped...");
                                }
                                else
                                {
                                    Queue.videoQueue.Enqueue(videoFileBuffer);
                                    consumer.LogMessage("[SERVER]: Video queued...");
                                }
                            }
                        }
                        else
                        {
                            consumer.LogMessage("[SERVER]: Error: File size mismatch. Received: " + totalBytesReceived + " bytes, Expected: " + fileSize + " bytes.");
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
