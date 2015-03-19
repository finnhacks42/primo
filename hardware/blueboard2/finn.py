
    
# put the leds in a grid
from pcbnew import *

for m in pcb.GetModules():
    print m.GetDescription()


def grid_LEDs(spacing,rows,columns):
    index = 0
    pcb = GetBoard()
    for m in pcb.GetModules():
        if "LED" in m.GetDescription():
            row = index/columns
            column = index%columns
            p = pcbnew.wxPointMM(row*spacing,column*spacing)
            m.SetPosition(p)
            index +=1
