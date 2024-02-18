import hcsr04
import servo
import L298D
import LM393
import time
from collections import namedtuple
import math
import copy
lidar_measure = namedtuple('lidar_measure', ['angle', 'dist'])
barrier_data = namedtuple('barrier_data', ['array', 'min', 'dist_left', 'gran_left', 'dist_right', 'gran_right'])

class Lidar():
	def __init__(self, motor, lm393):
		self.motor = motor
		self.lm393 = lm393
		self.servo = servo.Servo()
		self.lidar = hcsr04.HCSR04()
		self.max_difference = 7 # 5 cm
		self.max_distance = 18
		self.rotate_range = 70 #диапазон, в котором поворачивается сервопривод при сканировании # 54
		

	def scan_right_lidar(self): #measure_width_barrier(self):
		right_lidar_data = []
		step = self.servo.get_min_step()
		right_max_angle = self.servo.get_max_angle()
		right_servo_angle = right_max_angle
		self.servo.rotate_right(right_servo_angle)
		time.sleep(1)

		distance=-1
		while(1):
			stop=False
			if(right_servo_angle>(right_max_angle - self.rotate_range)):	
				self.servo.rotate_right(right_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_right()
				print(distance)
			else:
				self.servo.rotate_right(right_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_right()
				print(distance)
				stop=True
			right_lidar_data.append(lidar_measure(right_servo_angle, distance))
			right_servo_angle = right_servo_angle - step
			if(stop):
				break	
		self.servo.rotate_right(0)
		for i in range(0, len(right_lidar_data)):
			print("angle = ", right_lidar_data[i].angle, ", dist = ", right_lidar_data[i].dist)
		data_array=[]
		i=1
		size = len(right_lidar_data)
		print("sz = ", size)
		flags=[]
		not_full_barrier = False
		while(i<size):
			if(abs(right_lidar_data[i].dist-right_lidar_data[i-1].dist)<=self.max_difference): # i-й элемент является углом 
				barrier_tmp = []
				if(i==1):
					print("это не грань, а середина препятствия") # это не грань, а середина препятствия 
					flags.append(0)
				if((i >1)and((right_lidar_data[i-1].dist-right_lidar_data[i-2].dist)>self.max_difference)):
					flags.append(2)
					print("это правая грань")
				if((i >1)and((right_lidar_data[i-2].dist-right_lidar_data[i-1].dist)>self.max_difference)):
					flags.append(1)
					print("это левая грань")
				barrier_tmp.append(right_lidar_data[i-1])
				barrier_tmp.append(right_lidar_data[i])
				i = i + 1
				while((i<size)and(abs(right_lidar_data[i].dist-right_lidar_data[i-1].dist)<=self.max_difference)): #i-й элемент отличается от (i-1)-го не более, чем на 5 см):
					barrier_tmp.append(right_lidar_data[i])   #добавляем i-й элемент в двумерный массив, к последнему препятствию, i++. 
					if(i==(size-1)):
						not_full_barrier=True # если справа препятствие еще есть, то True
					i = i + 1
				if (len(barrier_tmp)>0):
					data_array.append(copy.copy(barrier_tmp))
				barrier_tmp.clear()	
			else:
				i = i + 1
		print("data size = ", len(data_array))
		barrier_array=[]
		print("len flags = ", len(flags))
		for i in range(0, len(data_array)):
			dist_array=[]
			gran_left=-1
			gran_right=-1
			for j in range(0, len(data_array[i])):
				dist_array.append(data_array[i][j].dist)
			if(i==0):
				if(flags[0]==0):
					gran_left=False
				else:
					gran_left=True
				if((len(data_array)==1)and(not_full_barrier)):  # случай, когда препятствие  сплошное, занимает весь сектор обзора лидара
					gran_right=False
				else:
					gran_right=True
			if(i>0):
				gran_left=True
				if((i==(len(data_array)-1))and(not_full_barrier)):
					gran_right=False
				else:
					gran_right=True
				 
			tmp=barrier_data(data_array[i], min(dist_array), dist_array[0], gran_left,dist_array[len(dist_array)-1], gran_right)
			barrier_array.append(tmp)
			#print(len(barrier_array[i]))
			#print("len_arr = ", barrier_array[i].angle, ", dist = ", right_lidar_data[i].dist)
			print("")
			print("sz = ", len(barrier_array[i].array))
			print("min = ", barrier_array[i].min)
			print("left_dist = ", barrier_array[i].dist_left)
			print("left_gran = ", barrier_array[i].gran_left)
			print("right_dist = ", barrier_array[i].dist_right)
			print("right_gran = ", barrier_array[i].gran_right)
			print("")
		return(barrier_array)


	def scan_left_lidar(self):
		left_lidar_data = []
		step = self.servo.get_min_step()
		left_max_angle = 0
		left_servo_angle = left_max_angle
		self.servo.rotate_left(left_servo_angle)
		time.sleep(1)
		distance=-1
		while(1):
			stop=False
			if(left_servo_angle < self.rotate_range):	
				self.servo.rotate_left(left_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_left()
				print(distance)
			else:
				self.servo.rotate_left(left_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_left()
				print(distance)
				stop=True
			left_lidar_data.append(lidar_measure(left_servo_angle, distance))
			left_servo_angle = left_servo_angle + step
			if(stop):
				break	
		self.servo.rotate_left(self.servo.get_max_angle())
		for i in range(0, len(left_lidar_data)):
			print("angle = ", left_lidar_data[i].angle, ", dist = ", left_lidar_data[i].dist)
		data_array=[]
		i=1
		size = len(left_lidar_data)
		print("sz = ", size)
		flags=[]
		not_full_barrier = False
		while(i<size):
			if(abs(left_lidar_data[i].dist-left_lidar_data[i-1].dist)<=self.max_difference): # i-й элемент является углом 
				barrier_tmp = []
				if(i==1):
					print("это не грань, а середина препятствия") # это не грань, а середина препятствия 
					flags.append(0)
				if((i >1)and((left_lidar_data[i-1].dist-left_lidar_data[i-2].dist)>self.max_difference)):
					flags.append(1)
					print("это левая грань")
				if((i >1)and((left_lidar_data[i-2].dist-left_lidar_data[i-1].dist)>self.max_difference)):
					flags.append(2)
					print("это правая грань")
				barrier_tmp.append(left_lidar_data[i-1])
				barrier_tmp.append(left_lidar_data[i])
				i = i + 1
				while((i<size)and(abs(left_lidar_data[i].dist-left_lidar_data[i-1].dist)<=self.max_difference)): #i-й элемент отличается от (i-1)-го не более, чем на 5 см):
					barrier_tmp.append(left_lidar_data[i])   #добавляем i-й элемент в двумерный массив, к последнему препятствию, i++. 
					if(i==(size-1)):
						not_full_barrier=True # если справа препятствие еще есть, то True
					i = i + 1
				if (len(barrier_tmp)>0):
					data_array.append(copy.copy(barrier_tmp))
				barrier_tmp.clear()	
			else:
				i = i + 1
		print("data size = ", len(data_array))
		barrier_array=[]
		print("len flags = ", len(flags))
		for i in range(0, len(data_array)):
			dist_array=[]
			gran_left=-1
			gran_left=-1
			for j in range(0, len(data_array[i])):
				dist_array.append(data_array[i][j].dist)
			if(i==0):
				if(flags[0]==0):
					gran_right=False
				else:
					gran_right=True
				if((len(data_array)==1)and(not_full_barrier)):  # случай, когда препятствие  сплошное, занимает весь сектор обзора лидара
					gran_left=False
				else:
					gran_left=True
			if(i>0):
				gran_right=True
				if((i==(len(data_array)-1))and(not_full_barrier)):
					gran_left=False
				else:
					gran_left=True
				 
			tmp=barrier_data(data_array[i], min(dist_array), dist_array[0], gran_left,dist_array[len(dist_array)-1], gran_right)
			barrier_array.append(tmp)
			#print(len(barrier_array[i]))
			#print("len_arr = ", barrier_array[i].angle, ", dist = ", left_lidar_data[i].dist)
			print("")
			print("sz = ", len(barrier_array[i].array))
			print("min = ", barrier_array[i].min)
			print("left_dist = ", barrier_array[i].dist_left)
			print("left_gran = ", barrier_array[i].gran_left)
			print("right_dist = ", barrier_array[i].dist_right)
			print("right_gran = ", barrier_array[i].gran_right)
			print("")
		return(barrier_array)
		
	def calculate_barrier_width(self):
		#фильтр
		unfiltred_left_barrier_array = self.scan_right_lidar()
		left_barrier_array = []
		const1 = ?
		const2 = ?
		for i in range(0, len(unfiltred_left_barrier_array)):
			if((unfiltred_left_barrier_array[i].gran_right < const1)and(unfiltred_left_barrier_array[i].min < const2)):
				left_barrier_array.append(unfiltred_left_barrier_array[i])
		#левый лидар
		#if обе границы в секторе
		#if левая граница в бесконечности
		
		time.sleep(0.5)
		self.motor.go_back_to_distance(8, self.lm393)
		time.sleep(0.5)
		self.scan_right_lidar()


	def close(self):
		self.servo.close()
		

"""servo = servo.Servo()
servo.rotate_right(124)
ultra = hcsr04.HCSR04()
while(True):
	print(ultra.get_mean_distance_right())
	time.sleep(0.5)"""

	
m=L298D.Motor()
lm393 = LM393.LM393()
lidar = Lidar(m, lm393)
lidar.calculate_barrier_width()
m.close()

"""m.go_back_to_distance(8, lm393)
#m1.rotate_left(200, 1)
lidar.close()
#m1.rotate_right(200, 1)"""
