
#########################################################################################
# Dependency Import
import urllib
import urllib.request
import cv2
import numpy as np

#########################################################################################

# Streaming URL from ESP32-CAM
url='http://192.168.96.167/cam-hi.jpg'

while True:
    # download the img from URL and decode it 
    imgResp = urllib.request.urlopen(url)
    imgNp = np.array(bytearray(imgResp.read()), dtype=np.uint8)
    img = cv2.imdecode(imgNp, -1)

    # CV 
    # all the opencv processing is done here
    # TODO 

    # visulize all the processing process
    cv2.imshow('test',img)
    if ord('q')==cv2.waitKey(10):
        exit(0)

'''
    Reference : https://stackoverflow.com/questions/60029614/esp32-cam-stream-in-opencv-python
'''

#########################################################################################