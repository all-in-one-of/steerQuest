#!/usr/bin/python

import socket
import sys
import threading
#create an INET, STREAMing socket

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
# and a well-known port
print ('Binding to port')
serversocket.bind(('localhost', 8010))
#serversocket.bind('/tmp/socket_blender')
#become a server socket
serversocket.listen(5)

def dump_data(data):
    print(data)

#(clientsocket, address) = serversocket.accept()
while 1:
    #accept connections from outside
    (clientsocket, address) = serversocket.accept()
	#now do something with the clientsocket
    #inthis case, we'll pretend this is a threaded server
    print ('Client Connected')
    data = clientsocket.recv(2048)
    if not data: break
    '''t = threading.Thread(target = dump_data(data))
    t.start()'''
    print(data)
    clientsocket.close()


