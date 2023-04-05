#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком      СВОБОДНЫ 5, 4, 3, 2, 1, 0
//#include <EEPROM.h>
//#include <RCSwitch.h>//приемник радио
//RCSwitch mySwitch = RCSwitch();
//RCSwitch mySwitch2 = RCSwitch();
//decode_results results;
iarduino_IR_TX transmitter(33);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 transmitter.send(0xFFE100);
 //delay(10);
}
