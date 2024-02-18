import spidev

spi = spidev.SpiDev()
spi.open(0, 0) # указываем номер шины и номер устройства (CS)

spi.max_speed_hz = 1000000 # устанавливаем скорость передачи данных

def read_data(register):
    cmd = [register | 0x80] # устанавливаем бит чтения
    response = spi.xfer2(cmd + [0x00]) # передаем команду и получаем ответ
    return response[1] # возвращаем значение

# Пример чтения данных из регистра WHO_AM_I (адрес 0x75)
who_am_i = read_data(0x75)
print("WHO_AM_I register: 0x{:02X}".format(who_am_i))

spi.close()