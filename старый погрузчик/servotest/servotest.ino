#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
Servo servo1;
void setup() {
   servo1.attach(31); // привязываем сервопривод к аналоговому выходу 11
   //pinMode(A1, INPUT);
   //Serial.begin(9600);
}

void loop() {
  //int i;
  //int t;
  int sklad=60;
  int st=175;
  int i;
   //servo1.write(170); // ставим угол поворота под 0, до 60
   //Serial.println(analogRead(A1));
   //delay(500);

   /*for (i=60; i<st; i++)
   {
    servo1.write(i);
    delay(20);
   }
   delay(3000);
   for (i=st; i<sklad; i--)
   {
    servo1.write(i);
    delay(20);
   }
   delay(3000);*/

   servo1.write(165);
    delay(2000);
    // servo1.write(100);//100
    //delay(2000);
}
