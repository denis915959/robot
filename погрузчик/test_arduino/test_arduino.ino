#define n A8

#define type 1 //0-digitalRead, 1- analog_read, 2- digitalWrite, 3- analogWrite
void setup() {
  // put your setup code here, to run once:
  if ((type==3)||(type==2))
    pinMode(n, OUTPUT);
  else
    pinMode(n, INPUT);
  
  Serial.begin(9600);
}
void softReset()
{
  asm volatile ("jmp 0");
}
void loop() {
  // put your main code here, to run repeatedly:
  int i;
  //delay(2000);
  /*for(i=0; i<30; i++)
    {
      Serial.println(digitalRead(n));
      delay(100);
    }*/
  /*Serial.println(digitalRead(n));
  delay(200);*/
  if(type==0)
  {
    Serial.println(digitalRead(n));
    delay(200);
  }

  if(type==1)
  {
    Serial.println(analogRead(n));
    delay(200);
  }

  if(type==2)
  {
    digitalWrite(n, LOW);
    delay(1500);
    digitalWrite(n, HIGH);
    delay(1500);
  }
  if(type==3)
  {
    for(i=0; i<255; i+=5)
    {
      analogWrite(n, i);
      delay(50);
    }
    for(i=255; i>0; i-=5)
    {
      analogWrite(n, i);
      delay(50);
    }
  }
  /* softReset();//работает
    for(i=0; i<30; i++)
    {
       Serial.println(digitalRead(n));
       delay(2000);
    }*/
  /* digitalWrite(44, HIGH);//ящик отъезжает
      digitalWrite(45, LOW);
      analogWrite(13, 255);*/

}
