#!/usr/bin/python3.10

import sys

def encrypt_payload(input_file, key):
    with open(input_file, 'rb') as f:
        payload = f.read()

    encrypted_payload = bytearray(payload)
    key_length = len(key)

    for i in range(len(encrypted_payload)):
        encrypted_payload[i] ^= ord(key[i % key_length])

    encrypted_payload_hex = ''.join([rf'\x{byte:02x}' for byte in encrypted_payload])
    print(encrypted_payload_hex)


def main():
    input_file = sys.argv[1]   
    key = "#12122$14112"    
    encrypt_payload(input_file, key)

main()
