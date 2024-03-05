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
m.stop()
lidar = hcsr04.HCSR04()
delta_distance = lidar.get_delta_distance()
servo = servo.Servo()
servo.front_left_vbok()
servo.back_left_vbok()
lm393 = LM393.LM393()
lm393.start_with_delay()


# корректировка курса
rotate_speed = 200
speed = 130	
const_distance_barrier = 30 #? предельная дальность до препятствия
const_move_before_stop = 0.7 # проезд робота перед остановкой при скорости 130 при движении вперед до срабатывания лидара  proezd robota 
const_distance_between_lidars = 28.75
const_tmp = 1.0 # ??????????????

servo.back_left_vbok
servo.front_left_vbok()
time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
distance_back = lidar.back_distance_left() - delta_distance
distance_front = lidar.distance_left() - delta_distance
if((distance_back<const_distance_barrier) and (distance_front<const_distance_barrier) and (abs(distance_back - distance_front)>2)): #2 ?
	if(distance_front < distance_back):
		while(distance_front < distance_back):
			m.rotate_right(rotate_speed, 0.05, non_stop=True)
			distance_front = lidar.distance_left() - delta_distance
			distance_back = lidar.back_distance_left() - delta_distance
		m.stop()
	else:
		while(distance_front > distance_back):
			m.rotate_left(rotate_speed, 0.05, non_stop=True)
			distance_front = lidar.distance_left() - delta_distance
			distance_back = lidar.back_distance_left() - delta_distance
		m.stop()
m.stop()
time.sleep(0.5)


# отъезд назад за границу препятствия
servo.back_right_to_start_position() # проверка препятствия сзади
servo.back_left_to_start_position()
time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
distance_back_left = lidar.back_distance_left() - delta_distance
distance_back_right = lidar.back_distance_right() - delta_distance
barrier_szadi = False
print("distance_back_left = ", distance_back_left)

servo.back_left_vbok()
time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
const_distance_barrier_back = 8
if((distance_back_left > const_distance_barrier_back)and(distance_back_right > const_distance_barrier_back)):	# движение назад
	distance_left = lidar.back_distance_left() - delta_distance
	print(distance_left)
	while(distance_left<const_distance_barrier):
		print("1")
		m.go_back(speed, 0.05, non_stop = True)
		distance_left = lidar.back_distance_left() - delta_distance
		distance_back = lidar.back_distance_right() - delta_distance
		if(distance_back<const_distance_barrier_back):
			barrier_szadi = True
			m.stop()
			break
		print(distance_left)
	m.go_back_to_distance(3, lm393)
	m.stop()
else:  # сзади препятствие, не движемся назад
	barrier_szadi = True
	
if(barrier_szadi == True):
	print("Error: szadi barrier") # теперь препятствие можно объехать только спереди!
else:
	time.sleep(0.5)

	# движение вперед
	distance_left_back = lidar.back_distance_left() - delta_distance
	while(distance_left_back>const_distance_barrier):
		m.go_pered(speed, 0.05, non_stop = True)
		distance_left_back = lidar.back_distance_left() - delta_distance
	m.stop()
	time.sleep(0.5)
	
	# взводим счетчик пути
	S_l = 0
	S_r = 0
	S = 0
	distance_front1 = lidar.distance_left() - delta_distance
	distance_front2 = distance_front1  # именно так - был случай, когда зависло
	print("distance_front2 = ", distance_front2)
	
	tmp_2_5 = 2.5 - 1.21 # это значение недоезда робота из-за того, что датчик имеет угол луча 10 градусов и срабатывает несколько раньше, чем надо
	if((distance_front1 > const_distance_barrier)and(distance_left_back < const_distance_barrier)): # движение назад
		distance_left_front = lidar.distance_left() - delta_distance
		print("distance_left_front = ", distance_left_front)
		while(distance_left_front>const_distance_barrier):
			m.go_back(speed, 0.05, non_stop = True)
			speed_l = lm393.get_speed_left()
			speed_r = lm393.get_speed_right()
			S_l = S_l+speed_l*0.05
			S_r = S_r+speed_r*0.05
			distance_left_front = lidar.distance_left() - delta_distance
			print(distance_left_front)
		m.stop()
		S= (S_l+S_r)/20
		print("S back = ", S)
		#delta_lidar = 2.5 # svuazano s uglom signala, srednee 2.5
		S = const_distance_between_lidars - S - 2*const_move_before_stop - 2*tmp_2_5 # -2*const_move_before_stop, так как результат больше соответствует истине!
		# const_distance_between_lidars - S - 2*delta_lidar # const_move_before_stop не нужен, t.k ostanovka vpered, a potom nazad neitralisuut this parametr
		
	else: # движение вперед
		while(abs(distance_front1-distance_front2)<=10):
			m.go_pered(speed, 0.05, non_stop = True)
			speed_l = lm393.get_speed_left()
			speed_r = lm393.get_speed_right()
			S_l = S_l+speed_l*0.05
			S_r = S_r+speed_r*0.05
			distance_front1 = distance_front2
			distance_front2 = lidar.distance_left() - delta_distance
		m.stop()
		S= (S_l+S_r)/20
		print("S front = ", S)
		"""if(S>=25):
			delta_front = -0.5 # 1.4 #
		else:
			delta_front = 1.2 # 1.0"""
		#S = S - delta_front + const_move_before_stop + const_distance_between_lidars - const_tmp
		S = const_distance_between_lidars - 2*tmp_2_5 + S
		
	print("S_l = ", S_l)
	print("S_r = ", S_r)
	print("S = ", S)
m.stop()

