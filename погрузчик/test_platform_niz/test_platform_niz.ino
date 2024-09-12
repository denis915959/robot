void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(32, INPUT);
}

void go_down()//ВЕРНО
{
   while (digitalRead(32)!=1)//подъем вверх
  {
     digitalWrite(12, HIGH);
     digitalWrite(13, LOW);
     ////Serial.println(1);
     delay(50);
  }
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}


void loop() {
  // put your main code here, to run repeatedly:

  go_down();
  delay(1000);
}
