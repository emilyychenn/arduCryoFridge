# arduCryoFridge

A project to control and monitor the PT410 CryoFridge that's keeping the 7T MRI Magnet cool and running. 
Allows users to configure the duration of ontime and offtime for each on/off cycle, switch on or off immediately, 
and/or delay the start of an on/off cycle by a user-specified duration of time. 
Will log button press events (i.e. when a switch is manually flipped) to a logfile that is named after the current date.

## Installation

Download the package
```
git clone https://git.sarlab.ca/DrSAR/arducryofridge.git 
```

## Usage

Deploy the `ino` file to the arduino Uno using a stanfard Arduino development environment. 
Hook it up to the 17-pin connector using the wiring diagram. 
Connect the USB cable to arduino for power and communication.

Start the python program for CLI
```
 arduCryoFridgeCLI.py --status
```

or start server
```
arduCryoFridgeServer
```

Note: if the arduino port fails to autoconnect when running `arduCryoFridgeCLI.py`,
(i.e. the program lists all open ports but does not connect to any), 
manually specify the port where Arduino is connected with `arduCryoFridgeCLI.py [--port=<USBportname>]`.

## Command Line Arguments

```
Usage:
  arduCryoFridgeCLI.py [--port=<USBportname>] configure [--ontime=<ontime>] [--offtime=<offtime>]
  arduCryoFridgeCLI.py [--port=<USBportname>] switch [--on | --off] [--now | --delay=<delay>]
  arduCryoFridgeCLI.py [--port=<USBportname>] [-s | --status]
  arduCryoFridgeCLI.py [--port=<USBportname>] [-q]
  arduCryoFridgeCLI.py -h | --help
  
Options:
  --port=<USBportname>  Specify USB port: done before running other commands.
  --ontime=<ontime>     Duration of ontime minutes.
  --offtime=<offtime>   Duration of offtime minutes.
  --delay=<delay>       Start on/off cycle in delay [default: 0] minutes.
  -s --status           Read out and report PT410 status.
  -q                    Query program version + version run on the arduino.
  -h --help             Show this screen.

```