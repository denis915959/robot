#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Ultrasonic pered(31, 26);
Ultrasonic platform(2, 32);
Servo servo1;
int distance1;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(22, INPUT);//правый датчик
pinMode(23, INPUT);//левый датчик

pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый

//pinMode(37, INPUT);//датчик на шасси
}
int t_180=3200;
const int n_rt=200;//15-200, скорость поворота для функций типа rotate
bool color_of_line=false;//0-белый, 1 - черный

void rotate_right_180()
{
  
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n_rt);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n_rt);
  delay(t_180);//конец поворота по таймеру
  /*analogWrite(6, 0);
  analogWrite(11, 0);
  delay(5000); //для подбора коэффициэнта t_180 в разных режимах */
  while (digitalRead(22)!=color_of_line)//поворот по датчику
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n_rt);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n_rt);
    delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10); 
}


void loop() {
  // put your main code here, to run repeatedly:
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
int N=100;//скорость движения по линии, а также скорость приближения к стеллажу
int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
int t_w=20;
int t_operezhenie=200;//доворот после срабатывания датчика
int tmp_nizhny_floor=2800;//должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков, 6500, лучше 3800, было 2500, потом 3000
int tmp=3900;//5000, 2500, 1400- на полный аккум было, 4100 последний
int go_front = 120;//200
int go_back_t=500;//450
int N_nazad_srabat=200;
int lin_speed=25;//было 20, затем 40, но не очень. 25 более-менее (вроде осечек не было, но медленно). Это скорость при срабатывании датчиков линии первого уровня
//либо ПМП делать
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин
int line_true=200;
int n_stell=130;
//int flag=0;
int t_start=1500;//задержка при повороте, на 100% аккуме 1200 достаточно
int t_radio=50;
int f=1, c=1, val;
int f1=0;
int n1=140; //на 100% аккуме 120
//int datchik=1;
int d=1000;//длительность остановка для отправки сигнала,1000
int number_povtor=8;//было 20
bool flag=false;
bool go_line=true;
bool radio_rotate=false;


rotate_right_180();
delay(5000);




}
