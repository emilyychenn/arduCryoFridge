import serial
import serial.tools.list_ports
import logging
from datetime import date
import time
import os

usbPort = '/dev/cu.usbmodem14101'
baud = 9600
ser = serial.Serial(usbPort, baud)

print(serial.__version__)

# log all button interrupts as warnings
logger = logging.getLogger(__name__)
logger.setLevel(logging.WARNING)
consoleHandler = logging.StreamHandler()
consoleHandler.setLevel(logging.WARNING)
logger.addHandler(consoleHandler)

today = date.today()
file = today.strftime('%m-%d-%Y')
logging.basicConfig(filename=(file + '.log'), level=logging.DEBUG, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')


# TODO: add condition to remove time.sleep?? needs to check if there has been a change, and then
# only display when there has been a change OR every hour
try:
    while True:
        if (ser.in_waiting != 0):
            time.sleep(0.05)
            ser.flush() # wait until all data is written
            warn = ser.readline(ser.in_waiting).strip(('\r\n').encode()) #removes '\r\n' at the end of each printed line
            # warn = ser.readline(ser.inWaiting()).strip(('\r\n').encode()) #removes '\r\n' at the end of each printed line
            logging.warning(warn)
            print("logged status changed")
        else:
            continue
except KeyboardInterrupt:
    print("\nLogging stopped")
