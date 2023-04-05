#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком      СВОБОДНЫ 5, 4, 3, 2, 1, 0
iarduino_IR_TX VS(9);
#include <SPI.h>     

void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);//реле
  pinMode(6, OUTPUT);//реле
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
    for (int j=0; j<20; j++)  //1 секунду ровно должно быть
    {
      VS.send(0xFFE01F);
     // delay(5);
    }
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(200);
}
