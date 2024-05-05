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
const int n_rt=220;//15-200, скорость поворота для функций типа rotate
const int t=1200;//время поворота без датчика, 1500
const int tcs230_delay=1000;//1000 - достаточно даже при напряжении 11,4 вольта, (на 1500 мертвая зона 18,5 см при 11,6 вольта)
int tcs230_counter=tcs230_delay;
int line_true_trassa=50;//только для движения вдоль стеллажей!!!   (было 200, все работало)
int line_true_zahvat=200;
int tmp_nizhny_floor=2800;//для нижнего этажа, должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков
int tmp_verhny_floor=5000;//для верхнего этажа, было 5000
bool red=false;
int kacheli_delay=300;  //когда возникает ситуаця, что линию видят только второй контур, возникают "качели". этот таймер преодолевает это расстояние
int delay_red=200;  //больше нельзя, т.к у задних датчиков возможна ситуация, что на линии сраьотают два одновременн (редко, но бывает). поэтому задержка небольшая должна быть
const int t_180=3200;//время поворота на 180 без датчика. 
const int t_180_old=1500;
const int N_nazad=185;

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
  Wire.begin(0x20/*SLAVE_ADDRESS*/);         // подключиться к шине i2c (адрес для мастера не обязателен)
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
}





void povorot_platformy()//поворот в БОЕВОЕ положение          //ВЕРНО
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=995;//1005
  t=0;
  t1=analogRead(A8);
  delay(10);
  t2=analogRead(A8);
  delay(10);
  t3=analogRead(A8);
  delay(10);
  t4=analogRead(A8);
  delay(10);
  while((t<gran)&&(digitalRead(51)==0))//(abs(t2-t1)<diap)
  {
    t5=analogRead(A8);
    if(t1==t2==t3==t4==t5) //этого не было раньше!
    {
      break;
    }
    t=(t1+t2+t3+t4+t5)/5;
    t1=t2;
    t2=t3;
    t3=t4;
    t4=t5;
    digitalWrite(2, HIGH);//поворот
    digitalWrite(3, LOW);
    //analogWrite(12, 150);
    //delay(10);
    //Serial.println(analogRead(A8));
    //t2=analogRead(A8);
  }
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  delay(500);
}

void vozvrat_platformy()//в положение ПОХОДНОЕ       //ВЕРНО
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=30;//5 
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
    digitalWrite(3, HIGH);//возвращение в исходную позицию
    digitalWrite(2, LOW);
    //analogWrite(12, 150);
    //delay(10);
    //Serial.println(analogRead(A8));
    //t2=analogRead(A8);
  }
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  delay(500);
}


void lenta_beret()//доработал, осталось проверить
{
  go_down_to_lenta();
  distance = ultrasonic.read();//47 - echo (белый провод)
  while (distance>4)
  {
     
     digitalWrite(4, HIGH);//ящик отъезжает
     digitalWrite(5, LOW);
     delay(100);//вместо это цикл while по датчику расстояния
     distance = ultrasonic.read(); 
  }
  digitalWrite(4, LOW);//ящик отъезжает
  digitalWrite(5, LOW);
}
void go_up()//ВЕРНО
{
   while (digitalRead(33)!=0)//подъем вверх
  {
     digitalWrite(13, HIGH);
     digitalWrite(12, LOW);
     ////Serial.println(1);
     delay(50);
  }
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}
void go_down_to_lenta() ///ВЕРНО
{
  while (digitalRead(36)==0)
  {
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(50);
  }
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}
void pogruzchik_kladet_box_na_lentu()//погрузчик кладет ящик на ленту, 
{
  go_up();
  //delay(200);
  vozvrat_platformy();
  delay(500);
  go_down_to_lenta();
  lenta_beret();
  povorot_platformy();
  delay(2000);//уменьшить до 500
}



void rotate_left()
{
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n_rt);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n_rt);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(23)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n_rt);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n_rt);
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
  analogWrite(6, n_rt);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n_rt);
  delay(t);//конец поворота по таймеру    
  while (digitalRead(43)!=color_of_line)//поворот по датчику
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, n_rt);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, n_rt);
    delay(20);
  }//конец поворота по датчику
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}






void rotate_right()
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n_rt);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n_rt);
  delay(t);//конец поворота по таймеру. где он определен??????????????????????????????????????????????????????????????
//  servo1.write(rotate_v);
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
  delay(t_operezhenie);//конец доворота по таймеру
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(10);
}



void rotate_right_stel()//поворот вправо
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n_rt);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n_rt);
  delay(t);//конец поворота по таймеру
//  servo1.write(rotate_v);
  while (digitalRead(44)!=color_of_line)//поворот по датчику
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

