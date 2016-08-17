// Receptor.ino
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
RH_ASK driver;

void setup()
{
    Serial.begin(115200);   // Debugging only
    
    Serial.println("Receptor.ino");
    
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
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        // Message with a good checksum received, dump it.
        driver.printBuffer("Got:", buf, buflen);
    }
}
