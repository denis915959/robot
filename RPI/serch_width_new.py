import hcsr04
import servo
#import L298D
import LM393
from hmc5883 import *
import time
from collections import namedtuple
import math
import copy
import smbus
import os
import RPi.GPIO as GPIO
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
lidar_measure = namedtuple('lidar_measure', ['angle', 'dist'])
barrier_data = namedtuple('barrier_data', ['array', 'min', 'dist_start', 'dist_left', 'gran_left', 'angle_left', 'dist_right', 'gran_right', 'angle_right'])

def writenumber_i2c(num):
    bus.write_byte(SLAVE_ADDRESS, int(num))
def write_array(a, b, c, d):
    bus.write_i2c_block_data(SLAVE_ADDRESS, a, [b, c, d])
def readnumber():
    num=int(bus.read_byte(SLAVE_ADDRESS))
    return num


class Search_Width:
    def __init__(self, m, lm393):
        self.m = m  # L298D.Motor()
        self.lidar = hcsr04.HCSR04()
        self.delta_distance = self.lidar.get_delta_distance()
        self.compass = hmc5883l()
        self.lm393 = lm393  # LM393.LM393()
        self.lm393.start_with_delay()
        self.servo = servo.Servo()
        
        self.speed = 130 
        self.rotate_speed = 230
        self.delta_distance = self.lidar.get_delta_distance()
        self.const_distance_barrier = 30 #? предельная дальность до препятствия
        self.const_move_before_stop = 0.7 # проезд робота перед остановкой при скорости 130 при движении вперед до срабатывания лидара  proezd robota 
        self.const_distance_between_lidars = 28.75
        self.tmp_2_5 = 2.5 - 1 # 1.21 # это значение недоезда робота из-за того, что датчик имеет угол луча 10 градусов и срабатывает несколько раньше, чем надо
        self.const_distance_barrier_back = 8
        
        self.dist_start_dynamic= -1 # pole dlya metoda dynamic_course_correction
        self.interval_start_dynamic = -1 # pole dlya metoda dynamic_course_correction
        self.correction_step = 5 # pole dlya metoda dynamic_course_correction
        self.correction_delta = 1 # 1
        
        self.pribavka_S = 2.5   # dlya find_width_left_koridor
        self.mnozhitel_S = 1.1   # dlya find_width_left_koridor
        
        self.dist_from_centr_to_back = 16.5  # dlya obiezd
        self.dist_from_centr_to_front = 12
        self.half_width = 10
        
        self.max_difference = 5 # for scan_servo_left
        self.left_start_angle = self.servo.get_left_start_angle()
        self.const_after_barrier = 29 # 25
        self.tcrt5000 = 18
        self.const_distance_correct_final = self.const_after_barrier - self.half_width #-10
        
        self.sleep_for_arduino_rotate = 7
        
		
		
    def rotate_left_kompass(self, angle):
        if(angle>180):
            print("нельзя повернуть на угол, больший 180 градусов")
            return
        data =  self.compass.get_calibrated() 
        angle_start = self.compass.heading(data['x'], data['y'])
        angle_now = angle_start # это не абсолютный угол, а угол, на который повернулся робот относительно своего изначального направления. но до цикла он абсолютный
        while(True): # именно так, иначе while будет проигнорирован
            self.m.rotate_left(self.rotate_speed, 0.05, non_stop=True)
            data =  self.compass.get_calibrated() 
            angle_now = self.compass.heading(data['x'], data['y'])
            delta = abs(angle_start - angle_now)
            if(delta > 180):
                angle_now = 360 - delta
            else:
                angle_now = delta
            if(angle_now > angle):
                break
        self.m.stop()
        
        
    def rotate_right_kompass(self, angle):
        if(angle>180):
            print("нельзя повернуть на угол, больший 180 градусов")
            return
        data =  self.compass.get_calibrated() 
        angle_start = self.compass.heading(data['x'], data['y'])
        angle_now = angle_start # это не абсолютный угол, а угол, на который повернулся робот относительно своего изначального направления. но до цикла он абсолютный
        while(True): # именно так, иначе while будет проигнорирован
            self.m.rotate_right(self.rotate_speed, 0.05, non_stop=True)
            data =  self.compass.get_calibrated() 
            angle_now = self.compass.heading(data['x'], data['y'])
            delta = abs(angle_start - angle_now)
            if(delta > 180):
                angle_now = 360 - delta
            else:
                angle_now = delta
            if(angle_now > angle):
                break
        self.m.stop()
        
        
    def dynamic_back_course_correction(self, S, dist): # dlya nebolshoi korrektirovki
        if(dist!=-1):
            self.dist_start_dynamic = dist
            self.interval_start_dynamic = S
        tmp_dist = self.lidar.back_distance_left() - self.delta_distance 
        if(tmp_dist < self.const_distance_barrier): # new
            if((S - self.interval_start_dynamic)>=7):
                dist_left = self.lidar.back_distance_left() - self.delta_distance
                if((dist_left -  self.dist_start_dynamic) >= self.correction_delta):
                    self.m.stop()
                    self.rotate_right_kompass(self.correction_step)
                elif((dist_left -  self.dist_start_dynamic) <= -self.correction_delta):
                    self.m.stop()
                    self.rotate_left_kompass(self.correction_step)
                dist_left = self.lidar.back_distance_left() - self.delta_distance
                self.interval_start_dynamic = S
                self.dist_start_dynamic = dist_left
            
            
    def dynamic_front_course_correction(self, S, dist): # dlya nebolshoi korrektirovki
        if(dist!=-1):
            self.dist_start_dynamic = dist
            self.interval_start_dynamic = S
        tmp_dist = self.lidar.distance_left() - self.delta_distance
        if(tmp_dist < self.const_distance_barrier): # new
            if((S - self.interval_start_dynamic)>=7):
                dist_left = self.lidar.distance_left() - self.delta_distance
                if((dist_left -  self.dist_start_dynamic) >= self.correction_delta):
                    self.m.stop()
                    self.rotate_left_kompass(self.correction_step)
                elif((dist_left -  self.dist_start_dynamic) <= -self.correction_delta):
                    self.m.stop()
                    self.rotate_right_kompass(self.correction_step)
                dist_left = self.lidar.distance_left() - self.delta_distance
                self.interval_start_dynamic = S
                self.dist_start_dynamic = dist_left
                
                
    def dynamic_front_course_correction_after_barrier(self, S, dist): # dlya nebolshoi korrektirovki
        if(dist!=-1):
            self.dist_start_dynamic = dist
            self.interval_start_dynamic = S
        tmp_dist = self.lidar.distance_right() - self.delta_distance
        print("tmp_dist = ", tmp_dist)
        if(tmp_dist < self.const_distance_barrier): # new
            if((S - self.interval_start_dynamic)>=7):
                dist_right = self.lidar.distance_right() - self.delta_distance
                print("method: dist_right = ", dist_right)
                print("method: self.dist_start_dynamic = ", self.dist_start_dynamic)
                if((dist_right -  self.dist_start_dynamic) >= self.correction_delta):
                    self.m.stop()
                    self.rotate_right_kompass(self.correction_step)
                elif((dist_right -  self.dist_start_dynamic) <= -self.correction_delta):
                    self.m.stop()
                    self.rotate_left_kompass(self.correction_step)
                dist_right = self.lidar.distance_right() - self.delta_distance
                self.interval_start_dynamic = S
                self.dist_start_dynamic = dist_right
        
        
    def course_correction(self): # корректировка курса
        self.servo.back_left_vbok()
        self.servo.front_left_vbok()
        time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
        distance_back = self.lidar.back_distance_left() - self.delta_distance
        distance_front = self.lidar.distance_left() - self.delta_distance
        if((distance_back<self.const_distance_barrier) and (distance_front<self.const_distance_barrier) and (abs(distance_back - distance_front)>1)): #2 ?
            if(distance_front < distance_back):
                while(distance_front < distance_back):
                    self.m.rotate_right(self.rotate_speed, 0.05, non_stop=True)
                    distance_front = self.lidar.distance_left() - self.delta_distance
                    distance_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
            else:
                while(distance_front > distance_back):
                    self.m.rotate_left(self.rotate_speed, 0.05, non_stop=True)
                    distance_front = self.lidar.distance_left() - self.delta_distance
                    distance_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
        self.m.stop()
        
        
    def right_course_correction(self): # корректировка курса sleva
        self.servo.back_right_vbok()
        self.servo.front_right_vbok()
        time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
        distance_back = self.lidar.back_distance_right() - self.delta_distance
        distance_front = self.lidar.distance_right() - self.delta_distance
        if((distance_back<self.const_distance_barrier) and (distance_front<self.const_distance_barrier) and (abs(distance_back - distance_front)>1)): #2 ?
            if(distance_front < distance_back):
                while(distance_front < distance_back):
                    self.m.rotate_left(self.rotate_speed, 0.05, non_stop=True)
                    distance_front = self.lidar.distance_left() - self.delta_distance
                    distance_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
            else:
                while(distance_front > distance_back):
                    self.m.rotate_right(self.rotate_speed, 0.05, non_stop=True)
                    distance_front = self.lidar.distance_left() - self.delta_distance
                    distance_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
        self.m.stop()
        
        
    """def measurement_kompass(self):"""
            
            
    def go_to_left_gran_barrier(self): # peremeshenie k levoi grani bariera
        self.servo.back_left_vbok()
        self.servo.back_right_to_start_position()
        self.servo.left_to_start_position()
        self.servo.right_to_start_position()
        d1=0
        distance_left_back = self.lidar.back_distance_left() - self.delta_distance
        barrier_front = False
        barrier_back = False
        if(distance_left_back > self.const_distance_barrier): # ne vidit prepyatstvie vnachale
            self.servo.back_left_vbok()
            self.servo.back_right_to_start_position()
            self.servo.left_to_start_position() # проверка препятствия сзади
            self.servo.right_to_start_position()
            
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_front_left = self.lidar.distance_left() - self.delta_distance
            distance_front_right = self.lidar.distance_right() - self.delta_distance
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_front_left > self.const_distance_barrier_back)and(distance_front_right > self.const_distance_barrier_back)):
				# pri dvizhenii vpered use front_lidar
                dist = self.lidar.distance_left() - self.delta_distance # dlya dynamic_correction        # back_
                S=0
                i = 0
                while(distance_left_back>self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    if(i==0):
                        self.servo.front_left_vbok()
                        time.sleep(0.5)
                    self.m.go_front(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                    self.servo.right_to_start_position()
                    distance_front_right = self.lidar.distance_right() - self.delta_distance
                    distance_front_left = self.lidar.distance_left() - self.delta_distance
                    
                    if(i==0):
                        self.dynamic_front_course_correction(S, dist)
                    else:
                        self.dynamic_front_course_correction(S, -1)
                    dist = self.lidar.distance_left() - self.delta_distance ###########
                    speed_l = self.lm393.get_speed_left()
                    speed_r = self.lm393.get_speed_right()
                    S_l = speed_l*0.05
                    S_r = speed_r*0.05
                    S = S + (S_l + S_r)/20
                    i = i + 1
                    
                    if((distance_front_right < self.const_distance_barrier_back)or(distance_front_left < self.const_distance_barrier_back)):
                        barrier_front = True
                        break
                time.sleep(1)
                while(distance_left_back < self.const_distance_barrier): 
                    self.m.go_back(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
                    
            else:
                barrier_front = True
        else:   # vidit prepyatstvie s samogo nachala
            self.servo.front_left_vbok()
            self.servo.front_right_vbok()
            self.servo.back_right_to_start_position() # проверка препятствия сзади
            self.servo.back_left_to_start_position()
            
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_back_left = self.lidar.back_distance_left() - self.delta_distance
            distance_back_right = self.lidar.back_distance_right() - self.delta_distance
            self.servo.back_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_back_left > (self.const_distance_barrier_back + 5))and(distance_back_right > (self.const_distance_barrier_back + 5))):
				# pri dvizhenii nazad use back_lidar
                dist = self.lidar.back_distance_left() - self.delta_distance # dlya dynamic_correction
                S=0
                i = 0
                d1 = dist
                while(distance_left_back < self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    if(i==0):
                        self.servo.back_left_vbok()
                        time.sleep(0.5)
                    self.m.go_back(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                    if(i==0):
                        self.dynamic_back_course_correction(S, dist)
                    else:
                        self.dynamic_back_course_correction(S, -1)
                    dist = self.lidar.back_distance_left() - self.delta_distance ###########
                    speed_l = self.lm393.get_speed_left()
                    speed_r = self.lm393.get_speed_right()
                    S_l = speed_l*0.05
                    S_r = speed_r*0.05
                    S = S + (S_l + S_r)/20
                    i = i + 1
                    distance_back_right = self.lidar.back_distance_right() - self.delta_distance
                    if(distance_back_right < self.const_distance_barrier_back):
                        barrier_back = True
                        break
                self.dist_start_dynamic= -1
                self.interval_start_dynamic = -1
                self.m.go_back_to_distance(5, self.lm393)
                self.m.stop()
                time.sleep(0.5)
                while(distance_left_back > self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    self.m.go_front(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                self.m.stop()
            else:
                barrier_back = True
        distance_back_left = self.lidar.back_distance_left() - self.delta_distance
        print("distance_back_left = ", distance_back_left)
        res=[]
        res.append(d1)
        if(barrier_front == True):
            res.append(-2)
            return(res)
        if(barrier_back == True):
            res.append(-1)
            return(res)
        if((barrier_front == False)and(barrier_back == False)):
            res.append(1)
            return(res)
		

    def go_to_right_gran_barrier(self): # peremeshenie k levoi grani bariera
        distance_left = self.lidar.distance_left() - self.delta_distance
        barrier_front = False
        barrier_back = False
        if(distance_left > self.const_distance_barrier): # ne vidit prepyatstvie vnachale
            self.servo.back_right_to_start_position() # проверка препятствия сзади
            self.servo.back_left_to_start_position()
            self.servo.front_left_vbok()
            self.servo.front_right_vbok()
            
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_back_left = self.lidar.back_distance_left() - self.delta_distance
            distance_back_right = self.lidar.back_distance_right() - self.delta_distance
            self.servo.front_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_back_left > self.const_distance_barrier_back)and(distance_back_right > self.const_distance_barrier_back)):
                dist = self.lidar.back_distance_left() - self.delta_distance # dlya dynamic_correction
                S=0
                i = 0
                while(distance_left > self.const_distance_barrier): # nazad, poka ne doedet do grani bariera
                    if(i==0):
                        self.servo.back_left_vbok()
                        time.sleep(0.5)
                    self.m.go_back(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                    if(i==0):
                        self.dynamic_back_course_correction(S, dist)
                    else:
                        self.dynamic_back_course_correction(S, -1)
                    dist = self.lidar.back_distance_left() - self.delta_distance 
                    speed_l = self.lm393.get_speed_left()
                    speed_r = self.lm393.get_speed_right()
                    S_l = speed_l*0.05
                    S_r = speed_r*0.05
                    S = S + (S_l + S_r)/20
                    i = i + 1
                    
                    distance_back_right = self.lidar.back_distance_right() - self.delta_distance
                    distance_back_left = self.lidar.back_distance_left() - self.delta_distance
                    distance_left = self.lidar.distance_left() - self.delta_distance
                    if(distance_back_right < self.const_distance_barrier_back):
                        barrier_back = True
                        break
            else:
                barrier_back = True  
        else:   # vidit prepyatstvie s samogo nachala
            self.servo.right_to_start_position() # проверка препятствия сзади
            self.servo.left_to_start_position()
            self.servo.back_left_vbok()
            self.servo.back_right_to_start_position()
            
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_front_left = self.lidar.distance_left() - self.delta_distance
            distance_front_right = self.lidar.distance_right() - self.delta_distance
            self.servo.front_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_front_left > (self.const_distance_barrier_back + 5))and(distance_front_right > (self.const_distance_barrier_back + 5))):
                dist = self.lidar.distance_left() - self.delta_distance # dlya dynamic_correction        # back_
                S=0
                i = 0
                while(distance_left < self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    if(i==0):
                        self.servo.front_left_vbok()
                        time.sleep(0.5)
                    self.m.go_front(self.speed, 0.05, non_stop = True)
                    distance_left = self.lidar.distance_left() - self.delta_distance
                    self.servo.right_to_start_position()
                    distance_front_right = self.lidar.distance_right() - self.delta_distance
                    
                    if(i==0):
                        self.dynamic_front_course_correction(S, dist)
                    else:
                        self.dynamic_front_course_correction(S, -1)
                    dist = self.lidar.distance_left() - self.delta_distance ###########
                    speed_l = self.lm393.get_speed_left()
                    speed_r = self.lm393.get_speed_right()
                    S_l = speed_l*0.05
                    S_r = speed_r*0.05
                    S = S + (S_l + S_r)/20
                    i = i + 1
                    
                    if(distance_front_right < (self.const_distance_barrier_back+5)):
                        barrier_front = True
                        break
                if(barrier_front == False):
                    self.m.go_front_to_distance(5, self.lm393)
                    time.sleep(0.5)
                    while(distance_left > self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                        self.m.go_back(self.speed, 0.05, non_stop = True)
                        distance_left = self.lidar.distance_left() - self.delta_distance
                    self.m.stop()
        if(barrier_front == True):
            return(-2)
        if(barrier_back == True):
            return(-1)
        if((barrier_front == False)and(barrier_back == False)):
            return(1)
            
            
    def scan_left_lidar(self):
        left_lidar_data = []
        step = self.servo.get_min_step()
        left_max_angle = 20
        left_servo_angle = left_max_angle
        self.servo.rotate_front_left(left_servo_angle)
        time.sleep(1)
        distance=-1
        rotate_front_range = self.servo.get_left_start_angle() + 90
        while(1):
            stop=False
            if(left_servo_angle < rotate_front_range):	
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
            if(left_lidar_data[i].dist <= self.const_distance_barrier):
                barrier_tmp=[]
                barrier_tmp.append(left_lidar_data[i])
                i = i + 1
                while((i<size)and(left_lidar_data[i].dist <= self.const_distance_barrier)):
                    barrier_tmp.append(left_lidar_data[i])
                    i = i + 1
                if(len(barrier_tmp)>0):
                    data_array.append(copy.copy(barrier_tmp))
                barrier_tmp.clear()
            else:
                i = i + 1
        
        barrier_array=[]
        for i in range(0, len(data_array)):
            dist_array=[]
            gran_left=False
            gran_right=False
            for j in range(0, len(data_array[i])):
                dist_array.append(data_array[i][j].dist)
            start_angle = data_array[i][0].angle
            end_angle = data_array[i][len(data_array[i]) - 1].angle
            e = self.servo.get_min_step()
            if(e%2!=0):
                e=(e//2)+1
            else:
                e=int(e/2)
            start_array=[]
            for j in range(0, len(data_array[i])):
                if(abs(data_array[i][j].angle-start_angle)<e):
                    start_array.append(data_array[i][j])     
            dist_start = start_array[0].dist
				 
            tmp=barrier_data(data_array[i], min(dist_array), dist_start, dist_array[0], gran_left, start_angle, dist_array[len(dist_array)-1], gran_right, end_angle)
            barrier_array.append(tmp)
            """print("")
            print("sz = ", len(barrier_array[i].array))
            print("min = ", barrier_array[i].min)
            print("dist_start = ", barrier_array[i].dist_start)
            print("left_dist = ", barrier_array[i].dist_left)
            print("left_gran = ", barrier_array[i].gran_left)
            print("left_angle = ", barrier_array[i].angle_left)
            print("right_dist = ", barrier_array[i].dist_right)
            print("right_gran = ", barrier_array[i].gran_right)
            print("right_angle = ", barrier_array[i].angle_right)
            print("")"""
        result = []
        for i in range(0, len(barrier_array)):
            if(barrier_array[i].min < self.const_distance_barrier):
                result.append(barrier_array[i])
        return(result)
            
            
    def find_width_left_koridor(self):
        S=0
        d2 = 0
        i = 0
        flag_left = -1
        result_left = 0
        self.servo.back_left_vbok()
        self.servo.back_right_to_start_position()
        self.servo.front_left_vbok()
        self.servo.right_to_start_position()
        i=0
        t=0
        t_0 = 0
        while(1):
            self.m.go_back(self.speed, 0.05, non_stop = True)
            speed_l = self.lm393.get_speed_left()
            speed_r = self.lm393.get_speed_right()
            if(i==0):
                t = 0.05
                t_0 = time.time()
            else:
                tmp = time.time()
                t = tmp - t_0
                t_0 = tmp
            S_l = speed_l*t
            S_r = speed_r*t
            S = S + (S_l + S_r)/20
            back_distance_right = self.lidar.back_distance_right()
            back_distance_left = self.lidar.back_distance_left()
            # proverki:
            if(S >= 35):
                flag_left = 1
                break
            if(back_distance_right < self.const_distance_barrier_back):
                flag_left = 0  # szadi barrier
                result_left = S + self.const_distance_barrier_back # tut dolzhen byt alpha
                break
            if((S >= 5)and(back_distance_left < self.const_distance_barrier)): # bez S robot tut zhe ostanavlivaetsya
                flag_left = 2  # naidena right granica stellazha.
                break
            i=i+1
        self.m.stop()
        if(flag_left != 0):
            self.m.go_back_to_distance(3, self.lm393)
            time.sleep(1)
            d2 = self.lidar.back_distance_left() - self.delta_distance 
            self.m.go_front_to_distance(3, self.lm393)
        if(S>25):
            S = S*self.mnozhitel_S
        else:
            S = S + self.pribavka_S
        if(flag_left == 1):
            result_left = S 
        if(flag_left == 2):
            result_left = S + 2*self.tmp_2_5
        print("S = ", S)
        print("result_left = ", result_left)      
        print("flag_left = ", flag_left)
        res = []
        res.append(S)
        res.append(result_left)
        res.append(d2)
        return(res)
       
       
       
       
    def obiezd_left(self):
        success = True
        self.rotate_right_kompass(90)
        time.sleep(0.5)
        self.course_correction()
        time.sleep(0.5)
        go_to_left_gran_res = self.go_to_left_gran_barrier()
        time.sleep(0.5)
        left_koridor = self.find_width_left_koridor()
        if(left_koridor[1] >= 35):
            distance = left_koridor[0]/2 - self.dist_from_centr_to_back + 2 
            if(distance > 0):
                self.m.go_front_to_distance(distance, self.lm393)
            if(distance < 0):
                self.m.go_back_to_distance(distance, self.lm393)
            time.sleep(1)
            d1 = go_to_left_gran_res[0]
            d2 = left_koridor[2]
            result_left = left_koridor[1]
            d1 = d1 - (self.dist_from_centr_to_front - self.half_width)
            d2 = d2 - (self.dist_from_centr_to_front - self.half_width)
            d3 = (result_left - 2*self.half_width)/2
            print("d1 = ", d1)
            print("d2 = ", d2)
            print("d3 = ", d3)
            g_r = math.sqrt(d2*d2 + d3*d3) # d3 - from right bok to left gran barrier, d2 - from front right lidar to blizhaishy storona(front) of barrier 
            cos_A_r = d2/g_r
            A_r_rad = math.acos(cos_A_r)
            g_l = math.sqrt(d1*d1 + d3*d3) # d3 - from right bok to left gran barrier, d1 - from front left lidar to blizhaishy storona(front) of stellazh 
            cos_A_l = d1/g_l
            calculate_angle_left_rad = math.acos(cos_A_l)
            calculate_angle_left = calculate_angle_left_rad/math.pi*180
            print("g_r = ", g_r)
            print("g_l = ", g_l)
            print()
            print("angle_right = ", A_r_rad/math.pi*180)
            print("calculate_angle_left = ", calculate_angle_left)
            time.sleep(0.5)
            self.rotate_left_kompass(90)
            time.sleep(0.5)
            res_scan_left_lidar = self.scan_left_lidar()
            len_res = len(res_scan_left_lidar)
            print("len_res = ", len_res)
            if(len_res>0):
                real_left_angle = res_scan_left_lidar[len(res_scan_left_lidar) - 1].angle_left
                real_left_angle = real_left_angle - self.servo.get_left_start_angle()
                print("real_left_angle = ", real_left_angle)
                delta = real_left_angle - calculate_angle_left
                print("delta = ", delta)
                while(delta>9): #podobrano 9, a ne 5, tak kak eto bolee krutoy ugol 
                    self.rotate_left_kompass(5)
                    time.sleep(0.5)
                    res_scan_left_lidar = self.scan_left_lidar()
                    len_res = len(res_scan_left_lidar)
                    print("while: len_res = ", len_res)
                    if(len_res>0):
                        real_left_angle = res_scan_left_lidar[len(res_scan_left_lidar) - 1].angle_left
                        real_left_angle = real_left_angle - self.servo.get_left_start_angle()
                        print("while: real_left_angle = ", real_left_angle)
                        delta = real_left_angle - calculate_angle_left
                        print("while: delta = ", delta)
                    else:
                        break
                        
            self.servo.left_to_start_position()
            self.servo.right_to_start_position()
            time.sleep(1)
            # zdes dolzhna nahoditsya proverka na barrier, flag is success
            self.servo.front_right_vbok()
            self.servo.front_left_vbok()
            time.sleep(0.5)
            S=0
            i = 0
            t=0
            t_0 = 0
            count = 0
            flag_counter = False
            start_S = 0
            counter_for_activate = 0
            flag_go_after_barrier = False
            left_koridor_len = left_koridor[1]
            const_obiezd = (left_koridor_len - self.half_width*2)/2
            print("const_obiezd = ", const_obiezd)
            while(1):
                # zdes kazhdye 15 cm stop and proverka na barrier, flag is success
                self.m.go_front(self.speed, 0.05, non_stop = True)
                speed_l = self.lm393.get_speed_left()
                speed_r = self.lm393.get_speed_right()
                if(i==0):
                    t = 0.05
                    t_0 = time.time()
                else:
                    tmp = time.time()
                    t = tmp - t_0
                    t_0 = tmp
                S_l = speed_l*t
                S_r = speed_r*t
                S = S + (S_l + S_r)/20
                i = i + 1
                print("S = ", S)
                left_distance = self.lidar.distance_left() - self.delta_distance
                right_distance = self.lidar.distance_right() - self.delta_distance
                print("right_distance = ", right_distance) 
                print("")
                if((right_distance<10)and(left_distance > const_obiezd)): # 10
                    self.m.stop()
                    self.rotate_left_kompass(5)
                elif((left_distance<10)and(right_distance > const_obiezd)): # 10
                    self.m.stop()
                    self.rotate_right_kompass(5)
                if(right_distance < self.const_distance_barrier):
                    counter_for_activate = counter_for_activate + 1
                if(counter_for_activate  == 5):
                    flag_counter = True
                if(flag_counter):
                    if(right_distance > self.const_distance_barrier):
                        count = count + 1
                    else:
                        count = 0
                if(count==10):
                    count = 0
                    flag_counter = False
                    start_S = S
                    print("start_S = ", start_S)
                    flag_go_after_barrier = True
                if((flag_go_after_barrier==True)and((S - start_S) >= self.const_after_barrier)):
                    break		
            self.m.stop()    
            time.sleep(0.5)
            self.rotate_right_kompass(90) 
            time.sleep(0.5)
            dist = self.lidar.distance_right() - self.delta_distance # dlya dynamic_correction
            self.servo.front_right_vbok()
            self.servo.back_right_vbok()
            self.servo.left_to_start_position()
            time.sleep(0.5)
            # zdes dolzhna nahoditsya proverka na barrier, flag is success
            S = 0
            i = 0
            t=0
            t_0 = 0
            self.dist_start_dynamic = -1

            dist = self.lidar.distance_right() - self.delta_distance
            print("dist kompass = ", dist)
            count = 0
            if(dist > self.const_distance_correct_final):
                while((dist > self.const_distance_correct_final)and(count<2)):
                    self.rotate_right_kompass(10)
                    time.sleep(0.5)
                    dist = self.lidar.distance_right() - self.delta_distance
                    print("dist kompass = ", dist)
                    count = count + 1
            else:
                while((dist < self.const_distance_correct_final)and(count<2)):
                    self.rotate_left_kompass(10)
                    time.sleep(0.5)
                    dist = self.lidar.distance_right() - self.delta_distance
                    count = count + 1
            
            while(1):
                # zdes dolzhna nahoditsya proverka na barrier, flag is success
                self.m.go_front(self.speed, 0.05, non_stop = True)
                dist = self.lidar.distance_right() - self.delta_distance ###########
                dist_back = self.lidar.back_distance_right() - self.delta_distance ###########
                if((dist>self.const_distance_barrier)and(dist_back>self.const_distance_barrier)):
                    self.right_course_correction()
                if(GPIO.input(self.tcrt5000) == 0):
                    break
                speed_l = self.lm393.get_speed_left()
                speed_r = self.lm393.get_speed_right()
                if(i==0):
                    t = 0.05
                else:
                    tmp = time.time()
                    t = tmp - t_0
                t_0 = tmp
                S_l = speed_l*0.05
                S_r = speed_r*0.05
                S = S + (S_l + S_r)/20
                if(i==0):
                    print("dist = ", dist)
                    self.dynamic_front_course_correction_after_barrier(S, dist)
                else:
                    self.dynamic_front_course_correction_after_barrier(S, -1)
                i = i + 1
                t_0 = time.time() 
            self.m.stop()
            self.m.activate_arduino_driver()
            time.sleep(1)
            mode = 2
            writenumber_i2c(mode)
            time.sleep(1)
            writenumber_i2c(1) # 1 - команда на захват линии с поворотом влево
            time.sleep(self.sleep_for_arduino_rotate) # inache poka ne rabotaet
            self.m.activate_rpi_driver()
            return(success)
            
            
		




#time.sleep(25)
"""
m = L298D.Motor()
lm393 = LM393.LM393()
m.activate_arduino_driver()
time.sleep(1)
mode = 2
writenumber_i2c(mode)
time.sleep(1)
writenumber_i2c(1) # 1 - команда на захват линии с поворотом влево
time.sleep(12) # inache poka ne rabotaet
m.activate_rpi_driver()
time.sleep(25)

test = Search_Width(m, lm393)
print(test.obiezd_left())
"""
