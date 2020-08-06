"""
Usage:
  serialWorker.py [--port=<USBportname>]
  serialWorker.py -h | --help

Options:
  --port=<USBportname>  Specify USB port.
  -h --help             Show this screen.

"""

from docopt import docopt
import serial
import time
import multiprocessing

# Change this to match your local settings
SERIAL_PORT = '/dev/ttyACM0'
baud = 9600


# will try to autodetect port first, if no port detected, will prompt user to input a port
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
    if not connected:
        print("No likely serial port found. Use command '--port=<USBportname>' to manually specify a port.")


if __name__ == "__main__":
    args = docopt(__doc__)  # docopt saves arguments and options as key:value pairs in a dictionary
    print(args)

    if args['--port'] is None:
        ser = autodetect()
    else:
        ser = serial.Serial(args['--port'], baud)


class SerialProcess(multiprocessing.Process):
 
    def __init__(self, input_queue, output_queue):
        multiprocessing.Process.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(SERIAL_PORT, baud, timeout=1)
 
    def close(self):
        self.sp.close()
 
    def write_serial(self, data):
        self.sp.write(data.encode())
        # time.sleep(1)
        
    def read_serial(self):
        return self.sp.readline()
 
    def run(self):
        self.sp.flushInput()

        while True:
            # look for incoming tornado request
            if not self.input_queue.empty():
                data = self.input_queue.get()
 
                # send it to the serial device
                self.write_serial(data)
                print("writing to serial: {}".format(data))
 
            # look for incoming serial data
            if self.sp.inWaiting() > 0:
                data = self.read_serial()
                print("reading from serial: {}".format(data))
                # send it back to tornado
                self.output_queue.put(data)
