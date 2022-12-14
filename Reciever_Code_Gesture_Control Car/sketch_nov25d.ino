/*
  This program receives the message from
  the transmitter and moves the car
  accordingly.

  This program is made by Shreyas for
  Electronics Champ YouTube Channel.
  Please subscribe to this channel
  Thank You
*/

//Include the libraries
#include <SPI.h>
#include <NRFLite.h>

//Initializing the variables
boolean x = 0;
int directionOfMovement = 0;
int leftMotorForward = 2;
int leftMotorBackward = 3;
int rightMotorForward = 4;
int rightMotorBackward = 5;
String message;
const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket { // Any packet up to 32 bytes can be sent.

  uint8_t FromRadioId;
  uint32_t Data;
  uint32_t FailedTxCount;

};

//Create NRF object
NRFLite _radio;
RadioPacket _radioData;

void setup() {

  Serial.begin(9600);

  //Set the pin modes
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {

    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.

  }

}

void loop() {

  while (_radio.hasData()) {

    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

    message = _radioData.Data;
    Serial.println(message);
    directionOfMovement = message.toInt();
    moveAccordingly();

  }

}

//this function moves the car according to the message
void moveAccordingly() {

  if (directionOfMovement == 1) {

    front();
    Serial.println("front");

  }

  else if (directionOfMovement == 2) {

    back();
    Serial.println("back");

  }

  else if (directionOfMovement == 3) {

    left();
    Serial.println("left");

  }

  else if (directionOfMovement == 4) {

    right();
    Serial.println("right");

  }

  else if (directionOfMovement == 0) {

    none();
    Serial.println("none");

  }

}

void front() {

  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);

}

void back() {

  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);

}

void left() {

  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);

}
void right() {

  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);

}

void none() {

  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);

}
