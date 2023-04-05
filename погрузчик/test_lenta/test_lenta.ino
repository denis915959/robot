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

pinMode(44, OUTPUT);//лента
pinMode(45, OUTPUT);//лента

pinMode(13, OUTPUT);//лента ШИМ

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(44, HIGH);//ящик отъезжает
     digitalWrite(45, LOW);
     analogWrite(13, 255);
}
