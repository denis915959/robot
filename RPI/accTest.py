import sys
sys.path.insert(1,'/home/pi/GY-85_Raspberry-Pi/i2clibraries')
from time import *
from quick2wire.i2c import I2CMaster, writing_bytes, reading
import time
import math



class i2c:
	def __init__(self, port, addr, debug = False):
		self.i2c_device = I2CMaster(port)
		self.addr = addr
		
		self.debug = debug
		
	def write_byte(self, *bytes):
		self.i2c_device.transaction(
			writing_bytes(self.addr, *bytes))
				
	def read_byte(self, register):
		byte = self.i2c_device.transaction(
			writing_bytes(self.addr, register),
			reading(self.addr, 1))[0][0]
		return byte
	
	def read_16bit(self, register, flip = False):
		data = self.i2c_device.transaction(
			writing_bytes(self.addr, register),
			reading(self.addr, 2))[0]
		
		if flip:
			bit16 = (data[1] << 8) | data[0]
		else:
			bit16 = (data[0] << 8) | data[1]
		
		if self.debug:
			print(hex(register)+": "+hex(bit16));
			
		return bit16
	
	def read_s16int(self, register, flip = False):
		s_int = self.read_16bit(register, flip)
		return self.twosToInt(s_int, 16)
	
	def read_3s16int(self, register, flip = False):
		data = self.i2c_device.transaction(
			writing_bytes(self.addr, register),
			reading(self.addr, 6))[0]
			
		if self.debug:
			print("3 signed 16: %s " % ', '.join(map(hex, data)))
			
		if flip:
			s_int1 = (data[1] << 8) | data[0]
		else:
			s_int1 = (data[0] << 8) | data[1]
			
		if flip:
			s_int2 = (data[3] << 8) | data[2]
		else:
			s_int2 = (data[2] << 8) | data[3]
			
		if flip:
			s_int3 = (data[5] << 8) | data[4]
		else:
			s_int3 = (data[4] << 8) | data[5]
			
		return (self.twosToInt(s_int1, 16), self.twosToInt(s_int2, 16), self.twosToInt(s_int3, 16) )
													
	def twosToInt(self, val, len):
		# Convert twos compliment to integer
		if(val & (1 << len - 1)):
			val = val - (1<<len)
			
		if self.debug:
			print(str(val))
		return val



