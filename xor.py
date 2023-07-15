#!/usr/bin/python3.11

import sys

key="#12122$14112"
j=0

def xor(plain: str):
	xored=""
	j=0
	for i in plain:
		xored+=str(i^ord(key[j%len(key)]))
		j+=1
	return xored	

def shell_code(xored: str):
	shell=''
	for i in xored:
		shell+="\\x"+hex(ord(i)).replace("0x","")
	print(shell)

def main():
	s=sys.argv[1]
	f=open(s,"rb")
	plain=f.read()
	xored=str(xor(plain))
	shell_code(xored)


main()
