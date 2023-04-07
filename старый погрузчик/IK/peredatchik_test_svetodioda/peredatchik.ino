
#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком
#include <RCSwitch.h>//приемник радио
RCSwitch mySwitch = RCSwitch();
RCSwitch mySwitch2 = RCSwitch();
//decode_results results;
iarduino_IR_TX VS(9);
//iarduino_IR_TX VD1(11);//располагать только в порядке возрастания номеров пинов
 

// Объявляем объект VD, с указанием вывода к которому подключён ИК-передатчик
void setup(){
mySwitch2.enableTransmit(4);
mySwitch.enableReceive(1);//на мега ноги 2 и 3. Желательно вертикальная антенна
Serial.begin(9600);
pinMode(13, OUTPUT);//реле
pinMode(12, OUTPUT);//реле
  VS.begin();//их перестановка ни на что не влияет
  //VD1.begin();  // Инициируем работу с ИК-передатчиком
  pinMode(8, INPUT);
   pinMode(6, OUTPUT);
   pinMode(5, OUTPUT);
 //VD.send(0xFFA857);
 //VD.send(0xFFE01F);//лучше
  //VD.send(0x00FFA25D);
  //VD.send(0xFFA995);//новое
  //VD.send(0xFFE100);//новое
  //VD.send(0xFDD54E);//новое
  //VD.send(0xFEE265);//новое
  // Однократно отправляем код 0x00FFA25D, без пакетов повторов
 
}
void loop(){
int c=1, value, f=1, val, v;
int i;
digitalWrite(13, LOW);//реле
digitalWrite(12, LOW);
delay(5);
if( mySwitch.available() ){//прием сигнала с погрузчика


        value = mySwitch.getReceivedValue();
        if (value<10){
        Serial.println(value);
        
        if (value==6)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика

while (c==1)
{
  
  if( mySwitch.available() ){//прием сигнала с погрузчика

        value = mySwitch.getReceivedValue();
        if (value<10){
        Serial.println(value);
        
        if (value==6)
{
          c=0;
          break;   
}
        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика
digitalWrite(13, LOW);//реле
digitalWrite(12, HIGH);//реле
//delay(1);
VS.send(0xFDD54E);//VD1.send(0xFEE265);
//delay(1);

}//конец цикла
c=1;
digitalWrite(13, LOW);//реле
digitalWrite(12, LOW);//реле
delay(5);
f=1;
c=1;

if( mySwitch.available() ){//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ


        v = mySwitch.getReceivedValue();
        if (v<10){
        Serial.println(v);
        
        if (v==4)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика
while (c==1)
{
  
  if( mySwitch.available() ){//прием сигнала с погрузчика

        v = mySwitch.getReceivedValue();
        if (v<10){
        Serial.println(v);
        
        if (v==4)
{
          c=0;
          break;   
}
        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
//delay(5);
VS.send(0xFFA995);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
//delay(1);

}//конец цикла
c=1;
val=0;
digitalWrite(13, LOW);//реле
digitalWrite(12, LOW);//реле
delay(5);

while (f==1)
{
if( mySwitch.available() ){//прием сигнала с погрузчика

        v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        
        if (v<10){
        Serial.println(val);
        
        if (v==9)
{
          f=0;
          
} 
 
}
mySwitch.resetAvailable();
}

    mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
    mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
    mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
delay(200);  //?

}








f=1;
c=1;
//                                                      А ЭТО КОНЕЦ ПОЛНОЙ ИТЕРАЦИИ
if( mySwitch.available() ){//прием сигнала с погрузчика


        v = mySwitch.getReceivedValue();
        if (v<10){
        Serial.println(v);
        
        if (v==5)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7). если что, было 7

          c=0;         

        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика
while (c==1)
{
  
  if( mySwitch.available() ){//прием сигнала с погрузчика

        v = mySwitch.getReceivedValue();
        if (v<10){
        Serial.println(v);
        
        if (v==5)//если что, было 7
{
          c=0;
          break;   
}
        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика
digitalWrite(13, LOW);
digitalWrite(12, HIGH);
//delay(5);
VS.send(0xFEE265);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
//delay(1);

}//конец цикла
c=1;
val=0;
digitalWrite(13, LOW);//реле
digitalWrite(12, LOW);//реле
delay(5);

for(i=0; i<200; i++)
{
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
delay(1);
VS.send(0xFE100);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
delay(5);
}
//порт 28

}