while (distance>13)     // было 11. Возможно, следует сделать 12
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






//проверить
void go_back_2()//после захвата ящика
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

while (distance>20)//(digitalRead(37)!=1)//было 5
{
  ////Serial.println(distance);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  analogWrite(11, N_nazad);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, N_nazad);
  
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
     while ((digitalRead(43)!=color_of_line) &&(digitalRead(42)!=color_of_line))  //$
     {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, N_nazad); //150
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, N_nazad); //150
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
    while ((digitalRead(44)!=color_of_line)&&(digitalRead(45)!=color_of_line))  // $
    {
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, N_nazad); //150
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      analogWrite(11, N_nazad); //150
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








void go_front_to_stellazh() //проверить
{
  //Serial.println("go_front_to_stellazh");
  int dist=pered.read();
  int distance2=platform.read();

  /*Serial.print("dist = ");
  Serial.println(dist);
  Serial.print("distance2 = ");
  Serial.println(distance2);*/

  
  while ((dist>4)&&(distance2>2))//dist было 3 
  {
    
  //Serial.println(dist);
  

  if (go_line==true)
  {
  //  servo1.write(rotate_v);
  int right_sensor_val = digitalRead(22);
  int left_sensor_val = digitalRead(23);
  int right_sensor_2_val = digitalRead(24);
  int left_sensor_2_val = digitalRead(25);
  int right_sensor_3_val = digitalRead(26);
  int left_sensor_3_val = digitalRead(27);
  int right_sensor_4_val = digitalRead(28);
  int left_sensor_4_val = digitalRead(29);


  ////Serial.println(left_sensor_val);   //левый 3 ловит 1 всегда, а должен 0!



  if (left_sensor_3_val==color_of_line) 
{
  trig=1;

  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
  while (digitalRead(22)!=color_of_line)
  {
    digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 }
 N=100;
 digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N_nazad);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N_nazad); //N

delay(100);
}

//Serial.println(
if (right_sensor_3_val==color_of_line)
{
  trig=0;

  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  }
 N=100;
 digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, N_nazad);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N_nazad); //N

delay(100);
}



if (left_sensor_4_val==color_of_line) 
{
  trig=1;
  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
 while (digitalRead(22)!=color_of_line)
 {
 digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 } 
}

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N


if (right_sensor_4_val==color_of_line)
{
  trig=0;
  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  } 
}

digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N





if (left_sensor_2_val==color_of_line) 
{
  trig=1;
  
  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
 while (digitalRead(22)!=color_of_line)
 {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, n);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, n);
        delay(20);
 } 
}
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N




if (right_sensor_2_val==color_of_line)
{
  trig=0;

  /*digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, 0);  //N
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, 0); //N
  delay(200);  //возможно, этот блок убрать*/
  
  while (digitalRead(23)!=color_of_line)
  {
     digitalWrite(8, HIGH);
     digitalWrite(7, LOW);
     analogWrite(6, n);
     digitalWrite(10, HIGH);
     digitalWrite(9, LOW);
     analogWrite(11, n);
     delay(20);
  } 
}
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
analogWrite(6, 0);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, 0); //N

  
  
  if (left_sensor_val==color_of_line) 
  {
    if(trig==0)
    {
      if(N_rotate<barrier)
      {
        N_rotate=N_rotate+speed_step;    //за 1секунду max мощность
      }
    }
    else
    {
      N_rotate=N_r_const;
    }
    trig=0;
    //N_rotate=150;
    analogWrite(6, lin_speed);
    analogWrite(11, N_rotate);
    delay(line_true_zahvat);  
  }
  N=100;
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);//желательно скорость 200 
  analogWrite(11, N);
 
  if (right_sensor_val==color_of_line)
  {
     if(trig==1)
    {
      if(N_rotate<barrier)
      {
        N_rotate+=speed_step;
      }
    }
    else
    {
      N_rotate=N_r_const;
    }
    trig=1;
    analogWrite(6, N_rotate);
    analogWrite(11, lin_speed);
    delay(line_true_zahvat);
  }
  N=100;
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
  
 }
 dist=pered.read();
 distance2=platform.read();
 /*Serial.print("dist = ");
  Serial.println(dist);
  Serial.print("distance2 = ");
  Serial.println(distance2);*/
}
 analogWrite(11, 0);
 analogWrite(6, 0);
 delay(10);
}





