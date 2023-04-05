#include "IRremote.h"
#include <RCSwitch.h>
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
Servo servo1;

IRrecv irrecv(27); // указываем вывод, к которому подключен приемник

decode_results results;


RCSwitch mySwitch = RCSwitch();

RCSwitch mySwitch2 = RCSwitch();
void setup() {
  // put your setup code here, to run once:
mySwitch.enableTransmit(26);
mySwitch2.enableReceive(1);
irrecv.enableIRIn(); // запускаем прием
Serial.begin(9600);
pinMode(22, INPUT);//правый датчик
pinMode(23, INPUT);//левый датчик
pinMode(24, INPUT);//правый датчик, возможно убрать
pinMode(25, INPUT);//возможно убрать
pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый
//pinMode(40, OUTPUT);
//платформа
pinMode(28, INPUT);
pinMode(29, INPUT);
pinMode(30, INPUT);//датчик на шасси
pinMode(33, INPUT);//датчик перед
pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
pinMode(37, INPUT);//датчик на шасси
servo1.attach(31);
pinMode(52, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(52));
  if (digitalRead(52)==1)//срабатывание кнопки
  {
mySwitch.send(4, 5);
delay(10);
mySwitch.send(4, 5);
delay(10);
mySwitch.send(4, 5);
delay(10);
mySwitch.send(4, 5);
delay(500);
  }
}
