#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Ultrasonic pered(31, 26);
Servo servo1;
int distance1;
void setup() {
  // put your setup code here, to run once:
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
//pinMode(37, INPUT);//датчик на шасси
}

void loop() {
  // put your main code here, to run repeatedly:
  int right_sensor_val;
int left_sensor_val;
int n=130;//скорость поворота, на 100% аккуме 110
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
if ((left_sensor_2_val==0)/*&&(datchik==1)*/) 
  {
     while (digitalRead(22)!=0)
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
  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);*/

if ((right_sensor_2_val==0)/*&&(datchik==1)*/)
  {
    while (digitalRead(23)!=0)
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
  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);*/
 



 
if (right_sensor_val==0)//для черного пола и белой линии
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_val==0)//для черного пола и белой линии
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
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
delay(1500);
}
