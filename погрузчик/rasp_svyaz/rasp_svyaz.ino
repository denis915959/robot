#include <Wire.h>
int size_arr=-1;
int status = 5;
int action[100];
int counter[100];
/*for (int i=0; i<100; i++)
{
  action[i]=-1;
  counter[i]=-1;
}*/
//byte val = 0
//int SLAVE_ADDRESS=0x20;//04, задается здесь!
void setup()
{
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);   // настроить последовательный порт для вывода
  Wire.begin(0x20/*SLAVE_ADDRESS*/);         // подключиться к шине i2c (адрес для мастера не обязателен)
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 // Wire.requestFrom(SLAVE_ADDRESS, 1);     // запросить 6 байтов от ведомого устройства #2
  
  /*if(size_arr!=-1)
  {
    delay(10000);
  }*/

 // delay(100);
}
/*void receiveData()
{
  while(Wire.available())     // ведомое устройство может послать меньше, чем запрошено
  { 
    int n = Wire.read();     // принять байт как символ
    Serial.print(n);          // напечатать символ
   
  }
}*/
void receiveData(int byteCount)//byteCount нельзя удалить, так как обработчик должен принимать один параметр типа int (так написано в документации)
{
  //int tmp[4];
  char data[1];//Wire.read может записывать данные ТОЛЬКО в массив. не в элемент этого массива, а именно в массив
  int i=0;
  int arr_count=0;
  bool flag_counter=false;
  while(Wire.available()) //это и есть цикл приема данных
  {
    if(i==0)
    {
      data[0]=Wire.read();
      //size_arr=data[0];
    }
    /*else
    {
      Serial.print("   else");
      if ((arr_count<=size_arr)&&(flag_counter==false))
      {
        recv_buf[0]=Wire.read();
        action[arr_count] =recv_buf[0];
        flag_counter=true;
      }
      if ((arr_count<=size_arr)&&(flag_counter==true))
      {
        recv_buf[0]=Wire.read();
        counter[arr_count] =recv_buf[0];
        flag_counter=false;
        arr_count++;
      }
    }*/
    Serial.println(i);
    i=i+1;
  }
  Serial.println(size_arr);
  for (int i=0; i<size_arr; i++)
  {
    Serial.print("Action = ");
    Serial.println(action[i]);
    Serial.print("Counter = ");
    Serial.println(counter[i]);
    Serial.print("\n");
  }
  
}

void sendData()
{
  Wire.write(status);
}
