// RX_TEST
//  Arduino                         Transmisor
//   GND------------------------------GND
//   D4-------------------------------Data
//   Vin------------------------------VCC
//
//
//  Arduino                         Receptor
//   GND------------------------------GND
//   D5-------------------------------Data
//   5V-------------------------------VCC


#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

//bps,Rdatos,Tdatos
RH_ASK driver(2000, 5, 4);

uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
uint8_t buflen = sizeof(buf);

void setup()
{
  Serial.begin(9600);   // Debugging only

  Serial.println("RH_RX_TEST");
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
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    // Message with a good checksum received, dump it.
    driver.printBuffer("Vuelco:", buf, buflen);
    Serial.println(buflen);
    buf[buflen] = 0;
    Serial.println((char *)buf);
  }
}
