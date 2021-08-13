import cv2
import time
import pickle

frames = []
PointFrame = []

fname = input("Video Stream: ")

capture = cv2.VideoCapture(fname)
frame_number = capture.get(cv2.CAP_PROP_FRAME_COUNT)
print(frame_number)
size_x = int(input("scale x:"))
size_y = int(input("scale y:"))

i = 0
while i < frame_number:
    try:
        ret, frame = capture.read()
        frame = cv2.resize(frame, (size_x, size_y), interpolation=cv2.INTER_CUBIC)
        cv2.imshow("Capture", frame)
        frames.append(frame)
        i += 1
        cv2.waitKey(1)
    except:
        break

capture.release()

print(len(frames))
try:
    for t in range(len(frames)):
        current = []
        for i in range(len(frames[t])):
            current_row = []
            for j in range(len(frames[t][i])):
                if frames[t][i][j][0] > 127:
                    current_row.append(1)
                else:
                    current_row.append(0)
            current.append(current_row)
        PointFrame.append(current)
except:
    file = open("Points.txt", "w")
    for i in PointFrame:
        for p in i:
            for c in p:
                file.write(str(c))
            file.write("\n")
    file.close()
else:
    file = open("Points.txt", "w")
    for i in PointFrame:
        for p in i:
            for c in p:
                file.write(str(c))
            file.write("\n")
    file.close()