/*

  Manchester Receiver example

  In this example receiver will receive array of n bytes per transmittion

  try different speeds using this constants, your maximum possible speed will
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/
#include "Manchester.h"

#define RX_PIN 5
#define LED_PIN 13

uint8_t moo = 1;
#define BUFFER_SIZE 22
uint8_t buffer[BUFFER_SIZE];

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  Serial.begin(115200);
  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
  Serial.println(F("Manchester Receiver"));
}

void loop()
{
  // check if data has been sent from the computer:
  while (Serial.available()) {
    // read the most recent byte
    char cmd = Serial.read();
    switch (cmd) {
      case 'v': //velocidad
        int velo = Serial.parseInt();
        if (velo > 7) velo = 7;
        if (velo < 0) velo = 0;
        man.setupReceive(RX_PIN, (byte)velo);
        man.beginReceiveArray(BUFFER_SIZE, buffer);
        break;
    }
  }

  if (man.receiveComplete())
  {
    uint8_t receivedSize = 0;

    //do something with the data in 'buffer' here 
    //before you start receiving to the same buffer again
    
//PROTOCOLO: Coloca longitud como primer byte del mensaje    
    receivedSize = buffer[0];
    for (uint8_t i = 1; i < receivedSize; i++)
      Serial.write(buffer[i]);

    Serial.println();

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    moo = ++moo % 2;
    digitalWrite(LED_PIN, moo);
  }
}
