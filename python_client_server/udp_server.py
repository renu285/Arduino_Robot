import socket

UDP_IP = "192.168.178.20"
UDP_PORT = 8089 

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print "received message:", data
    response = raw_input("Please enter response: ")
    sock.sendto(response,addr);
