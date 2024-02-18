import spidev
import time
#from fabolib import MPU9250
from mpu9250_i2c import MPU9250

spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 1000000

mpu9250 = MPU9250(spi)

while True:
    accel = mpu9250.readAccel()
    gyro = mpu9250.readGyro()
    mag = mpu9250.readMagnet()
    print("Accelerometer: ", accel)
    print("Gyroscope: ", gyro)
    print("Magnetometer: ", mag)
    time.sleep(0.1)
