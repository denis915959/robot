import sys
sys.path.insert(1,'/home/pi/i2clibraries')
#from i2c_itg3205 import *
from time import *
from quick2wire.i2c import I2CMaster, writing_bytes, reading
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
        
        


class i2c_itg3205():
    WhoAmI = 0x0
    SampleRateDivider = 0x15
    DLPFAndFullScale = 0x16
    InterruptConfig = 0x17
    InterruptStatus = 0x1A
    TempDataRegisterMSB = 0x1B
    TempDataRegisterLSB = 0x1C
    GyroXDataRegisterMSB = 0x1D
    GyroXDataRegisterLSB = 0x1E
    GyroYDataRegisterMSB = 0x1F
    GyroYDataRegisterLSB = 0x20
    GyroZDataRegisterMSB = 0x21
    GyroZDataRegisterLSB = 0x22
    PowerManagement = 0x3E
    
    # DLPF, Full Scale Setting
    FullScale_2000_sec = 0x18 # must be set at reset
    DLPF_256_8 = 0x00# Consult datasheet for explanation
    DLPF_188_1 = 0x01
    DLPF_98_1 = 0x02
    DLPF_42_1 = 0x03
    DLPF_20_1 = 0x04
    DLPF_10_1 = 0x05
    DLPF_5_1 = 0x06
    
    # Power Management Options
    PM_H_Reset = 0x80 # Reset device and internel registers to power-up-default settings
    PM_Sleep = 0x40 # Enables low power sleep mode
    PM_Standby_X = 0x20 # Put Gyro X in standby mode
    PM_Standby_Y = 0x10 # Put Gyro Y in standby mode
    PM_Standby_Z = 0x08 # Put Gyro Z in standby mode
    PM_Clock_Internal = 0x00 # Use internal oscillator
    PM_Clock_X_Gyro = 0x01
    PM_Clock_Y_Gyro = 0x02
    PM_Clock_Z_Gyro = 0x03
    PM_Clock_Ext_32_768 = 0x04
    PM_Clock_Ext_19_2 = 0x05
    
    # Interrupt Configuration
    IC_IntPinActiveLow = 0x80
    IC_IntPinOpen = 0x40
    IC_LatchUntilIntCleared = 0x20
    IC_LatchClearAnyRegRead = 0x10
    IC_IntOnDeviceReady = 0x04
    IC_IntOnDataReady = 0x01
    
    # Address will always be either 0x68 (104) or 0x69 (105)
    def __init__(self, port, addr=0x68):
        self.bus = i2c(port, addr)
        
        self.setPowerManagement(0x00)
        self.setSampleRateDivider(0x07)
        self.setDLPFAndFullScale(self.FullScale_2000_sec, self.DLPF_188_1)
        self.setInterrupt(self.IC_LatchUntilIntCleared, self.IC_IntOnDeviceReady, self.IC_IntOnDataReady)
    
    def setPowerManagement(self, *function_set):
        self.setOption(self.PowerManagement, *function_set)
    
    def setSampleRateDivider(self, divider):
        self.setOption(self.SampleRateDivider, divider)
        
    def setDLPFAndFullScale(self, *function_set):
        self.setOption(self.DLPFAndFullScale, *function_set)
        
    def setInterrupt(self, *function_set):
        self.setOption(self.InterruptConfig, *function_set)
        
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
        
    def getWhoAmI(self):
        whoami = self.bus.read_byte(self.WhoAmI)
        return whoami
        
    def getDieTemperature(self):
        temp = self.bus.read_s16int(self.TempDataRegisterMSB) 
        temp = round(35 + (temp + 13200) / 280, 2)
        return temp
    
    def getInterruptStatus(self):
        (reserved, reserved, reserved, reserved, reserved, itgready, reserved, dataready) = self.getOptions(self.InterruptStatus)
        return (itgready, dataready)
    
    def getOptions(self, register):
        options_bin = self.bus.read_byte(register)
        options = [False, False, False, False, False, False, False, False]

        for i in range(8):
            if options_bin & (0x01 << i):
                options[7 - i] = True
        
        return options
        
    def getAxes(self):
        gyro_x = self.bus.read_s16int(self.GyroXDataRegisterMSB)
        gyro_y = self.bus.read_s16int(self.GyroYDataRegisterMSB)
        gyro_z = self.bus.read_s16int(self.GyroZDataRegisterMSB)
        return (gyro_x, gyro_y, gyro_z)
    
    def getDegPerSecAxes(self):
        (gyro_x, gyro_y, gyro_z) = self.getAxes()
        return (gyro_x / 14.375, gyro_y / 14.375, gyro_z / 14.375)




"""itg3205 = i2c_itg3205(1)

while True:
    (itgready, dataready) = itg3205.getInterruptStatus ()

    if dataready: # no reading data
        #temp = itg3205.getDieTemperature ()
        (x, y, z) = itg3205.getAxes()#DegPerSecAxes ()
        #print ("Temp:" + str (temp ))
        print ("X:" + str (x ))
        print ("Y:" + str (y ))
        print ("Z:" + str (z ))
        print ("")

    sleep (1)"""
