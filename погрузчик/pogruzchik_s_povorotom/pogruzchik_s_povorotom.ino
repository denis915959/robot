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
servo1.attach(31);
}

void loop() {
  // put your main code here, to run repeatedly:
int right_sensor_val;
int left_sensor_val;
int n=110;//скорость поворота
int n_stell=130;
int t_w=20;
int t=1000;
//int flag=0;
int t_start=1200;//задержка при повороте
int t_radio=50;
int f=1, c=1, val;
int right_sensor_2_val;
int left_sensor_2_val;
int N=120;//основная скорость. Если что, на 120 все работает
int f1=0;
int n1=120;
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
      mySwitch.send(4, 4);
      delay(10);
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
       digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(1000);


       
while (digitalRead(30)!=0)
{
  //goto lin;

 N=100;
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
if (left_sensor_val==1) 
{
 analogWrite(6, 0); 
 analogWrite(11, 200);
 delay(250);
  }

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);//желательно скорость 200 
if (right_sensor_val==1)
{
  analogWrite(6, 200);
 analogWrite(11, 0);
 delay(250); 
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

digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(5000);









 N=120;     
        break;       
    case 0x808B0C15://это 0xFFA995
   mySwitch.send(4, 4);
      delay(10);
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
       digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(1000);


       
while (digitalRead(30)!=0)
{
  //goto lin;

N=100;
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
if (left_sensor_val==1) 
{
 analogWrite(6, 0);
 analogWrite(11, 200); 
 delay(250);
  }

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 analogWrite(6, 200);
 analogWrite(11, 0);
 delay(250); 
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

digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       analogWrite(6, 0);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
       analogWrite(11, 0);
       delay(5000);







  N=120;       
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
    delay(20);
 mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    N=100;
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
        
        while (c==1)//прием сигнала со склада
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
          break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

      switch(val){
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
    } 
        break;
    case 0xFDD54E://это 0xFDD54E, поворот влево

    
       analogWrite(6, 0);
    analogWrite(11, 0);
    delay(20);
 mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    N=100;
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
        
        while (c==1)//прием сигнала со склада
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
mySwitch.send(9, 4);//передача сигнала на склад, возможно 5
    delay(10);
          break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

      switch(val){
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
    } 
        break;


             
    case 0xE5D9AAC7://это 0xFEE265, поворот вправо
   analogWrite(6, 0);
    analogWrite(11, 0);
    delay(20);
 mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
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
        
        while (c==1)//прием сигнала со склада
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
          break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

      switch(val){
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
    } 
        break;  


        
    case 0xFEE265://это 0xFEE265
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(20);
 mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(7, 4);//передача сигнала на склад
    delay(10);
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
        
        while (c==1)//прием сигнала со склада
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
          mySwitch.send(9,4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад, возможно 5
    delay(10);
          break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 

      switch(val){
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
    } 
        break;  



    
                   
    } 
    //results.value=0;
     // val=0;   
    irrecv.resume(); // принимаем следующую команду
} 
//lin:
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
