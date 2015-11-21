
def find_port():
    try:
        import serial
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

def read_from_board(samples,max_attempts,color_function,tile_indices,results):
    """Reads specified number of samples from the specified indices and adds to results"""
    ser.flushInput()
    n = 0
    attempts = 0
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
            # The first 16 are with everything off, next red, then green, then blue.
            off = row[0:16]
            red = subtract_background(row[16:16*2],off)
            green = subtract_background(row[16*2:16*3],off)
            blue = subtract_background(row[16*3:],off)
            for tile in tile_indices:
                color = color_function(tile)
                output = [red[tile],blue[tile],green[tile],off[tile],tile,color]
                results.append(output)


def write_results(filename,samples,color_names,results):
    with open(filename,"w") as o:
        o.write("# samples per position:"+str(samples)+"\n")
        o.write("# color names:"+",".join(color_names)+"\n")
        for row in results:
            o.write(",".join([str(x) for x in row])+"\n")
            


def read_results(f):
    pass


def configurations(color_list):
    l = range(len(color_list))
    result = []
    for i in l:
        order = [(x - i)%len(color_list) for x in l]
        colors = [color_list[i] for i in order]
        result.append(colors)
    return result


def matrix_apply(data,columns,groupby,function):
    """ data is a list of lists. groupby is indices, columns is indicies, groups rows by groupby columns, then applies aggregator function on each specified column."""
    m = {}
    for row in data:
        key = ",".join([str(row[i]) for i in groupby])
        if key not in m:
            m[key]= [[] for i in columns]
        for indx,i in enumerate(columns):
            m[key][indx].append(row[i])

    for key,value in m:
        m[key] = [function(lst) for lst in m[key]]

    return m
