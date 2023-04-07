#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком
#include <RCSwitch.h>//приемник радио
RCSwitch mySwitch = RCSwitch();
RCSwitch mySwitch2 = RCSwitch();
//decode_results results;                                        ЧИСТО ТЕОРЕТИЧЕКИ РАБОТАЕТ, НО ПРЕДЫДУЩИЙ ВАРИАНТ ЛУЧШЕ
iarduino_IR_TX VS(9);
//#define N 4
//iarduino_IR_TX VD1(11);//располагать только в порядке возрастания номеров пинов
int c=1, value, f=1, val, v;//sig-номер сигнала, посылаемого светодиодом, led-номер светодиода, rotate-поворот вправо/влево 
struct LED
{
  int num_led;//номер светодиода, который собственно и посылает сигнал
  int num_sig;//номер сигнала, отправляемого светодиодом (ну и принимаемого передатчиком соответственно)
  int rotate;//отвечает за поворот или не поворот после отработки кода из главного свитч
};
// Объявляем объект VD, с указанием вывода к которому подключён ИК-передатчик
void setup(){
mySwitch2.enableTransmit(4);
mySwitch.enableReceive(1);//на мега ноги 2 и 3. Желательно вертикальная антенна
Serial.begin(9600);
pinMode(13, OUTPUT);//реле
pinMode(12, OUTPUT);//реле
  VS.begin();//их перестановка ни на что не влияет
  //VD1.begin();  // Инициируем работу с ИК-передатчиком
  pinMode(8, INPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop(){
struct LED led[4];
int c=1, value, f=1, val, v;//sig-номер сигнала, посылаемого светодиодом, led-номер светодиода, rotate-поворот вправо/влево
int i, n1, n2;//n1, n2 в перспективе заменить на массив
//struct LED led[4];
//struct LED led[4];
led[0].num_led=2;
led[1].num_led=3;
led[2].num_led=2;
led[0].num_sig=6;//6
led[1].num_sig=2;//3
led[2].num_sig=5;//
led[0].rotate=10;//10
led[1].rotate=11;//11
led[2].rotate=10;

led[3].num_led=0;//в дальнейшем, возможно, можно будет убрать
led[3].num_sig=0;
led[3].rotate=10;

for (i=0; led[i].num_led!=0; i++)//главний цикл всей программы 
{
  digitalWrite(13, LOW);//реле
  digitalWrite(12, LOW);//изначально реле выключены, т.е обнуляется их настройка (возможно, это как раз не надо)
  delay(5);
  //switch (num_sig)
  switch (led[i].num_led)
  {
    case 2:
       digitalWrite(13, LOW);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивает
       digitalWrite(12, HIGH);//реле               МОЖНО ПОКА ОСТАВИТЬ И ТАК, ВСЕ РАВНО НА РАСПБЕРРИ БУДУТ ДРУГИЕ НОМЕРА ПИНОВ 
    break;
    case 3:// на нем погрузчик берет ящик
       digitalWrite(12, LOW);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 
       digitalWrite(13, HIGH);
    break; 
  }
  f=1;
  c=1;

  if( mySwitch.available() )
    {//прием сигнала с погрузчика
        value = mySwitch.getReceivedValue();
        if (value<10){
        Serial.println(value);
        
        if (value==led[i].num_sig)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        mySwitch.resetAvailable();
    }//конец приема сигнала с погрузчика
    while (c==1)
    {
      if( mySwitch.available() )
      {//прием сигнала с погрузчика         ПОПЫТАТЬСЯ ЗАСУНУТЬ ЭТОТ IF И ЦИКЛ В ФУНКЦИЮ
          value = mySwitch.getReceivedValue();
          if (value<10)
          {
            Serial.println(value);
            if (value==led[i].num_sig)
            {
              c=0;
              break;   
            }
          }
          mySwitch.resetAvailable();
      }//конец приема сигнала с погрузчика
    //digitalWrite(13, LOW);//реле
    //digitalWrite(12, HIGH);//реле
      switch (led[i].num_sig)
      {
      case 1:
        VS.send(0xFFA857);//VD1.send(0xFEE265);
        break;
      case 2:
        VS.send(0xFFE01F);
        break;
      case 3:
        VS.send(0xFFA25D);
        break;
      case 4:
        VS.send(0xFFA995);
        break;
      case 5:
        VS.send(0xFEE265);
        break;
      case 6:
        VS.send(0xFDD54E);
        break;
      }
    }//конец цикла
  digitalWrite(13, LOW);//реле
  digitalWrite(12, LOW);//реле
  //delay(5);
  f=1;
  c=1;
  val=0;

  switch (led[i].rotate)
  {
  case 10:
    delay(1);
    break;
  case 11:
    while (f==1)
    {
      if( mySwitch.available() )
      {//прием сигнала с погрузчика
          v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        if (v<10)
        {
          Serial.println(val);
          if (v==9)//цифра 9 своебразный рубильник
          {
            f=0;         
          } 
        }
        mySwitch.resetAvailable();
      }
      mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад, !!!!!дома проверить, будет ли через поле структуры отправлять сигнал!!!!, на 4 номере сигнала отправляло цифру 12
      delay(10);
      mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(10);
      mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(10);
      delay(200);  //?
    }
  break;
  case 12:
    while (f==1)
    {
      if( mySwitch.available() )
      {//прием сигнала с погрузчика
        v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        if (v<10)
        {
          Serial.println(val);
          if (v==9)//цифра 9 своебразный рубильник
          {
            f=0;    
          } 
        }
        mySwitch.resetAvailable();
      }
      mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад, !!!!!дома проверить, будет ли через поле структуры отправлять сигнал!!!!, на 4 номере сигнала отправляло цифру 12
      delay(10);
      mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(10);
      mySwitch2.send(12, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(10);
      delay(200);  //?
    }
  break;
  }
}//КОНЕЦ ГЛАВНОГО ЦИКЛА
for(i=0; i<200; i++)
{
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(1);
  VS.send(0xFE100);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
  delay(5);
}
//порт 28
}
