#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком      СВОБОДНЫ 5, 4, 3, 2, 1, 0
//#include <RCSwitch.h>//приемник радио
//RCSwitch mySwitch = RCSwitch();
//RCSwitch mySwitch2 = RCSwitch();
//decode_results results;
iarduino_IR_TX VS(9);
#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку  для работы с nRF24L01+                                      // Подключаем библиотеку  для работы с сервоприводами
RF24           radio(8, 10);  //CE, CSN 
//#define N 4
//iarduino_IR_TX VD1(11);//располагать только в порядке возрастания номеров пинов
 int data[1];//массив приема/передачи данных из одного элемента
struct LED
{
  int num_led;//номер светодиода, который собственно и посылает сигнал
  int num_sig;//номер сигнала, отправляемого светодиодом (ну и принимаемого передатчиком соответственно)
  //int rotate;//отвечает за поворот или не поворот после отработки кода из главного свитч
};
int radio_p=150;
// Объявляем объект VD, с указанием вывода к которому подключён ИК-передатчик
void setup(){
//mySwitch2.enableTransmit(4);
//mySwitch.enableReceive(1);//на мега ноги 2 и 3. Желательно вертикальная антенна
Serial.begin(9600);
pinMode(7, OUTPUT);//реле
pinMode(6, OUTPUT);//реле
pinMode(2, OUTPUT);//реле
//pinMode(4, OUTPUT);//реле
  VS.begin();//их перестановка ни на что не влияет
  //VD1.begin();  // Инициируем работу с ИК-передатчиком
   //pinMode(8, INPUT);//скорее всего не нужно
  // pinMode(6, OUTPUT);
  // pinMode(5, OUTPUT);
  // pinMode(11, OUTPUT);
 //VD.send(0xFFA857);
 //VD.send(0xFFE01F);//лучше
  //VD.send(0x00FFA25D);
  //VD.send(0xFFA995);//новое
  //VD.send(0xFFE100);//новое
  //VD.send(0xFDD54E);//новое
  //VD.send(0xFEE265);//новое
  // Однократно отправляем код 0x00FFA25D, без пакетов повторов
 radio.begin();                                        // Инициируем работу nRF24L01+
 radio.setChannel(5);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
 radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
 radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
}
void ReadData()
{
  radio.openReadingPipe(1, 0xF0F0F0F0AA); // Какой канал считывать, 40-разрядный адрес
  radio.startListening();                 // Остановить передачу и начать прием
  
  if ( radio.available()) //если данные пришли - это чисто считывание данных
  {
    while (radio.available())
    {
      //radio.flush_tx();
      radio.read(&data, sizeof(data));
    }
    Serial.print("\nReceived:");
    Serial.println(data[0]);
    //delay(100);
  }
}

