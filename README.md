# arduCryoFridge

A project to control and monitor the PT410 CryoFridge that's keeping the 7T MRI Magnet cool and running.

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