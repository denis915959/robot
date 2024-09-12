


void setup() {
  // put your setup code here, to run once:

pinMode(4, OUTPUT);//лента
pinMode(5, OUTPUT);//лента


}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(4, HIGH);//ящик отъезжает
     digitalWrite(5, LOW);
     delay(100);
}
