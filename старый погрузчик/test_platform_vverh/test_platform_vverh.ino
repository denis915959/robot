#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
Servo servo1; // объявляем переменную servo типа "servo1"
void setup() {//скетч для заливки
  // put your setup code here, to run once:
pinMode(28, INPUT);
pinMode(29, INPUT);
pinMode(30, INPUT);//датчик на шасси
pinMode(32, INPUT);//кнопка
pinMode(33, INPUT);//датчик перед
//pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
Serial.begin(9600);
servo1.attach(31);
}

void loop() {
  // put your main code here, to run repeatedly:
  int s=175;
Serial.println(digitalRead(33));//удалить, 1-нет препятствия, 0-есть препятствие
//delay(250);//удалить
digitalWrite(35, LOW);
pinMode(36, LOW);
servo1.write(s);
delay(1000);
//if (digitalRead(32)==1)
//{
  servo1.write(s);
 while (digitalRead(29)!=0)
  {
    servo1.write(s);
digitalWrite(36, HIGH);
pinMode(35, LOW);
delay(50);
  }
  servo1.write(s);
 //digitalWrite(36, HIGH);
//pinMode(35, LOW);//типа продолжаем движение немного вверх
//delay(1000);//заменить на 800, если будет не хватать
/*digitalWrite(36, HIGH);
pinMode(35, LOW);
delay(500);*/
digitalWrite(35, LOW);
pinMode(36, LOW);
delay(1000);
digitalWrite(35, HIGH);
pinMode(36, LOW);
delay(2500);//5000
}
