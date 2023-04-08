
#include "IRremote.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

RCSwitch mySwitch2 = RCSwitch();
IRrecv irrecv(27); // указываем вывод, к которому подключен приемник
decode_results results;

#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Ultrasonic pered(31, 26);
Ultrasonic platform(2, 32);
Ultrasonic nazad(37, 4);
Servo servo1;
int distance1;

void setup() {
    //mySwitch.enableTransmit(4);
    mySwitch2.enableReceive(1);
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
pinMode(40, INPUT);//кнопка
pinMode(48, INPUT);
pinMode(49, INPUT);
pinMode(50, INPUT);
pinMode(51, INPUT);
//pinMode(37, INPUT);//датчик на шасси

}
int n=200;//скорость поворота, было 150
int t=1500;//время поворота без датчика, 1500
bool color_of_line=false;//0-белый, 1 - черный






void rotate_left_stel()
{
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(3)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n);
    delay(20);
  }//конец поворота по датчику
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}



void go_back_timer()
{
  int lin_speed=20;
  int distance;
  int N_nazad_srabat=220; // НАСТРОИТЬ!
  int N=120;
distance = 100;

//Serial.println(distance);

while (distance>7)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  analogWrite(11, N);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, N);
//delay(100);
  int right_sensor_szadi_val = digitalRead(19);
  int left_sensor_szadi_val = digitalRead(3);

  if (right_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(11, lin_speed);
     analogWrite(6, N_nazad_srabat);
     delay(100); //было 500
   }
  if (left_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(6, lin_speed);
      analogWrite(11, N_nazad_srabat);
     delay(100); //было 500
    }
    distance=nazad.read();
    
    //Serial.println(distance);
    
    //distance_szadi[0]=distance_szadi[1];
    //distance_szadi[1] = nazad.read();
   
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
lin_speed=40;
}









