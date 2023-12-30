
#include <Wire.h> // подключаем I2C библиотеку
#define addr 0x1E // I2C 7-битный адрес датчика HMC5883

void setup() {
    Serial.begin(9600); // инициализация последовательного порта 
    Wire.begin(); // инициализация I2C
  
    // Задаём режим работы датчика HMC5883:
    Wire.beginTransmission(addr);
    Wire.write(0x00); // выбираем регистр управления CRA (00)
    Wire.write(0x70); // записываем в него 0x70 [усреднение по 8 точкам, 15 Гц, нормальные измерения]
    Wire.write(0xA0); // записываем в регистр CRB (01) 0xA0 [чувствительность = 5]
    Wire.write(0x00); // записываем в регистр Mode (02) 0x00 [бесконечный режим измерения]
    Wire.endTransmission();
}


void getHeading()
{ 
  Wire.beginTransmission(addr);
  Wire.write(0x03); // переходим к регистру 0x03
  Wire.endTransmission();
  
  Wire.requestFrom(addr, 6); // запрашиваем 6 байтов
  while( Wire.available() )  
  { 
    int h = Wire.read(); // старший байт значения по оси X
    int l = Wire.read(); // младший байт значения по оси X
    int x = word(h, l);  // объединяем в двухбайтовое число

    int y = Wire.read(); // старший байт значения по оси Y
    y = y << 8; // сдвигаем влево на 8 битов
    y = y | Wire.read(); // объединяем с младшим байтом по OR

    int z = Wire.read() << 8; // читаем байт и сдвигаем влево на 8 битов
    z |= Wire.read(); // сокращённый синтаксис операции OR

    Serial.print("X = ");  
    Serial.println(x, DEC); 
    Serial.print("Y = ");  
    Serial.println(y, DEC); 
    Serial.print("Z = ");  
    Serial.println(z, DEC); 
    Serial.println();    
  }

}

void loop() { 
  
  getHeading();
  delay(600);
}

/*#include <Wire.h>
#include <MechaQMC5883.h>
 
MechaQMC5883 qmc;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  qmc.init();
  //qmc.setMode(Mode_Continuous,ODR_200Hz,RNG_2G,OSR_256);
}
 
void loop() {
  int x, y, z;
  int azimuth;
  //float azimuth; //is supporting float too
  qmc.read(&x, &y, &z,&azimuth);
  //azimuth = qmc.azimuth(&y,&x);//you can get custom azimuth
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.print(z);
  Serial.print(" a: ");
  Serial.print(azimuth);
  Serial.println();
  delay(300);
}*/