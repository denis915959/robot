#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
#include "IRremote.h"
IRrecv irrecv(27); // указываем вывод, к которому подключен приемник
//#include <RF24.h>
decode_results results;
Ultrasonic nazad(37, 4);
Ultrasonic pered(31, 26);
Servo servo1;
int distance[2];
int distance1;
void setup() {
  // put your setup code here, to run once:
  irrecv.enableIRIn(); // запускаем прием
Serial.begin(9600);
pinMode(22, INPUT);//правый датчик
pinMode(23, INPUT);//левый датчик
pinMode(19, INPUT);//правый датчик сзади
pinMode(20, INPUT);//левый датчик сзади
pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый
pinMode(24, INPUT);//правый датчик 2, возможно убрать
pinMode(25, INPUT);//возможно убрать
pinMode(40, INPUT);//стартовая кнопка
//pinMode(37, INPUT);//датчик на шасси


}

void loop() {
  // put your main code here, to run repeatedly:
  int right_sensor_val;
int left_sensor_val;
int n=180;//скорость поворота при срабатывании второй линии, было 130
int n_stell=130;
int t_w=20;
int t=1000;
//int flag=0;
int t_start=1200;//задержка при повороте, на 100% аккуме 1200 достаточно
int t_radio=50;
int f=1, c=1, val;
int right_sensor_2_val;
int left_sensor_2_val;
int N=120;//основная скорость. Если что, на 120 все работает на 100% аккуме, было 200
int f1=0;
int n1=120; //на 100% аккуме 120
int n2=150;//поворот к стеллажу, на 100% аккуме 150
int t1=100;//движение к ящику
int sklad=60;
int st=175;
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин
int lin_speed=20;//было 20. Это скорость при срабатывании датчиков линии первого уровня
int N_nazad_srabat=200;
/*if (digitalRead(40)==1)
{
 digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
  delay(50);

  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    switch ( results.value ) {
    case 0xA3C8EDDB: case 0xFFA857://это 0xFFA857, повернуть влево и взять ящик с первого этажа*/
distance[0] = nazad.read();
distance[1]=distance[0];
Serial.println(distance[1]);
while (distance[1]>10)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_szadi_val = digitalRead(19);
left_sensor_szadi_val = digitalRead(3);
/*while (digitalRead(37)!=1)
{
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
servo1.write(sklad);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(sklad); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(sklad);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N);
}*/
if (right_sensor_szadi_val==0)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_szadi_val==0)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  Serial.println(distance[1]);
  distance[0]=distance[1];
  distance[1] = nazad.read();
  while (abs(distance[0]-distance[1])>=2)
  {
    distance[1]=distance[0];
   //distance[0]=distance[1];
   distance[0] = nazad.read();
  }
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
/*delay(500);
distance1 = nazad.read();
Serial.println(distance1);
while (distance1>12)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10          ВОЗМОЖНО, УБРАТЬ!
{
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_szadi_val = digitalRead(19);
left_sensor_szadi_val = digitalRead(20);

if (right_sensor_szadi_val==1)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_szadi_val==1)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  
  distance1 = nazad.read();
  Serial.println(distance1);
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);*/
delay(500);
distance1 = pered.read();
Serial.println(distance1);
while (distance1>4)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
 right_sensor_2_val = digitalRead(24);
  left_sensor_2_val = digitalRead(25);
/*while (digitalRead(37)!=1)
{
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
servo1.write(sklad);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(sklad); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(sklad);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N);
}*/
if (right_sensor_val==1)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_val==1)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  if ((left_sensor_2_val==1)/*&&(datchik==1)*/) 
  {
     while (digitalRead(22)!=1)
     {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(50);
     } 
  }
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);

  if ((right_sensor_2_val==1)/*&&(datchik==1)*/)
  {
    while (digitalRead(23)!=1)
    {
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, n);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      analogWrite(11, n);
      delay(50);
    } 
  }
  Serial.println(distance1);
  
  distance1 = pered.read();
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
delay(5000);
/*break;
}
irrecv.resume(); // принимаем следующую команду

}
}*/
}
