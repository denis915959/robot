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

# случай 1 (поворот вправо 90)
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
	
speed = 130	
const_distance_barrier = 27 #? предельная дальность до препятствия
const_move_before_stop = 0.7 # проезд робота перед остановкой при скорости 130 при движении вперед до срабатывания лидара  proezd robota 
const_distance_between_lidars = 28.75
const_tmp = 1.0 # ??????????????
distance_back = lidar.back_distance_left() - delta_distance
while(distance_back>const_distance_barrier):
	m.go_pered(speed, 0.05)
	distance_back = lidar.back_distance_left() - delta_distance
	#print(distance_back)
#m.stop()
#time.sleep(1)	
	

distance_front1 = lidar.distance_left() - delta_distance
distance_front2 = distance_front1  # именно так - был случай, когда зависло

# vzvodim schetchik puti
S_l = 0
S_r = 0
while(abs(distance_front1-distance_front2)<=10):
	m.go_pered(speed, 0.05)
	
	speed_l = lm393.get_speed_left()
	speed_r = lm393.get_speed_right()
	S_l = S_l+speed_l*0.05
	S_r = S_r+speed_r*0.05
	
	distance_front1 = distance_front2
	distance_front2 = lidar.distance_left() - delta_distance
	print(distance_front2)
	
print("S_l = ", S_l)
print("S_r = ", S_r)

S= (S_l+S_r)/20
print("S 1 = ", S)
if(S>=25):
	delta_front = -0.5 # 1.4 #
else:
	delta_front = 1.2 # 1.0
S = S - delta_front + const_move_before_stop + const_distance_between_lidars - const_tmp
print("S = ", S)
m.stop()

