/*#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком
iarduino_IR_TX VS(9);
void setup() {
  // put your setup code here, to run once:
VS.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//VS.send(0xDDF900A);//
VS.send(0xEEF961);
//delay(10);
}*/





#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком
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
  struct LED led[2];
int c=1, value, f=1, val, v;//sig-номер сигнала, посылаемого светодиодом, led-номер светодиода, rotate-поворот вправо/влево
int i, n1, n2;//n1, n2 в перспективе заменить на массив

//struct LED led[4];
//struct LED led[4];

led[0].num_led=2;
led[0].num_sig=6;//6

led[1].num_led=3;
led[1].num_sig=2;

led[2].num_led=3;
led[2].num_sig=8;//если 3 или 4, то нужно 7 здесь, 7=5, 8=6; 8 и 7 только после взятия ящика, это аналог 11 и 12 по сути
//3-поворот вправа и взять ящик снизу

//led[0].rotate=10;
//led[1].rotate=12;//11
//led[2].rotate=10;

led[3].num_led=2;
led[3].num_sig=5;

led[4].num_led=0;//в дальнейшем, возможно, можно будет убрать
led[4].num_sig=0;
//led[4].rotate=10;

  digitalWrite(7, LOW);//реле                SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивает
  digitalWrite(6, HIGH);//реле
  VS.send(0xEEF943);
}
//порт 28
