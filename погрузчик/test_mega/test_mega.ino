int n1=48, n2=49;
void setup() {
  // put your setup code here, to run once:
pinMode(n2, OUTPUT);
pinMode(n1, OUTPUT);
Serial.begin(9600);
pinMode(22, INPUT);//правый датчик
pinMode(23, INPUT);//левый датчик
pinMode(24, INPUT);//правый датчик, возможно убрать
pinMode(25, INPUT);//возможно убрать
pinMode(6, OUTPUT);//правый
pinMode(7, OUTPUT);//правый минус
pinMode(8, OUTPUT);//правый плюс
pinMode(9, OUTPUT);//левый минус
pinMode(10, OUTPUT);//левый плюс
pinMode(11, OUTPUT);//левый
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println(digitalRead(33));
//delay(200);
int n=120;
  /*digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, n);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  analogWrite(11, n);*/
  digitalWrite(n1, HIGH);
  digitalWrite(n2, LOW);
  delay(1000);
  digitalWrite(n2, HIGH);
  digitalWrite(n1, LOW);
  delay(1000);
  
}
