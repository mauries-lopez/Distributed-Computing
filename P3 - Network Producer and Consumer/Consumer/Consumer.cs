using System.Net.Sockets;
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
        public void AddUIDTitle (string uniqueConnection)
        {
            if ( this.InvokeRequired )
            {
                this.Invoke(new Action(() => AddUIDTitle(uniqueConnection)));
            } else
            {
                this.Text = "Consumer (" + uniqueConnection + ")";
            }
        }

        public string RetrieveIPAddress()
        {
            // Disable UI
            ipAddressInput.Visible = false;
            ipAddressLabel.Visible = false;

            return ipAddressInput.Text;
        }

        // To retrieve the inputs from the text boxes
        public void RetrieveParameters()
        {
            // Initialize layer of checking
            int successChecks = 0;

            // Retrieve inputs from the UI
            List<TextBox> parameters = [numThreadsInput, maxQueueLengthInput];

            // Validate input
            foreach (TextBox param in parameters)
            {
                var returnedValues = validateInput(param.Text);
                //returnedValues.Item1 = layersPassed
                //returnedValues.Item2 = converted string to int

                // Button Change UI if all check layers passed
                if (returnedValues.Item1 == 2)
                {
                    if (param.Name == "numThreadsInput")
                    {
                        ConfigParameter.nConsumerThreads = returnedValues.Item2;
                        LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nConsumerThreads + " consumer thread/s.");
                    }
                    else if (param.Name == "maxQueueLengthInput")
                    {
                        ConfigParameter.nMaxQueueLength = returnedValues.Item2;
                        LogMessage("[SYSTEM]: Successfully initialized " + ConfigParameter.nMaxQueueLength + " max queue length.");
                    }

                    // UI
                    threadInputLabel.Visible = false;

                    // Button UI
                    mainBtn.Visible = false;

                    // Hide Input UI
                    numThreadsInput.Visible = false;
                    maxQueueLengthLabel.Visible = false;
                    maxQueueLengthInput.Visible = false;
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

            // Retrieve IPv4
            RetrieveIPAddress();

            // Connect To Server
            Client.Client.ConnectToServer(this); //Server folder -> Server.cs -> Function
        }
    }
}