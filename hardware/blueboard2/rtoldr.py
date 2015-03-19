from pcbnew import *
pcb = GetBoard()

ldrs = ["LDR"+str(i) for i in range(1,17)]
res = ["R" + str(i) for i in range(2,17)]
res.append("R1")

for i in range(16):
    ldr = pcb.FindModuleByReference(ldrs[i])
    resistor = pcb.FindModuleByReference(res[i])
    resistor.SetPosition(ldr.GetCenter())
