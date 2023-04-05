
/*void setup() {
  // код для настроек
  pinMode( 13 , OUTPUT );
  Serial.begin(9600);
}

void loop() {
  int datafromUser1=0, datafromUser2=0;
  // код для повторения в цикле
  while (Serial.available()==0);
  if(Serial.available() > 0)
  {
    datafromUser1=Serial.read();
    
    //Serial.println(datafromUser);
  }
  while (Serial.available()==0);
  if(Serial.available() > 0)
  {
    datafromUser2=Serial.read();
    
    //Serial.println(datafromUser);
  }
  if ((datafromUser1 == 1)&&(datafromUser2 == 0))//if(datafromUser == '1')
  {
    digitalWrite( 13 , HIGH );
  }
  else if ((datafromUser1 == 0)&&(datafromUser2 == 1))//else if(datafromUser == '0')
  {
    digitalWrite( 13, LOW);
  }
  
}*/







struct LED
{
  int num_led;//номер светодиода, который собственно и посылает сигнал
  int num_sig;//номер сигнала, отправляемого светодиодом (ну и принимаемого передатчиком соответственно)
  //int rotate;//отвечает за поворот или не поворот после отработки кода из главного свитч
};

void setup() {
  // код для настроек
  pinMode( 12 , OUTPUT );
  Serial.begin(9600);
}

void loop() {
  struct LED led[10];
  int size_signal;
  while (Serial.available()==0);
  if(Serial.available() > 0)
  {
    size_signal=Serial.read();
  }
  for (int i=0; i<size_signal; i++)//запись в массив
  {
    while (Serial.available()==0);
    if(Serial.available() > 0)
    {
      led[i].num_led=Serial.read();
    }
    while (Serial.available()==0);
    if(Serial.available() > 0)
    {
      led[i].num_sig=Serial.read();
    }
  }


  if ((led[0].num_led==3)&&(led[0].num_sig==6)&&(led[1].num_led==1)&&(led[1].num_sig==1)&&   (led[2].num_led==1)&&(led[2].num_sig==6)&&(led[3].num_led==2)&&(led[3].num_sig==2) &&(led[4].num_led==2)&&(led[4].num_sig==5)&&(led[5].num_led==3)&&(led[5].num_sig==5)&&(led[6].num_led==0)&&(led[6].num_sig==0))
  {
    digitalWrite(12 , HIGH);
    delay(5000);
    digitalWrite(12 , LOW);
  }






  Serial.write('1');
  delay(1000);
  
}








/*void setup() {
  // код для настроек
  pinMode( 13 , OUTPUT );
  Serial.begin(9600);
}
void loop() 
{
  //int tmp;
  int tmp;
  //while (Serial.available()==0);
  if(Serial.available() > 0)
  {
    tmp=Serial.read();
  }
  
  if (tmp==1)
  {
    digitalWrite(13 , HIGH);
    delay(5000);
    digitalWrite(13 , LOW);
    delay(50);
  }
  digitalWrite(13 , LOW);
tmp=0;
} */
