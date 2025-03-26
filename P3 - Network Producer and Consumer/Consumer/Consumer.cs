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
                // Create struct to contain the parameter
                ConfigParameter configParameter = new ConfigParameter();

                configParameter.nConsumerThreads = intNumThreads;

                LogMessage("[SYSTEM]: Successfully initialized to " + configParameter.nConsumerThreads + " consumer thread/s.");

                // Button UI
                mainBtn.Enabled = false;  // Grey out button
                mainBtn.Text = "UPLOAD"; // Change to upload text

                // Hide Input UI
                threadInputLabel.Visible = false;
                numThreadsInput.Visible = false;
            }
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

        internal void AddUIDTitle(Socket sender)
        {
            throw new NotImplementedException();
        }
    }
}