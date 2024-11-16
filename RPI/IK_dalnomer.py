"""
# correct ADS1115!
import time
import smbus

# ADS1115 I2C address
ADS1115_ADDRESS = 0x48

# ADS1115 registers
CONFIG_REGISTER = 0x01
CONVERSION_REGISTER = 0x00

# Configuration settings
GAIN = 1  # Gain setting for the ADC
SAMPLE_RATE = 0x00  # Sample rate setting (default)

# Initialize I2C bus
bus = smbus.SMBus(1)

def read_adc(channel):
    # Configure the ADS1115 for single-shot mode
    config = (0x8000 | (channel << 12) | (GAIN << 9) | SAMPLE_RATE)
    bus.write_i2c_block_data(ADS1115_ADDRESS, CONFIG_REGISTER, [(config >> 8) & 0xFF, config & 0xFF])
    
    # Wait for conversion to complete
    #time.sleep(0.1)

    # Read the conversion result
    result = bus.read_i2c_block_data(ADS1115_ADDRESS, CONVERSION_REGISTER, 2)
    value = (result[0] << 8) | result[1]

    # Convert to voltage
    #print("value = ", value)
    #voltage = value * (4.096 / 32768.0)  # Assuming VDD = 4.096V
    
    return value

def voltage_to_distance(voltage):
    # Convert voltage to distance in cm using the formula for GP2Y0A21YK0F
    distance = (16.2537 * voltage**4 - 129.893 * voltage**3 +
                382.268 * voltage**2 - 512.611 * voltage + 301.439)
    
    return distance

try:
    print("Starting distance measurement...")
    while True:
        # Read from AIN0 (channel 0)
        iteration = 100
        value=0
        for i in range(0, iteration):
            # Запрос значения с AIN0
            value_tmp = read_adc(0)
            value = value + value_tmp   # Чтение значения
        k_arduino = 23
        k_volts = 0.0048828125
        value = value/iteration/k_arduino
        volts = value*k_volts
        distance = 32*(volts**-1.10)/1.1
        #distance = (distance/iteration)
        print("volts = ", volts)
        print("value = ", value)
        print("distance = ", distance)
        print()
        time.sleep(0.2)  # Delay for 200 ms
        

except KeyboardInterrupt:
    print("Measurement stopped by User")
"""




import smbus
import time

# Укажите адреса PCF8591
address_1 = 0x48  # Адрес первого PCF8591
address_2 = 0x48 #0x49  # Адрес второго PCF8591

# Адрес TCA9548A
tca_address = 0x70

# Инициализация шины I2C
bus = smbus.SMBus(1)

# Константы
K = 0.0048828125
N = -1.1
iteration = 100

def select_channel(channel):
    """Выбор канала на TCA9548A"""
    bus.write_byte(tca_address, 1 << channel)

def read_distance(address):
    """Чтение расстояния с указанного PCF8591"""
    value = 0
    for _ in range(iteration):
        bus.write_byte(address, 0x43)  # Команда для чтения AIN1
        value += bus.read_byte(address)
    value /= iteration
    volts = value * K
    distance = 32 * (volts ** N) / 1.2
    return volts, value, distance

try:
    while True:
        # Чтение с первого датчика
        select_channel(0)  # Выбор первого канала
        volts1, value1, distance1 = read_distance(address_1)
        print("Sensor 1 - volts: {:.4f}, value: {}, distance: {:.4f}".format(volts1, value1, distance1))

        # Чтение со второго датчика
        select_channel(1)  # Выбор второго канала
        volts2, value2, distance2 = read_distance(address_2)
        print("Sensor 2 - volts: {:.4f}, value: {}, distance: {:.4f}".format(volts2, value2, distance2))

        print()
        time.sleep(0.4)  # Задержка между считываниями

except KeyboardInterrupt:
    print("Программа остановлена.")















# correct pcf8591
import smbus
import time

# Укажите адрес PCF8591
address = 0x48

# Инициализация шины I2C
bus = smbus.SMBus(1)
K = 0.0048828125 #27.86
N = -1.1 #5
iteration = 100 #10 

while True:
    distance = 0
    volts = 0
    value = 0
    for i in range(0, iteration):
        # Запрос значения с AIN0
        bus.write_byte(address, 0x41)  # Команда для чтения AIN1
        value = value + bus.read_byte(address)   # Чтение значения
        #volts = (value/255.0)*3.3
        #distance = 32*(volts**-1.10)
        #time.sleep(0.05)  # Задержка между считываниями
        
        
    k_arduino = 0.257 #2.57/10
    k_volts = 0.0048828125
    value = value/iteration/k_arduino
    volts = value*k_volts
    distance = 32*(volts**-1.10)/1.2 # /1.1
    #distance = (distance/iteration)
    print("volts = ", volts)
    print("value = ", value)
    print("distance = ", distance)
    print()
    
    #time.sleep(0.2)