class i2c_adxl345:
	DeviceID = 0x00
	TapThreshold = 0x1D
	XAxisOffset = 0x1E
	YAxisOffset = 0x1F
	ZAxisOffset = 0x20
	TapDuration = 0x21
	TapLatency = 0x22
	TapWindow = 0x23
	ActivityThreshold = 0x24
	InactivityThreshold = 0x25
	InactivityTime = 0x26
	AxesEnable = 0x27 # Axis enable control fro activity and inactivity detection
	FreeFallThreshold = 0x28
	FreeFallTime = 0x29
	TapAxes = 0x2A # Axis control for single tap/double tap
	TapAxesStatus = 0x2B # Source of tap
	BandwidthRate = 0x2C # Data rate and power mode control
	PowerControl = 0x2D
	InterruptEnable = 0x2E
	InterruptMapping = 0x2F
	InterruptSource = 0x30
	DataFormat = 0x31
	XAxisDataLSB = 0x32
	XAxisDataMSB = 0x33
	YAxisDataLSB = 0x34
	YAxisDataMSB = 0x35
	ZAxisDataLSB = 0x36
	ZAxisDataMSB = 0x37
	FIFOControl = 0x38
	FIFOStatus = 0x39
	
	# Settings for Activity/Inactivity Control
	AE_ActivityAC = 0x80
	AE_ActivityX = 0x40
	AE_ActivityY = 0x20
	AE_ActivityZ = 0x10
	AE_InactivityAC = 0x08
	AE_InactivityX = 0x04
	AE_InactivityY = 0x02
	AE_InactivityZ = 0x01
	
	# Tap Axes Configuration
	TA_Suppress = 0x08
	TA_TapXAxis = 0x04
	TA_TapYAxis = 0x02
	TA_TapZAxis = 0x01
	
	# Tap Status
	TS_ActivityX = 0x40
	TS_ActivityY = 0x20
	TS_ActivityZ = 0x10
	TS_Asleep = 0x08
	TS_TapX = 0x04
	TS_TapY = 0x02
	TS_TapZ = 0x01
	
	# Power Control Options
	PC_Link = 0x32
	PC_AutoSleep = 0x16
	PC_Measure = 0x08
	PC_Sleep = 0x04
	PC_Wakeup_8Hz = 0x00
	PC_Wakeup_4Hz = 0x01
	PC_Wakeup_2Hz = 0x02
	PC_Wakeup_1Hz = 0x03

	# Options for data format
	DF_Range_2g = 0x00
	DF_Range_4g = 0x01
	DF_Range_8g = 0x02
	DF_Range_16g = 0x03
	
	# Enable and Map Functions
	DataReady = 0x80
	SingleTap = 0x40
	DoubleTap = 0x20
	Activity = 0x10
	Inactivity = 0x08
	FreeFall = 0x04
	Watermark = 0x02
	Overrun = 0x01
	
	def __init__(self, port, addr=0x53):
		self.bus = i2c(port, addr)
		
		self.wakeUp();
		
		# Set defaults
		self.setScale();
		self.setTapThreshold()
		self.setTapDuration()
		self.setTapLatency()
		self.setTapWindow()
		self.setActivityThreshold()
		self.setInactivityThreshold()
		self.setInactivityTime()
		self.setFreeFallThreshold()
		self.setFreeFallTime()
		
	def __str__(self):
		ret_str = ""
		(x, y, z) = self.getAxes() 
		ret_str += "x:    "+str(x)+"\n"
		ret_str += "y:    "+str(y)+"\n"
		ret_str += "z:    "+str(z)+"\n"
		#ret_str = str(x) + ";" + str(y) + ";" + str(z) + ";"
		return ret_str

	def wakeUp(self):
		self.bus.write_byte(self.PowerControl, 0x00)
		self.bus.write_byte(self.PowerControl, self.PC_Measure)
		
	def setTapThreshold(self, g=3):
		# Figure out g's and then intervals based on 62.5 mg
		# Range 0-8 g
		intervals = math.floor(g / 0.0625)
		if intervals < 256:
			self.setOption(self.TapThreshold, intervals)
		
	def setTapDuration(self, millisec=10):
		# Figure out microseconds and then intervals based on 625 us
		# Range 0-159 millisec
		intervals = math.floor(millisec * 1000/625)
		if intervals < 256:
			self.setOption(self.TapDuration, intervals)
			
	def setTapLatency(self, millisec=150):
		# Figure out microseconds and then intervals based on 1.25 ms
		# Range 0-318
		intervals = math.floor(millisec / 1.25)
		if intervals < 256:
			self.setOption(self.TapLatency, intervals)
			
	def setTapWindow(self, millisec=100):
		# Figure out microseconds and then intervals based on 1.25 ms 
		# Range 
		intervals = math.floor(millisec / 1.25)
		if intervals < 256:
			self.setOption(self.TapWindow, intervals)
			
	# Scale can be 2, 4, 8, or 16 (default)
	def setScale(self, scale=16):
		if scale == 2:
			self.axesScale = 2;
			self.setOption(self.DataFormat, self.DF_Range_2g)
		elif scale == 4:
			self.axesScale = 4;
			self.setOption(self.DataFormat, self.DF_Range_4g)
		elif scale == 8:
			self.axesScale = 8;
			self.setOption(self.DataFormat, self.DF_Range_8g)
		elif scale == 16:
			self.axesScale = 16;
			self.setOption(self.DataFormat, self.DF_Range_16g)
	
	def setActivityThreshold(self, g=-1, axis='z', change=.5):
		# If g is left unset, assume currently inactive and set to current state
		if g == -1:	
			(x, y, z) = self.getAxes()
			if axis == 'x':
				self.addActivity(self.AE_ActivityX)
				g = math.fabs(x) + change
			elif axis == 'y':
				self.addActivity(self.AE_ActivityY)
				g = math.fabs(y) + change
			elif axis == 'z':
				self.addActivity(self.AE_ActivityZ)
				g = math.fabs(z) + change
	
		# Figure out g's and then intervals based on 62.5 mg
		# Range 0-16g
		intervals = math.floor(math.fabs(g) / 0.0625 )
		print( intervals);
		if intervals < 256:
			self.setOption(self.ActivityThreshold, intervals)
			
	def setInactivityThreshold(self, g=-1, axis='z', change=.1):
		# If g is left unset, assume currently inactive and set to current state
		if g == -1:	
			(x, y, z) = self.getAxes()
			if axis == 'x':
				self.addActivity(self.AE_InactivityX)
				g = math.fabs(x) + change
			elif axis == 'y':
				self.addActivity(self.AE_InactivityY)
				g = math.fabs(y) + change
			elif axis == 'z':
				self.addActivity(self.AE_InactivityZ)
				g = math.fabs(z) + change
				
		# Figure out g's and then intervals based on 62.5 mg
		# Range 0-16g
		intervals = math.floor(math.fabs(g) / 0.0625 )
		if intervals < 256:
			self.bus.write_byte(self.InactivityThreshold, intervals)
	
	def setInactivityTime(self, sec=1):
		# Figure out microseconds and then intervals based on 1.25 ms 
		# Range 0-255
		if sec < 256:
			self.setOption(self.InactivityTime, sec)
			
	def setFreeFallThreshold(self, g = .4):
		# Figure out g's and then intervals based on 62.5 mg
		# Range 0-8 g
		intervals = math.floor(g / 0.0625)
		if intervals < 256:
			self.setOption(self.FreeFallThreshold, intervals)
			
	def setFreeFallTime(self, sec=.0500):
		# Figure out microseconds and then intervals based on 5 ms 
		# Range 0-101
		intervals = math.floor(sec * 1000 / 5)
		if intervals < 256:
			self.setOption(self.FreeFallTime, intervals)
	
	def setActivity(self, *function_set):
		self.setOption(self.AxesEnable, *function_set)
		
	def addActivity(self, *function_set):
		self.addOption(self.AxesEnable, *function_set)
	
	def removeActivity(self, *function_set):
		self.removeOption(self.AxesEnable, *function_set)
		
	def setInterrupt(self, *function_set):
		self.setOption(self.InterruptEnable, *function_set)
		
	def setTapAxes(self, *function_set):
		self.setOption(self.TapAxes, *function_set)
	
	
	# Rewrites all options in register
	def setOption(self, register, *function_set):
		options = 0x00
		for function in function_set:
			options = options | function
		self.bus.write_byte(register, options)
	
	# Adds to existing options of register	
	def addOption(self, register, *function_set):
		options = self.bus.read_byte(register)
		for function in function_set:
			options = options | function
		self.bus.write_byte(register, options)
		
	# Removes options of register	
	def removeOption(self, register, *function_set):
		options = self.bus.read_byte(register)
		for function in function_set:
			options = options & (function ^ 0b11111111)
		self.bus.write_byte(register, options)
		
	def getActivity(self):
		#	Returns (actacdc, activityx, activityy, activityz, inactacdc, inactivityx, inactivityy, inactivityz)
		return self.getOptions(self.AxesEnable)
	
	def getInterrupt(self):
		#	Returns (dataready, singletap, doubletap, activity, inactivity, freefall, watermark, overrun)	
		return self.getOptions(self.InterruptEnable)
	
	def getTapAxes(self):
		#	Returns (reserved, reserved, reserved, reserved, suppress, tapx, tapy, tapz)	
		return self.getOptions(self.TapAxes)
		
	def getTapStatus(self):
		#	Returns (reserved, activityx, activityy, activityz, asleep, tapx, tapy, tapz)
		return self.getOptions(self.TapAxesStatus)
	
	def getInterruptStatus(self):
		#	Returns	(dataready, singletap, doubletap, activity, inactivity, freefall, watermark, overrun)	
		return self.getOptions(self.InterruptSource)
	
	def getOptions(self, register):
		options_bin = self.bus.read_byte(register)
		options = [False, False, False, False, False, False, False, False]
		for i in range(8):
			if options_bin & (0x01 << i):
				options[7 - i] = True
		
		return options
			
	def getRawAxes(self):
		return self.bus.read_3s16int(self.XAxisDataLSB, True)
	
	def getAxes(self):
		scaleFactor = (self.axesScale*2)/1024
		(accel_x, accel_y, accel_z) = self.bus.read_3s16int(self.XAxisDataLSB, True)
		return (accel_x * scaleFactor, accel_y * scaleFactor, accel_z * scaleFactor)
		



"""adxl345 = i2c_adxl345(1)


while True:
	(x, y, z) = adxl345.getAxes() 
	print(x)
	print(y)
	print(z)
	print("")
	
	print("")
	print("")
	print("")
	
	sleep(1)"""
