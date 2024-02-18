import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(10, GPIO.OUT)
GPIO.setup(3, GPIO.IN)

for i in range(0, 10):
	GPIO.output(10, 1)
	time.sleep(2)
	GPIO.output(10, 0)
	time.sleep(2)
	print(10)
try:
	while 1:
		if True:
			GPIO.cleanup(10)
		time.sleep(2)
		"""GPIO.cleanup()
		time.sleep(2)
		GPIO.setmode(GPIO.BCM)
		GPIO.setup(10, GPIO.OUT)
		GPIO.output(10, GPIO.LOW)
		time.sleep(2)
		GPIO.output(10, GPIO.HIGH)"""
		if True:
			GPIO.setup(10, GPIO.OUT)
		time.sleep(2)
	GPIO.cleanup()
except KeyboardInterrupt:
    GPIO.cleanup()
