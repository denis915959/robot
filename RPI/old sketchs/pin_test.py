import RPi.GPIO as GPIO
import time

# Настраиваем GPIO на Raspberry Pi
#GPIO.setmode(GPIO.BOARD)
GPIO.setmode(GPIO.BCM)

lm393_1 = 12
lm393_2 = 16
lm393_3 = 20
lm393_4 = 21
GPIO.setup(lm393_1, GPIO.OUT)
GPIO.output(lm393_1, 0)
GPIO.setup(lm393_2, GPIO.OUT)
GPIO.output(lm393_2, 0)
GPIO.setup(lm393_3, GPIO.OUT)
GPIO.output(lm393_3, 0)
GPIO.setup(lm393_4, GPIO.OUT)
GPIO.output(lm393_4, 1)
