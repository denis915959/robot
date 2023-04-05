#include <iarduino_IR_TX.h> // Подключаем библиотеку для работы с ИК-передатчиком      СВОБОДНЫ 5, 4, 3, 2, 1, 0
iarduino_IR_TX VS(9);
#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
 
struct LED
{
  int num_led;//номер светодиода, который собственно и посылает сигнал
  int num_sig;//номер сигнала, отправляемого светодиодом (ну и принимаемого передатчиком соответственно)
  //int rotate;//отвечает за поворот или не поворот после отработки кода из главного свитч
};
/*struct pin
{
  int pin_num;
  int led_num;
};*/

int radio_p=150;
// Объявляем объект VD, с указанием вывода к которому подключён ИК-передатчик
//struct pin pin_array[46];// на случай ардуино мега, тупо статический массив 

void setup()
{
Serial.begin(9600);

/*for (int j=0; j<46; j++)
{
    pin_array[j].pin_num=0;
    pin_array[j].led_num=0;
}*/
  pinMode(7, OUTPUT);//реле
  pinMode(6, OUTPUT);//реле
  pinMode(5, OUTPUT);//реле
  pinMode(4, OUTPUT);//реле
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  VS.begin();//их перестановка ни на что не влияет
}



void loop()
{
struct LED led_array[10];//с запасом

bool change_rotate=false;
int c=1, value, f=1, val, v;//sig-номер сигнала, посылаемого светодиодом, led-номер светодиода, rotate-поворот вправо/влево
int i, n1, n2;//n1, n2 в перспективе заменить на массив
int size_signal;
int light_scope=100;  // граница освещенности, НАСТРАИВАЕТСЯ
//int size_pin_array=0;
//int receive_pin_array;//флаг: принимать массив пинов или нет
/*while (Serial.available()==0);//принимает флаг
if(Serial.available() > 0)
{
  receive_pin_array=Serial.read();
}*/

//if (receive_pin_array==1)//начало блока приема и инициализации массива ПИНОВ
//{
  /*for (int i = 0; i < 46; i++) //заполнение памяти нулями (очистка)
    EEPROM[i]= 0;*/

    
  /*while (Serial.available()==0);
  if(Serial.available() > 0)//принимаем размер
  {
    size_pin_array=Serial.read();
  } */
  
  /*int tmp=size_pin_array*2;
  for(int i=0; i<tmp; i++)//считываем сам массив
  {
    while (Serial.available()==0);
    if(Serial.available() > 0)
    {
       EEPROM[i]= Serial.read();//четные номера (0, 2, 4, ....) - pin_num, нечетные - led_num  
    }
  }  */


  /*//тест
  if ((EEPROM[0]==2)&&(EEPROM[2]==3)&&(EEPROM[4]==4)&&(EEPROM[6]==5)  &&  (EEPROM[1]==1)&&(EEPROM[3]==2)&&(EEPROM[5]==3)&&(EEPROM[7]==4))
  {
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(2, HIGH);
    delay(1000);
  }//конец теста    */

  
  /*for(int i=0; i<size_pin_array; i++)//считываем сам массив. массив мы принимаем, но ничего при этом с ним не делаем!
  {
    while (Serial.available()==0);
    if(Serial.available() > 0)
    {
      pin_array[i].pin_num=Serial.read();
    }
    while (Serial.available()==0);
    if(Serial.available() > 0)
    {
      pin_array[i].led_num=Serial.read();
    }
  }*/  
  
//}//конец блока приема и инициализации массива ПИНОВ


/*int k=0;//конвертер, достает из долговременной памяти данные и записывает в pin_array
  for (int j=0; j< size_pin_array; j++)
  {
    pin_array[j].pin_num = EEPROM[k];
    k++;
    pin_array[j].led_num = EEPROM[k];
    k++;
  }//конец конвертера*/

/*for(int i=0; i<size_pin_array; i++)//
{
  pinMode(pin_array[i].pin_num, OUTPUT);
}*/

led_array[0].num_led=3; //ИМПОЛЬЗОВАЛОСЬ ЭТО!!!
led_array[0].num_sig=6;//6

led_array[1].num_led=2;
led_array[1].num_sig=2;// ПОТОМ УБРАТЬ!, было 3

led_array[2].num_led=2;
led_array[2].num_sig=12;

led_array[3].num_led=2;//5 - поворот вправо, 7 - дублирующий
led_array[3].num_sig=5;//если 3 или 4, то нужно 7 здесь, 7=5, 8=6; 8 и 7 только после взятия ящика, это аналог СТАРЫХ 11 и 12 по сути, было 8
//3-поворот вправо и взять ящик снизу
led_array[4].num_led=3;//5 - поворот вправо, 7 - дублирующий
led_array[4].num_sig=5;
//led_array[5].num_led=3;//в дальнейшем, возможно, можно будет убрать
//led_array[5].num_sig=5;

led_array[5].num_led=0;//в дальнейшем, возможно, можно будет убрать
led_array[5].num_sig=0;

for (int i=0; led_array[i].num_led!=0; i++)//главний цикл всей программы 
{
  int analog;
  switch (led_array[i].num_led)
  {
    case 1:
       
       digitalWrite(7, HIGH);//реле все, кроме работающего светодиода, должны быть HIGH               SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивае
       digitalWrite(6, HIGH);//реле
       digitalWrite(5, HIGH);//реле новое
       digitalWrite(4, LOW);//реле новое
       analog=0;
    break;
    case 2:
       digitalWrite(7, HIGH);//реле все, кроме работающего светодиода, должны быть HIGH               SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивае
       digitalWrite(6, HIGH);//реле
       digitalWrite(5, LOW);//реле новое
       digitalWrite(4, HIGH);//реле новое
       analog=1;
    break;
    case 3:// на нем погрузчик берет ящик
       digitalWrite(7, HIGH);//реле все, кроме работающего светодиода, должны быть HIGH               SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивае
       digitalWrite(6, LOW);//реле
       digitalWrite(5, HIGH);//реле новое
       digitalWrite(4, HIGH);//реле новое
       analog=2;
    break; 
    case 4://новый блок 
       digitalWrite(7, LOW);//реле все, кроме работающего светодиода, должны быть HIGH               SWITCH(NUM_LED) ДОЛЖЕН БЫТЬ НА РЕЛЕ ОБЯЗАТЕЛЬНО(ПРИВЯЗКА К НОМЕРУ СВЕТОДИОДА), 2 - это тот, на котором погрузчик сейчас влево поворачивае
       digitalWrite(6, HIGH);//реле
       digitalWrite(5, HIGH);//реле новое
       digitalWrite(4, HIGH);//реле новое
       analog=3;
    break;
  }
  
  /*int j;
  for (j=0; led_array[i].num_led!=pin_array[j].led_num; j++)
  {
    digitalWrite(pin_array[j].pin_num, HIGH);
  }
  digitalWrite(pin_array[j].pin_num, LOW);
  j++;
  for (; j<size_pin_array; j++)
  {
    digitalWrite(pin_array[j].pin_num, HIGH);
  }*/
  
  //delay(5000); //возможно, сделать 7. робот должен успеть принять сигнал и включить подсветку!
  
  f=1;
  c=1;
  int s1=analogRead(analog);
  /*int s1 = 0; //analogRead(A5);
  int s2=analogRead(A5);*/
  switch (led_array[i].num_sig)//          SWITCH(NUM_SIG) ДОЛЖЕН БЫТЬ НА НОМЕРЕ СИГНАЛА
  {

    
  case 1:
    
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xFFA857);//1-оставляем как есть(с повторами). Или учимся вообще его отключать(или, как вариант, вместо него другой ик-светодиод включать), FF500
      }
      s1=s2;
      s2=analogRead(A5);
    }*/
    
    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFFA857);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xFFA857);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 2:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xFFE01F);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/
    
    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFFE01F);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xFFE01F);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 3:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xFFA25D);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFFA25D);
        }
        break;
      }
      
      for(int j=0; j<7; j++) //чтобы считывать сигнал раз в 500 мс
      {
        VS.send(0xFFA25D);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 4:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xFFA995);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFFA995);
        }
        break;
      }
      
      for(int j=0; j<7; j++) //чтобы считывать сигнал раз в 500 мс
      {
        VS.send(0xFFA995);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 5: case 7:    //возможно раздельно их сделать!
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xFEE265);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<40; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFEE265);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xFEE265);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;



  
  case 6: case 8:
    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<40; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xFDD54E);//VD1.send(0xFEE265);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xFDD54E);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;







  
   case 9:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xEEF943);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xEEF943);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xEEF943);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 10:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xEEF945);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xEEF945);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xEEF945);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  case 11:
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
        VS.send(0xEEF961);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xEEF961);
        }
        break;
      }
      
      for(int j=0; j<7; j++)
      {
        VS.send(0xEEF961);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;


  
   case 12:;
    /*while ((s1>light_scope)||(s2>light_scope))
    {
      for (int j=0; j<20; j++)
      {
         VS.send(0xEEF941);
      }
      s1=s2;
      s2=analogRead(A5);
    }*/

    while (1)
    {
      if (s1<light_scope)
      {
        for (int j=0; j<60; j++) // обеспечивает задержку 5 секунд, одна отправка = 80 мс
        {
          VS.send(0xEEF941);
        }
        break;
      }

      for(int j=0; j<7; j++)
      {
        VS.send(0xEEF941);
      }
      
      s1=analogRead(analog);
      Serial.println(s1);
    }
  break;

  
  }
  digitalWrite(7, LOW);//реле
  digitalWrite(6, LOW);//реле
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  
  //delay(3000); //возможно, сделать 7. робот должен успеть принять сигнал и включить подсветку!
  
  change_rotate=!change_rotate;

  f=1;
  c=1;
  //val=0;
  value=0;

}//КОНЕЦ ГЛАВНОГО ЦИКЛА




change_rotate=false;
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
