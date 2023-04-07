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
//pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
pinMode(37, INPUT);//датчик на шасси для заднего хода (левый)
pinMode(52, INPUT);//стартовая кнопка
servo1.attach(31);
}

void loop() {
  // put your main code here, to run repeatedly:
int right_sensor_val;
int left_sensor_val;
int n=140;//скорость поворота, на 100% аккуме 110
int n_stell=130;
int t_w=20;
int t=1000;
//int flag=0;
int t_start=1500;//задержка при повороте, на 100% аккуме 1200 достаточно
int t_radio=50;
int f=1, c=1, val;
int right_sensor_2_val;
int left_sensor_2_val;
int N=120;//основная скорость. Если что, на 120 все работает на 100% аккуме
int N_z=190;
int f1=0;
int n1=140; //на 100% аккуме 120
int n2=140;//поворот к стеллажу, на 100% аккуме 150
int t1=100;//движение к ящику
int datchik=1;
int d=2000;//длительность остановка для отправки сигнала,1000
int t_operezhenie=300;//доворот после срабатывания датчика
int sklad=120;//100
int st=120;//170
int rotate_v=sklad;//st
int number_povtor=10;//было 20
bool flag=false;
if (digitalRead(52)==1)
flag=true;
digitalWrite(8, LOW);
digitalWrite(7, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
delay(5);
if (flag==true)
{
  
while (digitalRead(37)!=0)
{
if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    switch ( results.value ) {
    case 0xA3C8EDDB://это 0xFFA857
    mySwitch.send(1, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
       
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
       //digitalWrite(8, HIGH);
       //digitalWrite(7, LOW);
       //analogWrite(6, 200);
       //digitalWrite(9, HIGH);
       //digitalWrite(10, LOW);
       //analogWrite(11, 200);
       //delay(t_start);
       
        break; 
    case 0xFFA857://это 0xFFA857
    mySwitch.send(1, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
       
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
      
        break;
    case 0xFFE01F://это 0xFFE01F
    mySwitch.send(2, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
      
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
      
        break;
    case 0xF076C13B://это 0xFFE01F
    mySwitch.send(2, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
       
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
     
        break;
    case 0xFFA25D://это 0x00FFA25D
    mySwitch.send(3, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
       
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
       
        break;  
    case 0xE318261B://это 0x00FFA25D
      mySwitch.send(3, 4);
       digitalWrite(7, HIGH);
       digitalWrite(8, LOW);
       analogWrite(6, n);
       digitalWrite(9, HIGH);
       digitalWrite(10, LOW);
       analogWrite(11, n);
       delay(t);
       
       do{
        left_sensor_val = digitalRead(23);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }while (left_sensor_val!=1);
        
        break;  
    case 0xFFA995://это 0xFFA995
    rotate_v=sklad;
analogWrite(6, 0);
analogWrite(11, 0);
delay(d);
for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(4, 4);
      delay(10);
}
/*mySwitch.send(4, 4);
      delay(10);
mySwitch.send(4, 4);
      delay(10);
mySwitch.send(4, 4);
      delay(10);*/
   servo1.write(rotate_v);
   datchik=0;
      digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t);//конец поворота по таймеру
       servo1.write(rotate_v);
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
        delay(t_w);
        servo1.write(rotate_v);
       }//конец поворота по датчику
       digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(500);



 digitalWrite(35, LOW);//подъем
pinMode(36, LOW);
servo1.write(rotate_v);
delay(200);
//if (digitalRead(32)==1)
//{
  servo1.write(rotate_v);
 while (digitalRead(29)!=0)
  {
    servo1.write(rotate_v);
digitalWrite(36, HIGH);
pinMode(35, LOW);
delay(50);
  }
  servo1.write(rotate_v);
digitalWrite(35, LOW);
pinMode(36, LOW);
delay(100);//конец подъема


       
while (digitalRead(30)!=0)
{
  //goto lin;
servo1.write(rotate_v);
 N=80;
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
if (left_sensor_val==1) 
{
 analogWrite(6, 0); 
 analogWrite(11, 200);
 delay(t1);
  }

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);//желательно скорость 200 
if (right_sensor_val==1)
{
  analogWrite(6, 200);
 analogWrite(11, 0);
 delay(t1); 
  }
  digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
if (left_sensor_2_val==1) 
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

if (right_sensor_2_val==1) 
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
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);
 digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
  }
  rotate_v=st;
servo1.write(rotate_v);
digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(100);


digitalWrite(36, HIGH);
pinMode(35, LOW);//типа продолжаем движение немного вверх
delay(1000);//заменить на 800, если будет не хватать
digitalWrite(36, LOW);
pinMode(35, LOW);//типа продолжаем движение немного вверх
delay(100);
      



       while (digitalRead(37)!=1)//движение назад по линии
{
  
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N_z);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N_z);
servo1.write(rotate_v);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(rotate_v); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N_z);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(rotate_v);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N_z);
}//конец движения назад по линии
servo1.write(rotate_v);
analogWrite(11, 0);
analogWrite(6, 0);
delay(20);

digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(500);

 c=1;
 val=0;     
while (c==1)
{
  if( mySwitch2.available() ){

        val = mySwitch2.getReceivedValue();
        if (val>=10){
        Serial.println(val);
        if ((val==10)||(val==11)||(val==12))//11-влево, 12-вправо, 10 - ничего
{
          c=0; 
         analogWrite( 6, 0 );//убрать
        delay(10);
          for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(9, 4);//передача сигнала на склад. если что, было 7
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
}
          /*mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);*/
         // break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

digitalWrite(35, HIGH);
pinMode(36, LOW);//типа продолжаем движение немного вверх
delay(4000);
digitalWrite(35, LOW);
pinMode(36, LOW);//типа продолжаем движение немного вверх
delay(1000);

 N=120; 
//rotate_v=170;
 switch(val){//11-влево, 12-вправо, 10 - ничего
     case 10://использование сигнала, прибывшего со склада
        delay(1);
        break;
     case 11:
     analogWrite( 6, 140 );
        delay(1000);
     //while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
      //  delay(50);
    //}
        break;   
     case 12:
     digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t);//конец поворота по таймеру
       servo1.write(rotate_v);
while (digitalRead(22)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
        delay(t_w);
        servo1.write(rotate_v);
       }//конец поворота по датчику
       digitalWrite(8, HIGH);//в случае чего удалить именно этот блок
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t_operezhenie);//конец доворота по таймеру
    break;
    } 
     //sklad=170;
        break;       
    case 0x808B0C15://это 0xFFA995
    rotate_v=sklad;
analogWrite(6, 0);
analogWrite(11, 0);
delay(d);
   for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(4, 4);
      delay(10);
}
/*mySwitch.send(4, 4);
      delay(10);
mySwitch.send(4, 4);
      delay(10);
mySwitch.send(4, 4);
      delay(10);*/
   servo1.write(rotate_v);
   datchik=0;
      digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t);//конец поворота по таймеру
       servo1.write(rotate_v);
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
        delay(t_w);
        servo1.write(rotate_v);
       }//конец поворота по датчику
       digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(500);



 digitalWrite(35, LOW);//подъем
