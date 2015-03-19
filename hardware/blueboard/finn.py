
    
# put the leds in a grid

def grid_LEDs(pcbnew,spacing,rows,columns):
    index = 0
    pcb = pcbnew.GetBoard()
    for m in pcb.GetModules():
        if "LED" in m.GetDescription():
            row = index/columns
            column = index%columns
            p = pcbnew.wxPointMM(row*spacing,column*spacing)
            m.SetPosition(p)
            index +=1
