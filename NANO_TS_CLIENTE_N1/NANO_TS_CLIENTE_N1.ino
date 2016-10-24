#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <RHDatagram.h>

#define TRANSMISOR 1
#define SERVIDOR 10
/*
  RH_ASK::RH_ASK ( uint16_t  speed = 2000,
  uint8_t   rxPin = 11,
  uint8_t   txPin = 12,
  uint8_t   pttPin = 10,
  bool  pttInverted = false
  )
*/
RH_ASK driver(2000, 5, 4);
// Class to manage message delivery and receipt, using the driver declared above
RHDatagram manager(driver, TRANSMISOR);

struct dataStruct {
  float temp;
  float hum;
  unsigned long counter;
} DatosTipo1;

byte tx_buf[sizeof(DatosTipo1)] = {0};

void setup()
{
  Serial.begin(9600);    // Debugging only

  Serial.println(F("NANO_TS_CLIENTE_N1"));

  if (!manager.init())
    Serial.println(F("RH FALLA INIT"));
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
    Serial.println(from);
    Serial.println(to);
    Serial.println(id);
    Serial.println(flags);
    Serial.println("********************");
    
    if (id == 1) { //mensaje de datos tipo 1

      manager.setHeaderId(1);
      DatosTipo1.temp = 22.394;
      DatosTipo1.hum = 38.45;
      memcpy(tx_buf, &DatosTipo1, sizeof(DatosTipo1) );
      byte zize = sizeof(DatosTipo1);
      delay(100);
      manager.sendto((uint8_t *)tx_buf, zize, SERVIDOR);
      manager.waitPacketSent();
      Serial.println(DatosTipo1.counter);
      DatosTipo1.counter++;
      
    }else{
      driver.printBuffer("VUELCO:", buf, buflen);
    }
  }
}
