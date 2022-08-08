#!/usr/bin/python3
# avrunit-result.py Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com>
#
# Unpack avrunit result from AVR eeprom dump
#

import sys
import struct

AU_F_FAIL = 1
AU_F_SIZE = 4

def help():
    print("USAGE: " + sys.argv[0] + " <file>")

def au_unpack(stat_size, t_size, fmt, addr, flag_count):

    res =  [ [0] for i in range(flag_count) ]

    for i in range(stat_size):
        start = addr
        end = start + t_size
        flag, id_test = struct.unpack(fmt, data[start:end])
        res[flag][0] += 1
        res[flag].append(id_test)
        addr += t_size
    return res

def print_stat(flag_count):
    for flag in range(flag_count):

        flag_label = stat_fmt[flag][0]
        flag_data = str(stat[flag][0])

        id_label = stat_fmt[flag][1]
        id_data = [hex(i) for i in stat[flag][1:] if (i)]
        id_data = ", ".join(id_data)

        print(flag_label+":"+flag_data+"\t"+id_label+":"+id_data)

stat = []
stat_fmt = [
        ['P ','ID'],
        ['F ','ID'],
        ['B ','ID'],
        ['I ','ID']
        ]

if __name__ == '__main__':
    if len(sys.argv) < 2:
        help()
    with open(sys.argv[1], 'rb') as f:
        data = f.read()

    stat_size = struct.unpack('H',data[0:2])[0]

    #unpack stat_t
    stat_t_size = 2
    stat = au_unpack(stat_size, stat_t_size, 'BB', 2, AU_F_SIZE)

    print_stat(AU_F_SIZE)

    sys.exit(stat[AU_F_FAIL][0])
