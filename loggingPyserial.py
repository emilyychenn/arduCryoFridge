import serial
import serial.tools.list_ports
import logging
from datetime import date, time, timedelta, datetime
import time
import os

usbPort = '/dev/cu.usbmodem14101'
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


# display status of buttons every hour on the hour
def giveStatusUpdate():
    x = datetime.today()
    y = x.replace(day=x.day, hour=x.hour, minute=58, second=0, microsecond=0)
    delta_t = y.minute-x.minute
    delta_str = str(delta_t)
        
    if(float(delta_str) <= 1.0):
        t = time.localtime()
        currentTime = time.strftime("%H:%M:%S", t)
        statusUpdate = ("Status Update: " + currentTime)
        logging.info(statusUpdate)
        print(statusUpdate)


try:
    while True:
        if (ser.in_waiting != 0):
            time.sleep(0.05)
            ser.flush() # wait until all data is written
            warn = ser.readline(ser.in_waiting).strip(('\r\n').encode()) #removes '\r\n' at the end of each line
            logging.warning(warn)
            print("logged status changed")
            giveStatusUpdate()
        else:
            continue
except KeyboardInterrupt:
    print("\nLogging stopped")

