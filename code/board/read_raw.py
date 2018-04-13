import serial
from utils import find_port
from datetime import datetime as dt


BAUDRATE = 9600 # The baudrate to read data from the serial port on. Should match that set in your Arduino enviroment.

port = find_port()
print "Reading from ",port
filename = "data/raw_data_"+dt.strftime(dt.now(),'%Y%m%d%H%M')+".csv"

with serial.Serial(port,BAUDRATE) as ser, open(filename,'wb') as outFile:
    while True:
        line = ser.readline()
        print line
        print len(line.split(","))
        outFile.write(line)
        outFile.flush()
        

