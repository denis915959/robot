#include <iarduino_IR_TX.h>
iarduino_IR_TX VS(9);
void setup() {
  // put your setup code here, to run once:
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(7, INPUT);
VS.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //VD1.send(0xFEE265);
  while (digitalRead(7)==1)
 {
  digitalWrite(12, HIGH);
digitalWrite(13, LOW);
  VS.send(0xFDD54E);
  
delay(50);
 }
 while (digitalRead(7)==0)
 {
  digitalWrite(13, HIGH);
digitalWrite(12, LOW);
  VS.send(0xFFE100);

delay(50);
 }
}