"""import spidev
import RPi.GPIO as GPIO
import time
import math

# Настройка GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(8, GPIO.OUT)  # Устанавливаем пин 25 как выход

# Инициализация SPI
spi = spidev.SpiDev()
spi.open(0, 0)  # Шина 0, устройство 0 (CE0)
spi.max_speed_hz = 1350000

def read_adc(channel):
    #Чтение значения из ADC
    adc = spi.xfer2([1, (8 + channel) << 4, 0])
    return ((adc[1] & 3) << 8) + adc[2]

def ir_read():
    #Считывание и усреднение значений
    averaging = 0
    cycl = 3
    k = 1.1

    for _ in range(cycl):
        value = read_adc(0)  # Чтение с канала 0
        volts = value * (5.0 / 1023.0)  # Преобразование в напряжение
        print(volts)
        distance = 32 * (volts ** -1.10)  #math.pow(volts, -1.10)  # Расчет расстояния в см
        averaging += distance

    value = averaging / cycl  # Усреднение значений
    if value > 80:
        return -1.0
    else:
        return value / k

try:
    print("start")
    while True:
        value = ir_read()
        print(f"Расстояние: {value:.2f} см")
        time.sleep(0.2)

except KeyboardInterrupt:
    spi.close()
    GPIO.cleanup()
"""





"""
import spidev
import RPi.GPIO as GPIO
import time

# Настройка GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(8, GPIO.OUT)  # Устанавливаем GPIO 25 как выход для CE

# Инициализация SPI
spi = spidev.SpiDev()
spi.open(0, 0)  # Шина 0, устройство 0 (CE0)
spi.max_speed_hz = 1350000

try:
    while True:
        GPIO.output(8, GPIO.LOW)  # Активируем CE
        response = spi.xfer2([0x01])  # Отправляем данные
        GPIO.output(8, GPIO.HIGH)  # Деактивируем CE
        print(f"Ответ: {response}")
        time.sleep(1)

except KeyboardInterrupt:
    spi.close()
    GPIO.cleanup()

"""












"""import RPi.GPIO as GPIO
import time
import spidev

# Настройка GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.IN)  # Устанавливаем пин 25 как вход

def read_distance():
    # Чтение аналогового значения
    voltage = GPIO.input(25) * (5.0 / 1023.0)  # Преобразование в напряжение
    distance = calculate_distance(voltage)
    return distance

def calculate_distance(voltage):
    if(voltage<= 0):
        return float('inf')
    K = 27.86
    N = -1.15
    distance = K * (voltage ** N)
    return distance

try:
    while True:
        distance = read_distance()
        print(f"Расстояние: {distance:.2f} см")
        time.sleep(0.5)

except KeyboardInterrupt:
    GPIO.cleanup()
"""





"""
import RPi.GPIO as GPIO
import time

pin = 25

# Настройка GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin, GPIO.IN)

# Функция для измерения расстояния
def measure_distance():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)
    time.sleep(0.00002)
    GPIO.output(pin, GPIO.HIGH)
    GPIO.setup(pin, GPIO.IN)
    end = 0
    start = 0
    while GPIO.input(pin) == 0:
        start = time.time()
    
    while GPIO.input(pin) == 1:
        end = time.time()
    
    duration = end - start
    distance = duration / 330000 #* 17150
    return distance

# Основной цикл
try:
    while True:
        distance = measure_distance()
        print("Расстояние: {:.2f} см".format(distance))
        time.sleep(1)
        
except KeyboardInterrupt:
    GPIO.cleanup()

"""






"""import RPi.GPIO as GPIO
import time

# Настройка GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.IN)

# Функция для измерения расстояния
def measure_distance():
    # Считываем значение с датчика
    distance = GPIO.input(25)
    
    # Конвертируем значение в расстояние
    if distance == 0:
        return 80.0
    else:
        return 27.86 * pow(distance * 1.0, -1.15)

# Основной цикл
try:
    while True:
        distance = measure_distance()
        print("Расстояние: {:.2f} см".format(distance))
        time.sleep(1)
        
except KeyboardInterrupt:
    GPIO.cleanup()
    print("Программа завершена")
"""