void loop() {
  // put your main code here, to run repeatedly:
 int distance;

int t=1500;//время поворота без датчика, 1500             ЕСЛИ ЧТО, ПРОБЛЕМА С ПОВОРОТОМ МОЖЕТ БЫТЬ ЗДЕСЬ
int N_z=120;//230
int st=120;//170
int sklad=120;//100
int rotate_v=sklad;//st
//int n=200;//скорость поворота, было 150


int t1=100;//движение к ящику
int N=100;//скорость движения по линии, а также скорость приближения к стеллажу
int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
int t_w=20;
int t_operezhenie=200;//доворот после срабатывания датчика
int tmp_nizhny_floor=2800;//должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков, 6500, лучше 3800, было 2500, потом 3000
int tmp=3900;//5000, 2500, 1400- на полный аккум было, 4100 последний
int go_front = 120;//200
int go_back_t=500;//450
int N_nazad_srabat=200;

//либо ПМП делать
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин

int n_stell=130;
//int flag=0;
int t_start=1500;//задержка при повороте, на 100% аккуме 1200 достаточно
int t_radio=50;
int f=1, c=1, val;
int f1=0;
int n1=140; //на 100% аккуме 120
//int datchik=1;
int d=1000;//длительность остановка для отправки сигнала,1000
int number_povtor=8;//было 20
bool flag=false;
bool go_line=true;
bool radio_rotate=false;
bool color_of_line=false;//0-белый, 1 - черный

int rt_1=120;  //было 140
int lin_speed_1=40;  //ЭТАЛОН было 20, затем 40, но не очень. 25 более-менее (вроде осечек не было, но медленно). Это скорость при срабатывании датчиков линии первого уровня

int line_true_1=5;// было 200, на 10 уже хорошо работает

int rt_2=170;//170


int line_true=200;

int lin_speed=40; //было 30
int trig=-1;
int speed_step=5;
int barrier=220;  //было 220
int N_r_const=120; //было 120, это стартовая скорость внешних колес при повороте
int N_rotate=N_r_const; 

int trig_level2=-1;
int barrier_level2=255;
int N_rotate_level2=240; 
int lin_speed_2=30; 
int line_true_level2=50;

int line_true_2=50; //было 300


int n=150;//скорость разворота до 1 контура, было 200

bool key;








if (digitalRead(40)==1)//было 32
{
   flag=true;
   //povorot_platformy();
}
digitalWrite(8, LOW);
digitalWrite(7, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
delay(5);
int dist=0;
if (flag==true)
{
  //Serial.println(pered.read());
  dist=pered.read();
  while (dist>3)//2 
  {
    
  //Serial.println(dist);
  
  if ( irrecv.decode( &results )) 
  { // если данные пришли
    
      Serial.println( results.value, HEX );
      
     switch ( results.value ) {
      case 0xFFA995:
        //rotate_left_stel();
        //flag=false;
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(1000);
        rotate_left_stel();
        delay(1000);
        go_back_timer();
        
      break;
     }
    irrecv.resume();
  }

if (go_line==true)
{
//  servo1.write(rotate_v);
  int right_sensor_val = digitalRead(22);
  int left_sensor_val = digitalRead(23);
  int right_sensor_2_val = digitalRead(24);
  int left_sensor_2_val = digitalRead(25);
  int right_sensor_3_val = digitalRead(48);
  int left_sensor_3_val = digitalRead(49);
  int right_sensor_4_val = digitalRead(50);
  int left_sensor_4_val = digitalRead(51);


Serial.println(left_sensor_val);   //левый 3 ловит 1 всегда, а должен 0!



if (left_sensor_3_val==color_of_line) 
{
  trig=1;
 while (digitalRead(22)!=color_of_line)
 {
 digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 }
 N=100;
 digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N); //N

delay(100);
}

//Serial.println(
if (right_sensor_3_val==color_of_line)
{
  trig=0;
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  }
 N=100;
 digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N); //N

delay(100);
}








if (left_sensor_4_val==color_of_line) 
{
  trig=1;
 while (digitalRead(22)!=color_of_line)
 {
 digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 } 
}

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N


if (right_sensor_4_val==color_of_line)
{
  trig=0;
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  } 
}

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N











if (left_sensor_2_val==color_of_line) 
{
  trig=1;
  
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(500);  //возможно, этот блок убрать
  
 while (digitalRead(22)!=color_of_line)
 {
 digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 } 
}
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N








if (right_sensor_2_val==color_of_line)
{
  trig=0;

  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(500);  //возможно, этот блок убрать
  
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  } 
}
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N












  
/*
  if (left_sensor_2_val==color_of_line)
  {
    trig=0;
    N=N_rotate_level2;
    analogWrite(6, lin_speed_2);
    analogWrite(11, N_rotate_level2);
    delay(line_true);
    N=barrier;  
  }
  N=130;
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);//желательно скорость 200 
  analogWrite(11, N);




  if (right_sensor_2_val==color_of_line)
  {
    trig=0;
    N=N_rotate_level2;
    analogWrite(11, lin_speed_2);
    analogWrite(6, N_rotate_level2);
    delay(line_true);
    N=barrier;  
  }
  N=130;
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);//желательно скорость 200 
  analogWrite(11, N);
  
*/
  
  
  if (left_sensor_val==color_of_line) 
  {
    if(trig==0)
    {
      if(N_rotate<barrier)
      {
        N_rotate=N_rotate+speed_step;
      }
    }
    else
    {
      N_rotate=N_r_const;
    }
    trig=0;
    //N_rotate=150;
    analogWrite(6, lin_speed);
    analogWrite(11, N_rotate);
    delay(line_true);  
  }
  N=100;
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);//желательно скорость 200 
  analogWrite(11, N);


  
  if (right_sensor_val==color_of_line)
  {
     if(trig==1)
    {
      if(N_rotate<barrier)
      {
        N_rotate+=speed_step;
      }
    }
    else
    {
      N_rotate=N_r_const;
    }
    trig=1;
    analogWrite(6, N_rotate);
    analogWrite(11, lin_speed);
    delay(line_true);
  }
  N=100;
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
















  
  /*if ((left_sensor_2_val==color_of_line)) 
  {
     while (digitalRead(22)!=color_of_line)
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

  if ((right_sensor_2_val==color_of_line))
  {
    while (digitalRead(23)!=color_of_line)
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
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
*/
  
 }
 dist=pered.read();
}
}
}
