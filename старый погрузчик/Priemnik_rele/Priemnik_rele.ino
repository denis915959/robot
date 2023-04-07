#include "IRremote.h"
IRrecv irrecv(2); // указываем вывод, к которому подключен приемник

decode_results results;
void setup() {
  // put your setup code here, to run once:
irrecv.enableIRIn(); // запускаем прием
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    //delay(500);
    irrecv.resume(); // принимаем следующую команду
 }
  
}
