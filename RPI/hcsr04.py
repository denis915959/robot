#ultrazvuk + servo
import RPi.GPIO as GPIO
import time
 
#GPIO Mode (BOARD / BCM)
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)


class HCSR04():
    def __init__(self):
        self.GPIO_TRIGGER_LEFT = 27       # GPIO27 (1-27)
        self.GPIO_ECHO_LEFT = 17          # GPIO17 (1-27)
        self.GPIO_TRIGGER_RIGHT = 24      # GPIO24 (1-27)
        self.GPIO_ECHO_RIGHT = 23         # GPIO23 (1-27)
        self.GPIO_BACK_TRIGGER_LEFT = 8   # GPIO27 (1-27)
        self.GPIO_BACK_ECHO_LEFT = 7      # GPIO17 (1-27)
        self.GPIO_BACK_TRIGGER_RIGHT = 15 # GPIO24 (1-27)
        self.GPIO_BACK_ECHO_RIGHT = 14    # GPIO23 (1-27)
        self.mean_number=3  # число измерений дальности в методе get_mean_distance_right/left
        GPIO.setup(self.GPIO_TRIGGER_LEFT, GPIO.OUT)
        GPIO.setup(self.GPIO_ECHO_LEFT, GPIO.IN)
        GPIO.setup(self.GPIO_TRIGGER_RIGHT, GPIO.OUT)
        GPIO.setup(self.GPIO_ECHO_RIGHT, GPIO.IN)
        GPIO.setup(self.GPIO_BACK_TRIGGER_LEFT, GPIO.OUT)
        GPIO.setup(self.GPIO_BACK_ECHO_LEFT, GPIO.IN)
        GPIO.setup(self.GPIO_BACK_TRIGGER_RIGHT, GPIO.OUT)
        GPIO.setup(self.GPIO_BACK_ECHO_RIGHT, GPIO.IN)
        self.delta_distance = 1.5 #
        
    def get_delta_distance(self):
        return(self.delta_distance)

    def distance_left(self):
        GPIO.output(self.GPIO_TRIGGER_LEFT, GPIO.HIGH)  # Посылаем сигнал
        time.sleep(0.00001) # через 0.00001 секунду триггер Low (т.е сигнал послан, выключяаем отправку сигнала)
        GPIO.output(self.GPIO_TRIGGER_LEFT, GPIO.LOW)
        StartTime = time.time()
        StopTime = time.time()
        while GPIO.input(self.GPIO_ECHO_LEFT) == 0:
            StartTime = time.time()
        while GPIO.input(self.GPIO_ECHO_LEFT) == 1:
            StopTime = time.time()
        TimeElapsed = StopTime - StartTime # разница между стартом измерения и временем прихода сигнала
        distance = (TimeElapsed * 34300) / 2 # умножаем на скорость звука и делим на 2, так как туда и обратно сигнал идет
        return (distance + self.delta_distance)
    
    def distance_right(self):
        GPIO.output(self.GPIO_TRIGGER_RIGHT, True)
        time.sleep(0.00001) # через 0.00001 секунду триггер Low (т.е сигнал послан, выключяаем отправку сигнала)
        GPIO.output(self.GPIO_TRIGGER_RIGHT, False)
        StartTime = time.time()
        StopTime = time.time()
        while GPIO.input(self.GPIO_ECHO_RIGHT) == 0:
            StartTime = time.time()
        while GPIO.input(self.GPIO_ECHO_RIGHT) == 1:
            StopTime = time.time()
        TimeElapsed = StopTime - StartTime  # разница между стартом измерения и временем прихода сигнала
        distance = (TimeElapsed * 34300) / 2 # умножаем на скорость звука и делим на 2, так как туда и обратно сигнал идет
        return (distance + self.delta_distance)
        
    def back_distance_left(self):
        GPIO.output(self.GPIO_BACK_TRIGGER_LEFT, True)
        time.sleep(0.00001) # через 0.00001 секунду триггер Low (т.е сигнал послан, выключяаем отправку сигнала)
        GPIO.output(self.GPIO_BACK_TRIGGER_LEFT, False)
        StartTime = time.time()
        StopTime = time.time()
        while GPIO.input(self.GPIO_BACK_ECHO_LEFT) == 0:
            StartTime = time.time()
        while GPIO.input(self.GPIO_BACK_ECHO_LEFT) == 1:
            StopTime = time.time()
        TimeElapsed = StopTime - StartTime  # разница между стартом измерения и временем прихода сигнала
        distance = (TimeElapsed * 34300) / 2 # умножаем на скорость звука и делим на 2, так как туда и обратно сигнал идет
        return (distance + self.delta_distance)
        
    def back_distance_right(self):
        GPIO.output(self.GPIO_BACK_TRIGGER_RIGHT, True)
        time.sleep(0.00001) # через 0.00001 секунду триггер Low (т.е сигнал послан, выключяаем отправку сигнала)
        GPIO.output(self.GPIO_BACK_TRIGGER_RIGHT, False)
        StartTime = time.time()
        StopTime = time.time()
        while GPIO.input(self.GPIO_BACK_ECHO_RIGHT) == 0:
            StartTime = time.time()
        while GPIO.input(self.GPIO_BACK_ECHO_RIGHT) == 1:
            StopTime = time.time()
        TimeElapsed = StopTime - StartTime  # разница между стартом измерения и временем прихода сигнала
        distance = (TimeElapsed * 34300) / 2 # умножаем на скорость звука и делим на 2, так как туда и обратно сигнал идет
        return (distance + self.delta_distance)
        
    def get_back_mean_distance_left(self): # возвращает более точное измерение дальности за счет self.mean_number измерений
        sum_dist=0
        for i in range(0, self.mean_number): 
            sum_dist=sum_dist+self.back_distance_left()
        res=sum_dist/self.mean_number
        return(res)
        
    def get_back_mean_distance_right(self): # возвращает более точное измерение дальности за счет self.mean_number измерений
        sum_dist=0
        for i in range(0, self.mean_number): 
            sum_dist=sum_dist+self.back_distance_right()
        res=sum_dist/self.mean_number
        return(res)
    
    def get_mean_distance_left(self): # возвращает более точное измерение дальности за счет self.mean_number измерений
        sum_dist=0
        for i in range(0, self.mean_number): 
            sum_dist=sum_dist+self.distance_left()
        res=sum_dist/self.mean_number
        return(res)
        
    def get_mean_distance_right(self): # возвращает более точное измерение дальности за счет self.mean_number измерений
        sum_dist=0
        for i in range(0, self.mean_number): 
            sum_dist=sum_dist+self.distance_right()
        res=sum_dist/self.mean_number
        return(res)
        
    def set_mean_number(self, num): # изменить self.mean_number во время работы программы
        self.mean_number=num
    
    def get_mean_number(self):
        return(self.mean_number)
      
    
"""
hcsr04 = HCSR04() # тесты класса hcsr04
hcsr04.set_mean_number(5)
while(1):
  #print("left = ", hcsr04.get_mean_distance_left())
  print("right = ", hcsr04.get_mean_distance_right()) 
  time.sleep(0.5)    
GPIO.cleanup()
"""

