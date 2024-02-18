import RPi.GPIO as GPIO
import time

# Установка режима нумерации пинов!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
GPIO.setmode(GPIO.BOARD)

# Установка пина для сервопривода
servo_left_pin = 12  #pin NUMBER!
servo_right_pin = 11

GPIO.setup(servo_left_pin, GPIO.OUT) 
GPIO.setup(servo_right_pin, GPIO.OUT) 

# Создание объекта PWM (ШИМ) для сервопривода
pwm_left = GPIO.PWM(servo_left_pin, 50) # 50 Гц
pwm_right = GPIO.PWM(servo_right_pin, 50) # 50 Гц

# Установка начальной позиции сервопривода
pwm_left.start(0)
pwm_right.start(0)

def set_left_angle(angle):
	duty = angle / 18 + 2 +0.5
	GPIO.output(servo_left_pin, True)
	pwm_left.ChangeDutyCycle(duty)
	time.sleep(1)
	GPIO.output(servo_left_pin, False)
	pwm_left.ChangeDutyCycle(0)
	
def set_right_angle(angle):
	duty = angle / 18 + 2 +0.5
	GPIO.output(servo_right_pin, True)
	pwm_right.ChangeDutyCycle(duty)
	time.sleep(1)
	GPIO.output(servo_right_pin, False)
	pwm_right.ChangeDutyCycle(0)

try:
	# Поворот на 180 градусов
	servo_step=90 #10
	for i in range(0, 15):
		angle=0
		sz=180//servo_step
		for i in range(0, (sz+1)):
			set_left_angle(angle)
			print("left ", i)
			time.sleep(1)
			set_right_angle(angle)
			print("right ", i)
			angle=angle+servo_step
			time.sleep(1)
	"""time.sleep(1)
	set_left_angle(180)
	set_right_angle(180)
	time.sleep(1)
	set_left_angle(90)
	set_right_angle(90)
	time.sleep(1)
	set_left_angle(0)
	set_right_angle(0)
	time.sleep(1)"""

except KeyboardInterrupt:
	# Прерывание программы при нажатии Ctrl+C
	pwm_left.stop()
	pwm_right.stop()
	
GPIO.cleanup()
