import serial

COLORS = ["blue","red","green","yellow"]
PORT = "/dev/ttyUSB0"
BAUDRATE = 9600
SAMPLES_PER_POSITION = 10

def configurations(color_list):
    l = range(len(color_list))
    result = []
    for i in l:
        order = [(x - i)%len(color_list) for x in l]
        colors = [color_list[i] for i in order]
        result.append(colors)
    return result
        

configs = configurations(colors)
for config in configs:
    raw_input("Please set board to configuration {}. Press any key to continue".format(",".join(config)))
    ser = serial.Serial("/dev/ttyUSB0",9600)
    for i in range(SAMPLES_PER_POSITION):
        chars = ser.readline()
        data = chars.strip()
        print data
        


    


