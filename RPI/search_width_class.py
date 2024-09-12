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

class Search_Width:
    def __init__(self):
        self.m = L298D.Motor()
        self.lidar = hcsr04.HCSR04()
        self.delta_distance = self.lidar.get_delta_distance()
        self.compass = hmc5883l()
        self.lm393 = LM393.LM393()
        self.lm393.start_with_delay()
        self.servo = servo.Servo()
        
        self.speed = 130
        self.rotate_speed = 220
        self.delta_distance = self.lidar.get_delta_distance()
        self.const_distance_barrier = 30 #? предельная дальность до препятствия
        self.const_move_before_stop = 0.7 # проезд робота перед остановкой при скорости 130 при движении вперед до срабатывания лидара  proezd robota 
        self.const_distance_between_lidars = 28.75
        self.tmp_2_5 = 2.5 - 1.21 # это значение недоезда робота из-за того, что датчик имеет угол луча 10 градусов и срабатывает несколько раньше, чем надо
        self.const_distance_barrier_back = 8
        
        self.dist_start_dynamic= -1 # pole dlya metoda dynamic_course_correction
        self.interval_start_dynamic = -1 # pole dlya metoda dynamic_course_correction
        
		
		
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
        
        
    def dynamic_back_course_correction(self, S, dist):
        if(dist!=-1):
            self.dist_start_dynamic = dist
            self.interval_start_dynamic = S
        if((S - self.interval_start_dynamic)>=7):
            dist_left = self.lidar.back_distance_left() - self.delta_distance
            if((dist_left -  self.dist_start_dynamic)>=1):
                self.m.stop()
                self.rotate_right_kompass(10)
            elif((dist_left -  self.dist_start_dynamic)<=-1):
                self.m.stop()
                self.rotate_left_kompass(10)
            dist_left = self.lidar.back_distance_left() - self.delta_distance
            self.interval_start_dynamic = S
            self.dist_start_dynamic = dist_left
            
            
    def dynamic_front_course_correction(self, S, dist):
        if(dist!=-1):
            self.dist_start_dynamic = dist
            self.interval_start_dynamic = S
        if((S - self.interval_start_dynamic)>=7):
            dist_left = self.lidar.distance_left() - self.delta_distance
            if((dist_left -  self.dist_start_dynamic)>=1):
                self.m.stop()
                self.rotate_right_kompass(10)
            elif((dist_left -  self.dist_start_dynamic)<=-1):
                self.m.stop()
                self.rotate_left_kompass(10)
            dist_left = self.lidar.distance_left() - self.delta_distance
            self.interval_start_dynamic = S
            self.dist_start_dynamic = dist_left
        
        
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
            
            
    def go_to_left_gran_barrier(self): # peremeshenie k levoi grani bariera
        self.servo.back_left_vbok()
        distance_left_back = self.lidar.back_distance_left() - self.delta_distance
        barrier_front = False
        barrier_back = False
        if(distance_left_back > self.const_distance_barrier): # ne vidit prepyatstvie vnachale
            self.servo.right_to_start_position() # проверка препятствия сзади
            self.servo.left_to_start_position()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_front_left = self.lidar.distance_left() - self.delta_distance
            distance_front_right = self.lidar.distance_right() - self.delta_distance
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_front_left > self.const_distance_barrier_back)and(distance_front_right > self.const_distance_barrier_back)):
                while(distance_left_back>self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    self.m.go_front(self.speed, 0.05, non_stop = True)
                    distance_left_back = self.lidar.back_distance_left() - self.delta_distance
                    self.servo.right_to_start_position()
                    self.servo.left_to_start_position()
                    distance_front_right = self.lidar.distance_right() - self.delta_distance
                    distance_front_left = self.lidar.distance_left() - self.delta_distance
                    if((distance_front_right < self.const_distance_barrier_back)or(distance_front_left < self.const_distance_barrier_back)):
                        barrier_front = True
                        break
            else:
                barrier_front = True
        else:   # vidit prepyatstvie s samogo nachala
            self.servo.back_right_to_start_position() # проверка препятствия сзади
            self.servo.back_left_to_start_position()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_back_left = self.lidar.back_distance_left() - self.delta_distance
            distance_back_right = self.lidar.back_distance_right() - self.delta_distance
            self.servo.back_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_back_left > (self.const_distance_barrier_back + 5))and(distance_back_right > (self.const_distance_barrier_back + 5))):
                dist = self.lidar.back_distance_left() - self.delta_distance # dlya dynamic_correction
                S=0
                i = 0
                while(distance_left_back < self.const_distance_barrier): # vpered, poka ne doede do grani bariera
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
        if(barrier_front == True):
            return(-2)
        if(barrier_back == True):
            return(-1)
        if((barrier_front == False)and(barrier_back == False)):
            return(1)
		

    def go_to_right_gran_barrier(self): # peremeshenie k levoi grani bariera
        distance_left = self.lidar.distance_left() - self.delta_distance
        barrier_front = False
        barrier_back = False
        if(distance_left > self.const_distance_barrier): # ne vidit prepyatstvie vnachale
            self.servo.back_right_to_start_position() # проверка препятствия сзади
            self.servo.back_left_to_start_position()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_back_left = self.lidar.back_distance_left() - self.delta_distance
            distance_back_right = self.lidar.back_distance_right() - self.delta_distance
            self.servo.front_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_back_left > self.const_distance_barrier_back)and(distance_back_right > self.const_distance_barrier_back)):
                while(distance_left > self.const_distance_barrier): # nazad, poka ne doedet do grani bariera
                    self.m.go_back(self.speed, 0.05, non_stop = True)
                    distance_left = self.lidar.distance_left() - self.delta_distance
                    distance_back_right = self.lidar.back_distance_right() - self.delta_distance
                    distance_back_left = self.lidar.back_distance_left() - self.delta_distance
                    if((distance_back_right < self.const_distance_barrier_back)or(distance_back_left < self.const_distance_barrier_back)):
                        barrier_back = True
                        break
            else:
                barrier_back = True  
        else:   # vidit prepyatstvie s samogo nachala
            self.servo.right_to_start_position() # проверка препятствия сзади
            self.servo.left_to_start_position()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            distance_front_left = self.lidar.distance_left() - self.delta_distance
            distance_front_right = self.lidar.distance_right() - self.delta_distance
            self.servo.front_left_vbok()
            time.sleep(0.5) # иначе измеряет какую-то рандомную дистанцию
            if((distance_front_left > (self.const_distance_barrier_back + 5))and(distance_front_right > (self.const_distance_barrier_back + 5))):
                while(distance_left < self.const_distance_barrier): # vpered, poka ne doede do grani bariera
                    self.m.go_front(self.speed, 0.05, non_stop = True)
                    distance_left = self.lidar.distance_left() - self.delta_distance
                    self.servo.right_to_start_position()
                    distance_front_right = self.lidar.distance_right() - self.delta_distance
                    if(distance_front_right < (self.const_distance_barrier_back+5)):
                        barrier_front = True
                        break
                if(barrier_front == False):
                    self.m.go_front_to_distance(5, self.lm393)
                    self.m.stop()
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
		
		
		
		
		
		


		
		
test = Search_Width()
print(test.go_to_left_gran_barrier())

