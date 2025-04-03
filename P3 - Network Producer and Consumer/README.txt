The application is very memory-intensive, especially on the Consumer side. The more threads you use, the more CPU and memory it will consume from your hardware.

For example, setting the Consumer thread count to 100 will still allow the program to run as intended, but the high memory and CPU usage may cause the system to freeze or crash the application. Use a high number of threads at your own risk.

-----------------------------------------------------
The applications uses two dependencies
(1) .NET Framework
(2) FFmpeg
-----------------------------------------------------

----[Virtual Machine (Producer) to PC (Consumer)]----
Prerequisite:
- Virtual Machine MUST have:
	(1) FFmpeg is installed, and its path is set in the system's environment variables.
	(2) Firewall is disabled for public and private network

SETTING UP: PRODUCER SIDE
(1) Drag and drop the "Producer" folder into the Virtual Machine. It is located inside the "Application Executables" folder.
(2) Double-click "setup.exe"
(3) Click "Install"
(4) In the log box, copy the IPv4 Connection. This will be used on the Consumer side.

SETTUP UP: CONSUMER SIDE
(1) Open "Consumer" folder that is located inside the "Application Executables" folder.
(2) Double-click "setup.exe"
(3) Click "Install"
(4) In the IPv4 Address of Host input box, paste the IPv4 Connection obtained from the Producer side.

An installation guide and demonstration are shown in the video named "Hermit_VMtoPC_Demonstration.mp4." You may watch it if you find the instructions confusing.
-----------------------------------------------------

----------[PC (Producer) to PC (Consumer)]-----------
Prerequisite:
- PC must have:
	(1) FFmpeg is installed, and its path is set in the system's environment variables.
	(2 *optional*) Disabled firewall & antivirus for public and private network if application does not work as intended.

SETTING UP: PRODUCER SIDE
(1) Open "Producer" folder that is located inside the "Application Executables" folder.
(2) Double-click "setup.exe"
(3) Click "Install"
(4) In the log box, copy the IPv4 Connection. This will be used on the Consumer side.

SETTUP UP: CONSUMER SIDE
(1) Open "Consumer" folder that is located inside the "Application Executables" folder.
(2) Double-click "setup.exe"
(3) Click "Install"
(4) In the IPv4 Address of Host input box, paste the IPv4 Connection obtained from the Producer side.

An installation guide and demonstration are shown in the video named "Hermit_PCtoPC_Demonstration.mp4." You may watch it if you find the instructions confusing.
-----------------------------------------------------