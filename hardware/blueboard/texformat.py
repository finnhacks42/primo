#!/usr/bin/env python

from pcbnew import *
pcb = GetBoard()
moduleCount = 0
for aModule in pcb.GetModules():
    aModule.Reference().SetVisible(1)
    aModule.Reference().SetHeight(1000000)
    aModule.Reference().SetWidth(1000000)
    aModule.Value().SetVisible(1)
    aModule.Value().SetHeight(800000)
    aModule.Value().SetWidth(800000)
    moduleCount = moduleCount + 1

mc_str = str(moduleCount)
print('TextFormat script updated the text of '+mc_str+' Modules.')
print('[Board: '+pcb.GetFileName()+']')
