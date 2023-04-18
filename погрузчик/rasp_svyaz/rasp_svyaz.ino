#include <Wire.h>
int size_arr=-1;
int status = 102; //102 - не инициализирован
int action[100];
int counter[100];
int recv_i=0; //счетчик для приема данных
int arr_count=0; //счетчик номера массива при приеме данных
bool flag_counter=false; //для приема данных
bool start=false; //если true, то массив принят до конца и можно начинать движение
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

  if(start==true)
  {//здесь switch, циклы массива и т.д
    for (int j=0; j<size_arr; j++)
    {
      Serial.print("Action = ");
      Serial.println(action[j]);
      Serial.print("Counter = ");
      Serial.println(counter[j]);
      Serial.print("\n");
    }
    status=5;
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
    status = 102; 
    recv_i=0;
  }

 // delay(100);
}

void receiveData(int byteCount) //byteCount нельзя удалить, так как обработчик должен принимать один параметр типа int (так написано в документации)
{
  int recv_buf[1]; //так как обработчик вызывается при поступлении КАЖДОГО числа, то recv_buf - "посредник" для сохранения нем принятых данных

  while(Wire.available()) //это и есть цикл приема данных
  {
    recv_buf[0]=Wire.read();  
  }
  if(recv_i==0)
    size_arr=recv_buf[0];
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
  }
  
  if(/*recv_i*/arr_count==(size_arr-1))
  {
    start=true;
  }
  //Serial.print(size_arr);
  recv_i++;
}

void sendData()
{
  Wire.write(status);
}
