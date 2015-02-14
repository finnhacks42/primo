import serial

ser = serial.Serial("/dev/ttyUSB1",9600)
color = "wheels"
o = open(color+".txt","w")

for i in xrange(1000):
    chars = ser.readline()
    data = chars.strip()
    o.write(data+"\n")
    o.flush()
o.close()
print "DONE"
    


