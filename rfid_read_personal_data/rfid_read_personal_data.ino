#include <SPI.h>
#include <MFRC522.h>
#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include <WiFiClient.h>
constexpr uint8_t RST_PIN = D2;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  Serial.begin(9600);   
  WiFi.begin("DELTA","asdf4321@roy");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("..");
    digitalWrite(D0,HIGH);
    digitalWrite(D1,LOW);
    delay(200);
    
    }
   Serial.println();
   Serial.println("Wifi connected");

    digitalWrite(D1,HIGH);
    digitalWrite(D0,LOW);
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------
   char str[32] = "";
   array_to_string(mfrc522.uid.uidByte, 4, str); //Insert (byte array, length, char array for output)
   Serial.println(str);
  HTTPClient http;
  WiFiClient clientt;
  http.begin(clientt,"http://192.168.11.219",8080,"getfood/22135909/111");
  int httpCode = http.GET();
  if (httpCode>0){
    String paylode = http.getString();
    Serial.println(paylode);
    }
   http.end();
 

  Serial.println(F("\n**End Reading**\n"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}




void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}
//*****************************************************************************************//
