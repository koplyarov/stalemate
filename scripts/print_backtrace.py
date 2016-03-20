#!/usr/bin/python

import os
import pipes
import re
import subprocess
import sys


if len(sys.argv) != 2:
    print('Usage: ./print_backtrace.py <debug symbols>')
    exit(1)

file = sys.argv[1]

tmp = os.popen("nm -n " + file).read()

syms = []
r = re.compile('^([0-9A-Fa-f]+).*\s(\S+)$')
for s in tmp.split('\n'):
    m = r.match(s)
    if m:
        addr = int('0x' + m.group(1), 0)
        name = m.group(2)
        syms.append({'addr': addr, 'name': name})

demangler = subprocess.Popen("c++filt", stdin = subprocess.PIPE, stdout = subprocess.PIPE)

while True:
    try:
        backtrace = sys.stdin.readline()
    except KeyboardInterrupt:
        break

    if not backtrace:
        break

    print '================================================================================'

    for bt_addr_str in backtrace.split():
        bt_addr = int('0x' + bt_addr_str, 0)

        for i in xrange(1, len(syms)):
            if bt_addr >= syms[i - 1]['addr'] and bt_addr < syms[i]['addr']:
                symbol = syms[i - 1]['name']
                if demangler:
                        demangler.stdin.write(symbol.replace("__ZN", "_ZN") + "\n")
                        symbol = demangler.stdout.readline().strip()
                print "{0} +{1}".format(symbol, bt_addr - syms[i - 1]['addr'])
                break

    print '================================================================================'
