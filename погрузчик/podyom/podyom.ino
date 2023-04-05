
void setup() {
  // put your setup code here, to run once:
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);
pinMode(12, INPUT);
pinMode(11, INPUT);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  int n;
  i=digitalRead(12);
  n=digitalRead(11);
  Serial.println(n);
  delay(200);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);//вниз
  analogWrite(5, 255);
  //if (i==1)
//{
  //digitalWrite(7, HIGH);
  //digitalWrite(6, LOW);//
  //analogWrite(5, 255);
  //delay(5000);
  //}
 // digitalWrite(7, HIGH);
 // digitalWrite(6, LOW);//
 // analogWrite(5, 255);
  if (n==1)
  {
    digitalWrite(7, HIGH);
  digitalWrite(6, LOW);//вниз
  analogWrite(5, 255);
   delay(5000);
    }
  
  i=0;
  n=0;
  
  
  
}
