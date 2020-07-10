"""
Usage:
  arduCryoFridgeCLI.py [--port=<USBportname>] configure [--ontime=<ontime>] [--offtime=<offtime>]
  arduCryoFridgeCLI.py [--port=<USBportname>] switch [--on | --off] [--now | --delay=<delay>]
  arduCryoFridgeCLI.py [--port=<USBportname>] (-s | --status)
  arduCryoFridgeCLI.py [--port=<USBportname>] -q
  arduCryoFridgeCLI.py -h | --help
  
Options:
  --port=<USBportname>  Specify USB port: done before running other commands.
  --ontime=<ontime>     Duration of ontime minutes.
  --offtime=<offtime>   Duration of offtime minutes.
  --delay=<delay>       Start on/off cycle in delay [default: 0] minutes.
  -s --status           Read out and report PT410 status.
  -q                    Query program version + version run on the arduino.
  -h --help             Show this screen.

"""

from docopt import docopt
import serial
import serial.tools.list_ports

baud = 9600
programVersion = 1.0


# will try to autodetect port first, if no port detected, will prompt user to input a port
# doesn't work with third-party Arduino knockoffs (in which case, user specifies port)
def autodetect():
    ports = serial.tools.list_ports.comports()
    connected = False
    print("Available ports: ")
    for port, desc, hwid in sorted(ports):
        print("{}: {} [{}]".format(port, desc, hwid))
        if desc == "USB2.0-Serial":
            try:
                ser = serial.Serial(port, baud)
                print("Connected to: " + port + '\n')
                connected = True
                return ser
            except Exception as e:
                print("\nCouldn't open port: " + str(e))
                ser = None
    if not(connected):
        print("No likely serial port found. Use command '--port=<USBportname>' to manually specify a port.")


if __name__ == "__main__":
    args = docopt(__doc__)  # docopt saves arguments and options as key:value pairs in a dictionary
    print(args)

    if args['--port'] == None:
        ser = autodetect()
    else:
        ser = serial.Serial(args['--port'], baud)

        
    if args['configure'] == True:
        if args['--ontime'] != None:
            ontime = args['--ontime']
            print("Ontime = " + ontime)
            ser.readline() # waits until arduino prints "UNO is ready!"
            ser.write(('A'+ str(ontime)).encode())
        elif args['--offtime'] != None:
            offtime = args['--offtime']
            print("Offtime = " + offtime)
            ser.readline()
            ser.write(('B'+ str(offtime)).encode())
            
    elif args['switch'] == True:
        if args['--on'] == True:
            if args['--now'] == True:
                print("switch compressor on NOW")
                ser.readline()
                ser.write('G'.encode())
            else:
                delay = args['--delay']
                print("delay turning on by " + str(delay) + " minutes")
                ser.readline()
                ser.write(('Z'+str(delay)).encode())
        elif args['--off'] == True:
            if args['--now'] == True:
                print("switch compressor off NOW")
                ser.readline()
                ser.write('X'.encode())
            else:
                delay = args['--delay']
                print("delay turning off by " + str(delay) + " minutes")
                ser.readline()
                ser.write(('Z'+str(delay)).encode())
                print(ser.readline())
                
    elif args['--status'] != False:
        print("PT410 status: ")
        ser.readline()
        ser.write('S'.encode())
        LEDStatus = ser.readline()
        print(LEDStatus)
        button1Status = ser.readline()
        print(button1Status)
        button2Status = ser.readline()
        print(button2Status)
        button3Status = ser.readline()
        print(button3Status)
        
    elif args['-q'] != False:
        print("Python program version: " + str(programVersion))
        ser.readline()
        ser.write('Q'.encode())
        arduinoProgramVersion = ser.readline()
        print(str(arduinoProgramVersion))

    else:
        print('nothing left to do')
                
            
