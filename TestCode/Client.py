import socket
import time
import numpy

TCP_IP = '127.0.0.1'
TCP_PORT = 8888
stringArray = ["" for x in range(141)]
def createCoordinateArray():
    stringArraySize = 0
    for i in range(0,31):
        stringArray[stringArraySize] = i
        stringArraySize += 1
 #   for i in range(1, 41):
 #       stringArray[stringArraySize] = i,30
  #      stringArraySize += 1
    for i in range(30,-1,-1):
        stringArray[stringArraySize] = i
        stringArraySize += 1
 #   for i in range(40,-1,-1):
  #      stringArray[stringArraySize] = i,0
   #     stringArraySize += 1


def sendArrayToSocket():
    messageCounter = 0
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    while 1:
        for i in range(140):
            s.sendto(str(stringArray[messageCounter]).encode(),(TCP_IP, TCP_PORT))
            #print( type(stringArray[messageCounter]).__name__)
            messageCounter += 1
            print("Sended message : " + str(stringArray[messageCounter]))
            time.sleep(1)
        #s.close()
        messageCounter = 0
def main():
    createCoordinateArray()
    sendArrayToSocket()

main()