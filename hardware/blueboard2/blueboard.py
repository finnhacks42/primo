
# Currently Value seems to live on the F.Fab layer ... what does this mean for Gerber creation???
# TODO - Label pins on Arduino and on the Bluetooth module. Remove reference numbers for resistors and ldrs.
    
# put the leds in a grid
from pcbnew import *

leds = ["D"+str(i) for i in range(1,17)]
lres = ["R"+str(i) for i in range(49,65)]
rgbs = ["LED"+str(i) for i in range(1,17)]
ldrs = ["LDR"+str(i) for i in range(1,17)]

pcb = GetBoard()
index = 0
columns = 4
hspace = 51.8
vspace = 58.9

spacing = 55
rgap = 20


for m in pcb.GetModules():
    r = m.GetReference()
    if r.startswith("R") or r.startswith("LDR"):
        m.Value().SetPosition(m.GetCenter())

for tile in range(16):
    led = pcb.FindModuleByReference(leds[tile])
    resistor = pcb.FindModuleByReference(lres[tile])
    resistor.Value().SetPosition(resistor.GetCenter())
    ldr = pcb.FindModuleByReference(ldrs[tile])
    rgb = pcb.FindModuleByReference(rgbs[tile])                                                                      
    row = index/columns
    column = index%columns
    x = column*hspace
    y = row*vspace
    if column == 1:
        ldr.SetPosition(wxPointMM(x,y-5))
        rgb.SetPosition(wxPointMM(x,y))
        led.SetPosition(wxPointMM(x+rgap,y+2))
        resistor.SetPosition(wxPointMM(x+rgap,y+7))
        
    else:
        ldr.SetPosition(wxPointMM(x,y))
        rgb.SetPosition(wxPointMM(x,y-5))
        led.SetPosition(wxPointMM(x+rgap,y+2))
        resistor.SetPosition(wxPointMM(x+rgap,y+7))
    index +=1
            
    
    
def grid(spacing,rows,columns):
    index = 0
    pcb = GetBoard()
    for m in pcb.GetModules():
        row = index/columns
        column = index%columns
        p = wxPointMM(row*spacing,column*spacing)
        m.SetPosition(p)
        index +=1
