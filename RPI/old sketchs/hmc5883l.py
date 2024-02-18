#chat_gpt
import smbus

# Адрес магнетометра HMC5883L
ADDRESS = 0x1e

# Регистры для считывания данных о магнитном поле
DATA_REGISTER = 0x03
MODE_REGISTER = 0x02

# Конфигурация режима работы магнетометра
MODE_CONTINUOUS = 0x00
SCALE = 0x01

# Инициализация I2C
i2c_bus = smbus.SMBus(1)

# Установка режима работы магнетометра
i2c_bus.write_byte_data(ADDRESS, MODE_REGISTER, MODE_CONTINUOUS)
i2c_bus.write_byte_data(ADDRESS, SCALE, 0x01)

def read_angle():
    # Чтение данных магнитного поля
    data = i2c_bus.read_i2c_block_data(ADDRESS, DATA_REGISTER, 6)

    # Преобразование данных в угол поворота
    x = (data[0] << 8) | data[1]
    y = (data[4] << 8) | data[5]

    angle = math.atan2(y, x)
    if angle < 0:
        angle += 2 * math.pi

    angle_degrees = math.degrees(angle)
    return angle_degrees

while True:
    angle = read_angle()
    print("Угол поворота: %.2f градусов" % angle)
