import socket
import struct

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

#####################################################################################

# UDP listening configs
LocalIP     = "0.0.0.0"
LocalPort   = 4000

# Message buffer size
BufferSize  = 36

DbToken = os.environ.get("INFLUXDB_TOKEN")
DbOrg = os.environ.get("INFLUXDB_ORG")
DbUrl = "https://us-east-1-1.aws.cloud2.influxdata.com"
DbBucket="STM32-GeneratorMonitoring"

Debug = True

#####################################################################################

# CRC32 Hash Calculator
def crc32mpeg2(buf, crc=0xffffffff):
    for val in buf:
        crc ^= val << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (crc << 1) ^ 0x104c11db7
    return crc


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
     PackVoltage, PackDuty, PackFreq,
     PackTemp, PackHumid,
     PackRPM, PackDigits, _2,
     PackGyroX, PackGyroY,
     PackGyroZ, PackAccX,
     PackAccY, PackAccZ,
     PackCRC) = Packet
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|{}|{}|".format(n2h(PackVersion, 8), n2h(PackMsgID, 8), n2h(_1, 17)))
    print("\t\t+--------+--------+--------+--------+")
    print("\t\t|{}|".format(n2h(PackMCUID, 35)))
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
    print("\t\t|{}|".format(n2h(PackCRC, 35)))
    print("\t\t+--------+--------+--------+--------+")
    

# Gets message, processes and stores to db and returns non-zero MessageID upon successfull operation
def processMessage(Writer, Bytes):
    Packet = struct.unpack('<BBHLHBBHHHBBHHHHHHL', Bytes)
    if Debug: debugPacket(Packet)

    # Extract fields
    (PackVersion, PackMsgID, _,
     PackMCUID,
     PackVoltage, PackDuty, PackFreq,
     PackTemp, PackHumid,
     PackRPM, PackDigits, _,
     PackGyroX, PackGyroY,
     PackGyroZ, PackAccX,
     PackAccY, PackAccZ,
     PackCRC) = Packet
    
    Digital1 = PackDigits & 0x80
    Digital2 = PackDigits & 0x40

    # Todo: Check CRC
    if(crc32mpeg2(Bytes[:32]) != PackCRC):
        if Debug: print("CRC verification failed!")
        return 0

    DataPoint = (
        Point("Sensors")
        .tag("SensorID", PackMCUID)
        .field("Voltage", PackVoltage)
        .field("Duty", PackDuty)
        .field("Frequency", PackFreq)
        .field("Temperature", PackTemp)
        .field("Humidity", PackHumid)
        .field("MotorRPM", PackRPM)
        .field("Digital-1", Digital1)
        .field("Digital-2", Digital2)
        .field("Gyro-X", PackGyroX)
        .field("Gyro-Y", PackGyroY)
        .field("Gyro-Z", PackGyroZ)
        .field("Accelerometer-X", PackAccX)
        .field("Accelerometer-Y", PackAccY)
        .field("Accelerometer-Z", PackAccZ)
    )

    Writer.write(bucket=DbBucket, org=DbOrg, record=DataPoint)

    if Debug: print("Write data done!")

    return PackMsgID


# Send ACK Message to client
def sendACK(Socket, ClientAddress, MessageID):
    Packet = struct.pack('<BB', 0x01, MessageID)
    Socket.sendto(Packet, ClientAddress)

#####################################################################################

Server = setupServer()
print("UDP server up and listening...\n\n")

DataWriter = influxdb_client.InfluxDBClient(url=DbUrl, token=DbToken, org=DbOrg).write_api(write_options=SYNCHRONOUS)

# Listen for incoming datagrams
while(True):
    BytesPair = Server.recvfrom(BufferSize)
    ClientAddress = BytesPair[1]
    if Debug: print("Message Received from:", ClientAddress)
    MessageID = processMessage(DataWriter, BytesPair[0])

    if MessageID:
        # Sending a ACK to client
        sendACK(Server, ClientAddress, MessageID)
    if Debug: print("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n")
