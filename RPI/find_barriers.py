import hcsr04
import servo
#import L298D
import LM393
import time
from collections import namedtuple
import math
import copy
lidar_measure = namedtuple('lidar_measure', ['angle', 'dist'])
barrier_data = namedtuple('barrier_data', ['array', 'min', 'dist_start', 'dist_left', 'gran_left', 'angle_left', 'dist_right', 'gran_right', 'angle_right'])

class Lidar():
	def __init__(self, motor, lm393):
		self.motor = motor
		self.lm393 = lm393
		self.servo = servo.Servo()
		self.lidar = hcsr04.HCSR04()
		self.max_difference = 5 # 4 cm
		self.max_distance_4cm = 19 #dlya predelnih uglov!
		self.max_distance_8cm = 27 # 25?
		self.rotate_range = 90 #диапазон, в котором поворачивается сервопривод при сканировании # 70
		self.rotate_front_range = 90 #диапазон, в котором поворачивается сервопривод при сканировании # 70
		self.lidar_base = 16.6 #cm
		self.left_start_angle = self.servo.get_left_start_angle()
		self.right_start_angle = self.servo.get_right_start_angle()
		self.k_right = 0.4 #0.65 # right lidar koefficient
		self.k_left = 0.4 # left lidar koefficient
		self.k_4cm = 1 # k_left/right * k_4cm
		self.k_8cm = 0.5
		

	def scan_right_lidar(self): #measure_width_barrier(self):
		right_lidar_data = []
		step = self.servo.get_min_step()
		right_max_angle = self.servo.get_max_angle()
		right_servo_angle = right_max_angle
		self.servo.rotate_front_right(right_servo_angle)
		time.sleep(1)

		distance=-1
		while(1):
			stop=False
			if(right_servo_angle>(right_max_angle - self.rotate_range)):	
				self.servo.rotate_front_right(right_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_right()
				print(distance)
			else:
				self.servo.rotate_front_right(right_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_right()
				print(distance)
				stop=True
			right_lidar_data.append(lidar_measure(right_servo_angle, distance))
			right_servo_angle = right_servo_angle - step
			if(stop):
				break	
		self.servo.rotate_front_right(0)
		for i in range(0, len(right_lidar_data)):
			print("angle = ", right_lidar_data[i].angle, ", dist = ", right_lidar_data[i].dist)
		data_array=[]
		i=1
		size = len(right_lidar_data)
		flags=[]
		not_full_barrier = []
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
				while_flag = False
				not_full_barrier_tmp = False
				while((i<size)and(abs(right_lidar_data[i].dist-right_lidar_data[i-1].dist)<=self.max_difference)): #i-й элемент отличается от (i-1)-го не более, чем на 5 см):
					barrier_tmp.append(right_lidar_data[i])   #добавляем i-й элемент в двумерный массив, к последнему препятствию, i++. 
					if(i==(size-1)):
						not_full_barrier_tmp = True # если справа препятствие еще есть, то True
					i = i + 1
					while_flag = True
				if((i==size)and(while_flag==False)):
					not_full_barrier.append(True)
				else:
					not_full_barrier.append(not_full_barrier_tmp)
				if (len(barrier_tmp)>0):
					data_array.append(copy.copy(barrier_tmp))
				barrier_tmp.clear()	
			else:
				i = i + 1
		barrier_array=[]
		for i in range(0, len(data_array)):
			dist_array=[]
			gran_left=-1
			gran_right=-1
			angle_left = -1
			angle_right = -1
			for j in range(0, len(data_array[i])):
				dist_array.append(data_array[i][j].dist)
			if(i==0):
				if(flags[0]==0):
					gran_left=False
					angle_left = self.servo.get_max_angle()
				else:
					gran_left=True
					angle_left = data_array[i][0].angle
					
				if((len(data_array)==1)and(not_full_barrier[i])):  # случай, когда препятствие  сплошное, занимает весь сектор обзора лидара
					gran_right=False
					j=len(data_array[i])-1
					angle_right = data_array[i][j].angle
				else:
					gran_right=True
					j=len(data_array[i])-1
					angle_right = data_array[i][j].angle
			if(i>0):
				gran_left=True
				angle_left = data_array[i][0].angle
				if((i==(len(data_array)-1))and(not_full_barrier[i])):
					gran_right=False
				else:
					gran_right=True
				j=len(data_array[i])-1
				angle_right = data_array[i][j].angle
			start_angle=self.right_start_angle
			e = self.servo.get_min_step()
			if(e%2!=0):
				e=(e//2)+1
			else:
				e=int(e/2)
			start_array=[]
			for j in range(0, len(data_array[i])):
				if(abs(data_array[i][j].angle-start_angle)<e):
					start_array.append(data_array[i][j])
			dist_start = -1
			if(len(start_array)==2):
				if(abs(start_array[0].angle - e)<abs(start_array[1].angle - e)):
					dist_start = start_array[0].dist
				else:
					dist_start = start_array[1].dist
			if(len(start_array)==1):
				dist_start = start_array[0].dist
			
			tmp=barrier_data(data_array[i], min(dist_array), dist_start, dist_array[0], gran_left, angle_left, dist_array[len(dist_array)-1], gran_right, angle_right)
			barrier_array.append(tmp)
			print("")
			print("sz = ", len(barrier_array[i].array))
			print("min = ", barrier_array[i].min)
			print("dist_start = ", barrier_array[i].dist_start)
			print("left_dist = ", barrier_array[i].dist_left)
			print("left_gran = ", barrier_array[i].gran_left)
			print("left_angle = ", barrier_array[i].angle_left)
			print("right_dist = ", barrier_array[i].dist_right)
			print("right_gran = ", barrier_array[i].gran_right)
			print("right_angle = ", barrier_array[i].angle_right)
			print("")
		return(barrier_array)


	def scan_left_lidar(self):
		left_lidar_data = []
		step = self.servo.get_min_step()
		left_max_angle = 20
		left_servo_angle = left_max_angle
		self.servo.rotate_front_left(left_servo_angle)
		time.sleep(1)
		distance=-1
		while(1):
			stop=False
			if(left_servo_angle < self.rotate_front_range):	
				self.servo.rotate_front_left(left_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_left()
				print(distance)
			else:
				self.servo.rotate_front_left(left_servo_angle)
				time.sleep(0.5)
				distance = self.lidar.get_mean_distance_left()
				print(distance)
				stop=True
			left_lidar_data.append(lidar_measure(left_servo_angle, distance))
			left_servo_angle = left_servo_angle + step
			if(stop):
				break	
		self.servo.rotate_front_left(self.servo.get_max_angle())
		for i in range(0, len(left_lidar_data)):
			print("angle = ", left_lidar_data[i].angle, ", dist = ", left_lidar_data[i].dist)
		data_array=[]
		i=1
		size = len(left_lidar_data)
		flags=[]
		not_full_barrier = []
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
				while_flag = False
				not_full_barrier_tmp = False
				while((i<size)and(abs(left_lidar_data[i].dist-left_lidar_data[i-1].dist)<=self.max_difference)): #i-й элемент отличается от (i-1)-го не более, чем на 5 см):
					barrier_tmp.append(left_lidar_data[i])   #добавляем i-й элемент в двумерный массив, к последнему препятствию, i++. 
					if(i==(size-1)):
						not_full_barrier_tmp = True # если справа препятствие еще есть, то True
					i = i + 1
					while_flag = True
				if((i==size)and(while_flag==False)):
					not_full_barrier.append(True)
				else:
					not_full_barrier.append(not_full_barrier_tmp)
				if (len(barrier_tmp)>0):
					data_array.append(copy.copy(barrier_tmp))
				barrier_tmp.clear()	
			else:
				i = i + 1
		barrier_array=[]
		for i in range(0, len(data_array)):
			dist_array=[]
			gran_left=-1
			gran_left=-1
			angle_left=-1
			angle_right=-1
			for j in range(0, len(data_array[i])):
				dist_array.append(data_array[i][j].dist)
			if(i==0):
				if(flags[0]==0):
					gran_right=False
					angle_right=0
				else:
					gran_right=True
					angle_right=data_array[i][0].angle
				if((len(data_array)==1)and(not_full_barrier[i])):  # случай, когда препятствие  сплошное, занимает весь сектор обзора лидара
					gran_left=False
					j=len(data_array[i])-1
					angle_left = data_array[i][j].angle
				else:
					gran_left=True
					j=len(data_array[i])-1
					angle_left = data_array[i][j].angle
			if(i>0):
				gran_right=True
				angle_right = data_array[i][0].angle
				if((i==(len(data_array)-1))and(not_full_barrier[i])):
					gran_left=False
				else:
					gran_left=True
				j=len(data_array[i])-1
				angle_left = data_array[i][j].angle
			start_angle=self.left_start_angle
			e = self.servo.get_min_step()
			if(e%2!=0):
				e=(e//2)+1
			else:
				e=int(e/2)
			start_array=[]
			for j in range(0, len(data_array[i])):
				if(abs(data_array[i][j].angle-start_angle)<e):
					start_array.append(data_array[i][j])
			dist_start = -1
			if(len(start_array)==2):
				if(abs(start_array[0].angle - e)<abs(start_array[1].angle - e)):
					dist_start = start_array[0].dist
				else:
					dist_start = start_array[1].dist
			if(len(start_array)==1):
				dist_start = start_array[0].dist
				 
			tmp=barrier_data(data_array[i], min(dist_array), dist_start, dist_array[0], gran_left, angle_left, dist_array[len(dist_array)-1], gran_right, angle_right)
			barrier_array.append(tmp)
			print("")
			print("sz = ", len(barrier_array[i].array))
			print("min = ", barrier_array[i].min)
			print("dist_start = ", barrier_array[i].dist_start)
			print("left_dist = ", barrier_array[i].dist_left)
			print("left_gran = ", barrier_array[i].gran_left)
			print("left_angle = ", barrier_array[i].angle_left)
			print("right_dist = ", barrier_array[i].dist_right)
			print("right_gran = ", barrier_array[i].gran_right)
			print("right_angle = ", barrier_array[i].angle_right)
			print("")
		return(barrier_array)
	
	
	def find_barrier_width(self, distance):
		const_distance = -1 #16
		if(distance==4):
			const_distance = self.max_distance_4cm
		if(distance==8):
			const_distance = self.max_distance_8cm
		unfiltred_left_barrier_array = self.scan_left_lidar()
		left_barrier_array = []
		const_delta_angle = 25 
		const_delta_left_angle = self.left_start_angle + const_delta_angle
		
		for i in range(0, len(unfiltred_left_barrier_array)):
			if((unfiltred_left_barrier_array[i].angle_right < const_delta_left_angle)and(unfiltred_left_barrier_array[i].min < const_distance)):
				left_barrier_array.append(unfiltred_left_barrier_array[i])
		for i in range(0, len(left_barrier_array)):
			print("left filtred barriers")
			print("sz = ", len(left_barrier_array[i].array))
			print("min = ", left_barrier_array[i].min)
			print("dist_start = ", left_barrier_array[i].dist_start)
			print("left_dist = ", left_barrier_array[i].dist_left)
			print("left_gran = ", left_barrier_array[i].gran_left)
			print("left_angle = ", left_barrier_array[i].angle_left)
			print("right_dist = ", left_barrier_array[i].dist_right)
			print("right_gran = ", left_barrier_array[i].gran_right)
			print("right_angle = ", left_barrier_array[i].angle_right)
			print("")
			
		unfiltred_right_barrier_array = self.scan_right_lidar()
		right_barrier_array = []
		const_delta_right_angle = self.right_start_angle - const_delta_angle
		for i in range(0, len(unfiltred_right_barrier_array)):
			print(unfiltred_right_barrier_array[i].angle_left > const_delta_right_angle)
			print(unfiltred_right_barrier_array[i].min < const_distance)
			if((unfiltred_right_barrier_array[i].angle_left > const_delta_right_angle)and(unfiltred_right_barrier_array[i].min < const_distance)):
				right_barrier_array.append(unfiltred_right_barrier_array[i])
		for i in range(0, len(right_barrier_array)):
			print("right filtred barriers")
			print("sz = ", len(right_barrier_array[i].array))
			print("min = ", right_barrier_array[i].min)
			print("dist_start = ", right_barrier_array[i].dist_start)
			print("left_dist = ", right_barrier_array[i].dist_left)
			print("left_gran = ", right_barrier_array[i].gran_left)
			print("left_angle = ", right_barrier_array[i].angle_left)
			print("right_dist = ", right_barrier_array[i].dist_right)
			print("right_gran = ", right_barrier_array[i].gran_right)
			print("right_angle = ", right_barrier_array[i].angle_right)
			print("")
		
		width_barrier = -1
		if(len(left_barrier_array)==1):
			if(left_barrier_array[0].gran_left==True):
				if(left_barrier_array[0].gran_right==True):
					angle = abs(left_barrier_array[0].angle_right - left_barrier_array[0].angle_left)
					angle_rad = (angle*math.pi)/180
					width_barrier = math.sqrt(left_barrier_array[0].dist_left**2 + left_barrier_array[0].dist_right**2 - 2*left_barrier_array[0].dist_left*left_barrier_array[0].dist_right*math.cos(angle_rad))
			if(left_barrier_array[0].gran_right==False):
				if((len(right_barrier_array)==1)and(right_barrier_array[0].gran_left==False)):
					angle_left = left_barrier_array[0].angle_left - self.left_start_angle #angle_left
					left_minus=False
					if(angle_left<0):
						left_minus=True
						angle_left = abs(left_barrier_array[0].gran_left - left_barrier_array[0].gran_right) #-angle_left
					print("angle_left = ", angle_left)
					#print("minus = ", left_minus)
					
					angle_left_rad = (angle_left*math.pi)/180
					if(left_minus==False):
						left_width = math.sqrt(left_barrier_array[0].dist_left**2 + left_barrier_array[0].dist_start**2 - 2*left_barrier_array[0].dist_left*left_barrier_array[0].dist_start*math.cos(angle_left_rad))
					else:
						left_width = math.sqrt(left_barrier_array[0].dist_left**2 + left_barrier_array[0].dist_right**2 - 2*left_barrier_array[0].dist_left*left_barrier_array[0].dist_right*math.cos(angle_left_rad))
					angle_right = self.right_start_angle - right_barrier_array[0].angle_right #right
					right_minus=False
					if(angle_right<0):
						right_minus=True
						angle_right = abs(right_barrier_array[0].gran_left - right_barrier_array[0].gran_right)#-angle_right
					print("angle_right = ", angle_right)
					angle_right_rad = (angle_right*math.pi)/180
					if(right_minus==False):
						right_width = math.sqrt(right_barrier_array[0].dist_right**2 + right_barrier_array[0].dist_start**2 - 2*right_barrier_array[0].dist_right*right_barrier_array[0].dist_start*math.cos(angle_right_rad))
					else:
						
						right_width = math.sqrt(right_barrier_array[0].dist_right**2 + right_barrier_array[0].dist_left**2 - 2*right_barrier_array[0].dist_right*right_barrier_array[0].dist_left*math.cos(angle_right_rad))
					if(left_minus==True):
						print("left_width = -", left_width)
					else:
						print("left_width = ", left_width)
					if(right_minus==True):
						print("right_width = -", right_width)
					else:
						print("right_width = ", right_width)
					width_barrier = self.lidar_base
					flag=False
					if(distance==4):
						flag=True
						left_width = left_width*self.k_left*self.k_4cm
						right_width = right_width*self.k_right*self.k_4cm
					if(distance==8):
						flag=True
						left_width = left_width*self.k_left*self.k_8cm
						right_width = right_width*self.k_right*self.k_8cm
					if(flag==False):
						print("Uncorrect distance in argument")
					if(left_minus==True):
						width_barrier = width_barrier - left_width
					else:
						width_barrier = width_barrier + left_width
					if(right_minus==True):
						width_barrier = width_barrier - right_width
					else:
						width_barrier = width_barrier + right_width
					print("w b = ", width_barrier)
		return(width_barrier)		
		
		
	def calculate_barrier_width(self):
		width4 = self.find_barrier_width(4)
		self.motor.go_back_to_distance(4.5, self.lm393)
		width8 = self.find_barrier_width(8)
		width = -1
		if((width4>0)and(width8>0)):
			width = (width4 + width8)/2
		if((width4<0)and(width8>0)):
			width = width8
		if((width4>0)and(width8<0)):
			width = width4
		print("width 4cm = ", width4)
		print("width 8cm = ", width8)
		print("width = ", width)
		return(width)
		

	def close(self):
		self.servo.close()
		

"""servo = servo.Servo()
servo.rotate_front_right(124)
servo.rotate_front_left(10)
ultra = hcsr04.HCSR04()
while(True):
	print(ultra.get_mean_distance_right())
	time.sleep(0.5)"""

"""m=L298D.Motor()
lm393 = LM393.LM393()
lidar = Lidar(m, lm393)
lidar.scan_left_lidar()              #calculate_barrier_width()
m.close()"""

#this
"""
m=L298D.Motor()
lm393 = LM393.LM393()
lidar = Lidar(m, lm393)
print(lidar.calculate_barrier_width())
"""

"""m.go_back_to_distance(8, lm393)
#m1.rotate_front_left(200, 1)
lidar.close()
#m1.rotate_front_right(200, 1)"""
