import hcsr04
import servo
import time
from collections import namedtuple
import math
import copy
lidar_measure = namedtuple('lidar_measure', ['angle', 'dist'])
barrier_data = namedtuple('barrier_data', ['array', 'min', 'dist_left', 'gran_left', 'dist_right', 'gran_right'])

class Lidar():
	def __init__(self):
		self.servo = servo.Servo()
		self.lidar = hcsr04.HCSR04()
		self.max_difference = 7 # 5 cm
		self.max_distance = 18
		self.enough_right_angle = 70 #neobhodimy ugol

	def measure_width_barrier(self):
		right_lidar_data = []
		step = self.servo.get_min_step()
		right_servo_angle=self.servo.get_max_angle()
		self.servo.rotate_right(right_servo_angle)
		#servo.rotate_right(right_servo_angle)
		time.sleep(1)

		distance=-1
		while(1):
			stop=False
			if(right_servo_angle>self.enough_right_angle):	
				self.servo.rotate_right(right_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_right()
				print(distance)
			else:
				#right_servo_angle=
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
		not_all_barrier = False
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
						not_all_barrier=True
					i = i + 1
				if (len(barrier_tmp)>0):
					data_array.append(copy.copy(barrier_tmp))
				barrier_tmp.clear()	
			else:
				i = i + 1
		print("data size = ", len(data_array))
		barrier_array=[]
		for i in range(0, len(data_array)):
			dist_array=[]
			gran_left=-1
			gran_right=-1
			for j in range(0, len(data_array[i])):
				dist_array.append(data_array[i][j].dist)
			if(i==0): # rabotaet!!
				if(flags[0]==0):
					gran_left=False
				else:
					gran_left=True
				if((len(data_array)==1)and(not_all_barrier)):#len(data_array[0])==len(right_lidar_data))):
					gran_right=False
				else:
					gran_right=True
			if(i>0):
				gran_left=True
				print("if= ", len(data_array)-1)
				print(i)
				if((i==(len(data_array)-1))and(not_all_barrier)):
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
	def close(self):
		self.servo.close()
		
		
lidar = Lidar()
lidar.measure_width_barrier()
lidar.close()
