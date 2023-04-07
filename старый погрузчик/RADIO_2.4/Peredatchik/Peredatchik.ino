/*#include <nRF24L01.h>//передатчик = погрузчик
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>*/
#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку для работы с nRF24L01+
RF24           radio(48, 49);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int            data[1];//[2];                                   // Создаём массив для приёма данных, работает только с массивом
void setup(){
  //  pinMode(40, INPUT);
    pinMode(3, OUTPUT);
    Serial.begin(9600);
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(5);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    //radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
}

void ReadData()
{
  radio.openReadingPipe(1, 0xF0F0F0F0AA); // Какой канал считывать, 40-разрядный адрес
  radio.startListening();                 // Остановить передачу и начать прием

  if ( radio.available()) //если данные пришли - это чисто считывание данных
  {
    while (radio.available())
    {
      radio.read(&data, sizeof(data));
    }
    Serial.print("\nReceived:");
    Serial.println(data[0]);
    
  }
}
int t;
void WriteData()
{
  radio.stopListening();              //Остановить прием и начать передачу
  radio.openWritingPipe(0xF0F0F0F0AA);//Отправить данные на этот 40-разрядный адрес, 0xF0F0F0F066
  radio.write(&data, sizeof(data)); //это чисто отправа данных
 Serial.print("\nSent:");
  Serial.println(data[0]);
  delay(10);//задержка не нужна, оставим 10 (было 300)
}
int f=1;
void loop(){
  /*while(1)
  {
    //t=digitalRead(40);
    if (digitalRead(40)==1)
    {
      data[0]=2; 
      for(int i=0; i<5; i++)
      {
        WriteData();//заменить send на WriteData 
        //delay(50);
      } 
      while ((data[0]!=10)&&(data[0]!=11)&&(data[0]!=12))//
      {//
           ReadData();
      }////после этого добавить свитч
    //  t=0;
    }// отправляем данные из массива data указывая сколько байт массива мы хотим отправить. Отправить данные можно с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){данные приняты приёмником;}else{данные не приняты приёмником;}
  //t=0;
  delay(50);
  }*/
   
    int v=10;
  //if(digitalRead(40)==1)
   /*while (f==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
          //v = mySwitch.getReceivedValue();//пока не придет цифра 9 отправляем цифру 12
        ReadData();
        v=data[0];
        //if ((v<10)&&(v>0))
        //{
          
          if (v==2)//цифра 9 своебразный рубильник
          {
            Serial.println(v);//val
            f=0; 
            break;        
          } 
       // }
        //mySwitch.resetAvailable();
      //}
      //mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад, !!!!!дома проверить, будет ли через поле структуры отправлять сигнал!!!!, на 4 номере сигнала отправляло цифру 12
      //delay(10);
     // WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
      //delay(100);
     // WriteData();//mySwitch2.send(11, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
     // delay(100);
      //delay(200);  //?
      /*digitalWrite(11, HIGH);
      delay(100);
      digitalWrite(11, LOW);
      delay(10);/////////////////////
    }*/
    data[0]=4;
    for (int i=0; i<30; i++)
    { 
      
      WriteData();
      delay(200);//убрать
    }
}
