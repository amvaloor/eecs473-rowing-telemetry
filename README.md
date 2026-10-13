## EECS 473 Rowing Telemetry Project
The app/ folder contains all files for the android app
The firmware/ folder contains all files for the oar, central, and coach units

### App
TODO; Since the app implementation has not been started, this folder is empty and so cannot be committed.

### Firmware
The file structure is set up so that it can be edited in both the Arduino IDE and Neovim without issue. As a result, some of the files may look unnecessary or strange to you. If you do not know what a file does, please refrain from touching it.

#### Components
This folder contains each library or driver created for this project in a separate folder.

#### Src
This contains the code for each of the main units. They contain a header and cpp file for the implementation of the class corresponding to that unit and a .ino file, which is what is actually run by the microprocessor for that unit.

#### Test
Test cases to be run with platformio would go in this folder. It can be used for both unit testing and integration testing.

#### platformio.ini
This files handles how the project is built, compiled, and flashed. Does not do anything when you are using Arduino IDE, but useful if you are doing things via the command line.

### Scripts
This folder contains some scripts. You should run the appropriate setup script after cloning the repository if you are planning to compile and launch the code using the Arduino IDE. If you are not using Arduino IDE, you do not need to run either of the scripts. The .ps1 script is for Windows and the .sh script is for basically everything else.

### Makefile
The make commands are primarily to use build and flash via the command line. If you are using the Arduino IDE, these will not be particularly useful to you.
