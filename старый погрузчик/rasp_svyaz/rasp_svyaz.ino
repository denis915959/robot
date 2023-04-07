#include <Wire.h>
byte val = 0;
int SLAVE_ADDRESS=0x20;//04, задается здесь!
void setup()
{
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  Wire.begin(SLAVE_ADDRESS);         // подключиться к шине i2c (адрес для мастера не обязателен)
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.begin(9600);   // настроить последовательный порт для вывода
}

void loop() {
  // put your main code here, to run repeatedly:
 // Wire.requestFrom(SLAVE_ADDRESS, 1);     // запросить 6 байтов от ведомого устройства #2
  
 // delay(300);
}
/*void receiveData()
{
  while(Wire.available())     // ведомое устройство может послать меньше, чем запрошено
  { 
    int n = Wire.read();     // принять байт как символ
    Serial.print(n);          // напечатать символ
   
  }
}*/
void receiveData(int byteCount) 
{
  //int tmp[4];
  int data[4];
  int x=0;
  while(Wire.available()) 
  {               //Wire.available() returns the number of bytes available for retrieval with Wire.read(). Or it returns TRUE for values >0.
    data[x]=Wire.read();
    Serial.print(data[x]);          // напечатать символ
    x++;
  }
   Serial.print("\n");
  /*for (int i=0; i<4; i++)
  {
     Serial.print(data[i]);
     Serial.print("\t");
  }*/
}

void sendData()
{
  int num = digitalRead(12);
  Serial.print(num);
  Wire.write(num);
}
