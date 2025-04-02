using System.Diagnostics;
using System.Windows.Forms.VisualStyles;
using Producer.Configuration;

namespace Project
{
    public partial class Consumer : Form
    {
        public Consumer()
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

        // Add application title to contain unique identifier
        public void AddUIDTitle(string uniqueConnection)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new Action(() => AddUIDTitle(uniqueConnection)));
            }
            else
            {
                this.Text = "Consumer (" + uniqueConnection + ")";
            }
        }

        // To retrieve the inputs from the text boxes
        public bool RetrieveParameters()
        {
            // Retrieve inputs from the UI
            List<TextBox> parameters = [numThreadsInput, maxQueueLengthInput, ipAddressInput];

            bool threadInput = false, queueInput = false, ipInput = false;

            // Validate input
            foreach (TextBox param in parameters)
            {
                var returnedValues = validateInput(param);
                //returnedValues.Item1 = layersPassed
                //returnedValues.Item2 = converted string to int/string

                // Button Change UI if all check layers passed
                if (returnedValues.Item1 == 2)
                {
                    if (param.Name == "numThreadsInput")
                    {
                        ConfigParameter.nConsumerThreads = (int)returnedValues.Item2;
                        threadInput = true;
                    }
                    else if (param.Name == "maxQueueLengthInput")
                    {
                        ConfigParameter.nMaxQueueLength = (int)returnedValues.Item2;
                        queueInput = true;
                    }
                    else if (param.Name == "ipAddressInput")
                    {
                        ConfigParameter.ipAddress = (string)returnedValues.Item2;
                        ipInput = true;
                    }
                }
            }

            if (threadInput && queueInput && ipInput)
            {
                LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nConsumerThreads + " consumer thread/s.");
                LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nMaxQueueLength + " max queue length.");
                LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.ipAddress + " as the IPv4 Address.");
                LogMessage("[SYSTEM]: Compressed video(s) is set to " + ConfigParameter.shouldCompress + ".");

                // Button UI
                mainBtn.Visible = false;

                // Hide Input UI
                numThreadsInput.Visible = false;
                maxQueueLengthLabel.Visible = false;
                maxQueueLengthInput.Visible = false;
                compressedCheckBox.Visible = false;
                compressedCheckLabel.Visible = false;

                return true;
            }
            else
            {
                return false;
            }
        }

        private (int, object) validateInput(TextBox param)
        {
            // Initialize layer of checking
            int successChecks = 0;

            // Get the value
            string input = param.Text;

            if (param.Name == "numThreadsInput" || param.Name == "maxQueueLengthInput")
            {
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
            else
            {
                // Validation for IP Address
                // https://stackoverflow.com/questions/11412956/what-is-the-best-way-of-validating-an-ip-address

                if (String.IsNullOrWhiteSpace(input))
                {
                    return (0, "");
                }

                string[] numDots = input.Split('.');
                if (numDots.Length != 4)
                {
                    return (0, "");
                }

                byte tempIpParsed;
                bool canParseIp = numDots.All(r => byte.TryParse(r, out tempIpParsed));

                if (canParseIp == true)
                {
                    return (2, input);
                }
                else
                {
                    return (0, "");
                }
            }

        }

        // INITIALIZE/UPLOAD button click event listener
        private void mainBtn_Click(object sender, EventArgs e)
        {
            // Retrieve Compressed Check Box
            ConfigParameter.shouldCompress = compressedCheckBox.Checked;

            // Retrieve input parameters
            bool isValidInputs = RetrieveParameters();

            if (isValidInputs == true)
            {
                // Load all Video UI related
                LoadVideoUI();

                // Connect To Server
                Client.Client.ConnectToServer(this); //Server folder -> Server.cs -> Function
            }
            else
            {
                LogMessage("[SYSTEM ERROR]: Invalid Input(s)...");
            }
        }

        private void LoadVideoUI()
        {
            // Disable UI
            ipAddressInput.Visible = false;
            mainBtn.Visible = false;
            maxQueueLengthInput.Visible = false;
            numThreadsInput.Visible = false;

            // Enable Video UI
            thumbnailListView.Visible = true;
            thumbnailListView.Focus();
            windowsMediaPlayer.Visible = true;

            threadInputLabel.Text = "Downloaded Video(s):";
            ipAddressLabel.Text = "Video Preview:";

            //https://stackoverflow.com/questions/17381725/c-sharp-listview-item-image
            // Preload the ImageList settings
            thumbnailImageList.ImageSize = new Size(80, 70); // Change size as needed

            Thread thread = new Thread(() => GetThumbnails());
            thread.IsBackground = true; // Make it a background thread
            thread.Start();
        }

        //https://www.youtube.com/watch?v=H8VApsezaZM
        private void GetThumbnails()
        {
            string desktopDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            string saveFolderDirectory = Path.Combine(desktopDirectory, "Received");
            string thumbnailsDir = Path.Combine(saveFolderDirectory, "thumbnails");

            List<string> loadedImages = new List<string>(); // Track loaded images

            while (true)
            {
                try
                {
                    if (Directory.Exists(thumbnailsDir) && CollectionVideoList.collectionVideoList.Count > 0)
                    {
                        string[] paths = Directory.GetFiles(thumbnailsDir);

                        foreach (string path in paths)
                        {
                            if (!loadedImages.Contains(path)) // Prevent duplicate loading
                            {
                                loadedImages.Add(path);
                                using (Image originalImg = Image.FromFile(path)) // Load original image
                                {
                                    Image thumbnail = new Bitmap(originalImg, new Size(100, 100)); // Resize to thumbnail
                                    string fileName = Path.GetFileName(path);

                                    // Ensure UI updates happen on the main thread
                                    if (thumbnailListView.InvokeRequired)
                                    {
                                        thumbnailListView.Invoke((MethodInvoker)delegate
                                        {
                                            thumbnailImageList.Images.Add(thumbnail);
                                            thumbnailListView.LargeImageList = thumbnailImageList;
                                            thumbnailListView.Items.Add(new ListViewItem { Text = fileName, ImageIndex = thumbnailImageList.Images.Count - 1 });
                                            thumbnailListView.Refresh();
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    LogMessage($"[SYSTEM ERROR]: {ex}");
                }

                Thread.Sleep(2000); // 2 second cooldown (Should refresh the listview box for every 2 seconds
            }
        }

        // Function used to generate a thumbnail in "thumbnail" folder for every successfully downloaded video.
        public void GenerateThumbnail(string videoPath, string thumbnailPath, int timeInSeconds = 5)
        {
            if (!File.Exists(videoPath))
            {
                throw new FileNotFoundException("Video file not found.");
            }

            string ffmpegPath = "ffmpeg";

            // FFmpeg command to extract a frame
            string arguments = $"-i \"{videoPath}\" -ss {timeInSeconds} -vframes 1 \"{thumbnailPath}\"";

            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = ffmpegPath,
                Arguments = arguments,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true
            };

            using (Process process = new Process { StartInfo = startInfo })
            {
                process.Start();
                process.WaitForExit();
            }
        }

        //https://www.youtube.com/watch?v=5jahCtOQI1k&t=211s
        private System.Windows.Forms.Timer playbackTimer = new System.Windows.Forms.Timer();
        private void thumbnailListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Check if the playbackTimer is already running
            if (playbackTimer.Enabled == true)
            {
                playbackTimer.Stop();
                playbackTimer.Dispose();
            }

            // Ensure that an item is actually selected
            if (thumbnailListView.SelectedItems.Count == 0)
            {
                return;
            }

            // Get the first selected item
            ListViewItem selectedItem = thumbnailListView.SelectedItems[0];

            if (selectedItem != null)
            {
                string fileName = selectedItem.Text;

                foreach (var video in CollectionVideoList.collectionVideoList)
                {
                    if (video.thumbnailFileName == fileName)
                    {
                        string videoPath = video.videoFilePath;

                        // Ensure the video file exists before playing
                        if (File.Exists(videoPath))
                        {
                            windowsMediaPlayer.URL = videoPath;
                            windowsMediaPlayer.Ctlcontrols.play(); // Start playback
                            playbackTimer.Interval = 10000; // 10 seconds

                            playbackTimer.Tick += (s, e) =>
                            {
                                windowsMediaPlayer.Ctlcontrols.stop(); // Stop playback after 10 seconds
                                playbackTimer.Stop(); // Stop the timer
                                playbackTimer.Dispose();
                            };

                            // Start the timer
                            playbackTimer.Start();
                        }
                        else
                        {
                            LogMessage($"[ERROR]: Video file not found - {videoPath}");
                        }
                        break;
                    }
                }
            }
        }

        private void thumbnailListView_MouseClick(object sender, MouseEventArgs e)
        {
            // Stop any ongoing playback
            windowsMediaPlayer.Ctlcontrols.stop();

            // Ensure there is at least one selected item
            if (thumbnailListView.SelectedItems.Count > 0)
            {
                ListViewItem selectedItem = thumbnailListView.SelectedItems[0];

                if (selectedItem != null)
                {
                    string fileName = selectedItem.Text;

                    foreach (var video in CollectionVideoList.collectionVideoList)
                    {
                        if (video.thumbnailFileName == fileName)
                        {
                            string videoPath = video.videoFilePath;

                            // Check if the video file exists before attempting to open it
                            if (File.Exists(videoPath))
                            {
                                try
                                {
                                    ProcessStartInfo psi = new ProcessStartInfo
                                    {
                                        FileName = videoPath,
                                        UseShellExecute = true
                                    };
                                    Process.Start(psi);
                                }
                                catch (Exception ex)
                                {
                                    LogMessage($"Error opening video: {ex.Message}");
                                }
                            }
                            else
                            {
                                LogMessage($"[ERROR]: Video file not found - {videoPath}");
                            }
                            break;
                        }
                    }
                }
            }
        }

        private void compressedCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if ( compressedCheckBox.Checked)
            {
                compressedCheckLabel.Visible = true;
            } else
            {
                compressedCheckLabel.Visible = false;
            }
        }
    }
}