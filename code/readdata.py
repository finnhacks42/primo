import serial
from serial.tools import list_ports
from utils import *

SAMPLES_PER_POSITION = 10
BAUDRATE = 9600
COLORS = ["blue","red","green","yellow"]
OUTPUT_TO_FILE = True
FILE_NAME = "color_data2.csv"
PORT = find_port()
        
configs = configurations(COLORS)
ser = serial.Serial(PORT,BAUDRATE)
max_attempted_samples = SAMPLES_PER_POSITION*1.2 # allow a small failure corrupted data over serial line.

results = []
for config in configs:
    config_string = ",".join(config)
    raw_input("Please set board to configuration: {}. Press any key to continue".format(config_string))

    for order in range(4):
        raw_input("Please shuffle")
        ser.flushInput()
        samples = 0
        attempts = 0
        while (samples < SAMPLES_PER_POSITION):
            attempts +=1
            if attempts > max_attempted_samples:
                raise Exception("Maximum number of attempted samples exceeded")
            
            chars = ser.readline()
            data = chars.strip().strip(",")
            
            try:
                row = [int(x) for x in data.split(",")] # each row should be 16*4 long.
            except ValueError:
                continue
            
            if len(row) == 16*4:
                samples +=1
                # The first 16 are with everything off, next red, then green, then blue.
                off = row[0:16]
                red = subtract_background(row[16:16*2],off)
                green = subtract_background(row[16*2:16*3],off)
                blue = subtract_background(row[16*3:],off)
                for tile in range(16):
                    color = config[tile/4]
                    output = [red[tile],blue[tile],green[tile],off[tile],tile,color]
                    print output
                    results.append(output)
               

if OUTPUT_TO_FILE:
    with open(FILE_NAME,"w") as o:
        o.write("red,green,blue,off,position,color\n")
        for row in results:
            o.write(",".join([str(x) for x in row])+"\n")
print("DONE")
