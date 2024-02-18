import RPi.GPIO as GPIO
import time
import LM393
import copy

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
        self.tcrt5000 = 18
        GPIO.setup(self.pin6, GPIO.OUT)
        GPIO.setup(self.pin7, GPIO.OUT)
        GPIO.setup(self.pin8, GPIO.OUT)
        GPIO.setup(self.pin9, GPIO.OUT)
        GPIO.setup(self.pin10, GPIO.OUT)
        GPIO.setup(self.pin11, GPIO.OUT)
        GPIO.setup(self.RELE, GPIO.OUT)
        GPIO.setup(self.tcrt5000, GPIO.IN)
        GPIO.output(self.RELE, GPIO.LOW) # Rele Low - RPI драйвер включен!
        self.left_motor_pwm = GPIO.PWM(self.pin6, 1000)  # Создаем объектs PWM для управления скоростью вращения мотора
        self.right_motor_pwm = GPIO.PWM(self.pin11, 1000)  # 1000 - частота ШИМ (в Гц)
        self.left_motor_pwm.start(50) # Запускаем ШИМ с начальной скоростью 50%
        self.right_motor_pwm.start(50)
        self.speed_to_distance = 100 # оптимальная скорость для движения по прямой при 11.6-11.7 вольтах
        self.delta_to_distance = 8 # запас на остановку (в миллиметрах)
        self.delay_to_distance = 0.02
        
    def set_speed_go_pered_left(self, speed):  #, delay=-1): # speed 0-255, только для левого (движение вперед), правую сторону не трогаем, так как подразумевается, что она будет вызвана рядом (это парные функции)
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)
                self.stop()"""
        
    def set_speed_go_pered_right(self, speed):  #, delay=-1): # speed 0-255, только для правого (движение вперед), этот метод парный с методом set_speed_go_pered_left
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)
                self.stop()"""
        
    def set_speed_go_back_left(self, speed):  #, delay=-1): # speed 0-255, только для левого (движение назад), этот метод парный с методом set_speed_go_back_right
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)
                self.stop()"""
        
    def set_speed_go_back_right(self, speed):  #, delay=-1):  # speed 0-255, только для правого (движение назад)
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        """if(delay!=-1):
                time.sleep(delay)
                self.stop()"""
        
    def rotate_right(self, speed, delay=-1): # 0-255, разворот вправо
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
                self.stop()
        
    def rotate_left(self, speed, delay=-1): # 0-255, разворот влево
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
                self.stop()
        
    def go_pered(self, speed, delay=-1): # движение вперед
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin9, GPIO.HIGH)
        GPIO.output(self.pin10, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin8, GPIO.HIGH)
        GPIO.output(self.pin7, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
                self.stop()
        
    def go_back(self, speed, delay=-1): # движение назад
        self.activate_rpi_driver()
        speed = int(speed/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        if(delay!=-1):
                time.sleep(delay)
                self.stop()
                
    def go_back_to_distance(self, distance, lm393): # +/- 0.5 cm
        self.activate_rpi_driver()
        distance = distance*10 - self.delta_to_distance
        speed = int(self.speed_to_distance/2.55)
        GPIO.output(self.pin10, GPIO.HIGH)
        GPIO.output(self.pin9, GPIO.LOW)
        self.left_motor_pwm.ChangeDutyCycle(speed)
        GPIO.output(self.pin7, GPIO.HIGH)
        GPIO.output(self.pin8, GPIO.LOW)
        self.right_motor_pwm.ChangeDutyCycle(speed)
        lm393.start_with_delay()
        speed_l = lm393.get_speed_left()
        speed_r = lm393.get_speed_right()
        S = 0
        tmp_time = lm393.get_delay()
        delay_between_read_speed=self.delay_to_distance
        lm393.set_delay(delay_between_read_speed)
        while (S<distance):
            time.sleep(delay_between_read_speed)
            speed_l = lm393.get_speed_left()
            speed_r = lm393.get_speed_right()
            S_l = speed_l*delay_between_read_speed
            S_r = speed_r*delay_between_read_speed
            S = S + ((S_l + S_r)/2)
        lm393.set_delay(tmp_time)
        self.stop()
        
    def go_back_to_line(self,lm393, speed=120):
        while(1):
            self.go_back(speed, 0.05)
            if(GPIO.input(self.tcrt5000) == 0):
                break
        self.stop()
        
    def go_front_to_line(self,lm393, speed=120):
        while(1):
            self.go_pered(speed, 0.05)
            if(GPIO.input(self.tcrt5000) == 0):
                break
        self.stop()
            
    def set_all_motors_rotation(self, left_speed, pin7, pin8, pin9, pin10, right_speed, delay=-1): # Ручное управление направлением и скоростью моторов
        self.activate_rpi_driver()
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
                self.stop()
        
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
        self.activate_arduino_driver()
        #GPIO.cleanup() # cleanup только в конце всей программы, так как сбросится в том числе режим нумерации пинов!
        
        
"""m = Motor()
m.rotate_left(200, 0.5) #copy.copy(lm393)) - через copy Нельзя, не будет работать!
time.sleep(5)
m.rotate_right(200, 0.5)
m.close()"""



