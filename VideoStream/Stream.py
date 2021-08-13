import serial
import pickle
import time


Port = "COM4"
BaudRate = 115200
PointFile = open("Points.data", "rb")
HexList = []

MCU = serial.Serial(Port, BaudRate, timeout=5)
PointData = pickle.load(PointFile)
PointFile.close()

# Code
for Frame in PointData:
    CurrentFrame = b""
    for page in range(8):
        for col in range(128):
            HexBlock = ((Frame[page*8][col]) +
                        (Frame[page*8+1][col] << 1) +
                        (Frame[page*8+2][col] << 2) +
                        (Frame[page*8+3][col] << 3) +
                        (Frame[page*8+4][col] << 4) +
                        (Frame[page*8+5][col] << 5) +
                        (Frame[page*8+6][col] << 6) +
                        (Frame[page*8+7][col] << 7))
            HexStr = hex(HexBlock)
            HexStr = HexStr[2:]
            if len(HexStr) == 1:
                HexStr = "0"+HexStr
            CurrentFrame += bytes.fromhex(HexStr)
    HexList.append(CurrentFrame)

# Code
print("Start")
# Send
Total_Sent = 0

for f in HexList:
    print(MCU.write(f), f)
    Total_Sent += 1
    time.sleep(0.1)
# Send
