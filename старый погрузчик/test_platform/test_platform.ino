#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
Servo servo1; // объявляем переменную servo типа "servo1"
void setup() {
  // put your setup code here, to run once:
pinMode(28, INPUT);
pinMode(29, INPUT);
pinMode(30, INPUT);//датчик на шасси
pinMode(32, INPUT);//кнопка
pinMode(33, INPUT);//датчик перед
pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
Serial.begin(9600);
servo1.attach(31);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(digitalRead(32));//удалить
//delay(250);//удалить
digitalWrite(35, LOW);
pinMode(36, LOW);
servo1.write(175);
if (digitalRead(32)==1)
{
  servo1.write(175);
  while (digitalRead(28)!=0)
  {
    servo1.write(175);
digitalWrite(35, HIGH);
pinMode(36, LOW);
delay(50);
  }
 while (digitalRead(29)!=0)
  {
    servo1.write(175);
digitalWrite(36, HIGH);
pinMode(35, LOW);
delay(50);
  }
  servo1.write(175);
 digitalWrite(35, HIGH);
pinMode(36, LOW);
delay(2000);//заменить
digitalWrite(35, LOW);
pinMode(36, LOW);
delay(20);
while ((digitalRead(33)!=0)&&(digitalRead(30)!=0))//датчик
{
  servo1.write(175);
digitalWrite(37, HIGH);
delay(50);
digitalWrite(37, LOW);
delay(50);
}
servo1.write(175);
digitalWrite(37, LOW);
}
}
