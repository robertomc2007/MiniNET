// RH_RX_STRUCT_TEST
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver(2000, 5, 4);


struct dataStruct {
  byte id;
  float dato;
  unsigned long contador;
} DatosTipo1;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);

void setup()
{
  Serial.begin(115200);   // Debugging only

  Serial.println("RH_RX_STRUCT_TEST");
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
  buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));
    Serial.println(DatosTipo1.id);
    Serial.println(DatosTipo1.dato);
    Serial.println(DatosTipo1.contador);
  }
}
