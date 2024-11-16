import time  #RABOTAET!
from adafruit_servokit import ServoKit

class Servo:
    def __init__(self):
        self.coefficient_angle = 130/90 # коэффициэнт перевода угла, введенного пользователем, в реальный угол поворота
        self.kit = ServoKit(channels=16) # активируем плату PCA9685
        self.kit.frequency = 50  #устаеавливаем частоту ШИМ 50 Hz
        self.left_start=31 #34 левая стартовая позиция (датчик смотрит прямо по курсу робота)
        self.right_start=110 # правая стартовая позиция (датчик смотрит прямо по курсу робота)
        self.back_right_start=15
        self.back_left_start = 102
        self.min_step=5 # 7
        self.max_angle = int(180/self.coefficient_angle)
        

    def rotate_front_left(self,angle): # поворот влево на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if(angle>180):
            angle=180
        if(angle<0):
            angle=0
        self.kit.servo[14].angle = angle #14 # номер пина для левого серво в pca9685
        return(True)
            
    def rotate_front_right(self,angle): # поворот вправо на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if(angle>180):
            angle=180
        if(angle<0):
            angle=0
        self.kit.servo[15].angle = angle # номер пина для правого серво в pca9685
        return(True)
            
    def rotate_back_right(self,angle): # поворот вправо на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if(angle>180):
            angle=180
        if(angle<0):
            angle=0
        self.kit.servo[13].angle = angle # номер пина для правого серво в pca9685
        return(True)
            
    def rotate_back_left(self,angle): # поворот влево на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if(angle>180):
            angle=180
        if(angle<0):
            angle=0
        self.kit.servo[12].angle = angle # номер пина для левого серво в pca9685
        return(True)
            
    def left_to_start_position(self): # установить левый серво в стартовое положение, датчик смотрит прямо по курсу робота!
        self.rotate_front_left(self.left_start)
        
    def right_to_start_position(self): # установить правый серво в стартовое положение, датчик смотрит прямо по курсу робота!
        self. rotate_front_right(self.right_start)
        
    def back_right_to_start_position(self): # установить правый серво в стартовое положение, датчик смотрит nazad!
        self.rotate_back_right(self.back_right_start)
        
    def back_left_to_start_position(self): # установить правый серво в стартовое положение, датчик смотрит nazad!
        self.rotate_back_left(self.back_left_start)
        
    def front_left_vbok(self):
        self.rotate_front_left(self.left_start+88)
        
    def front_right_vbok(self):
        self.rotate_front_right(self.right_start-93)
        
    def back_left_vbok(self):
        self.rotate_back_left(self.back_left_start-90)
        
    def back_right_vbok(self):
        self.rotate_back_right(self.back_right_start+93)
        
    def close(self): # вызывать после завершения работы с объектом класса Servo
        time.sleep(1)
        self.kit.servo[14].angle = None
        self.kit.servo[15].angle = None
        
    def get_min_step(self): # узнать минимальный размер шага сервопривода 
        return(self.min_step)
        
    def get_max_angle(self):
        return(self.max_angle)
        
    def get_left_start_angle(self):
        return(self.left_start)
        
    def get_right_start_angle(self):
        return(self.right_start)
    










"""
servo = Servo()

servo.front_left_vbok()

time.sleep(1)
servo.back_left_to_start_position()
time.sleep(1) 
#servo.rotate_front_right(65)  # left = 80, right = 65
#time.sleep(2)
servo.close()"""





































