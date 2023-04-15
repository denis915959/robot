//рабочая версия!!!!

#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <Ultrasonic.h>
Ultrasonic pered(31, 30);
Ultrasonic platform(35, 34);
Ultrasonic nazad(41, 40);
Ultrasonic ultrasonic(39, 38);
int distance1;






int distance;

int N_z=120;//230
int st=120;//170
int sklad=120;//100
int rotate_v=sklad;//st
//int n=200;//скорость поворота, было 150


int t1=100;//движение к ящику
int N=80;//было 100, это на что то влияет??? скорость движения по линии, а также скорость приближения к стеллажу
int n2=140;//поворот к стеллажу, на свинцовом аккуме 150
int t_w=20;
int t_operezhenie=200;//доворот после срабатывания датчика
int go_front = 120;//200
int go_back_t=500;//450
int N_nazad_srabat=200;

//либо ПМП делать
int left_sensor_szadi_val;//20 пин
int right_sensor_szadi_val;//19 пин

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
bool color_of_line=false;//0-белый, 1 - черный

int rt_1=120;  //было 140
int lin_speed_1=40;  //ЭТАЛОН было 20, затем 40, но не очень. 25 более-менее (вроде осечек не было, но медленно). Это скорость при срабатывании датчиков линии первого уровня

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


int n=150;//скорость разворота до 1 контура, было 200

bool key;



//ЧИСТОВОЙ БЛОК ПЕРЕМЕННЫХ
const int n_rt=200;//15-200, скорость поворота для функций типа rotate
const int t=1200;//время поворота без датчика, 1500
const int tcs230_delay=1000;//1000 - достаточно даже при напряжении 11,4 вольта, (на 1500 мертвая зона 18,5 см при 11,6 вольта)
int tcs230_counter=tcs230_delay;
int line_true_trassa=50;//только для движения вдоль стеллажей!!!   (было 200, все работало)
int line_true_zahvat=200;
int tmp_nizhny_floor=2800;//для нижнего этажа, должно быть чуть выше, чем значение tmp из-за особенности регулировки концевиков
int tmp_verhny_floor=5000;//для верхнего этажа

// Пины подключения датчика цвета
int pinS0=46;
int pinS1=47;
int pinS2=48;
int pinS3=49;
int pinOut=50;








void setup() {
    //mySwitch.enableTransmit(4);
  Serial.begin(9600);


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

}






void povorot_platformy()//поворот в БОЕВОЕ положение          //ВЕРНО
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=940;//975
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
    digitalWrite(2, HIGH);//поворот
    digitalWrite(3, LOW);
    //analogWrite(12, 150);
    delay(10);
    Serial.println(analogRead(A8));
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
  int t1, t2, t3, t4, t5, t, gran=0;//5 
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
    delay(10);
    Serial.println(analogRead(A8));
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
  while (distance>6)
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
     Serial.println(1);
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
  delay(t);//конец поворота по таймеру
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





void rotate_right_180()
{
  
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, n_rt);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  analogWrite(11, n_rt);
  delay(t);//конец поворота по таймеру
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
  delay(t_operezhenie+t);//конец доворота по таймеру
  while (digitalRead(44)!=color_of_line)//поворот по датчику, было 23
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
  int lin_speed=20; // скорость внутренней стороны? 20
  int distance;
  int N_nazad_srabat=250; // НАСТРОИТЬ! было 220

  int lin_speed2=0; // скорость внутренней стороны
  int N_nazad_srabat2=200; // НАСТРОИТЬ! было 220
  
  int N=120;
distance = 100;

//Serial.println(distance);

while (distance>9)//(digitalRead(37)!=1)//было 7
{
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




  if ((right_sensor_szadi_val_2==color_of_line)/*&&(datchik==1)*/) 
  {
    /*analogWrite(11, lin_speed2);
     analogWrite(6, N_nazad_srabat2);
     delay(100); //было 500*/

     
    analogWrite(6, 0);
    analogWrite(11, 0);
     delay(150);
     while (digitalRead(43)!=color_of_line) //3
     {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, 150);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, 150);
        delay(50);
     } 
     
  }


  if ((left_sensor_szadi_val_2==color_of_line)/*&&(datchik==1)*/)
  {
   /*analogWrite(6, lin_speed2);
      analogWrite(11, N_nazad_srabat2);
     delay(100); //было 500*/
    analogWrite(6, 0);
    analogWrite(11, 0);
     delay(150);
    while (digitalRead(4)!=color_of_line)  //19
    {
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 150);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      analogWrite(11, 150);
      delay(50);
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
lin_speed=40;
}






