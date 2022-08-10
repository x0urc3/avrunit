#!/usr/bin/python3
# avrunit-result.py Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com>
#
# Unpack avrunit result from AVR eeprom or serial dump
#

import sys
import struct
import argparse
import serial
import time

AU_MAX_TEST = 20
AU_F_FAIL = 1
AU_F_SIZE = 4

sizeof_stat_t_size = 2
sizeof_record_t = 2

def help():
    print("USAGE: " + sys.argv[0] + " <file>")

def au_unpack(record_size, t_size, fmt, addr, flag_count):

    res =  [ [0] for i in range(flag_count) ]

    for i in range(record_size):
        start = addr
        end = start + t_size
        flag, id_test = struct.unpack(fmt, data[start:end])
        res[flag][0] += 1
        res[flag].append(id_test)
        addr += t_size
    return res

def print_record(flag_count):
    for flag in range(flag_count):

        flag_label = record_fmt[flag][0]
        flag_data = str(record[flag][0])

        id_label = record_fmt[flag][1]
        id_data = [hex(i) for i in record[flag][1:] if (i)]
        id_data = ", ".join(id_data)

        print(flag_label+":"+flag_data+"\t"+id_label+":"+id_data)

record = []
record_fmt = [
        ['P ','ID'],
        ['F ','ID'],
        ['B ','ID'],
        ['I ','ID']
        ]

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
            description="Dump avrunit result from EEPROM or Serial")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-f", "--file", help="EEPROM dump file")
    group.add_argument("-p", "--port", help="Serial port")
    parser.add_argument("-b", "--baud", type=int, help="Serial baud rate")
    args = parser.parse_args()
    if (args.port and args.baud is None):
        parser.error('Serial port requires --baud argument')

    if (args.file):
        with open(args.file, 'rb') as f:
            data = f.read()
    elif (args.port):
        s = serial.Serial(args.port, args.baud)

        time.sleep(2)
        s.write(b'r')

        data = s.read(sizeof_stat_t_size + (sizeof_record_t*AU_MAX_TEST))

    record_size = struct.unpack('H', data[0:sizeof_stat_t_size])[0]
    record = au_unpack(record_size, sizeof_record_t, 'BB', 2, AU_F_SIZE)

    print_record(AU_F_SIZE)

    sys.exit(record[AU_F_FAIL][0])