pinMode(36, LOW);
servo1.write(rotate_v);
delay(200);
//if (digitalRead(32)==1)
//{
  servo1.write(rotate_v);
 while (digitalRead(29)!=0)
  {
    servo1.write(rotate_v);
digitalWrite(36, HIGH);
pinMode(35, LOW);
delay(50);
  }
  servo1.write(rotate_v);
digitalWrite(35, LOW);
pinMode(36, LOW);
delay(100);//конец подъема


       
while (digitalRead(30)!=0)
{
  //goto lin;
servo1.write(rotate_v);
 N=80;
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
if (left_sensor_val==1) 
{
 analogWrite(6, 0); 
 analogWrite(11, 200);
 delay(t1);
  }

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);//желательно скорость 200 
if (right_sensor_val==1)
{
  analogWrite(6, 200);
 analogWrite(11, 0);
 delay(t1); 
  }
  digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
if (left_sensor_2_val==1) 
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

if (right_sensor_2_val==1) 
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
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);
 digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
  }
  rotate_v=st;
servo1.write(rotate_v);
digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(100);


digitalWrite(36, HIGH);
pinMode(35, LOW);//типа продолжаем движение немного вверх
delay(1000);//заменить на 800, если будет не хватать
digitalWrite(36, LOW);
pinMode(35, LOW);//типа продолжаем движение немного вверх
delay(100);
      



       while (digitalRead(37)!=1)//движение назад по линии
{
  
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N_z);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N_z);
servo1.write(rotate_v);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(rotate_v); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N_z);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(rotate_v);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N_z);
}//конец движения назад по линии
servo1.write(rotate_v);
analogWrite(11, 0);
analogWrite(6, 0);
delay(20);

digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(500);

 c=1;
 val=0;     
