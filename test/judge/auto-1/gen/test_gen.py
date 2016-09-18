#!/usr/bin/env python3

import subprocess
import os.path

DIR_DATA  = "../data"
GENERATOR = "./generate_hands"
SOLVER = "./solver/solver"
SORTER = "./solver/sorter"
PREFIX = "test-"

def generate(name, size, seed, info):
    
    path_in = "{data}/{prefix}{name}.in".format(data=DIR_DATA, prefix=PREFIX, name=name)
    path_tmp = "{data}/{prefix}{name}.tmp".format(data=DIR_DATA, prefix=PREFIX, name=name)
    path_sol = "{data}/{prefix}{name}.sol".format(data=DIR_DATA, prefix=PREFIX, name=name)
    path_info = "{data}/{prefix}{name}.info".format(data=DIR_DATA, prefix=PREFIX,name=name)

    command_gen = "{gen} {path_in} {size} {seed}".format(
            gen=GENERATOR, path_in=path_in, size=size, seed=seed)
    command_sol = "{sol} {path_in} {path_tmp}".format(
            sol=SOLVER, path_in=path_in, path_tmp=path_tmp)
    command_sort = "{sort} {path_tmp} {path_sol} inc".format(
            sort=SORTER, path_tmp=path_tmp, path_sol=path_sol)
    command_cln = "rm {path_tmp}".format(path_tmp=path_tmp) 
    
    print("Generating '{path}' ...".format(path=path_in), end=' ')
    subprocess.call(command_gen, shell=True)
    if os.path.exists(path_in):
        print("Success")
    else:
        print("FAILED!")

    print("Solving '{path}' ...".format(path=path_in), end=' ')
    subprocess.call(command_sol, shell=True)
    if os.path.exists(path_tmp):
        print("Success")
    else:
        print("FAILED!")
    
    print("Finalising '{path}' ...".format(path=path_sol), end=' ')
    subprocess.call(command_sort, shell=True)
    if os.path.exists(path_sol):
        print("Success")
    else:
        print("FAILED!")
    
    print("Writing info.")
    file_info = open(path_info, "w")
    file_info.write(info)
    file_info.close()

    print("Cleaning up.")
    subprocess.call(command_cln, shell=True)

def make_stamp():

    command = "touch {data}/testdata.stamp".format(data=DIR_DATA)
    subprocess.call(command, shell=True)

def erase_data():

    print("Emptying {data} folder.".format(data=DIR_DATA))
    command = "rm {data}/*.{{in,sol,tmp,info}} 2>/dev/null 1>/dev/null".format(
            data=DIR_DATA)
    subprocess.call(command, shell=True)




testcases = []
testcases.append(("1", 20000000, 0, "Randomly generated 20,000,000 hands."))

erase_data()
for t in testcases:
    generate(t[0], t[1], t[2], t[3])

make_stamp()

