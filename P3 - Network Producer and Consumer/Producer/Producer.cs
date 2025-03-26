using Producer.Configuration;
using System.Net.Sockets;
using System.Threading;

namespace Project
{
    public partial class Producer : Form
    {

        bool isFunctionLoaded = false;
        int selectedFileCount = 0;

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
            string tempNumThreads = numThreadsInput.Text;

            // Convert tempNumThreads to Integer (https://stackoverflow.com/questions/2344411/how-to-convert-string-to-integer-in-c-sharp)
            int i = 0;
            int intNumThreads = 0;
            bool isSuccessConvert = int.TryParse(tempNumThreads, out i);
            if (isSuccessConvert == false)
            {
                LogMessage("[SYSTEM ERROR]: " + i + " - Invalid Input. Only numerical values are allowed.");
            }
            else
            {
                successChecks++; // 1st layer of check
                intNumThreads = int.Parse(tempNumThreads); // Convert to integer
            }

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

            // Button Change UI if all check layers passed
            if (successChecks == 2)
            {
                ConfigParameter.nProducerThreads = intNumThreads;

                LogMessage("[SYSTEM]: Successfully initialized to " + ConfigParameter.nProducerThreads + " producer thread/s.");

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

            // Start distributing the threads to different folders
            // We are using a ThreadPool library that is already provided by C# 
            // https://medium.com/@fairushyn/mastering-thread-pooling-in-c-37f538f6e649
            ThreadPool.SetMaxThreads(ConfigParameter.nProducerThreads, ConfigParameter.nProducerThreads); // This does not set the number of threads the thread pool contains; it only limits the number of threads the thread pool can use.

            foreach (string folderPath in ConfigFolders.foldersFilePath)
            {
                ThreadPool.QueueUserWorkItem(new WaitCallback(UploadFolder), folderPath);
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
        private void UploadFolder(object objFolderPath)
        {
            string folderPath = objFolderPath.ToString(); // This is necessary because QueueUserWorkItem gives an object reference

            if (folderPath != null)
            {
                // Open the folder
                string[] videoFiles = Directory.GetFiles(folderPath, "*.*", SearchOption.AllDirectories)
                                               .Where(file => file.EndsWith(".mp4", StringComparison.OrdinalIgnoreCase) ||
                                                              file.EndsWith(".mov", StringComparison.OrdinalIgnoreCase) ||
                                                              file.EndsWith(".avi", StringComparison.OrdinalIgnoreCase) )
                                               .ToArray();

                if ( videoFiles != null && videoFiles.Length > 0 )
                {
                    foreach (string videoFile in videoFiles)
                    {
                        LogMessage($"[SYSTEM]: Uploading {videoFile} from " + folderPath);
                        SendVideoToServerFromUI(videoFile);
                    }
                }

                // Read all video files format (.mp4, .mov, .avi) *common video format*
                //LogMessage("[SYSTEM]: (" + folderPath + ") video file/s successfully uploaded!");
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
    }
}