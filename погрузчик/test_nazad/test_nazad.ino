#include <Wire.h>
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Ultrasonic pered(31, 30);
Ultrasonic platform(35, 34);
Ultrasonic nazad(41, 40);
Ultrasonic ultrasonic(39, 38);
int distance1;

int size_arr=-1;
int status = 25; //25 - не инициализирован, если все ок - ....
int action[25];
int counter[25];
int recv_i=0; //счетчик для приема данных
int arr_count=0; //счетчик номера массива при приеме данных
bool flag_counter=false; //для приема данных
bool start=false; //если true, то массив принят до конца и можно начинать движение
int mode=-1; //номер режима раьоты робота, 1 - к ящикам, 2 - объезд препятствия
int j_send=0; //номер итерации цикла по count, на котором препятствие возникло. нужно для нахождения номера последнего пройденного перекреска
int number_send_message_3=0;
bool rotate_answer_flag = false;
int counter_answer_flag = 0;



int distance;

int N_z=120;//230
//int st=120;//170
//int sklad=120;//100
//int rotate_v=sklad;//st
//int n=200;//скорость поворота, было 150


//int t1=100;//движение к ящику
int N=80;//было 100, это на что то влияет??? скорость движения по линии, а также скорость приближения к стеллажу
int const_dist_barrier = 4; // еси расстояние меньше, то препяьтствие на пути   //7
//int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
//int t_w=20;



//int go_front = 120;//200
//int go_back_t=500;//450
//int N_nazad_srabat=200;

//либо ПМП делать
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин

int n_stell=130;
int t_start=1500;//задержка при повороте, на 100% аккуме 1200 достаточно
int t_radio=50;
int f=1, c=1, val;
int f1=0;
int n1=140; //на 100% аккуме 120
//int datchik=1;
int d=1000;//длительность остановка для отправки сигнала,1000
int number_povtor=8;//было 20
//bool flag=false;
bool go_line=true;
bool radio_rotate=false;
bool color_of_line=false;//0-белый, 1 - черный

int rt_1=120;  //было 140
int lin_speed_1=40;  //ЭТАЛОН 50. Это скорость при срабатывании датчиков линии первого уровня

int line_true_1=5;// было 200, на 10 уже хорошо работает

int rt_2=170;//170



int lin_speed=40; //было 30
int trig=-1;
int speed_step=5;  
int barrier=220;  //было 220
int N_r_const=120; //было 120, это стартовая скорость внешних колес при повороте
int N_rotate=N_r_const; 

int trig_level2=-1;
int barrier_level2=255;
int N_rotate_level2=240; 
int lin_speed_2=30; 
int line_true_level2=50;

int line_true_2=50; //было 300


int n=200;//скорость разворота до 1 контура, было 150

bool key;



//ЧИСТОВОЙ БЛОК ПЕРЕМЕННЫХ
int t_operezhenie=0;//(200)  доворот после срабатывания датчика
const int n_rt=255;//220, скорость поворота для функций типа rotate
const int n_rt_mode_2=220;
const int t=1200;//время поворота без датчика, 1500
const int tcs230_delay=1000;//1000 - достаточно даже при напряжении 11,4 вольта, (на 1500 мертвая зона 18,5 см при 11,6 вольта)
int tcs230_counter=tcs230_delay;
int line_true_trassa=50;//только для движения вдоль стеллажей!!!   (было 200, все работало)
int line_true_zahvat=200;
int tmp_nizhny_floor=2800;//для нижнего этажа, должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков
int delay_verhny_floor=5000;//для верхнего этажа, было 5000
bool red=false;
int kacheli_delay=300;  //когда возникает ситуаця, что линию видят только второй контур, возникают "качели". этот таймер преодолевает это расстояние
int delay_red=200;  //больше нельзя, т.к у задних датчиков возможна ситуация, что на линии сраьотают два одновременн (редко, но бывает). поэтому задержка небольшая должна быть
const int t_180=3200;//время поворота на 180 без датчика. 
const int t_180_old=1500;
const int N_nazad=220; //185

// Пины подключения датчика цвета
int pinS0=46;
int pinS1=47;
int pinS2=48;
int pinS3=49;
int pinOut=50;










