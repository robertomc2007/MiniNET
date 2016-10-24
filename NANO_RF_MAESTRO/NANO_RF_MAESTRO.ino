// NANO_RF_MAESTRO.ino

#include <RHReliableDatagram.h>
#include <RH_ASK.h>
#include <SPI.h>

#define MAXCLI 1
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
RHReliableDatagram manager(driver, MAESTRO);

struct dataStruct {
  float temp;
  float hum;
  unsigned long counter;
} DatosTipo1;

// Dont put this on the stack:
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
byte tx_buf[sizeof(DatosTipo1)] = {0};

byte cliente = 1;

void setup()
{
  Serial.begin(9600);
  Serial.println("INICIO MAESTRO");
  
  if (!manager.init())
    Serial.println("RH FALLA");

//  manager.setRetries  ( 5 );
//  manager.setTimeout  ( 200 );

}

void loop()
{
  if (cliente > MAXCLI)
    cliente = 1;

  // INTERROGA AL CLIENTE
  if (manager.sendtoWait(tx_buf, sizeof(tx_buf), cliente))
  {
    // ESPERA RESPUESTA DEL CLIENTE
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));
      Serial.print("ESCLAVO: ");
      Serial.print(from);
      Serial.print(" Temp:");
      Serial.print(DatosTipo1.temp);
      Serial.print(" Humedad:");
      Serial.print(DatosTipo1.hum);
      Serial.print(" Contador:");
      Serial.println(DatosTipo1.counter);
    }
    else
    {
      Serial.print("ERROR RESPUESTA DE "); Serial.println(cliente);
    }
  } else {
    Serial.print("NO PUEDE CONECTARSE CON "); Serial.println(cliente);
  }

  cliente++;
  delay(500);
}
