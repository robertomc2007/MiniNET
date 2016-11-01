// RHSLAVE
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

#define MIID 2

#define MAESTRO 10

RH_ASK driver(1200, 5, 4);

struct dataStruct {
  byte tipo;
  byte to;
  byte from;
  float dato;
  unsigned long contador;
} DatosTipo1;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);
byte tx_buf[RH_ASK_MAX_MESSAGE_LEN] = {0};
boolean dump = false;

void setup()
{
  Serial.begin(115200);   // Debugging only

  Serial.println("RH SLAVE");
  Serial.println(RH_ASK_MAX_MESSAGE_LEN);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  if (!driver.init())
  {
    Serial.println("FALLA INICIO DRIVER");
    digitalWrite(13, HIGH);
  }
}

void loop()
{
  if ( Serial.available()) {

    char ch = Serial.read();
    switch (ch) {
      //    case '0'...'9':
      //      //v = v * 10 + ch - '0';
      //      v = ch - '0';
      //      break;
      case 'd': //vuelco
        {
          dump = !dump;
          break;
        }
    }// swith
  }

  buflen = RH_ASK_MAX_MESSAGE_LEN;
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    // Mensaje con checksum recibido.
    if (dump) {
      driver.printBuffer("Vuelco:", buf, buflen);
    }
    // Verifica tipo de estructura.
    if (buf[0] == 1) {
      memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));
      if (DatosTipo1.to == MIID) {
        delay(200);
        DatosTipo1.tipo = 1;
        DatosTipo1.to = MAESTRO;
        DatosTipo1.from = MIID;
        
        int digi = analogRead(A0);
        
        DatosTipo1.dato = (float)(digi * 5.0 / 1023.0);
        DatosTipo1.contador = millis();
        memcpy(tx_buf, &DatosTipo1, sizeof(DatosTipo1));
        driver.send(tx_buf, sizeof(DatosTipo1));
        driver.waitPacketSent();
      }
    }
  }
}
