import RPi.GPIO as GPIO
import time

# Настраиваем GPIO на Raspberry Pi
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

class Motor:
    def __init__(self):
        self.pin6=11
        self.pin7=5
        self.pin8=6
        self.pin9=13
        self.pin10=19
        self.pin11=26
        self.RELE = 10
        GPIO.setup(self.pin6, GPIO.OUT)
        GPIO.setup(self.pin7, GPIO.OUT)
        GPIO.setup(self.pin8, GPIO.OUT)
        GPIO.setup(self.pin9, GPIO.OUT)
        GPIO.setup(self.pin10, GPIO.OUT)
        GPIO.setup(self.pin11, GPIO.OUT)
        GPIO.setup(self.RELE, GPIO.OUT)
        GPIO.output(self.RELE, GPIO.LOW) # Rele Low - RPI драйвер включен!
        self.left_motor_pwm = GPIO.PWM(self.pin6, 1000)  # Создаем объектs PWM для управления скоростью вращения мотора
        self.right_motor_pwm = GPIO.PWM(self.pin11, 1000)  # 1000 - частота ШИМ (в Гц)
        self.left_motor_pwm.start(50) # Запускаем ШИМ с начальной скоростью 50%
        self.right_motor_pwm.start(50)
        
    def set_speed_go_pered_left(self, speed):  #, delay=-1): # speed 0-255, только для левого (движение вперед), правую сторону не трогаем, так как подразумевается, что она будет вызвана рядом (это парные функции)
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)"""
        
    def set_speed_go_pered_right(self, speed):  #, delay=-1): # speed 0-255, только для правого (движение вперед), этот метод парный с методом set_speed_go_pered_left
        speed = int(speed/2.55)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)"""
        
    def set_speed_go_back_left(self, speed):  #, delay=-1): # speed 0-255, только для левого (движение назад), этот метод парный с методом set_speed_go_back_right
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)"""
        
    def set_speed_go_back_right(self, speed):  #, delay=-1):  # speed 0-255, только для правого (движение назад)
        speed = int(speed/2.55)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)"""
        
    def rotate_right(self, speed, delay=-1): # 0-255, разворот вправо
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
        
    def rotate_left(self, speed, delay=-1): # 0-255, разворот влево
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
        
    def go_pered(self, speed, delay=-1): # движение вперед
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
        
    def go_nazad(self, speed, delay=-1): # движение назад
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
        
    def set_all_motors_rotation(self, left_speed, pin7, pin8, pin9, pin10, right_speed, delay=-1): # Ручное управление направлением и скоростью моторов
        left_speed = int(left_speed/2.55)
        right_speed = int(right_speed/2.55)
        GPIO.output(self.pin7, pin7)
        GPIO.output(self.pin8, pin8)
        GPIO.output(self.pin9, pin9)
        GPIO.output(self.pin10, pin10)
        self.right_motor_pwm.ChangeDutyCycle(right_speed)
        self.left_motor_pwm.ChangeDutyCycle(left_speed)
        if(delay!=-1):
                time.sleep(delay)
        
    def activate_rpi_driver(self): # включение rpi-драйвера
        GPIO.output(self.RELE, GPIO.LOW) # Rele Low - RPI driver activate!
       
    def activate_arduino_driver(self): # включение arduino-драйвера
        GPIO.output(self.RELE, GPIO.HIGH)
        
    def stop(self): # останавливает все моторы
        GPIO.output(self.pin7, GPIO.LOW)
        GPIO.output(self.pin8, GPIO.LOW)
        GPIO.output(self.pin9, GPIO.LOW)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(0)
        self.right_motor_pwm.ChangeDutyCycle(0)
        
        
    def close(self):  # останавливает все моторы и удаляет шим-объекты (вызывать только после завершения работы с объектом класса Motor)
        self.left_motor_pwm.stop()
        self.right_motor_pwm.stop()
        GPIO.output(self.pin7, GPIO.LOW)
        GPIO.output(self.pin8, GPIO.LOW)
        GPIO.output(self.pin9, GPIO.LOW)
        GPIO.output(self.pin10, GPIO.LOW)
        #GPIO.cleanup() # cleanup только в конце всей программы, так как сбросится в том числе режим нумерации пинов!
        
    
motor = Motor()
#motor.set_speed_left(90)
#motor.set_speed_right(90)
motor.set_speed_go_pered_right(240)
time.sleep(3)
motor.close() 
time.sleep(3)
        
        

