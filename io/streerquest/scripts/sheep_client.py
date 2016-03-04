# python 3
import GameLogic
import socket
import pickle # serializing and de-serializing
 
def main():
 
    # Get owner
    controller = GameLogic.getCurrentController()
    owner = controller.owner
    Host = 'localhost'
    ServerPort = 10000
     
    # Set socket server only one time at the first frame
    if not owner['OneTime']:
        # Set UDP socket
        GameLogic.sServer = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
     
        # Bind the socket to host
        GameLogic.sServer.bind((Host, ServerPort))
     
        # If no data are found, pass
        GameLogic.sServer.setblocking(0)
     
        # Set prop to pass test
        owner['OneTime'] = True
     
    # Get Position and Orientation
    PosCar = owner.worldPosition
    OriCar = owner.worldOrientation
     
    # New in python 3
    stupid = (PosCar[0], PosCar[1], PosCar[2], OriCar[0][0], OriCar[0][1], OriCar[0][2], OriCar[1][0], OriCar[1][1], OriCar[1][2], OriCar[2][0], OriCar[2][1], OriCar[2][2])
    print(stupid)
    # Serialize data
    Data = pickle.dumps(stupid)
     
    # Send Data to client
    GameLogic.sServer.sendto(Data, (Host, 10001))
    
    
main()