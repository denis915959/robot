import time  #RABOTAET!
from adafruit_servokit import ServoKit

class Servo:
    def __init__(self):
        self.coefficient_angle = 130/90 # коэффициэнт перевода угла, введенного пользователем, в реальный угол поворота
        self.kit = ServoKit(channels=16) # активируем плату PCA9685
        self.kit.frequency = 50  #устаеавливаем частоту ШИМ 50 Hz
        self.left_start=5 # левая стартовая позиция (датчик смотрит прямо по курсу робота)
        self.right_start=155 # правая стартовая позиция (датчик смотрит прямо по курсу робота)
        self.min_step=5 # 7
        self.max_angle = int(180/self.coefficient_angle)
        

    def rotate_left(self,angle): # поворот влево на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if((angle>180)or(angle<0)):
            return(False)
        else:
            self.kit.servo[14].angle = angle # номер пина для левого серво в pca9685
            return(True)
            
    def rotate_right(self,angle): # поворот вправо на угол (относительно 0)
        angle = self.coefficient_angle*angle
        if((angle>180)or(angle<0)):
            return(False)
        else:
            self.kit.servo[15].angle = angle # номер пина для правого серво в pca9685
            return(True)
            
    def left_to_start_position(self): # установить левый серво в стартовое положение, датчик смотрит прямо по курсу робота!
        self.kit.servo[14].angle = self.left_start
        
    def right_to_start_position(self): # установить правый серво в стартовое положение, датчик смотрит прямо по курсу робота!
        self.kit.servo[15].angle = self.right_start
        
    def close(self): # вызывать после завершения работы с объектом класса Servo
        time.sleep(1)
        self.kit.servo[14].angle = None
        self.kit.servo[15].angle = None
        
    def get_min_step(self): # узнать минимальный размер шага сервопривода 
        return(self.min_step)
        
    def get_max_angle(self):
        return(self.max_angle)
    


"""servo = Servo()
servo.rotate_right(20)
time.sleep(1)
servo.rotate_right(20)
time.sleep(2)
servo.close()"""





































