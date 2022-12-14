#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal.h>
const int rs =A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo servo1;
Servo servo2;

#define sensorPin1 2
#define sensorPin2 3
#define relay 5

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

int sensorState1 = 0;
int sensorState2 = 0;
int count=0;

int pos = 0;

int in1 = 8;
int in2 = 7;
int enb=6;
void setup() {
    Serial.begin(9600);
     SPI.begin(); 
     lcd.begin(16,2);
  
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode (sensorPin1,INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  servo1.attach(6);
  servo2.attach(5);
  servo1.write(0);
  servo2.write(90);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enb,OUTPUT);

  analogWrite(enb,150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
   lcd.setCursor(3,0);
  lcd.print("Metro Rail");
  delay(300);
  lcd.clear();
  lcd.print("Scan your Card");
  Serial.println("Put your card to the reader...");
  Serial.println();
  // put your setup code here, to run once:

}

void ir (){
    
int32_t period = 0.5 * 60000L;       // 1 minutes
  for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);
  //digitalWrite (relay, HIGH);
  //servo1.write(90);
  //delay(500);

  if(sensorState1 == LOW){
    count++; 
    delay(1000);
  }

  if(sensorState2 == LOW){
    count--; 
    delay(1000);
  }

    Serial.println("Visitors:   ");
    Serial.print(count);
    

   if(count<=0)
  { 
    Serial.println("No visitor");
    digitalWrite(relay, LOW); 
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("No Passanger");
      servo1.write(0);
    /*lcd.setCursor(0,1);
    lcd.print("No visitors    ");*/
  }
    else if (count>=0 && count<20){
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.print("Passanger: ");
      lcd.print(count);
      servo1.write(90);
      delay(500);
    digitalWrite(relay, HIGH);
    }
  
  else if(count>=20){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Compartment Full");
    digitalWrite(relay, LOW);
    servo1.write(0);
    delay(500);
  }
  }
}


void loop() {
 servo1.write(0);

 int32_t period = 0.5 * 60000L;       // 1 minutes
  for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
 rfid();
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
   analogWrite(enb,150);
  servo2.write(90);
ir();
}




 void rfid(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
   analogWrite(enb,150);
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "CC 00 D0 37") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    lcd.clear();
    lcd.print("Authorized Access");
    servo2.write(0);
    delay(200);
    servo2.write(90);

  }

  if (content.substring(1) == "CC 00 D0 37") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    lcd.clear();
    lcd.print("Authorized Access");
    servo2.write(0);
    delay(500);
    servo2.write(90);
  }
 
 else   {
    Serial.println(" Access denied");
    lcd.clear();
    lcd.print("Access Denied");
    servo2.write(90);
  }
  // put your main code here, to run repeatedly:
}
