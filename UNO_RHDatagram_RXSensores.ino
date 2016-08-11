/* RHDatagram_TXSensores
   Arduino                         Transmitter
    GND------------------------------GND
    D12------------------------------Data
    5V-------------------------------VCC

  Most receivers can be connected to Arduino like this:

   Arduino                         Receiver
    GND------------------------------GND
    D11------------------------------Data
    5V-------------------------------VCC
  // -*- mode: C++ -*-
  // Simple example of how to use RadioHead to transmit messages
  // with a simple ASK transmitter in a very simple way.
  // Implements a simplex (one-way) transmitter with an TX-C1 module
*/
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

#include <RHDatagram.h>

#define TRANSMISOR 1
#define RECEPTOR 2

RH_ASK driver;
RHDatagram manager(driver, 2);

struct dataStruct {
  float press_norm ;
  float press_hg;
  float temp;
  unsigned long counter;

} myData;


void setup()
{
  Serial.begin(9600);  // Debugging only

  Serial.println("RHDatagram_RXSensores");

  if (!manager.init())
    Serial.println("init failed");
}

void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  uint8_t from = 0;
  uint8_t to = 0;
  uint8_t id = 0;
  uint8_t flags = 0;

  if (manager.available()) //(buf, &buflen)) // Non-blocking
  {
    Serial.println("********************");
    
    manager.recvfrom(buf, &buflen, &from, &to, &id, &flags);

    // Message with a good checksum received, dump it.
    //  driver.printBuffer("Got:", buf, buflen);
    memcpy(&myData, buf, sizeof(myData));
    
    Serial.println(from);
    Serial.println(to);
    Serial.println(id);
    Serial.println(flags);   
    Serial.println("********************");

    Serial.print("press_norm:");
    Serial.print(myData.press_norm);

    Serial.print(" press_hg:");
    Serial.print(myData.press_hg);

    Serial.print(" temp:");
    Serial.print(myData.temp);

    Serial.print(" counter:");
    Serial.println(myData.counter);

  }
}
