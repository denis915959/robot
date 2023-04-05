#include "IRremote.h"
IRrecv irrecv(9); // указываем вывод, к которому подключен приемник
//case 0x77132F8B: case 0xEEF943://это 0xEEF943                      ПЕРВЫЙ
//case 0x8EDB65A1: case 0xEEF945://это 0xEEF945           ВТОРОЙ
// case 0x93D823C7: case 0xEEF961://это 0xEEF961          ТРЕТИЙ
// case 0xEAC25953: case 0xEEF941://это 0xEEF941          ЧЕТВЕРТЫЙ                   ЕСЛИ ЧТО - ПРОСТО РАЗНЕСТИ САМИ КОДЫ ПОДАЛЬШЕ (БОЛЕЕ РАЗНЫМИ СДЕЛАТЬ)
decode_results results;
void setup() {
  // put your setup code here, to run once:
irrecv.enableIRIn(); // запускаем прием
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if ( irrecv.decode( &results ))
{ // если данные пришли
    Serial.println( results.value, HEX );//0x26C10FB8; 0xDDF900A
    //delay(10);
    switch ( results.value )
    {
    case 0x93D823C7: case 0xEEF943://это 0xEEF961
   // case 0xA3C8EDDB: case 0xFFA857:
        digitalWrite( 13, HIGH );
        delay(100);
        digitalWrite( 13, LOW );
        break; 
    }
    irrecv.resume(); // принимаем следующую команду
}
}
