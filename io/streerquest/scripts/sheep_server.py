import bge, GameLogic, mathutils
from math import radians
import socket
import pickle # serializing and de-serializing 
 
def main():
 
    cont = bge.logic.getCurrentController()
    owner = cont.owner
    host = "localhost"
    print(owner.name)
    
    ClientPort = 10001
        
    # Set socket client only one time at the first frame
    if not owner['OneTime'] :
        # Set UDP socket
        GameLogic.socketClient = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
     
        # Bind the socket to host
        GameLogic.socketClient.bind((host, ClientPort))
     
        # Set Buffer size
        GameLogic.socketClient.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024)
     
        # If no data are found, pass
        GameLogic.socketClient.setblocking(0)
     
        # Set prop to pass test
        owner['OneTime'] = True
     
    #------------if RECEIVE UDP --------------#
    try:
        # Get data in buffer
        UpData, SRIP = GameLogic.socketClient.recvfrom(1024)
     
        # De-serialize, return tuples
        test = pickle.loads(UpData)
        print (test)
        # Set Position and orientation
        owner.worldPosition = (test[0], test[1], test[2])
     
        # Orientation must be a matrix
        owner_ori = mathutils.Matrix(((test[3], test[4], test[5]) , (test[6], test[7], test[8]) , (test[9], test[10], test[11])))
        #owner.worldOrientation = owner_ori
    except :
        print('No data found')
        pass
        
 
main()