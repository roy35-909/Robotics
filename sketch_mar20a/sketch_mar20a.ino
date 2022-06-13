int led = 12;
int Time = 400;
void setup() {
  
  pinMode(12,OUTPUT);

}

void loop() {
  digitalWrite(led,HIGH);
  delay(Time);
  digitalWrite(led,LOW);
  delay(Time);
}
