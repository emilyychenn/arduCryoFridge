"""
Usage:
  arduCryoFridgeCLI.py configure [--ontime=<ontime>] [--offtime=<offtime>]
  arduCryoFridgeCLI.py switch [--on | --off] [--now | --delay=<delay>]
  arduCryoFridgeCLI.py -s | --status
  arduCryoFridgeCLI.py -h | --help
  arduCryoFridgeCLI.py -q
  
Options:
  --ontime=<ontime>     duration of ontime [default: 5] minutes.
  --offtime=<offtime>   duration of offtime [default: 5] minutes.
  --delay=<delay>       start on/off cycle in delay [default: 0] minutes.
  -s --status           Read out and report PT410 status
  -h --help             Show this screen.
  -q                    Query program version + version run on the arduino

"""

from docopt import docopt
import serial

arduino = serial.Serial('/dev/cu.usbmodem14101', 9600)
programVersion = 1.0


if __name__ == "__main__":
    args = docopt(__doc__)  # docopt saves arguments and options as key:value pairs in a dictionary
    print(args)


# NOTE: when testing in python IDLE, make sure "restart shell" is NOT selected!
if args['configure'] == True:
    if args['--ontime'] != None:
        # do the thing with the value
        ontime = args['--ontime']
        print("Ontime = " + ontime)
        arduino.write(('A'+ str(ontime)).encode())
    elif args['--offtime'] != None:
        # do the thing with the value
        offtime = args['--offtime']
        print("Offtime = " + offtime)
        arduino.write(('B'+ str(ontime)).encode())
        
elif args['switch'] == True:
    if args['--on'] == True:
        if args['--now'] == True:
            # switch compressor on NOW (equivalent to 'G' case in serial arduino interface)
            print("switch compressor on NOW")
            arduino.write('G'.encode())
        else:
            # delay turning on by 'delay' minutes
            delay = args['--delay']
            print("delay turning on by " + str(delay) + " minutes")
            arduino.write(('Z'+str(delay)).encode())
    elif args['--off'] == True:
        if args['--now'] == True:
            # switch compressor off NOW (equivalent to 'X' case in serial arduino interface)
            print("switch compressor off NOW")
            arduino.write('X'.encode())
        else:
            # delay turning off by 'delay' minutes
            delay = args['--delay']
            print("delay turning off by " + str(delay) + " minutes")
            arduino.write(('Z'+str(delay)).encode())
            
elif args['--status'] != False:
    # Read out and report PT410 status
    # CHECK: is this the status of the switches?
    print("PT410 status: ")
    arduino.write('S'.encode())
    arduinoStatus = arduino.readline()
    print(arduinoStatus)
    
elif args['-q'] != False:
    print("Python program version: " + str(programVersion))
    arduino.write('Q'.encode())
    arduinoProgramVersion = arduino.readline()
    print("Arduino program version: " + str(arduinoProgramVersion))
            
        
