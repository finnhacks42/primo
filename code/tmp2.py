from math import sqrt
import numpy as np
import csv
import sklearn.preprocessing as prep
from serial.tools import list_ports
from itertools import combinations

#print list(list_ports.comports())

def load_data():
    with open("color_data2.csv") as f:
        x = []
        y = []
        data_iter = csv.reader(f)
        header = data_iter.next()
        for row in data_iter:
            x.append(row[:-1])
            y.append(row[-1])
        x = np.asarray(x,dtype=int)
        y = np.asarray(y)
        return x,y

def sort_by_column(array,column):
    return array[array[:,column].argsort()]

def print_diff(x,y):
    """ for each tile, prints out the differnces between the mean for that tile and the overall mean for that color"""
    results = []
    for color in ["red","green","blue","yellow"]:
        color_samples = sort_by_column(x[y==color],4)
        by_tile = np.split(color_samples,16)
        results.append((color_samples,by_tile))

    for i in range(16):
        for color_samples,by_tile in results:
            color_mean = np.mean(color_samples,axis=0).astype(int)
            tile_mean = np.mean(by_tile[i],axis=0).astype(int)
            diff = color_mean - tile_mean
            print diff,
        print ""

x,y = load_data()
colors = ["red","green","blue","yellow"]
centers = [[] for i in range(16)]
devs = [[] for i in range(16)]
for color in colors:
    color_samples = sort_by_column(x[y==color],4)[:,:-1]
    by_tile = np.split(color_samples,16)
    for i in range(16):
        tile_mean = np.mean(by_tile[i],axis=0).astype(int)
        tile_std = np.std(by_tile[i],axis=0)
        centers[i].append(tile_mean)
        devs[i].append(tile_std)

distances = []
for i in range(len(centers)):
    sd = devs[i]
    uncert_radii = [sqrt(sum((3*c[:-1])**2)) for c in devs[i]]
    for j, color1 in enumerate(colors):
        for k,color2 in enumerate(colors):
            if j > k:
                d = sqrt(sum((centers[i][k][:-1] - centers[i][j][:-1])**2))
                seperation = int(d - uncert_radii[j] - uncert_radii[k])       
                distances.append((i,color1,color2,seperation))
                
                
distances.sort(key = lambda x: x[3])
for d in distances:
    print d
    



        



# subtract a mean for each tile.



##import matplotlib.pyplot as plt
##from mpl_toolkits.mplot3d import Axes3D
##fig = plt.figure(figsize=(14,10))
##ax = fig.add_subplot(111, projection='3d') 
##ax.view_init(45, 45)
##ax.scatter(x[:,0], x[:,1], x[:,2], c=y, marker='o')
##ax.set_xlabel('Red')
##ax.set_ylabel('Green')
##ax.set_zlabel('Blue')
##plt.show()

# play with sklearn a bit 
        
    










def matrix_apply(data,columns,groupby,function):
    """ data is a list of lists. groupby is indices, columns is indicies, groups rows by groupby columns, then applies aggregator function on each specified column."""
    m = {}
    for row in data:
        key = ",".join([str(row[i]) for i in groupby])
        if key not in m:
            m[key]= [[] for i in columns]
        for indx,i in enumerate(columns):
            m[key][indx].append(row[i])

    
    for key,value in m.iteritems():
        m[key] = [function(lst) for lst in m[key]]

    return m


def median(lst):
    lst = sorted([int(x) for x in lst])
    return lst[len(lst)/2]



def distance(lst1,lst2):
    return sqrt(sum([pow(x - y,2) for x,y in zip(lst1,lst2)]))




def doplot():

    with open("color_data.csv") as data:
        data = [line.strip("\n").split(",") for line in data.readlines()]
        

    m = matrix_apply(data,range(3),[3,4],median)

    ##rows = []
    ##for key,value in m.iteritems():
    ##    num,col = key.split(",")
    ##    rows.append((int(num),col,value))
    ##
    ##from itertools import combinations
    ##
    ##rows = sorted(rows)
    ##for i in range(0,len(rows),4):
    ##    bgry = [rows[i][2],rows[i+1][2],rows[i+2][2],rows[i+3][2]]
    ##    print rows[i][0],bgry#,[int(distance(a,b)) for a,b in combinations(bgry,2)]

    x = []
    y = []
    z = []
    color = []
    for key,value in m.iteritems():
        print key,value
        x.append(value[0])
        y.append(value[1])
        z.append(value[2])
        color.append(key.split(",")[1])



    # plot in 3 dimensions, color by type
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    fig = plt.figure(figsize=(14,10))
    ax = fig.add_subplot(111, projection='3d') 
    ax.view_init(45, 45)
    ax.scatter(x, y, z, c=color, marker='o')
    ax.set_xlabel('Red')
    ax.set_ylabel('Green')
    ax.set_zlabel('Blue')
    plt.show()