//int SLAVE_ADDRESS=0x20;//04, задается здесь!
void setup()
{
  pinMode(pinS0, 1);
  pinMode(pinS1, 1);
  pinMode(pinS2, 1);
  pinMode(pinS3, 1);
  pinMode(pinOut, 0);

  // масштабирование 20%
  digitalWrite(pinS0,1);
  digitalWrite(pinS1,0);

  
  pinMode(22, INPUT);//правый датчик
  pinMode(23, INPUT);//левый датчик
  pinMode(24, INPUT);//правый датчик 2, возможно убрать
  pinMode(25, INPUT);//возможно убрать
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(28, INPUT);
  pinMode(29, INPUT);

  pinMode(42, INPUT);//правый датчик сзади
  pinMode(43, INPUT);//левый датчик сзади
  pinMode(44, INPUT);
  pinMode(45, INPUT);

  pinMode(6, OUTPUT);//правый
  pinMode(7, OUTPUT);//правый минус
  pinMode(8, OUTPUT);//правый плюс
  pinMode(9, OUTPUT);//левый минус
  pinMode(10, OUTPUT);//левый плюс
  pinMode(11, OUTPUT);//левый
  pinMode(37, INPUT);//кнопка стартовая

  pinMode(32, INPUT);//верхний концевик
  pinMode(33, INPUT);//нижний концевик
  pinMode(2, OUTPUT);//мотор разворот платформы
  pinMode(3, OUTPUT);//мотор разворот платформы
  pinMode(4, OUTPUT);//лента
  pinMode(5, OUTPUT);//лента
  pinMode(12, OUTPUT);//платформа вверх/вниз
  pinMode(13, OUTPUT);//платформа вверх/вниз
  pinMode(A8, INPUT);//потенциометр
  pinMode(36, INPUT);//концевик лента
  //Serial.begin(9600);   // настроить последовательный порт для вывода
  pinMode(51, INPUT);//концевик поворот платформы
}



void rotate_right_180_stel()
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n_rt);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n_rt);
  delay(t_180_old);//конец поворота по таймеру
//  servo1.write(rotate_v);
  while (digitalRead(44)!=color_of_line)//поворот по датчику
  {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, n_rt);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    analogWrite(11, n_rt);
    //delay(20);
    // servo1.write(rotate_v);
  }//конец поворота по датчику
  delay(t_operezhenie+t_180_old);//конец доворота по таймеру
  while ((digitalRead(45)!=color_of_line)&&(digitalRead(44)!=color_of_line)&&(digitalRead(27)!=color_of_line))//поворот по датчику (передний контур - страхующий, если задние конторы пройдут мимо линии)
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
  //delay(t_operezhenie);
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10); 
}




void go_back_1() //перед захватом ящика //проверить
{
  //int lin_speed=20; // скорость внутренней стороны.было 20
  int distance;
  int N_nazad_srabat=250; // НАСТРОИТЬ! было 220

  int lin_speed2=0; // скорость внутренней стороны
  int N_nazad_srabat2=200; // НАСТРОИТЬ! было 220
  
  int N=120;
  distance = 100;
  int datchik_num=0;

//Serial.println(distance);

while (distance>9)     // было 13
{
  ////Serial.println(distance);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  analogWrite(11, N);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, N);
  
//delay(100);
  int right_sensor_szadi_val = digitalRead(44);
  int left_sensor_szadi_val = digitalRead(43);
  int right_sensor_szadi_val_2 = digitalRead(45);
  int left_sensor_szadi_val_2 = digitalRead(42);

  int true_count=0;
  if(right_sensor_szadi_val==color_of_line)
    true_count+=1;
  if(right_sensor_szadi_val_2==color_of_line)
    true_count+=1;
  if(left_sensor_szadi_val==color_of_line)
    true_count+=1;
  if(left_sensor_szadi_val_2==color_of_line)
    true_count+=1;

  if(true_count>=2) //на большом квадрате было 3
  {
    delay(delay_red);  //tcs230_delay); //int(tcs230_delay/2));
  }

  if (right_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(11, lin_speed);
     analogWrite(6, N_nazad_srabat);
     delay(100); //было 500
     datchik_num=1;
   }
  if (left_sensor_szadi_val==color_of_line)
    {
 
      analogWrite(6, lin_speed);
      analogWrite(11, N_nazad_srabat);
     delay(100); //было 500
     datchik_num=2;
    }




  if ((right_sensor_szadi_val_2==color_of_line)/*&&(datchik==1)*/) 
  {
    /*analogWrite(11, lin_speed2);
     analogWrite(6, N_nazad_srabat2);
     delay(100); //было 500*/

     
    analogWrite(6, 0);
    analogWrite(11, 0);
    delay(150);
    if(datchik_num==4)
    {
      analogWrite(6, N);
      analogWrite(11, N);
      delay(kacheli_delay);
    }
    analogWrite(6, 0);
    analogWrite(11, 0);
     while ((digitalRead(43)!=color_of_line) &&(digitalRead(42)!=color_of_line))
     {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, N_nazad);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, N_nazad);
        datchik_num=3;
        //delay(50);
     } 
     
  }


  if ((left_sensor_szadi_val_2==color_of_line))  //поменял местами направление        /*&&(datchik==1)*/)
  {
   /*analogWrite(6, lin_speed2);
      analogWrite(11, N_nazad_srabat2);
     delay(100); //было 500*/
    analogWrite(6, 0);
    analogWrite(11, 0);
     delay(150);
     if(datchik_num==3)
    {
      analogWrite(6, N);
      analogWrite(11, N);
      delay(kacheli_delay);
    }
    analogWrite(6, 0);
    analogWrite(11, 0);
    while ((digitalRead(44)!=color_of_line)&&(digitalRead(45)!=color_of_line))  //19
    {
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, N_nazad); //150
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      analogWrite(11, N_nazad);//150
      datchik_num=4;
      //delay(50);
    } 
    
  }

  

    
    distance=nazad.read();
    
    //Serial.println(distance);
    
    //distance_szadi[0]=distance_szadi[1];
    //distance_szadi[1] = nazad.read();
   
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
delay(10);
//lin_speed=40;
}






