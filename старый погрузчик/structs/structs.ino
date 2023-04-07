#include <iarduino_IR_TX.h>
struct LED{
  int led;//номер светодиода, который собственно и посылает сигнал
  int sig;//номер сигнала, отправляемого светодиодом (ну и принимаемого передатчиком соответственно)
  int rotate;//отвечает за поворот или не поворот после отработки кода из главного свитч
};
iarduino_IR_TX VS(9);
void setup() {
  // put your setup code here, to run once:
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(11, OUTPUT);
pinMode(10, OUTPUT);
VS.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int i, j;
//struct LED led[3];
struct LED led[4];
led[0].sig=6;
led[0].rotate=12;
led[1].sig=7;
led[1].rotate=12;
led[2].sig=5;
led[2].rotate=12;
for (j=0; j<3; j++)//главный цикл, из него по сути состоит вся программа
{
switch (led[j].sig){//не убираем
  case 6:
  for (i=0; i<8; i++)
  {
  VS.send(0xFDD54E);//VD1.send(0xFEE265);
  delay(20);
  }
  delay(1500);
  break;
  case 7:
  for (i=0; i<8; i++)
  {
  VS.send(0xFFE100);//VD1.send(0xFEE265);
  delay(20);
  }
  delay(1500);
  break;
  default:
  for (i=0; i<8; i++)
  {
  VS.send(0xFF500);//VD1.send(0xFEE265);
  delay(20);
  }
  delay(1500);
  break;
}
}
//led.sig++;
}