//проверить после zahvat и go_back_timer и go_back
void zahvat_from_floor_1()//захват с первого яруса
{
    while (digitalRead(32)!=0)//опускание платформы по концевику 
    {
      //servo1.write(rotate_v);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      delay(50);
    }
    //servo1.write(rotate_v);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(100);//конец опускания  
    go_front_to_stellazh();
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);//типа продолжаем движение немного вверх и берем ящик
    delay(tmp_nizhny_floor);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);//конец опускания
    delay(100); 
}


//проверить после zahvat и go_back
void zahvat_from_floor_2()//захват со второго яруса
{
 
    go_up();
    //servo1.write(rotate_v);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(100);//конец подъема
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(tmp_verhny_floor);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    go_front_to_stellazh();
    
    while (digitalRead(33)!=0)
    {
      //servo1.write(rotate_v);
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      delay(50);
    }
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);//типа продолжаем движение немного вверх
    delay(300);
}
























void loop() {
  if(start==true)//пришел массив с rpi
  {//здесь switch, циклы массива и т.д
   
    bool first_box=false;
    vozvrat_platformy(); //при отключенном питании идет блокирование
    //Serial.println("GO!");
    tcs230_counter=tcs230_delay;//попробовать убрать
    int dist=100;
    /*for (int j=0; j<size_arr; j++)
    {
      Serial.print("Action = ");
      Serial.println(action[j]);
      Serial.print("Counter = ");
      Serial.println(counter[j]);
      Serial.print("\n");
    }*/
    for (int i=0; i<size_arr; i++)
    {
      /////Serial.print("act = ");
      /////Serial.println(action[i]);
      if( status <(size_arr-1))//убрать?
      {
        ////Serial.println("barrier");
        break;
      }
      if(dist<=7)
        break;
      for(int j=0; j<counter[i]; j++)
      {
        /*Serial.print("j = ");
        Serial.println(j);*/
        /*if(status!=102)//убрать?
        {
          
          Serial.println("barrier");
          break;
        }*/
        if(dist<=7)
          break;
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        delay(5);
        while(1)
        {
          //Serial.println(pered.read());
          dist=pered.read();
          while (dist>7) 
          {

            int freq_red=0;
            int freq_green=0;
            int freq_blue=0;
            //bool red=false;
  
            digitalWrite(pinS2,0);
            digitalWrite(pinS3,0);
            // Получение частоты на выходе
            freq_red = pulseIn(pinOut, 0);
            // вывод в последовательный порт
            /*Serial.print("R= ");
            Serial.print(freq_red);*/
            // установить G фильтр
            digitalWrite(pinS2,1);
            digitalWrite(pinS3,1);
            // Получение частоты на3 выходе
            freq_green = pulseIn(pinOut, 0);
            // вывод в последовательный порт
            /*Serial.print(" G= ");
            Serial.print(freq_green);*/
            // установить B фильтр
            digitalWrite(pinS2,0);
            digitalWrite(pinS3,1);
            // Получение частоты на выходе
            freq_blue = pulseIn(pinOut, 0);
            // вывод в последовательный порт
            /*Serial.print(" B= ");
            Serial.println(freq_blue);*/
            //Serial.println(tcs230_counter);
            if ((tcs230_counter>=tcs230_delay)&&(35<=freq_red)&&(100<=freq_green)&&(80<=freq_blue)&&(70>=freq_red)&&(145>=freq_green)&&(110>=freq_blue))//надо ли 145 в green? //((55<=freq_red)&&(120<=freq_green)&&(93<=freq_blue)&&(67>=freq_red)&&(140>=freq_green)&&(107>=freq_blue))//((18<=freq_red)&&(30<=freq_green)&&(25<=freq_blue)&&(25>=freq_red)&&(47>=freq_green)&&(40>=freq_blue))
            {//по идее, тут switch идет
              red=true;
              ////Serial.println("red");
              tcs230_counter=0;
            }
            if(red==true)//здесь ли??? 
            {
              break;
            }
            if (go_line==true)
            {
              int right_sensor_val = digitalRead(22);
              int left_sensor_val = digitalRead(23);
              int right_sensor_2_val = digitalRead(24);
              int left_sensor_2_val = digitalRead(25);
              int right_sensor_3_val = digitalRead(26);
              int left_sensor_3_val = digitalRead(27);
              int right_sensor_4_val = digitalRead(28);
              int left_sensor_4_val = digitalRead(29);

              int true_count=0;
              if(right_sensor_val==color_of_line)
                true_count+=1;
              if(right_sensor_2_val==color_of_line)
                true_count+=1;
              if(right_sensor_3_val==color_of_line)
                true_count+=1;
              if(right_sensor_4_val==color_of_line)
                true_count+=1;

              if(left_sensor_val==color_of_line)
                true_count+=1;
              if(left_sensor_2_val==color_of_line)
                true_count+=1;
              if(left_sensor_3_val==color_of_line)
                true_count+=1;
              if(left_sensor_4_val==color_of_line)
                true_count+=1;

              if(true_count>=3) //на большом квадрате было 3
              {
                delay(tcs230_delay);
                tcs230_counter+=tcs230_delay;
              }

              if (left_sensor_val==color_of_line) 
              {
                N=150;
                analogWrite(6, lin_speed);
                analogWrite(11, N);
                delay(line_true_trassa);  
                tcs230_counter+=line_true_trassa;
              }
              N=90;//100
              digitalWrite(8, HIGH);
              digitalWrite(7, LOW);
              analogWrite(6, N);//желательно скорость 200 
              if (right_sensor_val==color_of_line)
              {
                N=150;
                analogWrite(6, N);
                analogWrite(11, lin_speed);
                delay(line_true_trassa);
                tcs230_counter+=line_true_trassa;
              }
              N=90;//100
              digitalWrite(10, LOW);
              digitalWrite(9, HIGH);
              analogWrite(11, N);
              if ((left_sensor_2_val==color_of_line)||(left_sensor_3_val==color_of_line)||(left_sensor_4_val==color_of_line)/*&&(datchik==1)*/) 
              {
                while (digitalRead(22)!=color_of_line)
                {
                  digitalWrite(7, HIGH);
                  digitalWrite(8, LOW);
                  analogWrite(6, n);
                  digitalWrite(9, HIGH);
                  digitalWrite(10, LOW);
                  analogWrite(11, n);
                  delay(50);//50
                  tcs230_counter+=50;
                } 
              }
              digitalWrite(8, HIGH);
              digitalWrite(7, LOW);
              analogWrite(6, N);
              digitalWrite(10, LOW);
              digitalWrite(9, HIGH);
              analogWrite(11, N);

              if ((right_sensor_2_val==color_of_line)||(right_sensor_3_val==color_of_line)||(right_sensor_4_val==color_of_line)/*&&(datchik==1)*/)
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
                  tcs230_counter+=50;
                } 
              }
              digitalWrite(8, HIGH);
              digitalWrite(7, LOW);
              analogWrite(6, N);
              digitalWrite(10, LOW);
              digitalWrite(9, HIGH);
              analogWrite(11, N);

              /*if(tcs230_counter>=tcs230_delay)//&&(f_tmp==true))
              {
                Serial.println("GO!!!");
              }  */ 
            }

            dist=pered.read();
          }
          
          if(dist<=7)//оставить
          {
            status=i;
            j_send=j; //
           //// Serial.print("ststus");
           //// Serial.println(status);
            break;
          }
          if(red==true)//здесь ли??? 
          {
            break;
          }
          analogWrite(6, 0);
          analogWrite(11, 0);
          //delay(500);
        }
        tcs230_counter=0;  //ЭТОГО НЕ БЫЛО
        red=false;
      }
      //Serial.println("end J");
      red=false;

      tcs230_counter=0; //это я убрал!!!

      if(status<(size_arr-1))//оставить
      {
        ////Serial.println("barrier");
        analogWrite(6, 0);
        analogWrite(11, 0);
        break;

      }
      switch(action[i]) //посмотреть, используется ли команда 0?
      {
      case 0: //должен ли маршрут заканчиваться нулем? вроде как нет
        if(status<(size_arr-1))
        {
          ////Serial.println("barrier");
          analogWrite(6, 0);
          analogWrite(11, 0);
          break;
        }
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(10);
        break;
      case 1:
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_left_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_1();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 2:
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_left_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_2();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 3:
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_right_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_1();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 4:
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_right_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        //delay(500);
        zahvat_from_floor_2();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 5:
        rotate_right();
        delay(500);
        break;
      case 6: //
        rotate_left();
        delay(500);
        break;
      case 9: //без поворота и захват ящика с первого этажа
        /*delay(500);
        go_back_1(); */
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        go_back_1();
        delay(250);  
        povorot_platformy();
        delay(500);
        zahvat_from_floor_1();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 10: //без поворота и захват ящика со второго этажа
        /*delay(500);
        go_back_1(); */
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        go_back_1();
        delay(250);
        povorot_platformy();
        delay(500);
        zahvat_from_floor_2();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 11: //поворот на 180 градусов и захват ящика с первого этажа
        rotate_right_180_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_1();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;

      case 12: //поворот на 180 градусов и захват ящика со второго этажа
        rotate_right_180_stel();
        delay(250);
        if (first_box==true)//лента берет, если ящик уже есть на вилах
        {
          vozvrat_platformy();
          delay(250);
          lenta_beret();
          delay(250);
          go_up();
        }
        delay(250);
        go_back_1(); 
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_2();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        first_box=true;
        //возврат платформы должен быть 
        break;
      case 18: //протестировать этот режим !!!!!!!!!!!!!!!!!!!!!
        delay(250);
        rotate_right_180();
        delay(250);
        break;

      }
      if(status<(size_arr-1))
      {
        ////Serial.println("barrier");
        break;
      }
      //rotate_left();//МЕНЯТЬ ЭТОТ ПАРАМЕТРне относится к 4-й команде
      //delay(2000);
    }
    ////Serial.println("end for");
    delay(500);
    //все счетчики приема массива должны обнулиться
    for(int j=0; j<size_arr; j++)
    {
      action[j]=-1;
      counter[j]=-1;
    }
    arr_count=0; //счетчик номера массива при приеме данных
    flag_counter=false; //для приема данных
    start=false;
    size_arr=-1;
    status = 25;
    j_send=0; 
    recv_i=0;
    ////Serial.println("end!");
  }
}




