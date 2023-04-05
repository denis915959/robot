//передатчик
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
    mySwitch.enableTransmit(4);
    pinMode(7, INPUT);
   
}

void loop() {
  int b;
  int b2;
  
   Serial.println(digitalRead(7));
}
