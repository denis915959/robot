import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)

class LM393:
    def __init__(self):
        self.delay_speed=1 # время между прерываниями
        self.k_right=0.49 # 0.48 коэффициэнт, необходимый для точного определения скорости (так как при всех верных вводных данных показания от истины почему-то отличаются на этот коэффициэнт)
        self.k_left=0.51 # 0.5
        self.delay_between_read_speed=0.125 #0.125 # интервал между измерениями скорости, для измерения дистанции работает с погрешностью 2% в интервале 5.5 см/сек до 14 см/сек
        self.Left_S=0 # путь для левой стороны с момента start
        self.right_S=0 # путь для правой стороны с момента start
        # Пины датчиков скорости
        self.lm393_1 = 12
        self.lm393_2 = 16
        self.lm393_3 = 20
        self.lm393_4 = 21
        # Переменные для хранения данных о скорости колеса
        self.speed_count1=0
        self.speed_count2=0
        self.speed_count3=0
        self.speed_count4=0
        self.speed1=0
        self.speed2=0
        self.speed3=0
        self.speed4=0
        self.prev_time1 = time.time()
        self.prev_time2 = time.time()
        self.prev_time3 = time.time()
        self.prev_time4 = time.time()
        GPIO.setup(self.lm393_1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(self.lm393_2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(self.lm393_3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(self.lm393_4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        # Настройка пинов и прерывания, за счет этих штук происходит считывание прорези на диске
        GPIO.add_event_detect(self.lm393_1, GPIO.RISING, callback=lambda *args: self.calculate_speed(1), bouncetime=self.delay_speed)
        GPIO.add_event_detect(self.lm393_2, GPIO.RISING, callback=lambda *args: self.calculate_speed(2), bouncetime=self.delay_speed)
        GPIO.add_event_detect(self.lm393_3, GPIO.RISING, callback=lambda *args: self.calculate_speed(3), bouncetime=self.delay_speed)
        GPIO.add_event_detect(self.lm393_4, GPIO.RISING, callback=lambda *args: self.calculate_speed(4), bouncetime=self.delay_speed)
        
        
    def calculate_speed(self, channel):
        # Увеличение счетчика для каждого импульса датчика скорости
        if(channel==1):
            self.speed_count1 += 1
        if(channel==2):
            self.speed_count2 += 1
        if(channel==3):
            self.speed_count3 += 1
        if(channel==4):
            self.speed_count4 += 1
            
    def start_with_delay(self): # нужно вызвать прямо перед стартом считываний (обновляет таймеры)
        self.prev_time1 = time.time()
        self.prev_time2 = time.time()
        self.prev_time3 = time.time()
        self.prev_time4 = time.time()
        self.left_S=0
        self.right_S=0
        time.sleep(self.delay_between_read_speed) # для того, чтобы первая считанная скорость была корретной 0.25?????????
            
    def get_speed_right(self): # среднее по 2 правым датчикам
        tmp_speed_count1=0
        tmp_speed_count2=0
        tmp_speed_count1=self.speed_count1
        self.speed_count1 = 0  # Сброс счетчика
        time_interval1 = time.time() - self.prev_time1  # рассчитываем временной интервал
        self.prev_time1 = time.time()  # обновление временного счетчика
        tmp_speed_count2=self.speed_count2
        self.speed_count2 = 0  # Сброс счетчика
        time_interval2 = time.time() - self.prev_time2  # рассчитываем временной интервал
        self.prev_time2 = time.time()  # обновление временного счетчика
        speed1 = (tmp_speed_count1 * (7 /(44*40))*self.k_right *207)/time_interval1 # независимо рассчитываем 2 скорости
        speed2 = (tmp_speed_count2 * (7 /(44*40))*self.k_right *207)/time_interval2
        speed = (speed1+speed2)/2
        
        #print("1 = ", speed1)
        #print("2 = ", speed2)
        return(speed)
    
    def get_speed_left(self): # среднее по 2 левым датчикам
        tmp_speed_count3=0
        tmp_speed_count4=0
        tmp_speed_count3=self.speed_count3
        self.speed_count3 = 0  # Сброс счетчика
        time_interval3 = time.time() - self.prev_time3  # рассчитываем временной интервал
        self.prev_time3 = time.time()  # обновление временного счетчика
        tmp_speed_count4=self.speed_count4
        self.speed_count4 = 0  # Сброс счетчика
        time_interval4 = time.time() - self.prev_time4  # рассчитываем временной интервал
        self.prev_time4 = time.time()  # обновление временного счетчика
        speed1 = (tmp_speed_count3 * (7 /(44*40))*self.k_left *207)/time_interval3 # независимо рассчитываем 2 скорости
        speed2 = (tmp_speed_count4 * (7 /(44*40))*self.k_left *207)/time_interval4
        speed = (speed1+speed2)/2
        
        #print("1 = ", speed1)
        #print("2 = ", speed2)
        return(speed)
        
    def get_delay(self):
        return(self.delay_between_read_speed)

    """def calculate_speed(self): # error in this function! это пока так, но это не работает. эта серия функций будет создаваться под конкретную задачу
        speed = self.get_speed_left()
        self.left_S = self.left_S + speed*self.delay_between_read_speed
        speed = self.get_speed_right()
        self.right_S = self.right_S + speed*self.delay_between_read_speed

    def get_left_S(self):
        return(self.left_S) # 2 элемента - справа и слева выводить

    def get_right_S(self):
        return(self.right_S) # 2 элемента - справа и слева выводить"""

    
        

try:
    lm393 = LM393()
    lm393.start_with_delay()
    S_l = 0
    S_r = 0
    while True:
        delay_between_read_speed=lm393.get_delay()
        speed_l = lm393.get_speed_left()
        speed_r = lm393.get_speed_right()
        #print("speed = ", speed)
        
        time.sleep(delay_between_read_speed) # 0.5 - нормальная задержка
        S_l = S_l+speed_l*delay_between_read_speed
        print("S_l = ", S_l)
        S_r = S_r+speed_r*delay_between_read_speed
        print("S_r = ", S_r)

except KeyboardInterrupt:
    GPIO.cleanup()







































