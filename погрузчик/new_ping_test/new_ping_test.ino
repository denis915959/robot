#include <NewPing.h>
#define TRIGGER_PIN  37
#define ECHO_PIN     4
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
void setup() {
  Serial.begin(9600);
}
void loop() {
  delay(200);
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
}
