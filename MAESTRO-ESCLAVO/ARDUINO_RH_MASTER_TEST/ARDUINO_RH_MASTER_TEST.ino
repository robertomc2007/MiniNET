// Transmisor.ino
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

#define MAESTRO 10

RH_ASK driver(2000, 5, 4);

struct dataStruct {
  byte id;
  float dato;
  unsigned long contador;
} DatosTipo1;

unsigned long tiempo = 0;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = RH_ASK_MAX_MESSAGE_LEN;

byte esclavo = 0;

void setup()
{
  Serial.begin(9600);   // Debugging only

  Serial.println("Transmisor.ino");
  Serial.println(RH_ASK_MAX_MESSAGE_LEN);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  if (!driver.init())
  {
    Serial.println("FALLA INICIO DRIVER");
    digitalWrite(13, HIGH);
  }

  tiempo = millis();
}

void loop()
{
  buflen = RH_ASK_MAX_MESSAGE_LEN;
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    // Message with a good checksum received, dump it.
    //driver.printBuffer("Vuelco:", buf, buflen);

    memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));
    Serial.println(DatosTipo1.id);
    Serial.println(DatosTipo1.dato);
    Serial.println(DatosTipo1.contador);
  }

  if (millis() - tiempo > 2000) {
    esclavo++;
    tiempo = millis();
    //    const char *msg = "Esta es la voz de los marcianos...";
    //    driver.send((uint8_t *)msg, strlen(msg));
    //    sprintf(buftx, "%d,%ul", esclavo, tiempo);

    DatosTipo1.id = esclavo;
    DatosTipo1.dato = 0.0;
    DatosTipo1.contador = tiempo;

    memcpy(buf, &DatosTipo1, sizeof(DatosTipo1) );
    driver.send(buf, sizeof(DatosTipo1));
    driver.waitPacketSent();

    Serial.print("Enviado: "); Serial.println(DatosTipo1.id);

    if (esclavo > 3)
      esclavo = 0;
  }
}
