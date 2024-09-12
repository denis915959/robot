import hcsr04
import servo
import L298D
import LM393
from hmc5883 import *
import time
from collections import namedtuple
import math
import copy
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
m = L298D.Motor()

import smbus
import os
import copy
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20

def writenumber_i2c(num):
	bus.write_byte(SLAVE_ADDRESS, int(num))
def write_array(a, b, c, d):
	bus.write_i2c_block_data(SLAVE_ADDRESS, a, [b, c, d])
def readnumber():
	num=int(bus.read_byte(SLAVE_ADDRESS))
	return num
mode = 2




tcrt5000 = 18 # this blok perenesti v blok obiezda prepyatstvia!!
while(1):
    speed = 120
    m.go_pered(speed, 0.05)
    if(GPIO.input(tcrt5000) == 0):
        break
m.stop()

result_obiezd = []
result_obiezd_flag = True  # delete!!!
result_obiezd.append(2)    # delete!!!
result_obiezd.append(2)    # delete!!!   1- rotate_left, 2 - rotate_right
         
if(result_obiezd_flag==True):
	
    if(result_obiezd[1]>0): # т.е если удалось объехать препятствие
        m.activate_arduino_driver()
        time.sleep(1)
        writenumber_i2c(mode)
        time.sleep(1)
        writenumber_i2c(result_obiezd[1]) # 1 - команда на захват линии с поворотом влево
        time.sleep(10) # inache poka ne rabotaet
        m.activate_rpi_driver()
        print("after receive")
    """result_trip[0]=1
else:
    result_trip[0]=0"""
