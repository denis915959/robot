import hcsr04
import servo
import L298D
import LM393
from hmc5883 import *
import time
from collections import namedtuple
import math
import copy
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
m = L298D.Motor()
lidar = hcsr04.HCSR04()
delta_distance = lidar.get_delta_distance()
compass = hmc5883l()
lm393 = LM393.LM393()
lm393.start_with_delay()

def rotate_left_kompass(angle, speed):
	if(angle>180):
		print("нельзя повернуть на угол, больший 180 градусов")
		return
	data =  compass.get_calibrated() 
	angle_start = compass.heading(data['x'], data['y'])
	angle_now = angle_start # это не абсолютный угол, а угол, на который повернулся робот относительно своего изначального направления. но до цикла он абсолютный
	while(True): # именно так, иначе while будет проигнорирован
		m.rotate_left(speed, 0.05, non_stop=True)
		data =  compass.get_calibrated() 
		angle_now = compass.heading(data['x'], data['y'])
		delta = abs(angle_start - angle_now)
		if(delta > 180):
			angle_now = 360 - delta
		else:
			angle_now = delta
		if(angle_now > angle):
			break
	m.stop()
	
def rotate_right_kompass(angle, speed):
	if(angle>180):
		print("нельзя повернуть на угол, больший 180 градусов")
		return
	data =  compass.get_calibrated() 
	angle_start = compass.heading(data['x'], data['y'])
	angle_now = angle_start # это не абсолютный угол, а угол, на который повернулся робот относительно своего изначального направления. но до цикла он абсолютный
	while(True): # именно так, иначе while будет проигнорирован
		m.rotate_right(speed, 0.05, non_stop=True)
		data =  compass.get_calibrated() 
		angle_now = compass.heading(data['x'], data['y'])
		delta = abs(angle_start - angle_now)
		if(delta > 180):
			angle_now = 360 - delta
		else:
			angle_now = delta
		if(angle_now > angle):
			break
	m.stop()
	
def course_correction(rotate_speed): # корректировка курса
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


# случай 1 (поворот вправо 90)
servo = servo.Servo()
servo.front_left_vbok()
servo.back_left_vbok()

rotate_speed = 200
speed = 130	
const_distance_barrier = 30 #? предельная дальность до препятствия
const_move_before_stop = 0.7 # проезд робота перед остановкой при скорости 130 при движении вперед до срабатывания лидара  proezd robota 
const_distance_between_lidars = 28.75
# const_tmp = 1.0 # ??????????????
tmp_2_5 = 2.5 - 1.21 # это значение недоезда робота из-за того, что датчик имеет угол луча 10 градусов и срабатывает несколько раньше, чем надо
const_distance_barrier_back = 8

rotate_right_kompass(90, rotate_speed)
time.sleep(0.5)

course_correction(rotate_speed)
time.sleep(0.5)
servo.back_left_vbok
servo.front_left_vbok()

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
if((distance_back_left > const_distance_barrier_back)and(distance_back_right > const_distance_barrier_back)):	# движение назад
	distance_left = lidar.back_distance_left() - delta_distance
	print(distance_left)
	while(distance_left<const_distance_barrier):
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
	print("Ошибка: сзади препятствие") # теперь препятствие можно объехать только спереди!
else:
	time.sleep(0.5)

	# движение вперед
	distance_left_back = lidar.back_distance_left() - delta_distance
	while(distance_left_back>const_distance_barrier):
		m.go_pered(speed, 0.05, non_stop = True)
		distance_left_back = lidar.back_distance_left() - delta_distance
	m.stop()
	time.sleep(0.5)
	
	# возможно, надо проверки добавить
	course_correction(rotate_speed) # корректировка курса
	time.sleep(0.5)
	# взводим счетчик пути
	S_l = 0
	S_r = 0
	S = 0
	distance_front1 = lidar.distance_left() - delta_distance
	distance_front2 = distance_front1  # именно так - был случай, когда зависло
	
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
		S = const_distance_between_lidars - S - 2*const_move_before_stop - 2*tmp_2_5 # -2*const_move_before_stop, так как результат больше соответствует истине!
		
	else: # движение вперед
		space_flag = False
		while(abs(distance_front1-distance_front2)<=10):
			distance_back = lidar.back_distance_left() - delta_distance
			if(distance_back>const_distance_barrier):
				space_flag = True
				print("stop")
				break
			m.go_pered(speed, 0.05, non_stop = True)
			speed_l = lm393.get_speed_left()
			speed_r = lm393.get_speed_right()
			S_l = S_l+speed_l*0.05
			S_r = S_r+speed_r*0.05
			distance_front1 = distance_front2
			distance_front2 = lidar.distance_left() - delta_distance
		m.stop()
		S= (S_l+S_r)/20
		if(space_flag == True):
			delta = 1 # почему-то с этим параметром получается точнее
			S = S - 2*tmp_2_5 + delta
		else:
			S = const_distance_between_lidars - 2*tmp_2_5 + S
		print("S front = ", S)
		
	print("S_l = ", S_l)
	print("S_r = ", S_r)
	print("S = ", S)
m.stop()





