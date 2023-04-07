#define n 4
void setup() {
  // put your setup code here, to run once:
  pinMode(n, OUTPUT);
  //pinMode(n, OUTPUT);
  /*pinMode(44, OUTPUT);
    pinMode(45, OUTPUT);
    pinMode(13, OUTPUT);*/
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
  digitalWrite(n, LOW);
    delay(1500);
    digitalWrite(n, HIGH);
    delay(1500);
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
