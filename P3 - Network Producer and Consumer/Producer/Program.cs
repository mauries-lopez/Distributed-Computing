using Producer.Configuration;

namespace Project
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();

            // Create Producer Instance
            Producer producer = new Producer();

            // Create Server
            Server.Server.EstablishConnection(producer); //Server folder -> Server.cs -> Function

            // Run Producer
            Application.Run(producer);
        }
    }
}