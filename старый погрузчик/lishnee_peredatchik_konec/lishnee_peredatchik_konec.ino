void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
while (f==1)
{
if( mySwitch.available() ){//прием сигнала с погрузчика

        v = mySwitch.getReceivedValue();
        
        if (v<10){
        Serial.println(val);
        
        if (v==9)
{
          f=0;
          
} 
 
}
mySwitch.resetAvailable();
}

    mySwitch2.send(10, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
    mySwitch2.send(10, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
    mySwitch2.send(10, 5);//писать с запасом на 1 бит. отправляем сигналы на склад
    delay(10);
delay(200);  

}
}