void loop() {
  // put your main code here, to run repeatedly:
  
/*if (digitalRead(40)==1)
{
 digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
  delay(50);

  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX );
    switch ( results.value ) {
    case 0xA3C8EDDB: case 0xFFA857://это 0xFFA857, повернуть влево и взять ящик с первого этажа*/


    
/*distance[0] = nazad.read();
distance[1]=distance[0];
Serial.println(distance[1]);
while (distance[1]>10)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
{
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_szadi_val = digitalRead(19);
left_sensor_szadi_val = digitalRead(3);*/


/*while (digitalRead(37)!=1)
{
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
servo1.write(sklad);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(sklad); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(sklad);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N);
}*/



/*if (right_sensor_szadi_val==0)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_szadi_val==0)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  Serial.println(distance[1]);
  distance[0]=distance[1];
  distance[1] = nazad.read();
  while (abs(distance[0]-distance[1])>=2)
  {
    distance[1]=distance[0];
   //distance[0]=distance[1];
   distance[0] = nazad.read();
  }
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);*/



/*delay(500);
distance1 = nazad.read();
Serial.println(distance1);
while (distance1>12)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10          ВОЗМОЖНО, УБРАТЬ!
{
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
//delay(100);
right_sensor_szadi_val = digitalRead(19);
left_sensor_szadi_val = digitalRead(20);

if (right_sensor_szadi_val==1)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_szadi_val==1)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  
  distance1 = nazad.read();
  Serial.println(distance1);
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);*/



/*delay(500);
distance1 = pered.read();
Serial.println(distance1);
while (distance1>4)//(digitalRead(37)!=1)//движение назад по линии// 14 было, 10
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
  left_sensor_2_val = digitalRead(25);*/


  
/*while (digitalRead(37)!=1)
{
right_sensor_val = digitalRead(22);
left_sensor_val = digitalRead(23);
right_sensor_2_val = digitalRead(24);
left_sensor_2_val = digitalRead(25);
digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(11, N);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(6, N);
servo1.write(sklad);
if (left_sensor_val==1) 
{
 analogWrite(11, 20);
 servo1.write(sklad); 
 delay(50); 
  }

digitalWrite(7, HIGH);
digitalWrite(8, LOW);
analogWrite(11, N);//желательно скорость 200 
if (right_sensor_val==1)
{
 
 analogWrite(6, 20);
 servo1.write(sklad);
 delay(50); 
  }
  digitalWrite(9, LOW);
digitalWrite(10, HIGH);
analogWrite(6, N);
}*/



/*if (right_sensor_val==1)
  {
 
    analogWrite(11, lin_speed);
    analogWrite(6, N_nazad_srabat);
    delay(100); //было 500
  }
if (left_sensor_val==1)
  {
 
    analogWrite(6, lin_speed);
    analogWrite(11, N_nazad_srabat);
    delay(100); //было 500
  }
  if ((left_sensor_2_val==1)) 
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

  if ((right_sensor_2_val==1))
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
  Serial.println(distance1);
  
  distance1 = pered.read();
}
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0);
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);
delay(5000);*/


/*break;
}
irrecv.resume(); // принимаем следующую команду

}
}*/
  go_back_1();
  delay(5000);
}
