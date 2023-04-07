#include "IRremote.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

RCSwitch mySwitch2 = RCSwitch();
IRrecv irrecv(2); // указываем вывод, к которому подключен приемник

decode_results results;

void setup() {
    mySwitch.enableTransmit(4);
    mySwitch2.enableReceive(1);
  irrecv.enableIRIn(); // запускаем прием
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);//11 b 10 пины умерли
  pinMode(5, OUTPUT);
  pinMode(8, INPUT);
}

void loop() {
  int f=0;
  int val;
  int c=1;
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    switch ( results.value ) {
    case 0xA3C8EDDB://это 0xFFA857
        digitalWrite( 13, HIGH );
        delay(100);
        digitalWrite( 13, LOW );
        break; 
    case 0xFFA857://это 0xFFA857
        digitalWrite( 13, HIGH );
        delay(100);
        digitalWrite( 13, LOW );
        break;
    case 0xFFE01F://это 0xFFE01F
        digitalWrite( 12, HIGH );
        delay(100);
        digitalWrite( 12, LOW );
        break;
    case 0xF076C13B://это 0xFFE01F
        digitalWrite( 12, HIGH );
        delay(100);
        digitalWrite( 12, LOW );
        break;
    case 0xFFA25D://это 0x00FFA25D
        digitalWrite( 11, HIGH );
        delay(100);
        digitalWrite( 11, LOW );
        break;  
    case 0xE318261B://это 0x00FFA25D
        digitalWrite( 11, HIGH );
        delay(100);
        digitalWrite( 11, LOW );
        break;  
    case 0xFFA995://это 0xFFA995
        digitalWrite( 10, HIGH );
        delay(100);
        digitalWrite( 10, LOW );
        break;       
    case 0x808B0C15://это 0xFFA995
        digitalWrite( 10, HIGH );
        delay(100);
        digitalWrite( 10, LOW );
        break;  
    case 0xFFE100://это 0xFFE100
        digitalWrite( 9, HIGH );
        delay(100);
        digitalWrite( 9, LOW );
        break;  
    case 0xE45081D0://это 0xFFE100
        digitalWrite( 9, HIGH );
        delay(100);
        digitalWrite( 9, LOW );
        break;  
    case 0xF61C56://это 0xFDD54E
         mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    digitalWrite( 13, HIGH );//поворот по таймеру
    digitalWrite( 12, LOW );
    analogWrite( 6, 100 );
        delay(1500);
    while(digitalRead(8)==0)//do//поворот по датчику
    {
     
      analogWrite( 6, 100 );
      delay(50);  
      } //while(digitalRead(8)==0);//конец поворота по датчику
      analogWrite( 6, 0 );
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
        delay(1000);
          mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
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
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
    break;
    } 
        break;  
    case 0xFDD54E://это 0xFDD54E
         mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    digitalWrite( 13, HIGH );//поворот по таймеру
    digitalWrite( 12, LOW );
    analogWrite( 6, 100 );
        delay(1500);
    while(digitalRead(8)==0)//do//поворот по датчику
    {
     
      analogWrite( 6, 100 );
      delay(50);  
      } //while(digitalRead(8)==0);//конец поворота по датчику
      analogWrite( 6, 0 );
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
        delay(1000);
          mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
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
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
    break;
    } 
        break;  
    case 0xE5D9AAC7://это 0xFEE265
        mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    digitalWrite( 13, HIGH );//поворот по таймеру
    digitalWrite( 12, LOW );
    analogWrite( 6, 100 );
        delay(1500);
    while(digitalRead(8)==0)//do//поворот по датчику
    {
     
      analogWrite( 6, 100 );
      delay(50);  
      } //while(digitalRead(8)==0);//конец поворота по датчику
      analogWrite( 6, 0 );
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
        delay(1000);
          mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
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
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
    break;
    } 
        break;  
    case 0xFEE265://это 0xFEE265
        mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(6, 4);//передача сигнала на склад
    delay(10);
    digitalWrite( 13, HIGH );//поворот по таймеру
    digitalWrite( 12, LOW );
    analogWrite( 6, 100 );
        delay(1500);
       while(digitalRead(8)==0)// do//поворот по датчику
    { 
      analogWrite( 6, 100 );
      delay(50);  
      }//конец поворота по датчику
      analogWrite( 6, 0 );
        delay(10);
      while (c==1)//прием сигнала со склада
{
  
  if( mySwitch2.available() ){

        val = mySwitch2.getReceivedValue();
        if (val>=10){
        Serial.println(val);
        if ((val==10)||(val==11)||(val==12))
{
          //c=0; 
          analogWrite( 6, 0 );
        delay(1000);
          mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад. можно и 4, но не желательно
    delay(10);
    mySwitch.send(9, 5);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 5);//передача сигнала на склад
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
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
     while(digitalRead(8)==1)//do
    {
        analogWrite( 6, 140 );
        delay(50);
    }
        break;
    }  
        break;  
        
    } 
    
     
      //analogWrite( 6, 0 );//не нужно, так как дальше управляем этой скоростью все равно(хотя, не факт)
      //delay(1);
      results.value=0;
      val=0;
    irrecv.resume(); // принимаем следующую команду
  }//порт 3
  digitalWrite( 12, HIGH );//здесь будет блок движения по линии
    digitalWrite( 13, LOW );
    analogWrite( 6, 255 );
    delay(500);
}
