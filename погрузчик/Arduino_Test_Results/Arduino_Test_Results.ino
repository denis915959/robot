#include "Wire.h"
//#include "HMC5883L.h"
#include "math.h"
#define addr 0x1E // I2C 7-битный адрес датчика HMC5883

//HMC5883L compass; //Copy the folder "HMC5883L" in the folder "C:\Program Files\Arduino\libraries" and restart the arduino IDE.

int xv, yv, zv;  // float

//calibrated_values[3] is the global array where the calibrated data will be placed
//calibrated_values[3]: [0]=Xc, [1]=Yc, [2]=Zc
float calibrated_values[3];   
//transformation(float uncalibrated_values[3]) is the function of the magnetometer data correction 
//uncalibrated_values[3] is the array of the non calibrated magnetometer data
//uncalibrated_values[3]: [0]=Xnc, [1]=Ync, [2]=Znc
void transformation(int uncalibrated_values[3]) // float   
{
  //calibration_matrix[3][3] is the transformation matrix
  //replace M11, M12,..,M33 with your transformation matrix data
  double calibration_matrix[3][3] = 
  {
    {0.489, -0.06, -0.097},
    {0, -0.025, 0.121},
    {0.001, 2.187, -0.006}  
  };
  //bias[3] is the bias
  //replace Bx, By, Bz with your bias data
  double bias[3] = 
  {
    8.339,
    -0.509,
    -0.414
  };  
  //calculation
  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

void setup()
{   
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

void loop()
{
  int values_from_magnetometer[3]; //float
  
  getHeading();
  values_from_magnetometer[0] = xv;
  values_from_magnetometer[1] = yv;
  values_from_magnetometer[2] = zv;
  /*Serial.println(values_from_magnetometer[0]);
  Serial.println(values_from_magnetometer[1]);
  Serial.println(values_from_magnetometer[2]);*/
  
  transformation(values_from_magnetometer);

  /*Serial.flush(); 
  Serial.print(calibrated_values[0]); 
  Serial.print(",");
  Serial.print(calibrated_values[1]);
  Serial.print(",");
  Serial.print(calibrated_values[2]);
  Serial.println();*/

  // сюда расчет углов!
  float heading = atan2(calibrated_values[1], calibrated_values[0]);
  if(heading<0)
    heading += 2*PI;
  if(heading>2*PI)
    heading -= 2*PI;

  float headingDegrees = heading* 180/M_PI;
  Serial.println(headingDegrees);
  Serial.print(" Degrees   \t");
  delay(500); 
} 

/*void setupHMC5883L()
{  
  compass.SetScale(0.88);
  compass.SetMeasurementMode(Measurement_Continuous);
}*/
 
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
    xv = word(h, l);  // объединяем в двухбайтовое число

    yv = Wire.read(); // старший байт значения по оси Y
    yv = yv << 8; // сдвигаем влево на 8 битов
    yv = yv | Wire.read(); // объединяем с младшим байтом по OR

    zv = Wire.read() << 8; // читаем байт и сдвигаем влево на 8 битов
    zv |= Wire.read(); // сокращённый синтаксис операции OR

    /*Serial.print("X = ");  
    Serial.println(xv, DEC); 
    Serial.print("Y = ");  
    Serial.println(yv, DEC); 
    Serial.print("Z = ");  
    Serial.println(zv, DEC); 
    Serial.println();*/   
  }
}

/*void getHeading()
{ 
  MagnetometerRaw raw = compass.ReadRawAxis();
  xv = (float)raw.XAxis;
  yv = (float)raw.YAxis;
  zv = (float)raw.ZAxis;
}*/



