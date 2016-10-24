// NANO_RF_ESCLAVO.ino

#include <RHReliableDatagram.h>
#include <RH_ASK.h>
#include <SPI.h>

//CAMBIAR CON NUMERO DE CADA PARTICIPANTE
#define CLIENTE 1

#define MAESTRO 10

// Singleton instance of the radio driver
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
RHReliableDatagram manager(driver, CLIENTE);

struct dataStruct {
  float temp;
  float hum;
  unsigned long counter;
} DatosTipo1;

// Dont put this on the stack:
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
byte tx_buf[sizeof(DatosTipo1)] = {0};

void setup()
{
  Serial.begin(9600);
  Serial.println("INICIO ESCLAVO");
  if (!manager.init())
    Serial.println("RH FALLA");
}

void loop()
{
  if (manager.available())
  {
    // ESPERA MENSAJE DESDE MAESTRO
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      //      Serial.print("got request from : 0x");
      //      Serial.print(from, HEX);
      DatosTipo1.temp = 22.394;
      DatosTipo1.hum = 38.45;
      memcpy(tx_buf, &DatosTipo1, sizeof(DatosTipo1) );
      byte zize = sizeof(DatosTipo1);
      // ENVIA RESPUESTA AL MAESTRO
      if (!manager.sendtoWait(tx_buf, sizeof(tx_buf), from))
        Serial.println("FALLA ENVIO AL MAESTRO");
    }
  }
}