# оценка ширины
go_back_shirina_const = 6
const_go_front_for_rotate = 27 # 35 # возможно, побольше сделать
const_check_front_delta = 10 # запас для барьера в процессе движения go_front_to_distance
servo.left_to_start_position()
servo.right_to_start_position()
time.sleep(0.5)
distance_left_front = lidar.distance_left() - delta_distance
distance_right_front = lidar.distance_right() - delta_distance
left_side = False # с какой стороны лучше обьезжать
rotate_flag = False
barrier_flag = False
#S_all_flag = False
S_all = 0
if((distance_left_front < (const_go_front_for_rotate + const_check_front_delta))or(distance_right_front < (const_go_front_for_rotate + const_check_front_delta))):
	left_side = True

if(left_side == False):
	m.go_front_to_distance(const_go_front_for_rotate, lm393)
	# поворот влево 90
	rotate_left_kompass(90, rotate_speed)
	rotate_flag = True
	servo.right_to_start_position()
	servo.front_left_vbok()
	time.sleep(0.5)
	
	const_distance_bok = 20 # 20?
	S_l_1 = 0
	S_r_1 = 0
	distance_left_front = lidar.distance_left() - delta_distance
	while(distance_left_front > const_distance_bok):
		m.go_pered(speed, 0.05, non_stop = True)
		speed_l = lm393.get_speed_left()
		speed_r = lm393.get_speed_right()
		S_l_1 = S_l_1 +speed_l*0.05
		S_r_1 = S_r_1 +speed_r*0.05
		distance_left_front = lidar.distance_left() - delta_distance
		distance_right = lidar.distance_right() - delta_distance
		if(distance_right < const_distance_barrier_back):
			S_all = S_all + (S_l_1 + S_r_1)/20
			barrier_flag = True
			break
	"""if(S_all>0):
		S_all_flag = True"""
	m.stop()
	time.sleep(0.5) # заменить на 0.5


	if(barrier_flag == False):
		find_depth_flag = True   #####
		S_all = S_all + (S_l_1 + S_r_1)/20
		while(find_depth_flag):
			servo.back_left_vbok()
			time.sleep(0.5)
			S_l_2 = 0
			S_r_2 = 0
			# движение назад в случае осечки
			distance_left_front = lidar.distance_left() - delta_distance
			while(distance_left_front > const_distance_bok):
				m.go_pered(speed, 0.05, non_stop = True) # go back?
				speed_l = lm393.get_speed_left()
				speed_r = lm393.get_speed_right()
				S_l_2 = S_l_1 +speed_l*0.05
				S_r_2 = S_r_1 +speed_r*0.05
				distance_left_front = lidar.distance_left() - delta_distance
			m.stop()
			time.sleep(0.5)
				
				
			lm393.get_speed_left()
			lm393.get_speed_right()
			distance_left_front = lidar.distance_left() - delta_distance
			while(distance_left_front < const_distance_bok):
				m.go_pered(speed, 0.05, non_stop = True)
				speed_l = lm393.get_speed_left()
				speed_r = lm393.get_speed_right()
				S_l_2 = S_l_2 +speed_l*0.05
				S_r_2 = S_r_2 +speed_r*0.05
				distance_left_front = lidar.get_mean_distance_left() - delta_distance  # distance_left() - delta_distance
				print("distance_left_front = ", distance_left_front)
				distance_right = lidar.distance_right() - delta_distance
				if(distance_right < const_distance_barrier_back):
					barrier_flag = True
					break
			m.stop()

			S_bar = (S_l_2 + S_r_2)/20 # - 2*tmp_2_5 # изменить имя переменной
			S_all = S_all + S_bar
			print("barrier_flag = ", barrier_flag)
			if((S_bar<3)and(barrier_flag==False)): # 3, т.к меньшее расстояние не распознается из-за неточности самих лидаров
				print("осечка")
				servo.back_left_to_start_position()
				servo.back_right_to_start_position()
				time.sleep(0.5)
				dist_left = lidar.get_back_mean_distance_left() - delta_distance
				dist_right = lidar.get_back_mean_distance_right() - delta_distance
				if((dist_left>(S_bar + go_back_shirina_const) + const_distance_barrier_back)and(dist_right>(S_bar + go_back_shirina_const) + const_distance_barrier_back)):
					m.go_back_to_distance(S_all + go_back_shirina_const, lm393)
					S_all = S_all - (S_bar + go_back_shirina_const) # ?????????
				else:
					print("Ошибка: внезапно возникшее сзади препятствие")
					break
			else:
				find_depth_flag = False   #####
		print("S_all = ", S_all)
		print("S_bar = ", S_bar)
		time.sleep(0.5) # 0.5
		# если препятствия спереди нет - отъезжать назад не надо. Надо продолжить проверку окружения робота на наличие препятствий
if((barrier_flag == True)or(left_side ==True)):
	time.sleep(0.5)
	print("Sprava barrier")	
	print("barrier_flag = ", barrier_flag)
	print("left_side = ", left_side)
	print("rotate_flag = ", rotate_flag)
	if(S_all > 0):
		m.go_back_to_distance(S_all + go_back_shirina_const, lm393)
		time.sleep(0.5)
	if(rotate_flag == True):
		rotate_right_kompass(90, rotate_speed)
	
