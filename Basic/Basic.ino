#if defined(ESP32)
   // Configurable, see typical pin layout above
#include <WiFi.h>

#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
constexpr uint8_t RST_PIN = D2;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;  
#endif
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#define WIFI_SSID "DELTA"
#define WIFI_PASSWORD "asdf4321@roy"
#define API_KEY "AIzaSyDaiCS8IMgSfFt7Xfad_UezZW96HhxsP0g"
#define DATABASE_URL "nodemcu-59210-default-rtdb.firebaseio.com"
#define USER_EMAIL "roy35-909@diu.edu.bd"
#define USER_PASSWORD "123456789"


Servo servo1;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{
  
  Serial.begin(115200);
  servo1.attach(0);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read

}

void loop()
{
  
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
   array_to_string( mfrc522.uid.uidByte, 4, str); //Insert (byte array, length, char array for output)
   Serial.println(str);
   Serial.println(F("\n**End Reading**\n"));
 //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  if(Firebase.ready())
  {
    FirebaseJson content ;
    content.set("felds/temp/doublevalue","Hello world");
 
   Firebase.Firestore.createDocument(fbdo, "nodemcu-59210","","Home/path",content.raw())? Serial.println("Done firestore") : Serial.println("Dont finished yet bro...");
   }
   delay(2000);
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
