import socket
import sys
import os

UDP_IP = sys.argv[1] 
UDP_PORT = 8089 

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
os.system('clear')
print "  -> Waiting for client to connect ... "
data, addr = sock.recvfrom(16) # buffer size is 1024 bytes

print "  -> Client connected              ... "
while True:
    print "Enter the command to the bot : "
    print " 1. Move forward() "
    print " 2. Move reverse() "
    print " 3. Move left() "
    print " 4. Move right() "
    print " 5. Stop() "

    # Receive command from user
    cmd = raw_input("Waiting for User command : ")
    #print "received message:", data
    sock.sendto(cmd,addr);
    os.system('clear')
