#include <Ultrasonic.h>

Ultrasonic ultrasonic(46, 47);
int distance;
int n=150;//скорость поворота, на свинцовом аккуме 140
int t=1500;//время поворота без датчика, 1200
int N_z=230;
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
  // put your setup code here, to run once:
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
Serial.begin(9600);

}
void povorot_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=1000;//1010
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
  delay(5000);
}

void vozvrat_platformy()
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=30;//30
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
  delay(5000);
}
void lenta_beret()//возможно, сделать 2 функции (на первый и второй ящики), или просто переменная flag
{
  go_down_to_lenta();
  distance = ultrasonic.read();//47 - echo (белый провод)
  while (distance>6)
  {
     digitalWrite(36, LOW);
     digitalWrite(35, LOW);
     delay(100);
     digitalWrite(44, HIGH);//ящик отъезжает
     digitalWrite(45, LOW);
     analogWrite(13, 255);
     delay(100);//вместо это цикл while по датчику расстояния
     distance = ultrasonic.read(); 
  }
  digitalWrite(44, LOW);//ящик отъезжает
  digitalWrite(45, LOW);
}
void go_up()
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
void go_down_to_lenta()
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
void pogruzchik_kladet_box_na_lentu()//погрузчик кладет ящик на ленту
{
  //go_up();
  //delay(200);
  povorot_platformy();
  delay(500);
  //go_down_to_lenta();
  lenta_beret();
  vozvrat_platformy();
  delay(2000);//уменьшить до 500
}
void loop() {//если сверху - разворот. если снизу - ничего не трогаем
  // put your main code here, to run repeatedly:
  //pogruzchik_kladet_box_na_lentu();
povorot_platformy();
delay(3000);
vozvrat_platformy();
//Serial.println(analogRead(A8));
//delay(300);
}
