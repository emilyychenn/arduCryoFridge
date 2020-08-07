import serial
import time
import multiprocessing

baud = 9600


class SerialProcess(multiprocessing.Process):
 
    def __init__(self, input_queue, output_queue, serial_port):
        multiprocessing.Process.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial_port
 
    def close(self):
        self.sp.close()
 
    def write_serial(self, data):
        self.sp.write(data.encode())
        
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
