#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку  для работы с nRF24L01+
//#include <iarduino_4LED.h>                                // Подключаем библиотеку  для работы с четырёхразрядным LED индикатором
//#include <Servo.h>                                        // Подключаем библиотеку  для работы с сервоприводами
RF24           radio(10, 8);  //CE, CSN                             // Создаём объект radio   для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
//iarduino_4LED  dispLED(2,3);                              // Создаём объект dispLED для работы с функциями библиотеки iarduino_4LED, с указанием выводов дисплея ( CLK , DIO ) 
//Servo          myservo;                                   // Создаём объект myservo для работы с функциями библиотеки Servo
int            data[1];//[2];                                   // Создаём массив для приёма данных, ПРИЕМНИК
void setup(){
    delay(1000);
   // pinMode(40, INPUT);
    //myservo.attach(4);
   // pinMode(3, OUTPUT); 
    Serial.begin(9600);
    //dispLED.begin();                                      // Инициируем работу индикатора
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(7);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_250KBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH); 
 //   pinMode(3, OUTPUT);// Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
}

void ReadData()
{
  /*radio.openReadingPipe(1, 0xF0F0F0F0AA); // Какой канал считывать, 40-разрядный адрес
  radio.startListening();                 // Остановить передачу и начать прием

  if ( radio.available()) //если данные пришли - это чисто считывание данных
  {
    while (radio.available())
    {
      radio.read(&data, sizeof(data));
    }
    Serial.print("\nReceived:");
    Serial.println(data[0]);
  }*/
  radio.openReadingPipe(1, 0xF0F0F0F066); // Какой канал считывать, 40-разрядный адрес      0xF0F0F0F0AA
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
/*void WriteData()
{
  radio.stopListening();              //Остановить прием и начать передачу
  radio.openWritingPipe(0xF0F0F0F0AA);//Отправить данные на этот 40-разрядный адрес, 0xF0F0F0F066
  radio.write(&data, sizeof(data)); 
 // Serial.print("\nSent:");
  //Serial.println(data[0]);
  delay(10);//задержка не нужна, оставим 10 (было 300)
}*/
//int c=1;
void loop(){
  /*int c=1, val=0;
  if(digitalRead(40)==1)
  {
    data[0]=2;
   for (int i=0; i<10; i++)
            {
             // mySwitch.send(9, 4);//передача сигнала на склад. если что, было 7
              //delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
              
              WriteData();
              delay(100);
            }   
    while (c==1)
    {
      //if( mySwitch2.available() )
      //{
        //val = mySwitch2.getReceivedValue();
        ReadData();
        val=data[0];
        //if (val>=10)
        //{
         
          if ((val==10)||(val==11)||(val==12))//11-влево, 12-вправо, 10 - ничего
          {
            c=0;
             Serial.println(val);
            delay(2000); 
            //analogWrite( 6, 0 );//убрать
            //delay(10);//убрать
            data[0]=9;//цифра 9 - своеобразный рубильник
            for (int i=0; i<10; i++)
            {
             // mySwitch.send(9, 4);//передача сигнала на склад. если что, было 7
              //delay(10);//возможно, 10. Но возможны повторные отправки цифры 8
              
              WriteData();
              delay(100);
            }   
          }
        //}
        //mySwitch2.resetAvailable();
      //}
    }//конец приема сигнала со склада*/
    /*data[0]=0;
    ReadData();//результат пишется в data
    if (data[0]==2)
    {
      data[0]=12;
      for (int i=0; i<10; i++)
      {
        WriteData();
      }
    }*/
    /*while (c==1)
    {
      //if( mySwitch.available() )
      //{//прием сигнала с погрузчика
         // value = mySwitch.getReceivedValue();
         data[0]=0;
         ReadData();
         value=data[0];
          if ((value<10)&&(value>0))
          {
            Serial.println(value);
            if (value==2)
            {
              c=0;
             
              //delay(2000);
              break;   
            }
          }
         // mySwitch.resetAvailable();
    //}//конец приема сигнала с погрузчика
    //VS.send(0xFDD54E);//VD1.send(0xFEE265);
    //digitalWrite(3, HIGH);
    //delay(5);
   // digitalWrite(3, LOW);
    //delay(5);
    }//конец цикла*/
    ReadData();
    Serial.println(data[0]);
    
    //delay(100);
}
//}
