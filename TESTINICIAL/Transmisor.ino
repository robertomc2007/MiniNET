// Transmisor.ino
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

void setup()
{
    Serial.begin(115200);   // Debugging only
    
    Serial.println("Transmisor.ino");
    
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
    delay(2000);
    const char *msg = "Esta es la voz de los marcianos...";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    Serial.println("Enviado");
}
