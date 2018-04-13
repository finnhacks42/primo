import math
def find_port():
    try:
        from serial.tools import list_ports
        # detects connected communication ports.    
        valid_ports = [p[0] for p in list_ports.comports() if p[2] != 'n/a']
        if len(valid_ports) == 1:
            return valid_ports[0]
        elif len(valid_ports) == 0:
            return raw_input("Serial Port not detected automatically. Please enter port address:")
        else:
            mapping = dict(zip(range(1,len(valid_ports)+1),valid_ports))
            port_key = raw_input("Enter the number of the serial port your board is connected to: "+str(mapping))
            return mapping[int(port_key)]
    except (Exception):
        return raw_input("Serial Port not detected automatically. Please enter port address:")
        

def subtract_background(values,background):
    return [i - j for i, j in zip(values, background)]

def read_raw_data_from_board(samples,ser):
    ser.flushInput()
    n = 0
    attempts = 0
    max_attempts = samples*1.2
    results = []
    while (n < samples):
        attempts +=1
        if attempts > max_attempts:
            raise Exception("Maximum number of samples exceeded")
        chars = ser.readline()
        results.append(chars)
        n += 1
    return results
        

def read_from_board(samples,color_function,tile_indices,results,ser):
    """Reads specified number of samples from the specified indices and adds to results"""
    ser.flushInput()
    n = 0
    attempts = 0
    max_attempts = samples*1.2 # allow some serial corruption
    while (n < samples):
        attempts +=1
        if attempts > max_attempts:
            raise Exception("Maximum number of attempted samples exceeded")
        
        chars = ser.readline()
        data = chars.strip().strip(",")
        
        try:
            row = [int(x) for x in data.split(",")] # each row should be 16*4 long.
        except ValueError:
            continue
        
        if len(row) == 16*4:
            n +=1
            # each row is rgbo for each tile
            #off = row[0:16]
            #red = subtract_background(row[16:16*2],off)
            #green = subtract_background(row[16*2:16*3],off)
            #blue = subtract_background(row[16*3:],off)
            for tile in tile_indices:
                color = color_function(tile)
                off = row[tile*4+3]
                red = row[tile*4]-off
                green = row[tile*4+1]-off
                blue = row[tile*4+2]-off
                output = [red,green,blue,off,tile,color]
                #output = [red[tile],blue[tile],green[tile],off[tile],tile,color]
                results.append(output)


def write_results(filename,results):
    with open(filename,"w") as o:
        o.write("red,green,blue,off,position,color\n")
        for row in results:
            o.write(",".join([str(x) for x in row])+"\n")
            


def read_results(filename):
    results = []
    with open(filename,"r") as f:
        f.readline()
        for line in f.readlines():
            row = line.strip("\n").split(",")
            for i in range(5):
                row[i] = int(row[i])
            results.append(row)
    return results
        


def configurations(color_list):
    l = range(len(color_list))
    result = []
    for i in l:
        order = [(x - i)%len(color_list) for x in l]
        colors = [color_list[i] for i in order]
        result.append(colors)
    return result

def separation(centers1,centers2):
    """ Each centers is a list of tuples where the first value is the mean and the second is the uncertainty"""
    if len(centers1) != len(centers2):
        raise Exception("cannot calculate seperation between lists of different lenghts")
    u1 = max([c[1] for c in centers1])
    u2 = max([c[1] for c in centers2])
    z = zip(centers1,centers2)
    
    d = math.sqrt(sum([pow(x[0][0] - x[1][0],2) for x in z]))
    return d - u1 - u2
    

def group_data(results,columns,groupby):
    m = {}
    for row in results:
        key = ",".join([str(row[i]) for i in groupby])
        if key not in m:
            m[key]= [[] for i in columns]
        for indx,i in enumerate(columns):
            m[key][indx].append(row[i])
    return m

def apply_to_group(group,function):
    m = {}
    for key,value in group.iteritems():
        m[key] = [function(lst) for lst in value]
    return m

def mean(lst):
    return sum(lst)/float(len(lst))
     
def mean_plus_minus(lst):
    m = mean(lst)
    d1 = max(lst) - m
    d2 = m - min(lst)
    return (int(round(m)),int(round(max(d1,d2))))


