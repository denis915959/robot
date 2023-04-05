#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>     //20 пин умер!

#include "IRremote.h"
//#include <RCSwitch.h>//радио 330 МГц
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Servo servo1;
#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
//#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
//#include <RF24.h>               
RF24 radio(48, 49);   
IRrecv irrecv(27); // указываем вывод, к которому подключен приемник
//#include <RF24.h>
decode_results results;
int distance_szadi[2];
Ultrasonic ultrasonic(46, 47);
Ultrasonic nazad(37, 4);
Ultrasonic pered(31, 26);
int data[1];//массив приема/передачи данных из одного элемента
int distance;
int n=200;//скорость поворота, было 150
int t=1500;//время поворота без датчика, 1500             ЕСЛИ ЧТО, ПРОБЛЕМА С ПОВОРОТОМ МОЖЕТ БЫТЬ ЗДЕСЬ
int N_z=120;//230
int st=120;//170
int sklad=120;//100
int rotate_v=sklad;//st
int right_sensor_val;
int left_sensor_val;
int right_sensor_2_val;
int left_sensor_2_val;
int t1=100;//движение к ящику
int N=150;//скорость движения по линии, а также скорость приближения к стеллажу
int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
int t_w=20;
int t_operezhenie=200;//доворот после срабатывания датчика
int tmp_nizhny_floor=2800;//должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков, 6500, лучше 3800, было 2500, потом 3000
int tmp=3900;//5000, 2500, 1400- на полный аккум было, 4100 последний
int go_front = 120;//200
int go_back_t=500;//450
int N_nazad_srabat=200;
int lin_speed=30;//было 20, затем 40, но не очень. Это скорость при срабатывании датчиков линии первого уровня
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин
int line_true=200;
bool color_of_line=false;//0-белый, 1 - черный
void setup() {
  // put your setup code here, to run once:
irrecv.enableIRIn(); // запускаем прием
Serial.begin(9600);
pinMode(19, INPUT);//правый датчик сзади
pinMode(3, INPUT);//левый датчик сзади
pinMode(22, INPUT);//правый датчик
pinMode(23, INPUT);//левый датчик
pinMode(24, INPUT);//правый датчик, возможно убрать
pinMode(25, INPUT);//возможно убрать
pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый
pinMode(32, INPUT);//кнопка перезагрузки(пока не работает)
//платформа
pinMode(28, INPUT);//нижний концевик
pinMode(29, INPUT);//верхний концевик
pinMode(5, INPUT);//концевик для ленты
pinMode(30, INPUT);//датчик на шасси
pinMode(33, INPUT);//датчик перед
//pinMode(37, OUTPUT);//светодиод
pinMode(35, OUTPUT);//драйвер
pinMode(36, OUTPUT);//драйвер
//pinMode(37, INPUT);//датчик на шасси для заднего хода (левый), так как уже занят ультрасоником
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
servo1.attach(53);//было 31
radio.begin();                                        // Инициируем работу nRF24L01+
radio.setChannel(5);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
}
void povorot_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=980;//1010
  t=0;
  t1=analogRead(A8);
  delay(10);
  t2=analogRead(A8);
  delay(10);
  t3=analogRead(A8);
  delay(10);
  t4=analogRead(A8);
  delay(10);
  while(t<gran)//(abs(t2-t1)<diap)
  {
    t5=analogRead(A8);
    t=(t1+t2+t3+t4+t5)/5;
    t1=t2;
    t2=t3;
    t3=t4;
    t4=t5;
    digitalWrite(42, HIGH);//поворот
    digitalWrite(43, LOW);
    analogWrite(12, 150);
    delay(10);
    Serial.println(analogRead(A8));
    //t2=analogRead(A8);
  }
  digitalWrite(43, LOW);
  digitalWrite(42, LOW);
}

void vozvrat_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=30;//25
  t=1000;
  t1=analogRead(A8);
  delay(10);
  t2=analogRead(A8);
  delay(10);
  t3=analogRead(A8);
  delay(10);
  t4=analogRead(A8);
  delay(10);
  while(t>gran)//(abs(t2-t1)<diap)
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
void go_up()//подъем платформы
{
   while (digitalRead(29)!=0)//подъем вверх
  {
     digitalWrite(36, HIGH);
     digitalWrite(35, LOW);
     delay(50);
  }
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);
}
void go_down_to_lenta()//опускание платформы на высоту ленты
{
  while (digitalRead(5)==0)
  {
    digitalWrite(35, HIGH);
    digitalWrite(36, LOW);
    delay(50);
  }
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);
}
void lenta_beret()//возможно, сделать 2 функции (на первый и второй ящики), или просто переменная flag    ПРОВЕРИТЬ ЭТУ ФУНКЦИЮ!!!!!!!!
{
  povorot_platformy();
  go_down_to_lenta();
  distance = ultrasonic.read();//47 - echo (белый провод)
  while (distance>6)
  {
     digitalWrite(36, LOW);
     digitalWrite(35, LOW);
     delay(10);
     digitalWrite(44, HIGH);//ящик отъезжает
     digitalWrite(45, LOW);
     analogWrite(13, 255);
     delay(100);//вместо это цикл while по датчику расстояния
     distance = ultrasonic.read(); 
  }
  digitalWrite(44, LOW);//ящик отъезжает
  digitalWrite(45, LOW);
  go_up();
}
void pogruzchik_kladet_box_na_lentu()//погрузчик кладет ящик на ленту
{
  povorot_platformy();
  delay(500);
  lenta_beret();
  go_up();
  vozvrat_platformy();
  delay(2000);//уменьшить до 500
}
void loop() {
  // put your main code here, to run repeatedly:
pogruzchik_kladet_box_na_lentu();
delay(5000);
}
