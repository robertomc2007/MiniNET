// Transmisor.ino
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 5, 4);

//AJUSTAR AL VALOR ADECUADO
byte miid = 1;

unsigned long tiempo = 0;

struct dataStruct {
  byte id;
  float dato;
  unsigned long contador;
} DatosTipo1;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);

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

  if (millis() - tiempo > 2000) {
    tiempo = millis();
    
    DatosTipo1.id = miid;
    
    DatosTipo1.dato = analogRead(A0);
    
    DatosTipo1.contador = millis();
    
    memcpy(buf, &DatosTipo1, sizeof(DatosTipo1));
    driver.send(buf, sizeof(DatosTipo1));
    driver.waitPacketSent();
    
    Serial.print("Enviado: "); Serial.println((char *)buf);
  }
}
