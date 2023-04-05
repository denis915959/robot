#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком
#include <RCSwitch.h>//приемник радио
//decode_results results;
iarduino_IR_TX VS(8);//IK

void setup() {
  // put your setup code here, to run once:
VS.begin();
pinMode(9, OUTPUT);//транзистор 1, попытаться использовать не ШИМ выход
pinMode(11, OUTPUT);//транзистор 2
pinMode(10, INPUT);//кнопка тактовая 
}

void loop() {
  // put your main code here, to run repeatedly:
while (digitalRead(10)==1)
{
  digitalWrite(9, 255);
  digitalWrite(11, 0);
  VS.send(0xFEE265);//E5D9AAC7
  delay(200);
}
{
  digitalWrite(11, 255);
  digitalWrite(9, 0);
  VS.send(FFA995);//808B0C15
  delay(200);
}
}
