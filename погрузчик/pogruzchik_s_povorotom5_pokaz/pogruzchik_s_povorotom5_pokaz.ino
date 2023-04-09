#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>     //20 пин умер!

#include "IRremote.h"

#include <Ultrasonic.h>
#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
               
RF24 radio(48, 49);   
IRrecv irrecv(27); // указываем вывод, к которому подключен приемник
//#include <RF24.h>
decode_results results;
//int distance_szadi[2];
Ultrasonic ultrasonic(46, 47);
Ultrasonic nazad(37, 4);
Ultrasonic pered(31, 26);
Ultrasonic platform(2, 32);
int data[1];//массив приема/передачи данных из одного элемента
int distance;//если не получится с циклами, то distance2 и 1 объявить, в функции их не объявлять!!!!!
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
int tmp_nizhny_floor=2800;
int tmp=4000;
int go_front = 120;
int go_back_t=500;//450
int N_nazad_srabat=200;
int lin_speed=30;
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
//servo1.attach(53);//было 31
radio.begin();                                        // Инициируем работу nRF24L01+
radio.setChannel(7);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
radio.setDataRate     (RF24_250KBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
radio.setPALevel      (RF24_PA_MAX);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
}
void ReadData()//результат пишется в data
{
  radio.openReadingPipe(1, 0xF0F0F0F0AA); // Какой канал считывать, 40-разрядный адрес
  radio.startListening();                 // Остановить передачу и начать прием

  if ( radio.available()) //если данные пришли - это чисто считывание данных
  {
    while (radio.available())
    {
      radio.read(&data, sizeof(data));
    }
    //Serial.print("\nReceived:");
    //Serial.println(data[0]);
  }
}

void WriteData()
{
  radio.stopListening();              //Остановить прием и начать передачу
  radio.openWritingPipe(0xF0F0F0F066);//Отправить данные на этот 40-разрядный адрес, 0xF0F0F0F0AA
  radio.write(&data, sizeof(data)); 
  delay(10);//задержка не нужна, оставим 10 (было 300)
}
void rotate_left()
{
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(23)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n);
    delay(20);
  }//конец поворота по датчику
  delay(t_operezhenie);//конец доворота по таймеру
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}
void rotate_left_stel()
{
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(3)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n);
    delay(20);
  }//конец поворота по датчику
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}

void rotate_right_180()
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру

  while (digitalRead(22)!=color_of_line)//поворот по датчику
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n);
    delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  delay(t_operezhenie+t);//конец доворота по таймеру
  while (digitalRead(19)!=color_of_line)//поворот по датчику, было 23
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n);
    delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  //delay(t_operezhenie);
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
  
}

void rotate_right()
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру
//  servo1.write(rotate_v);
  while (digitalRead(22)!=color_of_line)//поворот по датчику
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n);
    delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  delay(t_operezhenie);//конец доворота по таймеру
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}

void Switch(int VAL)
{
  switch(VAL)
    {//11-влево, 12-вправо, 10 - ничего
     case 11:
        rotate_left();
        break;   
     case 12:
        rotate_right();
        break;
    } 
}

void rotate_right_stel()//поворот вправо
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру
//  servo1.write(rotate_v);
  while (digitalRead(19)!=color_of_line)//поворот по датчику
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n);
    delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}

void go_front_to_stellazh()
{
  int distance = pered.read();
  int distance2=platform.read();
  while (distance<=0)
  {
    distance = pered.read();
  }
  lin_speed=20;//возможно, убрать!
Serial.println(distance);
while ((distance>4)&&(distance2>2))//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
 right_sensor_2_val = digitalRead(24);
  left_sensor_2_val = digitalRead(25);

if (right_sensor_val==color_of_line)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_val==color_of_line)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  if ((left_sensor_2_val==color_of_line)/*&&(datchik==1)*/) 
  {
     while (digitalRead(22)!=color_of_line)
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

  if ((right_sensor_2_val==color_of_line)/*&&(datchik==1)*/)
  {
    while (digitalRead(23)!=color_of_line)
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
  Serial.println(distance);
  

  distance = pered.read();
  distance2 = platform.read();
  
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
lin_speed=40;
  }

  void rotate_left_switch()//поворот влево
{
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(22)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n);
    delay(20);
  }//конец поворота по датчику
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}

