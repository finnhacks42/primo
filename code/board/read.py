#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sun Mar 18 14:31:48 2018

@author: finn
"""

import serial
from utils import find_port
from datetime import datetime as dt
from time import sleep
from utils import read_results, group_data, apply_to_group, compute_normed_centers, mean, dict_of_lists_to_cstring
BAUDRATE = 9600 # The baudrate to read data from the serial port on. Should match that set in your Arduino enviroment.


def is_valid(line):
    return False

def parse_line(line,config):
    tile_readings = [l.strip(",") for l in line.split("|")]
    if len(tile_readings) != 16:
        return None
    
    for values in tile_readings:
        if len(values.split(",")) != 5:
            return None
        
    tile_rows = [int(v.split(",")[0])//4 for v in tile_readings]
    
    rows = [v.strip()+"," + config[r] for (v,r) in zip(tile_readings,tile_rows)]
    return "\n".join(rows)+"\n"

def parse_test_color_line(line,color):
    tile_readings = [l.strip(",") for l in line.split("|")]
    if len(tile_readings) != 16:
        return None
    
    tile1 = tile_readings[0]
    if len(tile1.split(",")) != 5:
        return None
    
    return tile1+","+color+"\n"
    

def test_colors(colors,samples_per_setting = 5,settings_per_config = 5):
    port = find_port()
    print "Reading from ",port
    filename = "data/colors_"+dt.strftime(dt.now(),'%Y%m%d%H%M')+".csv"
    
    with serial.Serial(port,BAUDRATE) as ser, open(filename,'wb') as outFile:
        for color in colors:
            raw_input("Please place {} tile in slot 1. Press any key to continue".format(color))
            sleep(1)
            ser.flushInput()
            for i in range(settings_per_config):
                count = 0
                if i > 0:
                    raw_input("Shuffle. Press any key to continue")
                    sleep(1)
                ser.flushInput()
                while count < samples_per_setting:
                    line = ser.readline()
                    
                    parsed = parse_test_color_line(line,color)
                    
                    if parsed is not None:
                        count +=1
                        outFile.write(parsed)
                        outFile.flush()
                        print "VALID",parsed
                    else:
                        print "INVALID",line
    
    return 
    

def read_board_data(samples_per_setting = 20, settings_per_config = 3):
    port = find_port()
    print "Reading from ",port
    configs = ["Red|Green|Blue|Yellow", "Yellow|Red|Green|Blue","Blue|Yellow|Red|Green","Green|Blue|Yellow|Red"]
    filename = "data/data_"+dt.strftime(dt.now(),'%Y%m%d%H%M')+".csv"
    with serial.Serial(port,BAUDRATE) as ser, open(filename,'wb') as outFile:
        for config in configs:
            config_list = config.split("|")
            raw_input("PLEASE SET BOARD TO CONFIGURATION: {}. Press any key to continue".format(config))
            sleep(5)
            ser.flushInput()
            for i in range(settings_per_config):
                count = 0
                if i > 0:
                    raw_input("Shuffle. Press any key to continue")
                    sleep(5)    
                    ser.flushInput()
                while count < samples_per_setting:
                    line = ser.readline()
                    
                    parsed = parse_line(line,config_list)
                    
                    if parsed is not None:
                        #line = line.strip()+","+config+"\n"
                        count +=1
                        outFile.write(parsed)
                        outFile.flush()
                        print "VALID",parsed
                    else:
                        print "INVALID",line
    return filename
        
                
                
    
def output_arduino_code_fragment(datafile):
    data = read_results(datafile)
    g = group_data(data,[2,3,4],[0],lambda k: "{:02d}".format(int(k)))
    average_brightness = apply_to_group(g, mean,lambda x: int(round(x)))
    centers = compute_normed_centers(data,average_brightness)
    print(dict_of_lists_to_cstring(average_brightness, sorted(average_brightness.keys()),'average_brightness','int'))
    print(dict_of_lists_to_cstring(centers, ['Blue','Green','Red','Yellow'],'centers','int')) 


datafile = read_board_data()
output_arduino_code_fragment(datafile)
            
