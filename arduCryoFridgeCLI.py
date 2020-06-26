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


if args['configure'] == True:
    if args['--ontime'] != None:
        # do the thing with the value
        ontime = args['--ontime']
        print("Ontime = " + ontime)
    elif args['--offtime'] != None:
        # do the thing with the value
        offtime = args['--offtime']
        print("Offtime = " + offtime)
        
elif args['switch'] == True:
    if args['--on'] != None:
        if args['--now'] != False:
            # switch compressor on NOW (equivalent to 'G' case in serial arduino interface)
            print("switch compressor on NOW")
        else:
            # delay turning on by 'delay' minutes
            print("delay turning on by 'delay' minutes")
    elif args['--off'] != None:
        if args['--now'] != False:
            # switch compressor off NOW (equivalent to 'X' case in serial arduino interface)
            print("switch compressor off NOW")
        else:
            # delay turning off by 'delay' minutes
            print("delay turning off by 'delay' minutes")
            
elif args['--status'] != False:
    # Read out and report PT410 status
    print("PT410 status: ")
    
elif args['-q'] != False:
    print("Python program version: " + str(programVersion))
    print("Arduino program version: ") # GET ARDUINO PROGRAM VERSION
            
        
