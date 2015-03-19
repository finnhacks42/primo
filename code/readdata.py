import serial

N = 100
FILE_NAME = "board.txt"


o = open(FILE_NAME,"w")

for config in range(4):
    raw_input("Please set board to configuration {}. Press any key to continue".format(config+1))
    ser = serial.Serial("/dev/ttyUSB0",9600)
    for i in range(N):
        chars = ser.readline()
        data = chars.strip()
        o.write(data+"\n")
        o.flush()

o.close()
print("DONE")
    


