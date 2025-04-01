using Producer.Configuration;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;

namespace Project
{
    public partial class Producer : Form
    {

        bool isFunctionLoaded = false;
        int selectedFileCount = 0;

        // To limit the number of threads in the thread pool to be used.
        Semaphore semaphore;

        public Producer()
        {
            InitializeComponent();
            //this.Load += new EventHandler(LogBox_Load);  // Attach the form's Load event
        }

        // To print a message in the log box
        public void LogMessage(string message)
        {
            if (logBox.InvokeRequired)
            {
                logBox.Invoke(new Action(() => LogMessage(message)));
            }
            else
            {
                logBox.AppendText(message + Environment.NewLine);
            }
        }

        // To add the client in the list of connected clients
        public void LogConnectedClient(Socket client)
        {
            if (connectedClientList.InvokeRequired)
            {
                connectedClientList.Invoke(new Action(() => LogConnectedClient(client)));
            }
            else
            {
                // Use the client's remote IP address and port as the unique identifier
                string clientInfo = client.RemoteEndPoint.ToString();
                connectedClientList.Items.Add(clientInfo);
            }
        }

        // To retrieve the inputs from the text boxes
        public void RetrieveParameters()
        {
            // Initialize layer of checking
            int successChecks = 0;

            // Retrieve input from the UI
            List<TextBox> parameters = new List<TextBox>();
            parameters.Add(numThreadsInput); // [0]

            // Validate input
            foreach (TextBox param in parameters)
            {
                var returnedValues = validateInput(param.Text);
                //returnedValues.Item1 = layersPassed
                //returnedValues.Item2 = converted string to int

                // Button Change UI if all check layers passed
                if (returnedValues.Item1 == 2)
                {
                    if ( param.Name == "numThreadsInput")
                    {
                        ConfigParameter.nProducerThreads = returnedValues.Item2;
                        LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nProducerThreads + " producer thread/s.");
                    }
                    else if ( param.Name == "maxQLengthInput")
                    {
                        ConfigParameter.nMaxQLength = returnedValues.Item2;
                        LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nMaxQLength + " max queue length.");
                    }


                    // Button UI
                    mainBtn.Enabled = false;  // Grey out button
                    mainBtn.Text = "UPLOAD"; // Change to upload text

                    // Hide Input UI
                    numThreadsInput.Visible = false;

                    // Show Upload UI
                    threadInputLabel.Text = "Add a folder you would like to upload";
                    browseBtn.Visible = true;
                    selectedFileLabel.Visible = true;
                    selectedFileLog.Visible = true;
                }
            }
        }

        private (int, int) validateInput(string input)
        {
            // Initialize layer of checking
            int successChecks = 0;

            // Convert tempNumThreads to Integer (https://stackoverflow.com/questions/2344411/how-to-convert-string-to-integer-in-c-sharp)
            int i = 0;
            int intNumThreads = 0;
            bool isSuccessConvert = int.TryParse(input, out i);

            // 1st Layer
            if (isSuccessConvert == false)
            {
                LogMessage("[SYSTEM ERROR]: " + i + " - Invalid Input. Only numerical values are allowed.");
            }
            else
            {
                successChecks++; // 1st layer of check
                intNumThreads = int.Parse(input); // Convert to integer
            }

            // 2nd Layer
            // Check for validation
            if (intNumThreads <= 0 && isSuccessConvert == true) // Check for negative values
            {
                LogMessage("[SYSTEM ERROR]: Only positive values are allowed.");
            }
            else if ((intNumThreads >= int.MaxValue) && isSuccessConvert == true) // Check for maximum
            {
                LogMessage("[SYSTEM ERROR]: Value is too large.");
            }
            else
            {
                successChecks++;
            }

            return (successChecks, intNumThreads);
        }

        // INITIALIZE/UPLOAD button click event listener
        private void mainBtn_Click(object sender, EventArgs e)
        {
            // Retrieve input parameters
            RetrieveParameters();
        }

        // UPLOAD function for main button
        private void mainBtn_Upload_Click(object sender, EventArgs e)
        {
            LogMessage("[SYSTEM]: Uploading " + ConfigFolders.foldersFilePath.Count() + " selected file/s...");

            // Disable Browse button
            browseBtn.Enabled = false;

            // Show Progress Bar
            progressBar.Visible = true;
            progressBar.Maximum = ConfigFolders.foldersFilePath.Count();

            // Start distributing the threads to different folders
            // We are using a ThreadPool library that is already provided by C# 
            // https://medium.com/@fairushyn/mastering-thread-pooling-in-c-37f538f6e649

            semaphore = new Semaphore(ConfigParameter.nProducerThreads, ConfigParameter.nProducerThreads);

            while (ConfigFolders.foldersFilePath.Count != 0)
            {
                // Upload folder
                string folderPath = ConfigFolders.foldersFilePath[0];
                ConfigFolders.foldersFilePath.RemoveAt(0); // Remove the first element

                ThreadPool.QueueUserWorkItem(state =>
                {
                    // Wait for an available slot in the semaphore (blocking)
                    semaphore.WaitOne();

                    try
                    {
                        Interlocked.Increment(ref ConfigParameter.taskCounter);
                        UploadFolder(folderPath);
                    }
                    finally
                    {
                        semaphore.Release();
                        if (progressBar.InvokeRequired)
                        {
                            progressBar.Invoke(new Action(() =>
                            {
                                progressBar.Value += 1; 
                            }));
                        }
                    }
                });
            }

            // Disable Upload BTN until file/s upload complete
            mainBtn.Enabled = false;
            mainBtn.Text = "Uploading...";
        }

        // BROWSE Button click event listener
        private void browseBtn_Click(Object sender, EventArgs e)
        {
            // Unfortunately, there is no way of selecting multiple folders on one browsing. 
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                //Check if the selected file is already been selected
                if (ConfigFolders.foldersFilePath.Contains(folderBrowserDialog1.SelectedPath))
                {
                    LogMessage("[SYSTEM ERROR]: Selected folder has been added already.");
                }
                else
                {
                    selectedFileLog.AppendText("[" + selectedFileCount + "] " + folderBrowserDialog1.SelectedPath + Environment.NewLine);
                    ConfigFolders.foldersFilePath.Add(folderBrowserDialog1.SelectedPath);
                    selectedFileCount++;
                }
            }

            // If there is atleast one folder selected, allow to upload
            if (ConfigFolders.foldersFilePath.Count > 0 && isFunctionLoaded == false)
            {
                isFunctionLoaded = true; // So mainBtn.Click += ... does not continuously keep on adding new functionality to the button. This will only allow one pass in this if statement

                // Enable Upload Button
                mainBtn.Enabled = true;
                mainBtn.Click -= mainBtn_Click; // Remove previous functionality
                mainBtn.Click += new EventHandler(mainBtn_Upload_Click); // Add new functionality
            }
        }

        // This is the function used by the threads in ThreadPool
        // 1 thread 1 UploadFolder function -> this opens 1 folder

        // How our leaky bucket algorithm works:
        // 1. Each folder has its own thread
        // 2. Each thread will wait for each other to be ready
        // 3. if all threads are ready, all threads will try to queue their video files at the same time. Discarding all video files that didnt get to be queued. Note: while it did queue all the videos at the same time, these videos are being uploaded immediately as well so it can free some spaces in the queue.
        // 4. Each thread will take turns to upload A VIDEO!, thread 1 -> thread 2 -> thread 1 -> thread 2...

        // each producer thread will send their video files to a consumer thread??
        // OR!!!
        // 1 thread to send video files to consumer and consumer threads are used to save the video files?

        private void UploadFolder(object objFolderPath)
        {
            //LogMessage($"FIRST: {Interlocked.Read(ref ConfigParameter.taskCounter)}");
            string folderPath = objFolderPath.ToString(); // This is necessary because QueueUserWorkItem gives an object reference

            if (folderPath != null)
            {
                // Open the folder
                List<string> videoFilePaths = new List<string>();
                videoFilePaths.AddRange(Directory.GetFiles(folderPath, "*.*", SearchOption.AllDirectories)
                                                 .Where(file => file.EndsWith(".mp4", StringComparison.OrdinalIgnoreCase) ||
                                                                file.EndsWith(".mov", StringComparison.OrdinalIgnoreCase) ||
                                                                file.EndsWith(".avi", StringComparison.OrdinalIgnoreCase))
                                                 .ToArray());

                if (videoFilePaths != null && videoFilePaths.Count > 0)
                {
                    LogMessage($"[SYSTEM]: Uploading {folderPath}...");

                    for (int i = videoFilePaths.Count - 1; i >= 0; i--)
                    {
                        string videoFile = videoFilePaths[i];
                        SendVideoToServerFromUI(videoFile);

                        // Remove uploaded video
                        videoFilePaths.RemoveAt(i);
                    }
                    Interlocked.Decrement(ref ConfigParameter.taskCounter);
                    LogMessage($"[SYSTEM]: Folder Path - {folderPath} is successfully uploaded...");
                } else
                {
                    Interlocked.Decrement(ref ConfigParameter.taskCounter);
                    LogMessage($"[SYSTEM]: Folder Path - {folderPath} is successfully uploaded...");
                }

                //LogMessage($"SECOND: {Interlocked.Read(ref ConfigParameter.taskCounter)}");
                if (Interlocked.Read(ref ConfigParameter.taskCounter) == 0)
                {
                    // All tasks are finished, perform necessary actions
                    LogMessage("[SYSTEM]: All folders successfully uploaded.");
                    EnableUploadUI();
                };
            }
        }

        // A function to send the video files to the server
        private void SendVideoToServerFromUI(string videoFile)
        {
            Server.Server.SendVideoToClient(videoFile, this);
        }

        // A function for the selection of which client the videos will be sent to.
        private void connectedClientList_SelectedIndexChanged(object sender, EventArgs e)
        {
            //https://stackoverflow.com/questions/10553323/checkedlistbox-allowing-only-one-item-to-be-checked
            int idx = connectedClientList.SelectedIndex;

            for (int i =0; i < connectedClientList.Items.Count; i++)
            {
                if ( i != idx)
                {
                    connectedClientList.SetItemChecked(i, false);
                }
            }

            // Every select change, update the selected Socket Client
            if (ClientSettings.clientSocket != null && idx >= 0 && idx < ClientSettings.clientSocket.Count)
            {
                ClientSettings.selectedSocket = ClientSettings.clientSocket[idx];
                //LogMessage("[SYSTEM] Client Selected: " + ClientSettings.selectedSocket.RemoteEndPoint.ToString());
            }
        }

        public void EnableUploadUI()
        {
            if (browseBtn.InvokeRequired)
            {
                // Use Invoke to ensure the code is executed on the UI thread
                browseBtn.Invoke(new Action(() =>
                {

                    browseBtn.Enabled = true;

                    // Clear all folder variables
                    selectedFileLog.Clear();
                    ConfigFolders.foldersFilePath.Clear();

                    // Enable Main Button
                    mainBtn.Enabled = true;
                    mainBtn.Text = "UPLOAD";

                    // Progress bar
                    progressBar.Visible = false;
                    progressBar.Value = 0;
                }));
            }
            else
            {
                browseBtn.Enabled = true;

                // If already on the UI thread, execute the logic directly
                selectedFileLog.Clear();
                ConfigFolders.foldersFilePath.Clear();

                mainBtn.Enabled = true;
                mainBtn.Text = "UPLOAD";

                // Progress bar
                progressBar.Visible = false;
                progressBar.Value = 0;
            }
        }
    }
}