void WriteData()
{
  radio.stopListening();              //Остановить прием и начать передачу
  radio.openWritingPipe(0xF0F0F0F0AA);//Отправить данные на этот 40-разрядный адрес, 0xF0F0F0F066
  radio.write(&data, sizeof(data)); 
  Serial.print("\nSent:");
  Serial.println(data[0]);
  delay(10);//задержка не нужна, оставим 10 (было 300)
}
void loop(){
  struct LED led[7];//было 6
int c=1, value, f=1, val, v;//sig-номер сигнала, посылаемого светодиодом, led-номер светодиода, rotate-поворот вправо/влево
int i, n1, n2;//n1, n2 в перспективе заменить на массив

//struct LED led[4];
//struct LED led[4];

led[0].num_led=2; //ИМПОЛЬЗОВАЛОСЬ ЭТО!!!
led[0].num_sig=6;//6

led[1].num_led=4;
led[1].num_sig=4;// ПОТОМ УБРАТЬ!, было 3

led[2].num_led=4;
led[2].num_sig=7;//было 12

led[3].num_led=3;//5 - поворот вправо, 7 - дублирующий
led[3].num_sig=2;//если 3 или 4, то нужно 7 здесь, 7=5, 8=6; 8 и 7 только после взятия ящика, это аналог СТАРЫХ 11 и 12 по сути, было 8
//3-поворот вправо и взять ящик снизу

//led[0].rotate=10;
//led[1].rotate=12;//11
//led[2].rotate=10;

led[4].num_led=3;
led[4].num_sig=7;//

led[5].num_led=2;
led[5].num_sig=5;//было 5

led[6].num_led=0;//в дальнейшем, возможно, можно будет убрать
led[6].num_sig=0;
//led[4].rotate=10;

/*led[0].num_led=3;
led[0].num_sig=3;// ПОТОМ УБРАТЬ!, было 4

led[1].num_led=3;
led[1].num_sig=11;//было 12

led[2].num_led=3;//5 - поворот вправо, 7 - дублирующий
led[2].num_sig=7;//если 3 или 4, то нужно 7 здесь, 7=5, 8=6; 8 и 7 только после взятия ящика, это аналог СТАРЫХ 11 и 12 по сути
//3-поворот вправа и взять ящик снизу

//led[0].rotate=10;
//led[1].rotate=12;//11
//led[2].rotate=10;

led[3].num_led=2;
led[3].num_sig=5;

led[4].num_led=0;//в дальнейшем, возможно, можно будет убрать
led[4].num_sig=0;*/
/*led[0].num_led=3;
led[0].num_sig=1;
led[1].num_led=0;
led[1].num_sig=0;*/



for (int i=0; led[i].num_led!=0; i++)//главний цикл всей программы 
{
  digitalWrite(7, LOW);//реле
  digitalWrite(6, LOW);//изначально реле выключены, т.е обнуляется их настройка (возможно, это как раз не надо)
  digitalWrite(2, LOW);
  delay(5);
  //switch (num_sig)
  switch (led[i].num_led)
  {
    case 2:
       digitalWrite(2, HIGH);//реле новое
       digitalWrite(7, LOW);//реле все, кроме работающего светодиода, должны быть HIGH               SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивает
        
       digitalWrite(6, HIGH);//реле
    break;
    case 3:// на нем погрузчик берет ящик
       digitalWrite(2, HIGH);//реле новое
       digitalWrite(6, LOW);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 

       digitalWrite(7, HIGH);
    break; 
    case 4://новый блок 
       digitalWrite(6, HIGH);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 
       digitalWrite(7, HIGH);
       digitalWrite(2, LOW);//реле новое
    break;
  }
  f=1;
  c=1;
  
  /*if( mySwitch.available() ){//прием сигнала с погрузчика


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
  
  if( mySwitch.available() ){//прием сигнала с погрузчика

        value = mySwitch.getReceivedValue();
        if (value<10){
        Serial.println(value);
                                                                    ЕСЛИ ЧТО, ОСТАВИТЬ (ЭТО НЕМНОГО СОКРАТИТ КОД)
        if (value==led[i].num_sig)
{
          c=0;
          break;   
}
        }
        mySwitch.resetAvailable();
}//конец приема сигнала с погрузчика*/
/*digitalWrite(13, LOW);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА)
digitalWrite(12, HIGH);//реле
*/
  switch (led[i].num_sig)//          SWITCH(NUM_SIG) ДОЛЖЕН БЫТЬ НА НОМЕРЕ СИГНАЛА
  {
  case 1:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==1)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
      //  v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        if (value==1)
        {
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFFA857);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 2:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
        //v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==2)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
      //  mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        if (value==2)
        {
          c=0;
          break;   
        }
        }
       // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFFE01F);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 3:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
        //v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==3)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
   // {//прием сигнала с погрузчика
      //  value = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        if (value==3)
        {
          c=0;
          break;   
        }
        }
       // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFFA25D);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 4:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        /*ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==4)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }*/
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
        //value = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        if (value==4)
        {
          data[0]=0;
          value=0;
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFFA995);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 5:
    //if( mySwitch.available() ){//прием сигнала с погрузчика
        //v = mySwitch.getReceivedValue();
        /*ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);  
        if (value==5)
        {//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7). если что, было 7
          c=0; 
          data[0]=0;
          value=0;
          delay(1500);
        }        
        }*/
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10)
        {
          Serial.println(value);
          if (value==5)//если что, было 7
          {
            c=0;
            data[0]=0;
            value=0;
            delay(1500);
            break;   
          }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFEE265);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 6:
//VS.send(0xFDD54E);//если будет тормозить, то в этот switch закинем весь цикл приема сигнала с погрузчика
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
       // value = mySwitch.getReceivedValue();
        /*ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==6)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }*/
       // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    while (c==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
         // value = mySwitch.getReceivedValue();
         data[0]=0;
         ReadData();
         value=data[0];
          if (value<10)
          {
            Serial.println(value);
            if (value==6)
            {
              c=0;
              break;   
            }
          }
         // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFDD54E);//VD1.send(0xFEE265);
    }//конец цикла
    //c=1;//убрать
  break;

case 7:
    //if( mySwitch.available() ){//прием сигнала с погрузчика
        //v = mySwitch.getReceivedValue();
        /*ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);  
        if (value==5)
        {//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7). если что, было 7
          c=0; 
          data[0]=0;
          value=0;
          delay(1500);
        }        
        }*/
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10)
        {
          Serial.println(value);
          if (value==7)//если что, было 7
          {
            c=0;
            data[0]=0;
            value=0;
            delay(1500);
            break;   
          }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFEE265);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  
  case 8:
