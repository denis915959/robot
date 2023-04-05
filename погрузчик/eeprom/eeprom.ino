#include <EEPROM.h>
struct pin
{
  int pin_num;
  int led_num;
};
struct pin Pin;
struct pin PiN;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
 // EEPROM.get(0, Pin);   // прочитать из адреса 0
  //EEPROM.get(1, PiN);
  // меняем
  /*Pin.pin_num = 4;
  Pin.led_num = 3;
  PiN.pin_num = 5;
  PiN.led_num = 7;*/
  EEPROM[0]= 1;   // поместить в EEPROM по адресу 0
  EEPROM[1]= 2;
  EEPROM[2]= 3;
}

void loop() {
  // put your main code here, to run repeatedly:
  /*EEPROM.get(0, Pin);
  Serial.println(Pin.led_num);
  struct pin p1;
  p1.pin_num = 22;
  p1.led_num = 11;
  
  Serial.println(p2.led_num);*/
  Serial.println(EEPROM[0]);
  Serial.println(EEPROM[1]);
  Serial.println(EEPROM[2]);
  EEPROM.clear();
}
