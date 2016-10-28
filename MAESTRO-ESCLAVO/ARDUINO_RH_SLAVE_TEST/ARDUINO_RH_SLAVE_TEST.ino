// Receptor.ino
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

#define MIID 1

#define MAESTRO 10

RH_ASK driver(2000, 5, 4);

struct dataStruct {
  byte id;
  float dato;
  unsigned long contador;
} DatosTipo1;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);
byte tx_buf[sizeof(DatosTipo1)] = {0};

void setup()
{
  Serial.begin(9600);   // Debugging only

  Serial.println("Receptor.ino");
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
  buflen = RH_ASK_MAX_MESSAGE_LEN;
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    // Message with a good checksum received, dump it.
    //driver.printBuffer("Vuelco:", buf, buflen);
    //    Serial.println(buflen);
    //    buf[buflen] = 0;
    //    Serial.println((char *)buf);
    memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));
    Serial.println(DatosTipo1.id);
    Serial.println(DatosTipo1.dato);
    Serial.println(DatosTipo1.contador);
  }

  if(DatosTipo1.id == MIID){
    DatosTipo1.id = 10;
    DatosTipo1.dato = analogRead(A0);
    DatosTipo1.contador = millis();
    memcpy(buf, &DatosTipo1, sizeof(DatosTipo1));
    driver.send(buf, sizeof(DatosTipo1));
    driver.waitPacketSent();
    delay(10);
  }
}