void go_back()
{
   int distance = nazad.read();
   /*while (distance<=0)
   {
     distance = nazad.read();
   }*/
   digitalWrite(9, LOW);
   digitalWrite(10, LOW);
   digitalWrite(7, LOW);
   digitalWrite(8, LOW);
   while (distance>20)// while (digitalRead(37)!=1)//движение назад,  было 22
    {  
     
      
      distance = nazad.read();
     
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      analogWrite(11, N_z);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      analogWrite(6, N_z);
     // servo1.write(rotate_v);//оставить
      delay(50);
      Serial.println(distance);
    
    }//конец движения назад по линии
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
}
void zahvat_from_floor_1()//захват с первого яруса
{
    while (digitalRead(28)!=0)//опускание платформы по концевику 
    {
      //servo1.write(rotate_v);
      digitalWrite(35, HIGH);
      digitalWrite(36, LOW);
      delay(50);
    }
    //servo1.write(rotate_v);
    digitalWrite(35, LOW);
    digitalWrite(36, LOW);
    delay(100);//конец опускания  
    go_front_to_stellazh();
    digitalWrite(36, HIGH);
    digitalWrite(35, LOW);//типа продолжаем движение немного вверх и берем ящик
    delay(tmp_nizhny_floor);
    digitalWrite(36, LOW);
    digitalWrite(35, LOW);//конец опускания
    delay(100); 
}

void zahvat_from_floor_2()//захват со второго яруса
{
 
    go_up();
    //servo1.write(rotate_v);
    digitalWrite(35, LOW);
    digitalWrite(36, LOW);
    delay(100);//конец подъема
    digitalWrite(35, HIGH);
    digitalWrite(36, LOW);
    delay(tmp);
    digitalWrite(35, LOW);
    digitalWrite(36, LOW);
    go_front_to_stellazh();
    
    while (digitalRead(29)!=0)
    {
      //servo1.write(rotate_v);
      digitalWrite(36, HIGH);
      digitalWrite(35, LOW);
      delay(50);
    }
    digitalWrite(36, LOW);
    digitalWrite(35, LOW);//типа продолжаем движение немного вверх
    delay(300);
}
void povorot_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=985;//1000
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
    //Serial.println(analogRead(A8));
    //t2=analogRead(A8);
  }
  digitalWrite(43, LOW);
  digitalWrite(42, LOW);
}

void vozvrat_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=15;//30
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
   // Serial.println(analogRead(A8));
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
  int distance = ultrasonic.read();//47 - echo (белый провод)
  while (distance<=0)
  {
    distance = ultrasonic.read();
  }
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
     while (distance<=0)
     {
       distance = ultrasonic.read();
     }
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
  vozvrat_platformy();
  delay(2000);//уменьшить до 500
}

void go_back_timer()
{
  int lin_speed=20;
distance = nazad.read();
Serial.println(distance);
while (distance>10)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  analogWrite(11, N);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, N);
//delay(100);
  right_sensor_szadi_val = digitalRead(19);
  left_sensor_szadi_val = digitalRead(3);

  if (right_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(11, lin_speed);
     analogWrite(6, N_nazad_srabat);
     delay(100); //было 500
   }
  if (left_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(6, lin_speed);
      analogWrite(11, N_nazad_srabat);
     delay(100); //было 500
    }
    distance=nazad.read();
    Serial.println(distance);
    //distance_szadi[0]=distance_szadi[1];
    //distance_szadi[1] = nazad.read();
   
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
lin_speed=40;
}


























