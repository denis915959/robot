#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
//Servo servo1; // объявляем переменную servo типа "servo1"
void setup() {
  // put your setup code here, to run once:
pinMode(28, INPUT);//нижний концевик
pinMode(29, INPUT);
pinMode(30, INPUT);//датчик на шасси
pinMode(33, INPUT);//датчик перед
//pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
Serial.begin(9600);
//servo1.attach(31);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(digitalRead(28));//удалить
digitalWrite(35, LOW);
pinMode(36, LOW);
  while (digitalRead(29)!=0)
  {
  digitalWrite(36, HIGH);
digitalWrite(35, LOW);
delay(30);
  }
  digitalWrite(36, LOW);
delay(2000);

digitalWrite(35, HIGH);
digitalWrite(36, LOW);
delay(5500);

digitalWrite(35, LOW);
delay(2000);

/*delay(5000);
digitalWrite(36, HIGH);
digitalWrite(35, LOW);
delay(3000);
digitalWrite(36, LOW);
delay(5000);*/

/*digitalWrite(35, LOW);
digitalWrite(36, LOW);
delay(4000);

digitalWrite(35, HIGH);
digitalWrite(36, LOW);
delay(4500);

digitalWrite(35, LOW);
digitalWrite(36, LOW);
delay(2000);*/
}
