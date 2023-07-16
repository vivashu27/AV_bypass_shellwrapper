#!/usr/bin/python3.10

import sys

key = b"#12122$14112"
j = 0

def xor(plain: bytes):
    xored = b""
    j = 0
    for i in plain:
        xored += bytes([i ^ key[j % len(key)]])
        j += 1
    return xored

def shell_code(xored: bytes):
    shell = ""
    for i in xored:
        shell += "\\x" + hex(i)[2:].zfill(2)
    print(shell)

def main():
    if len(sys.argv) < 2:
        print("Usage: python encrypt.py <filename>")
        return

    filename = sys.argv[1]
    with open(filename, "rb") as f:
        plain = f.read()
        xored = xor(plain)
        shell_code(xored)

main()
