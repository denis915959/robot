
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом

Servo servo1; // объявляем переменную servo типа "servo1"

void setup() {
   servo1.attach(31); // привязываем сервопривод к аналоговому выходу 11
   pinMode(44, OUTPUT);
   pinMode(45, OUTPUT);
   pinMode(13, OUTPUT);//ШИМ
   pinMode(35, OUTPUT);//драйвер
   pinMode(36, OUTPUT);//драйвер
   pinMode(5, INPUT);//драйвер
}
void vozvrat_platformy()
{
  int t1, t2, t3, t4, t5, t;
  t=1000;
  t1=analogRead(A8);
  delay(10);
  t2=analogRead(A8);
  delay(10);
  t3=analogRead(A8);
  delay(10);
  t4=analogRead(A8);
  delay(10);
  while(t>25)//(abs(t2-t1)<diap)
  {
    t5=analogRead(A8);
    t=(t1+t2+t3+t4+t5)/5;
    t1=t2;
    t2=t3;
    t3=t4;
    t4=t5;
    digitalWrite(43, HIGH);//возвращение в исходную позицию
    digitalWrite(42, LOW);
    analogWrite(12, 150);
    delay(10);
    Serial.println(analogRead(A8));
    //t2=analogRead(A8);
  }
  digitalWrite(43, LOW);
  digitalWrite(42, LOW);
}
void lenta_beret()//возможно, сделать 2 функции (на первый и второй ящики), или просто переменная flag
{
  digitalWrite(44, HIGH);//ящик отъезжает
  digitalWrite(45, LOW);
  analogWrite(13, 255);
  delay(4000);//вместо это цикл while по датчику расстояния 
  digitalWrite(44, LOW);//ящик отъезжает
  digitalWrite(45, LOW);
}
void loop() {
  //vozvrat_platformy();
  digitalWrite(36, HIGH);
     digitalWrite(35, LOW);
  delay(10000);
  /*int i;
   servo1.write(70); // положение 0
   delay(2000); // ждем 2 секунды
   servo1.write(175);//вжатые вилы
   delay(2000);*/
  /*digitalWrite(44, HIGH);//ящик отъезжает
  digitalWrite(45, LOW);
  analogWrite(13, 255);*/
}
