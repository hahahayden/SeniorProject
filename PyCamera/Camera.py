import cv2

from video_capture import VideoCaptureAsync
import time

capture = VideoCaptureAsync(src = 0, width = 1920, height = 1080)

fourcc = cv2.VideoWriter_fourcc()
