import serial

N = 100
FILE_NAME = "board.txt"
PORT = "/dev/ttyUSB0"
BAUDRATE = 960
CONFIGS = ["blue, red, green, yellow","yellow, blue, red, green","green, yellow, blue, red", "red, green, yellow, blue"]


o = open(FILE_NAME,"w")
ser = serial.Serial("/dev/ttyUSB0",9600)


for config in range(4):
    # calculate mean from last round if not at start ...
    
    raw_input("Please set board to configuration {}. Press any key to continue".format(config+1))
    ser.flushInput()
    rowcount = 0
    reds = []
    greens = []
    blues = []
    
    for i in range(N):
        chars = ser.readline()
        data = chars.strip().strip(",")
        o.write(data+"\n")
        o.flush()

        row = [int(x) for x in data.split(",")] # each row should be 16*4 long.
        if len(row) == 16*4:
            rowcount +=1
            # The first 16 are with everything off, next red, then green, then blue.
            red = row[16:16*2] - row[0:16]
            green = row[16*2:16*3] - row[0:16]
            blue = row[16*3:] - row[0:16]
            reds.append(red)
            blues.append(blue)
            greens.append(green)
        

o.close()
print("DONE")
    
# for each tile I want the mean value for each color as an array (ie r,g,b)

