import socket
import struct

#####################################################################################

# UDP listening configs
LocalIP     = "0.0.0.0"
LocalPort   = 4000

# Message buffer size
BufferSize  = 36

Debug = True

#####################################################################################


# Setup UDP listening
def setupServer():
    # Create a datagram socket
    UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    # Bind to address and ip
    UDPServerSocket.bind((LocalIP, LocalPort))
    return UDPServerSocket

def n2h(Number, Pad):
    return hex(Number).center(Pad, " ")

def debugPacket(Packet):
    (PackVersion, PackMsgID, _1,
     PackMCUID,
     PackCRC,
     PackVoltage, PackDuty, PackFreq,
     PackTemp, PackHumid,
     PackRPM, PackDigits, _2,
     PackGyroX, PackGyroY,
     PackGyroZ, PackAccX,
     PackAccY, PackAccZ) = Packet
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|{}|".format(n2h(PackVersion, 8), n2h(PackMsgID, 8), n2h(_1, 17)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|".format(n2h(PackMCUID, 35)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|".format(n2h(PackCRC, 35)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|{}|".format(n2h(PackVoltage, 17), n2h(PackDuty, 8), n2h(PackFreq, 8)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|".format(n2h(PackTemp, 17), n2h(PackHumid, 17)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|{}|".format(n2h(PackRPM, 17), n2h(PackDigits, 8), n2h(_2, 8)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|".format(n2h(PackGyroX, 17), n2h(PackGyroY, 17)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|".format(n2h(PackGyroZ, 17), n2h(PackAccX, 17)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|".format(n2h(PackAccY, 17), n2h(PackAccZ, 17)))
    print("\t\t+--------+--------+--------+--------+")
    

# Gets message, processes and stores to db and returns non-zero MessageID upon successfull operation
def processMessage(Bytes):
    Packet = struct.unpack('<BBHLLHBBHHHBBHHHHHH', Bytes)
    if Debug: debugPacket(Packet)
    # Extract fields
    (PackVersion, PackMsgID, _,
     PackMCUID,
     PackCRC,
     PackVoltage, PackDuty, PackFreq,
     PackTemp, PackHumid,
     PackRPM, PackDigits, _,
     PackGyroX, PackGyroY,
     PackGyroZ, PackAccX,
     PackAccY, PackAccZ) = Packet

    return PackMsgID


# Send ACK Message to client
def sendACK(Socket, ClientAddress, MessageID):
    Packet = struct.pack('<BB', 0x01, MessageID)
    Socket.sendto(Packet, ClientAddress)


Server = setupServer()
print("UDP server up and listening...\n\n")

# Listen for incoming datagrams
while(True):
    BytesPair = Server.recvfrom(BufferSize)
    ClientAddress = BytesPair[1]
    if Debug: print("Message Received from:", ClientAddress)
    MessageID = processMessage(BytesPair[0])

    if MessageID:
        # Sending a ACK to client
        sendACK(Server, ClientAddress, MessageID)
    if Debug: print("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n")
