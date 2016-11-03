/*

  Manchester Transmitter example

  In this example transmitter will send 10 bytes array  per transmittion

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

#include <Manchester.h>

#define TX_PIN  4  //pin where your transmitter is connected
#define LED_PIN 13 //pin for blinking LED

uint8_t moo = 1; //last led status
//PROTOCOLO: Coloca longitud como primer byte del mensaje

uint8_t data[20] = {18, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'};
uint8_t datalength = 2; //por lo menos 2 bytes

void setup()
{
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  //  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_300);
  Serial.println(F("Manchester Transmitter"));
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
        if(velo >7) velo=7;
        if(velo<0) velo=0;
        man.setupTransmit(TX_PIN, (byte)velo);
        break;
    }
  }

//PROTOCOLO: Coloca longitud como primer byte del mensaje
  data[0] = datalength;

  man.transmitArray(datalength, data);
  
  moo = ++moo % 2;
  digitalWrite(LED_PIN, moo);

  delay(800);
  datalength++;
  if (datalength > 18) datalength = 2;
}
