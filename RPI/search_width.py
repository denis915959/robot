import hcsr04
import servo
import L298D
import LM393
import time
from collections import namedtuple
import math
import copy
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# slychai 1 (rotate right 90)
m = L298D.Motor()
lidar = hcsr04.HCSR04()
delta_distance = lidar.get_delta_distance()
servo = servo.Servo()
servo.front_left_vbok()
servo.back_left_vbok()
lm393 = LM393.LM393()
lm393.start_with_delay()

distance_back = lidar.back_distance_left() - delta_distance
distance_front = lidar.distance_left() - delta_distance
if((distance_back<=20)and(distance_front<=20)):
	print("Regulirovka")
distance_front1 = lidar.distance_left() - delta_distance
distance_front2 = lidar.distance_left() - delta_distance
print(distance_front2)
speed = 120
S_l = 0
S_r = 0
delta_front = 1.4 #
while(abs(distance_front1-distance_front2)<=10):
	m.go_pered(speed, 0.05)
	
	speed_l = lm393.get_speed_left()
	speed_r = lm393.get_speed_right()
	S_l = S_l+speed_l*0.05
	S_r = S_r+speed_r*0.05
	
	distance_front1 = distance_front2
	distance_front2 = lidar.distance_left() - delta_distance
	
print("S_l = ", S_l)
print("S_r = ", S_r*2)
S= (S_l+S_r*2)/20 - delta_front
print("S = ", S)
m.stop()