// отрицательные сообщения не принимаются!
void receiveData(int byteCount) //byteCount нельзя удалить, так как обработчик должен принимать один параметр типа int (так написано в документации)
{
  int recv_buf[1]; //так как обработчик вызывается при поступлении КАЖДОГО числа, то recv_buf - "посредник" для сохранения нем принятых данных

  while(Wire.available()) //это и есть цикл приема данных
  {
    recv_buf[0]=Wire.read();  
  }

  if(recv_i==0)
  {
    mode=recv_buf[0];  
   // Serial.print("mode   ");
   // Serial.println(mode);    
  }
  
  if((recv_i==1)&&(mode==1))
  {
    size_arr=recv_buf[0];
   // Serial.print("size_arr = ");
   // Serial.println(size_arr);
  }
  if ((recv_i>1)&&(mode==1))
  {
   // Serial.print("arr_count   ");
   // Serial.println(arr_count);
    if ((arr_count<=size_arr)&&(flag_counter==false))
    {
      //recv_buf[0]=Wire.read();
      action[arr_count] =recv_buf[0];//
      flag_counter=true;
    }
    else
    {
      //recv_buf[0]=Wire.read();
      counter[arr_count] =recv_buf[0];
      flag_counter=false;
      arr_count++;
    }
  }
  if((recv_i==1)&&(mode==2)) // новй блок
  {
    int action = int(recv_buf[0]); // если типу int не привести, то ничего не будет работать
    if(action == 2)
    {
       // Serial.println("rotate right");
        rotate_right();
        rotate_answer_flag = true;
    }
    if(action == 1)
    {
       // Serial.println("rotate left");
        rotate_left();
        rotate_answer_flag = true;
    }
    recv_i=-1; // так как дальше будет i_recv++
  }

  //если все верно раотает (проверить в т.ч при mode = 2), то удалить то, что снизу
  /*if((recv_i==0)||(recv_i==1))
  {
    if(recv_i==1)
      size_arr=recv_buf[0];
    if(recv_i==0) //режим 1 или режим 2 считываем
      mode=recv_buf[0];
  }
  else
  {
    if ((arr_count<=size_arr)&&(flag_counter==false))
    {
      //recv_buf[0]=Wire.read();
      action[arr_count] =recv_buf[0];//
      flag_counter=true;
    }
    else
    {
      //recv_buf[0]=Wire.read();
      counter[arr_count] =recv_buf[0];
      flag_counter=false;
      arr_count++;
    }
  }*/
  
  if(arr_count==size_arr) //(size_arr-1))
  {
    if(mode == 1)
    {
      //Serial.println("start!");
      start=true;
    }
    recv_i=-1; // этой строки не было до 6.05.24
    //if (mode==2), то mode2_flag =true {...............}
  }
  recv_i++;
}

void sendData()
{
  /*if(rotate_answer_flag == true)
  {
    char message=120;
    Wire.write(message);
    
    counter_answer_flag++;
    Serial.print("counter_answer_flag = ");
    Serial.println(counter_answer_flag);
    if(counter_answer_flag >= 1)
      rotate_answer_flag = false;
  }
  else{*/
   // Serial.println("send else");
    if(number_send_message_3==0) //начало пакета 
    {
      char message=255;
      Wire.write(message);
    }
    if(number_send_message_3==1)
    {
      char message=j_send;
      Wire.write(message); 
    }
    if(number_send_message_3==2) //status отправляется последним, чтобы распберри успела считать j_send
    {
      char message=status;
      Wire.write(message); 
    }
    number_send_message_3=(number_send_message_3+1)%3;
  //}

}
