import hcsr04
import servo
import L298D
import LM393
import time
from collections import namedtuple
import math
import copy
import RPi.GPIO as GPIO


GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
lm393 = LM393.LM393()

m = L298D.Motor()
m.go_front_to_line(lm393)
