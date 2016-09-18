#!/usr/bin/env python3

import os
import os.path
import shutil
import subprocess
import filecmp

DIR_SETTINGS = "settings"
DIR_TESTDATA = "data"
DIR_IO = "io"

class Settings:
    def __init__(self,name):
        self._name = name
        self._info = "N/A"
        path_info = "{folder}/{name}.info".format(folder=DIR_SETTINGS, name=name)
        if os.path.exists(path_info):
            finfo = open(path_info, "r")
            self._info = finfo.read().strip()
            finfo.close()

class TestCase:
    def __init__(self,name):
        self._name = name
        self._info = "N/A"
        path_info = "{folder}/{name}.info".format(folder=DIR_TESTDATA, name=name)
        if os.path.exists(path_info):
            finfo = open(path_info, "r")
            self._info = finfo.read().strip()
            finfo.close()


def find_settings():
    list_settings = []
    list_dir = os.listdir(DIR_SETTINGS)
    for f in list_dir:
        name, ext = os.path.splitext(f)
        if ext == ".h":
            list_settings.append(Settings(name))
    return list_settings

def find_testcases():
    list_cases = []
    list_dir = os.listdir(DIR_TESTDATA)
    for f in list_dir:
        name, ext = os.path.splitext(f)
        if ext == ".in":
            list_cases.append(TestCase(name))
    return list_cases

def empty_io():
    for f in os.listdir(DIR_IO):
        os.remove(DIR_IO+"/"+f)

def prepare_settings(s):
    file_src = "{folder}/{file}.h".format(folder=DIR_SETTINGS, file=s._name)
    file_trg = "{folder}/settings.h".format(folder=DIR_IO, file=s._name)
    shutil.copy(file_src, file_trg)
    
    subprocess.call("make", shell=True)

    empty_io()

def prepare_test(t):
    file_in_src = "{folder}/{file}.in".format(folder=DIR_TESTDATA, file=t._name)
    file_in_trg = "{folder}/test.in".format(folder=DIR_IO)
    file_sol_src = "{folder}/{file}.sol".format(folder=DIR_TESTDATA, file=t._name)
    file_sol_trg = "{folder}/test.sol".format(folder=DIR_IO)
    shutil.copy(file_in_src, file_in_trg)
    shutil.copy(file_sol_src, file_sol_trg)

def run_test():
    command = "./test {folder}/test.in {folder}/test.tmp".format(folder=DIR_IO)
    subprocess.call(command, shell=True)

def run_sorter():
    command = "checker/sorter {folder}/test.tmp {folder}/test.out inc".format(
            folder=DIR_IO)
    subprocess.call(command, shell=True)

def check():
    file_out = "{folder}/test.out".format(folder=DIR_IO)
    file_sol = "{folder}/test.sol".format(folder=DIR_IO)
    result = filecmp.cmp(file_out, file_sol, shallow=False)
    print("Test passed: {result}".format(result=result))

def initialise():
    empty_io()
    subprocess.call("(cd checker && make)", shell=True)
    subprocess.call("make -f Makefile-tests", shell=True)


initialise()

all_settings = find_settings()
all_testcases = find_testcases()

for s in all_settings:
    prepare_settings(s)
    for t in all_testcases:
        prepare_test(t)
        run_test()
        run_sorter()
        check()
        empty_io()

empty_io()
