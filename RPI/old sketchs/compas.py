import smbus
import time

# Define I2C address of HMC5883L magnetometer
address = 0x0d
arduino_ADDRESS = 0x20       #04

def writenumber(num):
	bus.write_byte(arduino_ADDRESS, int(num))

# Initialize I2C bus
bus = smbus.SMBus(1)

# Configure HMC5883L magnetometer
bus.write_byte_data(address, 0x00, 0x70) # Set measurement mode
bus.write_byte_data(address, 0x01, 0xA0) # Set data output rate
bus.write_byte_data(address, 0x02, 0x00) # Set measurement configuration

# Read magnetic field values and print them
def compas():
    data = bus.read_i2c_block_data(address, 0x03, 6)
    x = (data[0] << 8) | data[1]
    z = (data[2] << 8) | data[3]
    y = (data[4] << 8) | data[5]
    if x > 32767:
        x -= 65536
    if z > 32767:
        z -= 65536
    if y > 32767:
        y -= 65536
    print("Magnetic field (x, y, z): ({}, {}, {})".format(x, y, z))
    time.sleep(1)

while(1):
	writenumber(1)
	time.sleep(1)
	compas()
	time.sleep(1)




"""import smbus2
import time

# Define I2C address of GY271 magnetometer
address = 0x0d

# Define register addresses for magnetic field values
x_high = 0x03
x_low = 0x04
z_high = 0x05
z_low = 0x06
y_high = 0x07
y_low = 0x08

# Initialize I2C bus
bus = smbus2.SMBus(1)

# Read magnetic field values and print them
while True:
    x = bus.read_byte_data(address, x_high) << 8 | bus.read_byte_data(address, x_low)
    y = bus.read_byte_data(address, y_high) << 8 | bus.read_byte_data(address, y_low)
    z = bus.read_byte_data(address, z_high) << 8 | bus.read_byte_data(address, z_low)
    print("Magnetic field (x, y, z): ({}, {}, {})".format(x, y, z))
    time.sleep(1)"""






from i2c_hmc5883l import HMC5883
from time import sleep
import time
#Затем задаем bot значение гаусса и склонение для объекта HMC5883L:
time.sleep(1)
i2c_HMC5883l = HMC5883(gauss=1.3)
i2c_HMC5883l.set_declination(0, 0)
#Последний цикл печатает считанные значения. При каждом чтении сон продолжительностью 1 секунда немного замедляет чтение / печать, чтобы сделать его видимым для чтения человеком:


while True:
   print(i2c_HMC5883l.get_heading())
   sleep(1)
