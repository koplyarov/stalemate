#!/usr/bin/python

import os
import pipes
import re
import subprocess
import sys


def normalize_hex(hex_str):
    return hex_str if hex_str.startswith('0x') else '0x{}'.format(hex_str)


def hex_to_int(hex_str):
    return int(normalize_hex(hex_str), 0)


def main():
    if len(sys.argv) != 2:
        print('Usage: ./print_backtrace.py <debug symbols>')
        exit(1)

    filename = sys.argv[1]
    symbols_file = os.popen("nm -n " + filename).read()

    syms = []
    r = re.compile('^(?:0x)?([0-9A-Fa-f]+).*\s(\S+)$')
    for s in symbols_file.split('\n'):
        m = r.match(s)
        if m:
            addr = hex_to_int(m.group(1))
            name = m.group(2)
            syms.append({'addr': addr, 'name': name})

    demangler = subprocess.Popen("c++filt", stdin = subprocess.PIPE, stdout = subprocess.PIPE)

    print('Symbols file loaded successfully, you can paste the backtrace here.')
    print('Press Ctrl-C to exit.')
    while True:
        try:
            backtrace = sys.stdin.readline()
        except KeyboardInterrupt:
            break

        if not backtrace:
            break

        print '================================================================================'

        for bt_addr_str in backtrace.split():
            bt_addr = hex_to_int(bt_addr_str)

            for i in xrange(1, len(syms)):
                if bt_addr >= syms[i - 1]['addr'] and bt_addr < syms[i]['addr']:
                    symbol = syms[i - 1]['name']
                    if demangler:
                            demangler.stdin.write(symbol.replace("__ZN", "_ZN") + "\n")
                            symbol = demangler.stdout.readline().strip()
                    print "{0} +{1}".format(symbol, bt_addr - syms[i - 1]['addr'])
                    break

        print '================================================================================'


if __name__ == '__main__':
    main()