void loop() {
  // put your main code here, to run repeatedly:
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
bool change_rotate=false;//чередуется значение при каждом повороте, в других случаях не чередуется. если два раза подряд поворот, то радио будет глючить. Если хотя бы одна другая команда между ними, то цифра в буфер меняется
//bool first_box=false;
if (digitalRead(40)==1)//было 32
{
   flag=true;
   povorot_platformy();
}
digitalWrite(8, LOW);
digitalWrite(7, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
delay(5);
if (flag==true)
{
bool first_box=false;
while (pered.read()>5)//10
{
if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    switch ( results.value ) {
    case 0xA3C8EDDB: case 0xFFA857://это 0xFFA857, повернуть влево и взять ящик с первого этажа
    rotate_v=sklad;
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
    data[0]=1;
    change_rotate=!change_rotate;
    for (int i=0; i<number_povtor; i++)
    {
      WriteData();//mySwitch.send(1, 4);//задержка внутри функции стоит
    }
    rotate_left_stel();//функция поворота
    if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
    {
       lenta_beret();
    }
    go_back_timer();// ПЕРЕМИМЕНОВАТЬ!!!!!!!!!1
    vozvrat_platformy();
    digitalWrite(35, LOW);
    digitalWrite(36, LOW);
//    servo1.write(rotate_v);
    delay(10);//сделать меньше
    //servo1.write(rotate_v);
    zahvat_from_floor_1();
    go_back();
    //servo1.write(rotate_v);
    analogWrite(11, 0);
    analogWrite(6, 0);
    delay(20);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    delay(500); 
    povorot_platformy();//раньше не было
    N=120; 
    //rotate_v=170;
   // Switch(val);
    go_line=false;
    first_box=true;
    //sklad=170;
        break; 
    
    case 0xFFE01F: case 0xF076C13B://это 0xFFE01F, повернуть влево и взять ящик со второго этажа
    rotate_v=sklad;
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
    change_rotate=!change_rotate;
    data[0]=2;
    for (int i=0; i<number_povtor; i++)
    {
      WriteData();
    }
   
    //val=data[0];
    Serial.println(val);
   
    rotate_left_stel();//функция поворота
    if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
    {
       lenta_beret();
    }
    go_back_timer();
    vozvrat_platformy();
    digitalWrite(35, LOW);//подъем
    digitalWrite(36, LOW);
    //servo1.write(rotate_v);
    delay(10);
    //servo1.write(rotate_v);
    zahvat_from_floor_2();
    go_back();
    //servo1.write(rotate_v);
    analogWrite(11, 0);
    analogWrite(6, 0);
    delay(20);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    delay(500);
   
    povorot_platformy();
    N=120; 
    //rotate_v=170;
    //Switch(val);
    //sklad=170;
    go_line=false;
    first_box=true;
        break;
   
    case 0xFFA25D: case 0xE318261B://это 0x00FFA25, повернуть вправо и взять ящик с первого этажа
    rotate_v=sklad;
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
    data[0]=3;
    change_rotate=!change_rotate;
    for (int i=0; i<number_povtor; i++)
    {
      WriteData();
    }
   
    rotate_right_stel();//функция поворота
    if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
    {
       lenta_beret();
    }
    go_back_timer();
    vozvrat_platformy();
    digitalWrite(35, LOW);//подъем
    digitalWrite(36, LOW);
    //servo1.write(rotate_v);
    delay(10);//сделать меньше
    
    zahvat_from_floor_1();
    go_back();
    //servo1.write(rotate_v);
    analogWrite(11, 0);
    analogWrite(6, 0);
    delay(20);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    delay(500);
    povorot_platformy();//раньше не было
    N=120; 
    //rotate_v=170;
    //Switch(val);
    //sklad=170;
    go_line=false;
    first_box=true;
    break;   
    
    case 0xFFA995: case 0x808B0C15://это 0xFFA995, повенуть вправо и взять ящик со 2 этажа
    rotate_v=sklad;
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
    data[0]=4;
    change_rotate=!change_rotate;
    for (int i=0; i<number_povtor; i++)
    {
      WriteData();
    }
  
    rotate_right_stel();//функция поворота
    if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
    {
       lenta_beret();
    }
    go_back_timer();
    vozvrat_platformy();
    digitalWrite(35, LOW);//подъем
    digitalWrite(36, LOW);
    //servo1.write(rotate_v);
    delay(10);
    //servo1.write(rotate_v);
    zahvat_from_floor_2();
    go_back();
    //servo1.write(rotate_v);
    analogWrite(11, 0);
    analogWrite(6, 0);
    delay(20);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    delay(500);
    povorot_platformy();
    N=120; 
    //rotate_v=170;
    //val=data[0];
   // Switch(val);
    //sklad=170;
    go_line=false;
    first_box=true;
    break;       
    
   
    
    case 0xF61C56: case 0xFDD54E://это 0xFDD54E, поворот влево
   analogWrite(6, 0);
   analogWrite(11, 0);
   delay(d);
   if (change_rotate==true)//8 после взятия ящика только
    {
      for (int i=0; i<number_povtor; i++)
      {
        data[0]=8;
        WriteData();
      }
    }
    else
    for (int i=0; i<number_povtor; i++)
    {
       data[0]=6;
       WriteData();
    }
    change_rotate=!change_rotate;
   N=100;
   //datchik=1;
   rotate_left();
   go_line=true;    
//c=1;
        break;

             
    case 0xE5D9AAC7: case 0xFEE265://это 0xFEE265, поворот вправо
    //servo1.write(rotate_v);
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(d);
    
    if (change_rotate==true)//7 после взятия ящика только
    {
      for (int i=0; i<number_povtor; i++)
      {
        data[0]=7;
        WriteData();
      }
    }
    else
      for (int i=0; i<number_povtor; i++)
      {
         data[0]=5;
         WriteData();
      }
    change_rotate=!change_rotate;
    N=100;
    //datchik=1;
    rotate_right();
    go_line=true;
    break; 
    case 0x77132F8B: case 0xEEF943://это 0xEEF943, без поворота и захват ящика с первого этажа
      analogWrite(6, 0);
      analogWrite(11, 0);
      delay(d);
      data[0]=9;
      change_rotate=!change_rotate;
      for (int i=0; i<number_povtor; i++)
      {
        WriteData();//mySwitch.send(1, 4);//задержка внутри функции стоит
      }
      
     
      //val=data[0];
      if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
      {
         lenta_beret();
      }
      //servo1.write(rotate_v);
      //datchik=0;
      go_back_timer();// ПЕРЕМИМЕНОВАТЬ!!!!!!!!!1
      vozvrat_platformy();
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
      //servo1.write(rotate_v);
      delay(10);//сделать меньше
      //servo1.write(rotate_v);
      zahvat_from_floor_1();
      go_back();
      //servo1.write(rotate_v);
      analogWrite(11, 0);
      analogWrite(6, 0);
      delay(20);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(6, 0);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      analogWrite(11, 0);
      delay(500); 
      povorot_platformy();//раньше не было
      N=120; 
      //rotate_v=170;
      // Switch(val);
      go_line=false;
      first_box=true;
      //sklad=170;
      break;
    case 0x8EDB65A1: case 0xEEF945://это 0xEEF945, без поворота и захват ящика со второго этажа
       rotate_v=sklad;
      analogWrite(6, 0);
      analogWrite(11, 0);
      delay(d);
      data[0]=10;
      change_rotate=!change_rotate;
      for (int i=0; i<number_povtor; i++)
      {
        WriteData();
      }
      
      if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
      {
         lenta_beret();
      }
      go_back_timer();
      vozvrat_platformy();
      digitalWrite(35, LOW);//подъем
      digitalWrite(36, LOW);
      //servo1.write(rotate_v);
      delay(10);
      //servo1.write(rotate_v);
      zahvat_from_floor_2();
      go_back();
      //servo1.write(rotate_v);
      analogWrite(11, 0);
      analogWrite(6, 0);
      delay(20);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(6, 0);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      analogWrite(11, 0);
      delay(500);
      povorot_platformy();
      N=120; 
      go_line=false;
      first_box=true;
      break;
   case 0x93D823C7: case 0xEEF961://это 0xEEF961, поворот на 180 градусов и захват ящика с первого этажа
      analogWrite(6, 0);
      analogWrite(11, 0);
      delay(d);
      data[0]=11;
      change_rotate=!change_rotate;
      for (int i=0; i<number_povtor; i++)
      {
        WriteData();//mySwitch.send(1, 4);//задержка внутри функции стоит
      }
      rotate_right_180();
      
      
      if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
      {
         lenta_beret();
      }
      //servo1.write(rotate_v);
      //datchik=0;
      go_back_timer();// ПЕРЕМИМЕНОВАТЬ!!!!!!!!!1
      vozvrat_platformy();
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
      //servo1.write(rotate_v);
      delay(10);//сделать меньше
      //servo1.write(rotate_v);
      zahvat_from_floor_1();
      go_back();
      //servo1.write(rotate_v);
      analogWrite(11, 0);
      analogWrite(6, 0);
      delay(20);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(6, 0);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      analogWrite(11, 0);
      delay(500);
      povorot_platformy();//раньше не было 
      N=120; 
      //rotate_v=170;
      // Switch(val);
      go_line=false;
      first_box=true;
      //sklad=170;
      break; 
   case 0xEAC25953: case 0xEEF941://это 0xEEF941, поворот на 180 градусов и захват ящика со второго этажа
      rotate_v=sklad;
      analogWrite(6, 0);
      analogWrite(11, 0);
      delay(d);
      data[0]=12;
      change_rotate=!change_rotate;
      for (int i=0; i<number_povtor; i++)
      {
        WriteData();
      }
     
   
      rotate_right_180();//функция поворота
      if (first_box==true)//Тогда сделать так, чтобы не платформа не поворачивалась назад
      {
         lenta_beret();
      }
      go_back_timer();
      vozvrat_platformy();
      digitalWrite(35, LOW);//подъем
      digitalWrite(36, LOW);
      //servo1.write(rotate_v);
      delay(10);
      //servo1.write(rotate_v);
      zahvat_from_floor_2();
      go_back();
      //servo1.write(rotate_v);
      analogWrite(11, 0);
      analogWrite(6, 0);
      delay(20);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(6, 0);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      analogWrite(11, 0);
      delay(500);
      povorot_platformy();
      N=120; 
   
      go_line=false;
      first_box=true;
      break;
                   
    } 
    val=0;   
    irrecv.resume(); // принимаем следующую команду
} 
//lin:
if (go_line==true)
{
  right_sensor_val = digitalRead(22);
  left_sensor_val = digitalRead(23);
  right_sensor_2_val = digitalRead(24);
  left_sensor_2_val = digitalRead(25);
  if (left_sensor_val==color_of_line) 
  {
    N=150;
    analogWrite(6, lin_speed);
    analogWrite(11, N);
    delay(line_true);  
  }
  N=100;
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);//желательно скорость 200 
  if (right_sensor_val==color_of_line)
  {
 
    N=150;
    analogWrite(6, N);
    analogWrite(11, lin_speed);
    delay(line_true);
  }
  N=100;
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
  if ((left_sensor_2_val==color_of_line)/*&&(datchik==1)*/) 
  {
     while (digitalRead(22)!=color_of_line)
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

  if ((right_sensor_2_val==color_of_line)/*&&(datchik==1)*/)
  {
    while (digitalRead(23)!=color_of_line)
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
}
  flag=false;
  
}
}
