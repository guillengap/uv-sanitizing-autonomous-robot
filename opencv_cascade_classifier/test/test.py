import numpy as np
import cv2

backless_stool_cascade = cv2.CascadeClassifier('backless_stool.xml')

img = cv2.imread('test_2.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

backless_stool = backless_stool_cascade.detectMultiScale(gray, 1.3, 5)
for (x,y,w,h) in backless_stool:
    img = cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

cv2.imshow('img',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
