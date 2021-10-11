# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import serial
import struct
a=0
b=0
x1=0
y1=0
ser = serial.Serial('/dev/ttyUSB0',9600)

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

#Load a cascade file for detecting faces
backless_stool_cascade = cv2.CascadeClassifier('backless_stool.xml')

# allow the camera to warmup
time.sleep(0.1)
count = 0

# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        image = frame.array
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        backless_stool = backless_stool_cascade.detectMultiScale(gray, 1.3, 5)
        for (x,y,w,h) in backless_stool:
                a=int((2*x+w)/2)
                b=int((2*y+h)/2)
                x1=int(a/3.66)
                y1=int(b/2.55)
                ser.write(struct.pack('>BB', x1,y1))
                cv2.rectangle(image, (x,y), (x+w,y+h), (255,0,0), 2)
                count += 1
        # show the frame
        cv2.imshow("Frame", image)
        key = cv2.waitKey(1) & 0xFF

        # clear the stream in preparation for the next frame
        rawCapture.truncate(0)

        # if the `q` key was pressed, break from the loop
        if key == ord("q"):
                break
