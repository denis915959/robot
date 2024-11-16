#!/usr/bin/env python
# -*- coding: utf-8 -*-

from hmc5883 import *

compass = hmc5883l()

while(True):
	#print(compass.get_calibrated())
	data = compass.get_calibrated()
	
	print(compass.heading(data['x'], data['y']))
	#print(math.atan2(data['x'], data['y']))
	
	time.sleep(0.5)
