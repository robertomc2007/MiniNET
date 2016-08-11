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
#include <SPI.h> // Not actually used but needed to compile
#include <RHDatagram.h>

#define TRANSMISOR 1
#define RECEPTOR 2

RH_ASK driver;
RHDatagram manager(driver, TRANSMISOR);

struct dataStruct {
  float press_norm ;
  float press_hg;
  float temp;
  unsigned long counter;

} myData;

byte tx_buf[sizeof(myData)] = {0};

void setup()
{
  Serial.begin(9600);    // Debugging only

  Serial.println("RHDatagram_TXSensores");

  if (!manager.init())
    Serial.println("init failed");

  myData.press_norm = 1000.11;
  myData.press_hg = 0.59;
  myData.temp = 22.394;

}

void loop()
{
  manager.setHeaderId(1);
  memcpy(tx_buf, &myData, sizeof(myData) );
  byte zize = sizeof(myData);

  manager.sendto((uint8_t *)tx_buf, zize,RECEPTOR);
  manager.waitPacketSent();

  myData.counter++;
delay(200);
  manager.setHeaderId(2);
  manager.sendto((uint8_t *)tx_buf, zize,RECEPTOR);
  manager.waitPacketSent();
  
  delay(2000);
}
