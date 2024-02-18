import RPi.GPIO as GPIO
import time

# Настраиваем GPIO на Raspberry Pi
#GPIO.setmode(GPIO.BOARD)
GPIO.setmode(GPIO.BCM)

# Определяем пины, к которым подключен L298D
#IN1 = 5  # GPIO5 (1-27)
#IN2 = 6  # GPIO6 (1-27)
#EN = 13  # GPIO13 (1-27)
pin6=11
pin7=5
pin8=6
pin9=13
pin10=19
pin11=26
RELE = 10

# Настраиваем пины на выход
GPIO.setup(pin6, GPIO.OUT)
GPIO.setup(pin7, GPIO.OUT)
GPIO.setup(pin8, GPIO.OUT)
GPIO.setup(pin9, GPIO.OUT)
GPIO.setup(pin10, GPIO.OUT)
GPIO.setup(pin11, GPIO.OUT)
GPIO.setup(RELE, GPIO.OUT)

# Создаем объект PWM для управления скоростью вращения мотора
left_motor_pwm = GPIO.PWM(pin6, 1000)  # 1000 - частота ШИМ (в Гц)
right_motor_pwm = GPIO.PWM(pin11, 1000)  # 

# Запускаем ШИМ с начальной скоростью 50%
left_motor_pwm.start(50)
right_motor_pwm.start(50)

# Функция для изменения скорости вращения мотора
"""def change_motor_speed(speed):
    # Ограничиваем скорость от 0 до 100%
    GPIO.output(IN1, GPIO.HIGH) # REle in 
    GPIO.output(IN2, GPIO.LOW)
    if speed < 0:
        speed = 0
    elif speed > 100:
        speed = 100
    
    # Изменяем скорость вращения мотора
    motor_pwm.ChangeDutyCycle(speed)"""

# Пример использования:
try: # diapazon speed: 0-100 (in % ot max)
    # Устанавливаем скорость 80%
    speed = 30  #100- 22.85, 90 - 21.9 cm/sec, 80 - 20 cm/sec, 70 - 18.85cm/sec, 60 - 17.8 cm/sec, 50 - 15 cm/sec, 40 - 12.4 cm/sec, 30 - 7.85 cm/sec, 20 - min(3 cm/sec), 
    
    GPIO.output(RELE, GPIO.LOW)
    
    GPIO.output(pin9, GPIO.HIGH)#
    GPIO.output(pin10, GPIO.LOW)#
    left_motor_pwm.ChangeDutyCycle(speed)
    GPIO.output(pin8, GPIO.HIGH)
    GPIO.output(pin7, GPIO.LOW)
    right_motor_pwm.ChangeDutyCycle(speed)
    
    
    #GPIO.output(RELE, GPIO.LOW)
    #change_motor_speed(50)
    time.sleep(700)  # Мотор будет вращаться со скоростью 80% в течение 5 секунд
    #GPIO.output(RELE, GPIO.LOW)
    GPIO.output(RELE, GPIO.HIGH)
    
    print(2)
    
    # Устанавливаем скорость 30%
    """change_motor_speed(30)
    time.sleep(2)  # Мотор будет вращаться со скоростью 30% в течение 3 секунд
    
    # Останавливаем мотор
    change_motor_speed(80)
    time.sleep(2)  # Мотор будет остановлен на 2 секунды
    
    # Включаем мотор на полную мощность
    change_motor_speed(80) # speed 100 - est nuansi (nestabilno nemnogo)
    time.sleep(2)  # Мотор будет вращаться со скоростью 100% в течение 4 секунды
    """
    
except KeyboardInterrupt:
    GPIO.cleanup()

# Останавливаем мотор и очищаем настройки GPIO
left_motor_pwm.stop()
right_motor_pwm.stop()
GPIO.cleanup()
