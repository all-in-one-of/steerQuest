#!/usr/bin/python

import socket

#create an INET, STREAMing socket
port = 8010
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
# and a well-known port
print 'Binding to port'
serversocket.bind(('localhost', port))

#become a server socket
serversocket.listen(5)

while 1:
	#accept connections from outside
	(clientsocket, address) = serversocket.accept()
	print 'Client Connected'
	#serversocket.setblocking(False)
	#now do something with the clientsocket
    #inthis case, we'll pretend this is a threaded server
    	
	
	data = clientsocket.recv(2048)
	if not data: break
	print data
	clientsocket.close()	

