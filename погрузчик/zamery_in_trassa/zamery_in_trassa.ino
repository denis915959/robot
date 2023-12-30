void setup() {  // порт 5, если провод слева
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, OUTPUT);//мотор разворот платформы
  pinMode(3, OUTPUT);
}

void loop() {
  int N=83;
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, N);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  analogWrite(11, N);
}
