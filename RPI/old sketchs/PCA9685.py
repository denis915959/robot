import time  #RABOTAET!
from adafruit_servokit import ServoKit
coefficient_angle = 130/90

# Set up the PCA9685 board
kit = ServoKit(channels=16)
kit.frequency = 50  # Set the PWM frequency to 50 Hz

# Define the functions to set the angle of the servos
def set_left_angle(angle):
    if((angle>180)or(angle<0)):
        return(False)
    else:
        kit.servo[14].angle = angle # number of pin in pca9685
        return(True)
    

def set_right_angle(angle):
    if((angle>180)or(angle<0)):
        return(False)
    else:
        kit.servo[15].angle = angle
        return(True)

try:
    """set_right_angle(0)
    time.sleep(0.1)
    set_right_angle(180)
    """
    # Rotate the servos by 20 degrees at a time
    angle=-100
    print(angle*coefficient_angle)
    a = int(angle*coefficient_angle)
    print(set_right_angle(a)) # 0 - 180
    set_left_angle(50)  #180-0
    """servo_step = 10
    for i in range(0, 5):
        angle = 0
        sz = 180 // servo_step
        set_left_angle(angle)
        set_right_angle(angle)
        time.sleep(2)
        for i in range(0, sz):
            angle = angle + servo_step
            set_left_angle(angle)
            set_right_angle(angle)
            time.sleep(0.1) #pause for me (checking rotate)
        time.sleep(2)"""
        

except KeyboardInterrupt:
    # Clean up the PCA9685 board
    kit.servo[0].angle = None
    kit.servo[1].angle = None




"""import time
import smbus
from adafruit_servokit import ServoKit
#from adafruit_pca9685 import PCA9685
#import PCA9685
#import ServoPCA9685

#from adafruit_servokit import ServoKit

bus = smbus.SMBus(1)
pca9685 = PCA9685(bus)
#servo00 = ServoPCA9685.ServoPCA9685(pca9685, PCA9685.CHANNEL00)

# 0 - > 180
for angle in range(0, 180 + 1):
    servo00.set_angle(angle)
    time.sleep(0.01)

# 180 -> 0
for angle in reversed(range(0, 180 + 1)):
    servo00.set_angle(angle)
    time.sleep(0.01)

servo00.disable()
"""


#chatGPT version
"""import time
from smbus import SMBus
import PCA9685

# Инициализация модуля PCA9685
bus = smbus.SMBus(1)
pca9685 = PCA9685.PCA9685(bus)
pca9685.set_pwm_frequency(50)

# Номера каналов для сервоприводов
servo_channel_1 = 0
servo_channel_2 = 1

# Функция для установки положения сервопривода
def set_servo_angle(channel, angle):
    angle = max(0, min(angle, 180))
    pulse_width = int(4096 * angle / 180)
    pca9685.set_pwm(channel, 0, pulse_width)

# Управление сервоприводами
try:
    while True:
        angle_1 = int(input("Введите угол для сервопривода 1 (0-180): "))
        set_servo_angle(servo_channel_1, angle_1)

        angle_2 = int(input("Введите угол для сервопривода 2 (0-180): "))
        set_servo_angle(servo_channel_2, angle_2)

except KeyboardInterrupt:
    pass

# Выключение сервоприводов перед выходом
pca9685.set_all_pwm(0, 0)
bus.close()"""


