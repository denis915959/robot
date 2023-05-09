#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Servo servo1;

Ultrasonic ultrasonic(41, 40);//(31, 30)
int distance;
int n=150;//скорость поворота, на свинцовом аккуме 140
int t=1500;//время поворота без датчика, 1200
int N_z=120;
int st=120;//170
int sklad=120;//100
int rotate_v=sklad;//st
int right_sensor_val;
int left_sensor_val;
int right_sensor_2_val;
int left_sensor_2_val;
int t1=100;//движение к ящику
int N=150;//основная скорость. Если что, на 120 все работает на 100% аккуме
int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
int t_w=20;
int t_operezhenie=300;//доворот после срабатывания датчика
int tmp_nizhny_floor=2500;//должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков, 6500, лучше 3800
int tmp=1400;//5000, 2500, 1400- на полный аккум
int go_front = 120;//200
int go_back_t=500;//450
void setup() {
  Serial.begin(9600);
 // pinMode(22, INPUT);//правый датчик
//pinMode(23, INPUT);//левый датчик
//pinMode(24, INPUT);//правый датчик, возможно убрать
//pinMode(25, INPUT);//возможно убрать
pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый
//pinMode(32, INPUT);//кнопка перезагрузки
//платформа
pinMode(28, INPUT);//нижний концевик
pinMode(29, INPUT);//верхний концевик
pinMode(5, INPUT);//концевик для ленты
pinMode(30, INPUT);//датчик на шасси
pinMode(33, INPUT);//датчик перед
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
pinMode(40, INPUT);//стартовая кнопка
pinMode(42, OUTPUT);//мотор разворот платформы
pinMode(43, OUTPUT);//мотор разворот платформы
pinMode(44, OUTPUT);//лента
pinMode(45, OUTPUT);//лента
pinMode(12, OUTPUT);//мотор разворот платформы ШИМ
pinMode(13, OUTPUT);//лента ШИМ
pinMode(35, OUTPUT);//драйвер платформы
pinMode(36, OUTPUT);//драйвер платформы
pinMode(A8, INPUT);//потенциометр
pinMode(5, INPUT);//концевик лента
}
void go_back_timer()
{
  int distance1 = ultrasonic.read();
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  while (distance1>22)//(digitalRead(37)!=1)//движение назад по линии//>23
    {  
      distance1 = ultrasonic.read();
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      analogWrite(11, N_z);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      analogWrite(6, N_z);
     // servo1.write(rotate_v);//оставить
      delay(50);
      Serial.println(distance1);
    }//конец движения назад по линии
     digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(7, LOW);
     digitalWrite(8, LOW);
}
void go_front_to_stellazh()
{
  distance = ultrasonic.read();
  while (/*(digitalRead(30)!=0)*//*&&(digitalRead(33)!=0)*/distance > 9)//отрубили датчик на платформе и исключили ИК датчик на шасси, если будет врезаться - сделать расстояние 8
    {
      //goto lin;
      distance = ultrasonic.read();
      servo1.write(rotate_v);
      N=80;
      right_sensor_val = digitalRead(22);
      left_sensor_val = digitalRead(23);
      right_sensor_2_val = digitalRead(24);
      left_sensor_2_val = digitalRead(25);
      if (left_sensor_val==1) 
      {
        analogWrite(6, 0); 
        analogWrite(11, 200);
        delay(t1);
      }
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, N);//желательно скорость 200 
      if (right_sensor_val==1)
      {
        analogWrite(6, 200);
        analogWrite(11, 0);
        delay(t1); 
      }
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      analogWrite(11, N);
      if (left_sensor_2_val==1) 
      {
        while (digitalRead(22)!=1)
        {
          digitalWrite(7, HIGH);
          digitalWrite(8, LOW);
          analogWrite(6, n);
          digitalWrite(9, HIGH);
          digitalWrite(10, LOW);
          analogWrite(11, n);
          delay(50);
        } 
      }
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, N);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      analogWrite(11, N);
      if (right_sensor_2_val==1) 
      {
        while (digitalRead(23)!=1)
        {
          digitalWrite(8, HIGH);
          digitalWrite(7, LOW);
          analogWrite(6, n);
          digitalWrite(10, HIGH);
          digitalWrite(9, LOW);
          analogWrite(11, n);
          delay(50);
        } 
      }
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, N);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      analogWrite(11, N);
    }
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, N);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    analogWrite(11, N);
    delay(go_front);
    rotate_v=st;
    servo1.write(rotate_v);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    delay(100);
  }
void loop() {
  // Pass INC as a parameter to get the distance in inches
 distance = ultrasonic.read();//4 - echo (белый провод)
 // go_back_timer();
  //delay(3000);
  /*go_front_to_stellazh();
  delay(2000);*/
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(300);
  
}
