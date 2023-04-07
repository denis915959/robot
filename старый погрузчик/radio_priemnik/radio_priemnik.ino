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
mySwitch.enableReceive(0);//на мега ноги 2 и 3. Желательно вертикальная антенна
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
  int value;
    if( mySwitch.available() ){
        value = mySwitch.getReceivedValue();
        Serial.println(value);

        mySwitch.resetAvailable();
    }
    
}