//VS.send(0xFDD54E);//если будет тормозить, то в этот switch закинем весь цикл приема сигнала с погрузчика
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
       // value = mySwitch.getReceivedValue();
        /*ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==6)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }*/
       // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    while (c==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
         // value = mySwitch.getReceivedValue();
         data[0]=0;
         ReadData();
         value=data[0];
          if (value<10)
          {
            Serial.println(value);
            if (value==8)
            {
              c=0;
              break;   
            }
          }
         // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xFDD54E);//VD1.send(0xFEE265);
    }//конец цикла
    //c=1;//убрать
  break;
  
   case 9:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        
        if (value==9)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
      //  v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value<10){
        Serial.println(value);
        if (value==9)
        {
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xEEF943);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 10:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>=10){
        Serial.println(value);
        
        if (value==10)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
      //  v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>=10){
        Serial.println(value);
        if (value==10)
        {
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xEEF945);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  case 11:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>10){
        Serial.println(value);
        
        if (value==11)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
      //  v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>10){
        Serial.println(value);
        if (value==11)
        {
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xEEF961);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
   case 12:
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика        ЭТО НАЧАЛО ОДНОЙ ПОЛНОЙ ИТЕРАЦИИ
       // v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>10){
        Serial.println(value);
        
        if (value==12)//возможное место проблем, в тслучае чего добавить mySwitch2.send(10, 5);if (value==7)

          c=0;         

        }
        //mySwitch.resetAvailable();
   // }//конец приема сигнала с погрузчика
    while (c==1)
    {
    //if( mySwitch.available() )
    //{//прием сигнала с погрузчика
      //  v = mySwitch.getReceivedValue();
        ReadData();
        value=data[0];
        if (value>10){
        Serial.println(value);
        if (value==12)
        {
          c=0;
          break;   
        }
        }
        //mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    VS.send(0xEEF941);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
    }//конец цикла
  break;
  }
  digitalWrite(7, LOW);//реле
  digitalWrite(6, LOW);//реле
  digitalWrite(2, LOW);
  //delay(5);
  f=1;
  c=1;
  //val=0;
  value=0;

  
  /*digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  delay(10);/////////////////////*/



/*v=0;
  switch (led[i].rotate)
  {
  case 11:*/                              //ВОЗМОЖНО, ВЕРНУТЬ!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    /*while (f==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
          //v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        ReadData();
        v=data[0];
        if (v<10)
        {
          Serial.println(v);//val
          if (v==9)//цифра 9 своебразный рубильник
          {
            f=0;         
          } 
        }
        //mySwitch.resetAvailable();
      //}
      data[0]=11;
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад, !!!!!дома проверить, будет ли через поле структуры отправлять сигнал!!!!, на 4 номере сигнала отправляло цифру 12
      delay(100);
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(100);
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      delay(100);*/
      //delay(200);  //?
      /*digitalWrite(11, HIGH);
      delay(100);
      digitalWrite(11, LOW);
      delay(10);/////////////////////*/
     /* data[0]=11;
    for (int i=0; i<radio_p; i++)
    { 
      
      WriteData();
    }
    //}
  break;
  case 12:*/                              //ВОЗМОЖНО, ВЕРНУТЬ!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    /*while (f==1)
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
    }*/
    /*while (f==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
          //v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        ReadData();
        v=data[0];
        if (v<10)
        {
          Serial.println(v);//val
          if (v==9)//цифра 9 своебразный рубильник
          {
            f=0;         
          } 
        }
        //mySwitch.resetAvailable();
      //}
      data[0]=12;
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад, !!!!!дома проверить, будет ли через поле структуры отправлять сигнал!!!!, на 4 номере сигнала отправляло цифру 12
      //delay(10);
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      //delay(10);
      WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      //delay(10);
      delay(200);  //?*/
      /*digitalWrite(11, HIGH);
      delay(100);
      digitalWrite(11, LOW);
      delay(10);/////////////////////*/
    //}
   /* data[0]=12;                                                                   //ВОЗМОЖНО, ВЕРНУТЬ!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    for (int i=0; i<radio_p; i++)
    { 
      WriteData();
      delay(50);
    }
  break;
  }*/
}//КОНЕЦ ГЛАВНОГО ЦИКЛА
for(i=0; i<200; i++)
{
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  
  digitalWrite(7, HIGH);
  delay(1);
  VS.send(0xFE100);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
  delay(5);
}
//порт 28

}
