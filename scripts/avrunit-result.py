#!/usr/bin/python3
# avrunit-result.py Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com>
#
# Unpack avrunit result from AVR eeprom dump
#

import sys
import struct

AU_F_FAIL = 1
AU_F_SIZE = 4

stat_t_size = 1 + 10
stat_fmt_t_size = 2 + 2

def help():
    print("USAGE: " + sys.argv[0] + " <file>")

def au_unpack(t_size, fmt, addr, flag_count):
    res = []
    for i in range(flag_count):
        start = addr
        end = start + t_size
        d = struct.unpack(fmt, data[start:end])
        res.append(d)
        addr += t_size
    return res

def print_stat(flag_count):
    for flag in range(flag_count):

        flag_label = stat_fmt[flag][0].decode()
        flag_data = str(stat[flag][0])

        id_label = stat_fmt[flag][1].decode()
        id_data = [hex(i) for i in stat[flag][1:] if (i)]
        id_data = ", ".join(id_data)

        print(flag_label+":"+flag_data+"\t"+id_label+":"+id_data)

stat = []
stat_fmt = []

if __name__ == '__main__':
    if len(sys.argv) < 2:
        help()
    with open(sys.argv[1], 'rb') as f:
        data = f.read()

    #unpack stat_t
    stat = au_unpack(stat_t_size, 'B10B', 0, AU_F_SIZE)

    #unpack stat_fmt
    addr = stat_t_size * AU_F_SIZE
    stat_fmt = au_unpack(stat_fmt_t_size, '2s2s', addr, AU_F_SIZE)

    print_stat(AU_F_SIZE)

    sys.exit(stat[AU_F_FAIL][0])
