void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(33, INPUT);
}

int tmp_verhny_floor=5000;

void go_up()//ВЕРНО
{
   while (digitalRead(33)!=0)//подъем вверх
  {
     digitalWrite(13, HIGH);
     digitalWrite(12, LOW);
     ////Serial.println(1);
     delay(50);
  }
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

void zahvat_from_floor_2()//захват со второго яруса
{
 
    go_up();
    //servo1.write(rotate_v);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(100);//конец подъема
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(tmp_verhny_floor);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    //go_front_to_stellazh();
    delay(2000);
    while (digitalRead(33)!=0)
    {
      //servo1.write(rotate_v);
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      delay(50);
    }
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);//типа продолжаем движение немного вверх
    delay(300);
}



void loop() {
  // put your main code here, to run repeatedly:

  zahvat_from_floor_2();
  delay(2000);
}
