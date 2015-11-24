import serial
from utils import *
from datetime import datetime as dt
import sys

"""This script can either read color response data from your programming board or from a previously saved file.
It will warn you if any of positions on the board cannot distinguish particular colors
and will then print out the data you need to paste into the top of your Arduino script"""

SAMPLES_PER_POSITION = 10 # this is the number of data points that will be taken for each setting of the board.
BAUDRATE = 9600 # The baudrate to read data from the serial port on. Should match that set in your Arduino enviroment.
COLORS = ["blue","red","green","yellow"] # A list of all the different color tiles you are using.
OUTPUT_TO_FILE = True # If True, the data will be written out to a timestamped file in the data directory 'board_response_dataYYYYmmddHHMM.csv'

def read_board_data():
    configs = configurations(COLORS)
    port = find_port()
    ser = serial.Serial(port,BAUDRATE)
    try:
        results = []
        for config in configs:
            config_string = ",".join(config)
            raw_input("Please set board to configuration: {}. Press any key to continue".format(config_string))
            for order in range(4):
                if order > 0:
                    raw_input("Please shuffle tiles within current color configuration. Press any key to continue")
                read_from_board(SAMPLES_PER_POSITION,lambda tile:config[tile/4],range(16),results)
    finally:
       ser.close()

    if OUTPUT_TO_FILE:
        filename = "data/board_response_data"+dt.strftime(dt.now(),'%Y%m%d%H%M')+".csv"
        write_results(filename,results)       
    return results

def print_seperation_information(failed,seps):
    if len(failed) > 0:
        print "WARNING, some colors could not be seperated for some tiles"
        print "tile,colors,seperation"
        for f in failed:
            print f
        print "------------------------------------------\n"
    else:
        print "Excelent, all colors were seperable for all tiles!\n"
        
    print "Seperations for all tiles and colors are:"
    print "-----------------------------------------------------------------------------------"
    for key,value in seps.iteritems():
        print str(key).ljust(2)," | ".join(value)
    print "-----------------------------------------------------------------------------------\n"

def calculate_seperations(centers):
    by_tile = {}
    for tile_color,center in centers.iteritems():
        tile,color = tile_color.split(",")
        tile = int(tile)
        if tile not in by_tile:
            by_tile[tile] = []
        by_tile[tile].append((color,center))
    
    seps = dict(zip(range(16),[[] for x in range(16)]))
    failed = []
    for tile in range(16):
        values = by_tile[tile]
        values.sort() # sort by color for consistancy
        for i,v1 in enumerate(values):
            for j,v2 in enumerate(values):
                if i > j:
                    s = int(round(separation(v1[1],v2[1])))
                    if s <= 0:
                        failed.append([tile,v1[0]+"-"+v2[0],s])
                    seps[tile].append(v1[0]+"-"+v2[0]+", "+str(s).ljust(3))
    return (failed,seps)

def print_center_data(centers):
    tmp = []
    for tile in range(16):
        tile_vals = []
        for color in COLORS:
            rgb = "{"+ ",".join([str(int(round(x))) for x in centers[str(tile)+","+color]])+"}"
            tile_vals.append(rgb)
        tile_vals = "{"+",".join(tile_vals)+"}"
        tmp.append(tile_vals)
    print "Paste the following into the top of the Arduino script\n"
    print "int centers[16][4][4] = {"+",".join(tmp)+"};"


if len(sys.argv) == 1:
    results = read_board_data()
else:
    results = read_results(sys.argv[1])


by_tile_color = group_data(results,range(3),[4,5])
centers_with_uncert = apply_to_group(by_tile_color,mean_plus_minus)
failed,seperations = calculate_seperations(centers_with_uncert)
print_seperation_information(failed,seperations)
centers = apply_to_group(by_tile_color,mean)
print_center_data(centers)