while (c==1)
{
  if( mySwitch2.available() ){

        val = mySwitch2.getReceivedValue();
        if (val>=10){
        Serial.println(val);
        if ((val==10)||(val==11)||(val==12))//11-влево, 12-вправо, 10 - ничего
{
          c=0; 
         analogWrite( 6, 0 );//убрать
        delay(10);
        for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(9, 4);//передача сигнала на склад. если что, было 7
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
}
          /*mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);*/
         // break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

digitalWrite(35, HIGH);
pinMode(36, LOW);//типа продолжаем движение немного вверх
delay(4000);
digitalWrite(35, LOW);
pinMode(36, LOW);
delay(1000);

 N=120; 
//rotate_v=170;
 switch(val){//11-влево, 12-вправо, 10 - ничего
     case 10://использование сигнала, прибывшего со склада
        delay(1);
        break;
     case 11:
     analogWrite( 6, 140 );
        delay(1000);
     //while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
      //  delay(50);
    //}
        break;   
     case 12:
     digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t);//конец поворота по таймеру
       servo1.write(rotate_v);
while (digitalRead(22)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
        delay(t_w);
        servo1.write(rotate_v);
       }//конец поворота по датчику
       digitalWrite(8, HIGH);//в случае чего удалить именно этот блок
        digitalWrite(7, LOW);
        analogWrite(6, n2);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n2);
       delay(t_operezhenie);//конец доворота по таймеру
    break;
    } 
    //sklad=170;
        break;  
    case 0xFFE100://это 0xFFE100
    // mySwitch.send(5, 4);
       digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
       
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(1000);
       
        break;  
    case 0xE45081D0://это 0xFFE100
   // mySwitch.send(5, 4);
       digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
       
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(1000);
       
        break;  
    case 0xF61C56://это 0xFDD54E, поворот влево
   analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
 for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(6, 4);
      delay(10);
}
/*mySwitch.send(6, 4);
      delay(10);
mySwitch.send(6, 4);
      delay(10);
mySwitch.send(6, 4);
      delay(10);*/
    N=100;
    datchik=1;
    digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
       
while (digitalRead(22)!=1)//поворот по датчику
{
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(10);
        
        /*while (c==1)//прием сигнала со склада
{
  if( mySwitch2.available() ){

        val = mySwitch2.getReceivedValue();
        if (val>=10){
        Serial.println(val);
        if ((val==10)||(val==11)||(val==12))
{
         // c=0; 
         analogWrite( 6, 0 );//убрать
        delay(10);
          mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
          //break;  
          c=0;   
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада */
c=1;
     /* switch(val){
     case 10://использование сигнала, прибывшего со склада
        delay(1);
        break;
     case 11:
     analogWrite( 6, 140 );
        delay(1000);
     //while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
      //  delay(50);
    //}
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
    // while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
     //   delay(50);
    //}
    break;
    }*/ 
        break;
    case 0xFDD54E://это 0xFDD54E, поворот влево

    servo1.write(rotate_v);
       analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(6, 4);
      delay(10);
}
/*mySwitch.send(6, 4);
      delay(10);
mySwitch.send(6, 4);
      delay(10);
mySwitch.send(6, 4);
      delay(10);*/
    N=100;
    datchik=1;
    digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
     servo1.write(rotate_v);  
while (digitalRead(22)!=1)//поворот по датчику
{
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(10);
        servo1.write(rotate_v);
        
servo1.write(rotate_v);
c=1;
     
        break;


             
    case 0xE5D9AAC7://это 0xFEE265, поворот вправо
    
    servo1.write(rotate_v);
   analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(5, 4);//передача сигнала на склад. если что, было 7
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
}
/*mySwitch.send(5, 4);
      delay(10);
mySwitch.send(5, 4);
      delay(10);
mySwitch.send(5, 4);
      delay(10);*/
    
    datchik=1;
    digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
       
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(10);
        
        break;  


        
    case 0xFEE265://это 0xFEE265
    servo1.write(rotate_v);
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
 for (int i=0; i<number_povtor; i++)
{
    mySwitch.send(5, 4);//передача сигнала на склад. если что, было 7
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
}
/*mySwitch.send(5, 4);
      delay(10);
mySwitch.send(5, 4);
      delay(10);
mySwitch.send(5, 4);
      delay(10);*/
    datchik=1;
    digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
       delay(t);//конец поворота по таймеру
       
while (digitalRead(23)!=1)//поворот по датчику
{
        digitalWrite(8, HIGH);
        digitalWrite(7, LOW);
        analogWrite(6, n);
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        analogWrite(11, n);
        delay(t_w);
       }//конец поворота по датчику
    
      analogWrite( 6, 0 );
        analogWrite( 11, 0 );
        delay(10);
        servo1.write(rotate_v);
        
        break;  



    
                   
    } 
    //results.value=0;
     // val=0;   
    irrecv.resume(); // принимаем следующую команду
} 
//lin:
servo1.write(rotate_v);
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
if (left_sensor_val==1) 
{
 analogWrite(6, 20); 
 delay(100); 
  }

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(11, 20);
 delay(100); 
  }
  digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
if ((left_sensor_2_val==1)&&(datchik==1)) 
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

if ((right_sensor_2_val==1)&&(datchik==1))
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
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);
 digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
}
flag=false;
}
}
