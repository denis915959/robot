#include <Ultrasonic.h>

//У НОВОГО ПОТЕНЦИОМЕТРА ИЗМЕРЕНИЕ В ДРУГУЮ СТОРОНУ!!!!


Ultrasonic ultrasonic(39, 38);
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
Serial.begin(9600);

}
void povorot_platformy()//поворот в БОЕВОЕ положение          //ВЕРНО
{
  go_up();
  delay(200);
  int t1, t2, t3, t4, t5, t, gran=950;//975
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




void loop() {//если сверху - разворот. если снизу - ничего не трогаем
  // put your main code here, to run repeatedly:
  //pogruzchik_kladet_box_na_lentu();
  //go_up();
//povorot_platformy();
//pogruzchik_kladet_box_na_lentu();
//lenta_beret();

vozvrat_platformy();

delay(1000);
go_down_to_lenta();
delay(1000);
go_up();
povorot_platformy();


//delay(2000);
//Serial.println(analogRead(A8));
//delay(300);
}
