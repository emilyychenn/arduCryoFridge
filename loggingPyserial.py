import serial
import serial.tools.list_ports
import logging
from datetime import date
import time
import os

usbPort = '/dev/cu.usbmodem14201'
baud = 9600
ser = serial.Serial(usbPort, baud)

# log all button interrupts as warnings
logger = logging.getLogger(__name__)
logger.setLevel(logging.WARNING)
consoleHandler = logging.StreamHandler()
consoleHandler.setLevel(logging.WARNING)
logger.addHandler(consoleHandler)

today = date.today()
file = today.strftime('%m-%d-%Y')
logging.basicConfig(filename=(file + '.log'), level=logging.DEBUG, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')

try:
    while True:
        time.sleep(0.05)
        logging.warning(ser.readline(ser.inWaiting()))
        print("logged status changed")
except KeyboardInterrupt:
    print("Logging stopped")
