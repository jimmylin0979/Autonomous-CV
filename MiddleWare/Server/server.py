#########################################################################################

# CV
import cv2
import numpy as np

# OS
import os
import threading
from multiprocessing import Value

# Server
from flask import Flask, jsonify, request, abort, send_file, render_template
import json
import urllib
import urllib.request

app = Flask(__name__, static_url_path="")

#########################################################################################
### Globla Variables ###

# 
Autonomous_dir = 0		# next direction that Autonomous should go
Autonomous_makeDir = False

#########################################################################################

# declare counter variable
counter = Value('i', 0)

def save_img(img):
	with counter.get_lock():
		counter.value += 1
		count = counter.value
	img_dir = "esp32_imgs"
	if not os.path.isdir(img_dir):
		os.mkdir(img_dir)
	cv2.imwrite(os.path.join(img_dir,"img_"+str(count)+".jpg"), img)
	# print("Image Saved", end="\n") # debug

@app.route('/')
@app.route('/index', methods=['GET'])
def index():
	return "ESP32-CAM Flask Server", 200


@app.route('/upload', methods=['POST','GET'])
def upload():
	received = request
	img = None
	if received.files:
		print(received.files['imageFile'])
		# convert string of image data to uint8
		file  = received.files['imageFile']
		nparr = np.fromstring(file.read(), np.uint8)
		# decode image
		img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
		save_img(img)
		
		return "[SUCCESS] Image Received", 201
	else:
		return "[FAILED] Image Not Received", 204

@app.route('/direction', methods=['GET'])
def direction():
	global Autonomous_makeDir

	if Autonomous_makeDir:
		Autonomous_makeDir = False
		return str(Autonomous_dir), 200
	
	return str(-1), 200

#########################################################################################

##  Computer Vision Part ##
# 
# def CV_Process(img) -> int:
	
# 	# 
# 	blur = cv2.GaussianBlur(img, (5, 5), 0)
# 	edges = cv2.Canny(blur, 150, 250)
	
# 	return dir

def CV():
	
	# Streaming URL from ESP32-CAM
	url='http://192.168.96.167/cam-hi.jpg'

	while True:
		# download the img from URL and decode it 
		imgResp = urllib.request.urlopen(url)
		imgNp = np.array(bytearray(imgResp.read()), dtype=np.uint8)
		img = cv2.imdecode(imgNp, -1)
		
		#############################################
		# CV 
		# all the opencv processing is done here

		try:
			# 計算影象高度的一半
			HEIGHT = (img.shape[0] * 3) // 4
			WIDTH = (img.shape[1])

			# canny邊緣檢測
			blur = cv2.GaussianBlur(img, (5, 5), 0)
			# blur = img
			edges = cv2.Canny(blur, 0, 50)
			cv2.imshow('img-canny', edges)

			# 引數有輸入的影象，畫素精度，弧度單位，最小點集，最小長度，最大間隔
			img_houghP = np.copy(edges)
			img_output = np.copy(img)

			lines = cv2.HoughLinesP(img_houghP, 1, np.pi/360, 10, minLineLength=10, maxLineGap=25)

			m = []  # 斜率集合
			p_midx = []	# 中間 x 集合，可用以判斷路線是否在正中央

			print('=' * 100)

			if lines is not None:
				for i in range(0, len(lines)):
					l = lines[i][0]
					
					if int((l[1] + l[3]) / 2) < HEIGHT:
						continue
					
					# ignore the case when line is horizontal
					# 
					if abs(l[3] - l[1]) < 15:
						continue
					
					# _m == 0 if line is vertical
					_m = (l[2] - l[0]) / (l[3] - l[1])
					if abs(_m) > 0.001:
						p_midx.append((l[2] + l[0])/2)
						m.append(_m)

					cv2.line(img_output, (l[0], l[1]), (l[2], l[3]), (0, 255, 0), 5)
					
					print(f'Line : {l}; m : {_m}')

			# 
			m_avg = sum(m) / len(m)
			p_midx_avg = sum(p_midx) / len(p_midx)
			print(f'm_avg :\t{m_avg}')
			print(f'p_midx_avg :\t{p_midx_avg}')

			M_BOUND_R, M_BOUND_L = -0.3, 0.3

			if m_avg > 5 or m_avg < -5:
				dir = 4
			
			else:

				if m_avg < M_BOUND_R:
					dir = 3
					print('Direction : Right')
				elif m_avg > M_BOUND_L:
					dir = 2
					print('Direction : Left')
				else:
					if p_midx_avg < (WIDTH // 2) / 4:
						dir = 2
						print('Direction : Left')

					elif p_midx_avg > (WIDTH // 2) * 3 / 4:
						
						dir = 3
						print('Direction : Right')

					else:
						dir = 0
						print('Direction : Forward')

			print('=' * 100)

		except:
			# Unable to determine next direction
			# Stop 
			dir = 4

		#############################################

		global Autonomous_dir, Autonomous_makeDir
		Autonomous_dir = dir
		Autonomous_makeDir = True if Autonomous_makeDir != 4 else False

		# visulize all the processing process
		cv2.imshow('output', img_output)
		if ord('q')==cv2.waitKey(10):
			exit(0)

#########################################################################################

if __name__ == "__main__":

	# CV()
	
	# thr = threading.Thread(target=hello)
	thr = threading.Thread(target=CV)
	thr.start()

    # Start Flask App
	# debug=True will cast main function trigger twice
	app.run(host='127.0.0.1', port=5000, debug=False, threaded=True)