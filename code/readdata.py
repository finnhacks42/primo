import serial

ser = serial.Serial("/dev/ttyACM0",9600)
color = "yellow"
o = open("color_"+color+".txt","w")

for i in xrange(100):
    chars = ser.readline()
    data = chars.strip()
    o.write(data+","+color+"\n")
    o.flush()
o.close()

print "DONE"
    


