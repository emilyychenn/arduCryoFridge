import serial
import time
import multiprocessing

## Change this to match your local settings
SERIAL_PORT = '/dev/cu.usbserial-1420'
SERIAL_BAUDRATE = 9600 #115200

class SerialProcess(multiprocessing.Process):
 
    def __init__(self, input_queue, output_queue):
        multiprocessing.Process.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1)
 
    def close(self):
        self.sp.close()
 
    def writeSerial(self, data):
        self.sp.write(data.encode())
        # time.sleep(1)
        
    def readSerial(self):
        return self.sp.readline()
 
    def run(self):
        self.sp.flushInput()

        while True:
            # look for incoming tornado request
            if not self.input_queue.empty():
                data = self.input_queue.get()
 
                # send it to the serial device
                self.writeSerial(data)
                print("writing to serial: {}".format(data))
 
            # look for incoming serial data
            if (self.sp.inWaiting() > 0):
                data = self.readSerial()
                print("reading from serial: {}".format(data))
                # send it back to tornado
                self.output_queue.put(data)
