import socket
import struct

# UDP listening configs
localIP     = "0.0.0.0"
localPort   = 4000

# Message buffer size
bufferSize  = 36

# Setup UDP listening
def setupServer():
    # Create a datagram socket
    UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    # Bind to address and ip
    UDPServerSocket.bind((localIP, localPort))
    return UDPServerSocket

# Gets message, processes and stores to db and returns non-zero MessageID upon successfull operation
def processMessage(Bytes):
    Packet = struct.unpack('<BBHLLHBBHHHBBHHHHHH', Bytes)
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
print("UDP server up and listening...")

# Listen for incoming datagrams
while(True):
    BytesPair = Server.recvfrom(bufferSize)
    ClientAddress = BytesPair[1]

    MessageID = processMessage(BytesPair[0])

    if MessageID:
        # Sending a ACK to client
        sendACK(Server, ClientAddress, MessageID)
