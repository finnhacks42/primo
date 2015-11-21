import numpy as np
from math import sqrt
from itertools import combinations
from utils import *


"""
This script allows you to determine the distance between different color tiles.
"""

SAMPLES_PER_POSITION = 20
BAUDRATE = 9600


def calc_score(distances):
    """ takes as input all the distances between colors in a distribution.
        returns a score that will sort first by the smallest distance, then next smallest, etc. """
    return tuple(sorted(distances))
    

def best_set(n):
    if len(color_names) <= n:
        raise Exception("Can't ask for best set of {0} from only {1}".format(n,len(color_names)))
                        
    combs = list(combinations(color_names,n))
    scores = []
    for i,c in enumerate(combs):
        pairs = combinations(c,2)
        dists = [distances[p] for p in pairs]
        score = calc_score(dists)
        scores.append(score)
    combs = [sorted(list(c)) for c in combs]
    best = max(scores)
    z = zip(combs,scores)
    z.sort(key = lambda x:x[1])
    top = [x for x in z if x[1] >= best]
    for c in top:
        print c    
    return z  
    
port = find_port()
ser = serial.Serial(port,BAUDRATE)
max_attempted_samples = SAMPLES_PER_POSITION*1.2 # allow a small failure corrupted data over serial line.
colors = int(raw_input("Enter the number of colors to test: "))
indx = int(raw_input("Enter the index of the position you are testing in (from 0 to 15): "))


results = []
color_names = []
for color in range(colors):
    color_name = raw_input("Enter the name of the color you are testing: ")
    color_names.append(color_name)
    read_from_board(SAMPLES_PER_POSITION,max_attempted_samples,lambda(x):color_name,[indx],results)

with open("color_test.txt","w") as o:
    o.write(str(SAMPLES_PER_POSITION)+"\n")
    o.write(",".join(color_names)+"\n")
    for row in results:
        o.write(",".join([str(x) for x in row])+"\n")


data = np.zeros((colors*SAMPLES_PER_POSITION,5),dtype=int)
for i,row in enumerate(results):
    data[i,:] = row[0:-1]
data = np.split(data,colors)

centers = []
devs = []
for c in data:
    centers.append(np.median(c[:,0:3],axis=0))
    devs.append(np.std(c[:,0:3],axis=0))

distances = {}
for i,color1 in enumerate(color_names):
    for j,color2 in enumerate(color_names):
            d = sqrt(sum((centers[i] - centers[j])**2))
            distances[(color1,color2)] = d
            
print("The best set of 4 colors is:")
z = best_set(4)