//проверить
void go_back_2()//после захвата ящика
{
   
  int lin_speed=20; // скорость внутренней стороны? 20
  int distance;
  int N_nazad_srabat=250; // НАСТРОИТЬ! было 220

  int lin_speed2=0; // скорость внутренней стороны
  int N_nazad_srabat2=200; // НАСТРОИТЬ! было 220
  
  int N=120;
distance = 100;

//Serial.println(distance);

while (distance>20)//(digitalRead(37)!=1)//было 5
{
  Serial.println(distance);
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




  if ((right_sensor_szadi_val_2==color_of_line)/*&&(datchik==1)*/) 
  {
    /*analogWrite(11, lin_speed2);
     analogWrite(6, N_nazad_srabat2);
     delay(100); //было 500*/

     
    analogWrite(6, 0);
    analogWrite(11, 0);
     delay(150);
     while (digitalRead(43)!=color_of_line) //3
     {
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, 150);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, 150);
        delay(50);
     } 
     
  }


  if ((left_sensor_szadi_val_2==color_of_line)/*&&(datchik==1)*/)
  {
   /*analogWrite(6, lin_speed2);
      analogWrite(11, N_nazad_srabat2);
     delay(100); //было 500*/
    analogWrite(6, 0);
    analogWrite(11, 0);
     delay(150);
    while (digitalRead(4)!=color_of_line)  //19
    {
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, 150);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      analogWrite(11, 150);
      delay(50);
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
lin_speed=40;
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


  Serial.println(left_sensor_val);   //левый 3 ловит 1 всегда, а должен 0!



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
analogWrite(6, N);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N); //N

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
analogWrite(6, N);  //N
digitalWrite(10, LOW);
digitalWrite(9, HIGH);
analogWrite(11, N); //N

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
  // put your main code here, to run repeatedly:



if (digitalRead(37)==1)//было 32
{
   flag=true;
   //povorot_platformy();
}
digitalWrite(8, LOW);
digitalWrite(7, LOW);
digitalWrite(9, LOW);
digitalWrite(10, LOW);
delay(5);
int dist=0;
if (flag==true)
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
    if (/*(tcs230_counter>=tcs230_delay)&&*/(35<=freq_red)&&(100<=freq_green)&&(80<=freq_blue)&&(70>=freq_red)&&(145>=freq_green)&&(110>=freq_blue))//надо ли 145 в green? //((55<=freq_red)&&(120<=freq_green)&&(93<=freq_blue)&&(67>=freq_red)&&(140>=freq_green)&&(107>=freq_blue))//((18<=freq_red)&&(30<=freq_green)&&(25<=freq_blue)&&(25>=freq_red)&&(47>=freq_green)&&(40>=freq_blue))
    {//по идее, тут switch идет
    //red=true;
      int num=2;
      switch(num)
      {
      case 1:
        tcs230_counter=0; //это 2-я команда, отличаются функцией right/left
        //Serial.println("true");
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_left_stel();
        delay(500);
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
        //возврат платформы должен быть 
        break;
      case 2:
        tcs230_counter=0; //это 2-я команда, отличаются функцией right/left
        //Serial.println("true");
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_left_stel();
        delay(500);
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
        //возврат платформы должен быть 
        break;
      case 3:
        tcs230_counter=0; //это 2-я команда, отличаются функцией right/left
        //Serial.println("true");
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_right_stel();
        delay(500);
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
        //возврат платформы должен быть 
        break;
      case 4:
        tcs230_counter=0; //это 4-я команда, отличаются функцией right/left
        //Serial.println("true");
        analogWrite(6, 0);
        analogWrite(11, 0);
        delay(700);
        rotate_right_stel();
        delay(500);
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
        tcs230_counter=0; //это 4-я команда, отличаются функцией right/left
        //Serial.println("true");
        /*delay(500);
        go_back_1(); */
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_1();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        //возврат платформы должен быть 
        break;
      case 10: //без поворота и захват ящика со второго этажа
        tcs230_counter=0; 
        //Serial.println("true");
        /*delay(500);
        go_back_1(); */
        delay(500);
        //лента берет, если ящик уже есть на вилах
        povorot_platformy();
        delay(500);
        zahvat_from_floor_2();
        delay(500);
        go_back_2();
        delay(500);
        vozvrat_platformy();
        //возврат платформы должен быть 
        break;
      case 11: //поворот на 180 градусов и захват ящика с первого этажа
        tcs230_counter=0; 
        //Serial.println("true");
        rotate_right_180();
        delay(500);
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
        //возврат платформы должен быть 
        break;

      case 12: //поворот на 180 градусов и захват ящика со второго этажа
        tcs230_counter=0; 
        //Serial.println("true");
        rotate_right_180();
        delay(500);
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
        //возврат платформы должен быть 
        break;
      }


      rotate_left();//МЕНЯТЬ ЭТОТ ПАРАМЕТРне относится к 4-й команде
      delay(2000);
    }
    if (go_line==true)
    {
    
    //tcs230_counter+=50;
    //delay(50);
  
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
      delay(1000);
      tcs230_counter+=1000;
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
  flag=false;
  analogWrite(6, 0);
  analogWrite(11, 0);
  delay(3000);
}
//delay(3000);
}
