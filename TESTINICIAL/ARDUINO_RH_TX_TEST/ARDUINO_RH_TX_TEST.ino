// TX_TEST
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
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 5, 4);

unsigned long tiempo = 0;

byte buf[20];

void setup()
{
  Serial.begin(9600);   // Debugging only

  Serial.println("RH_TX_TEST");
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
    
//    const char *msg = "Esta es la voz de los marcianos...";
//    driver.send((uint8_t *)msg, strlen(msg));
    
    sprintf(buf, "10,%ul", tiempo);
    
    driver.send((uint8_t *)buf, strlen(buf));
    
    driver.waitPacketSent();
    
    Serial.print("Enviado: "); Serial.println((char *)buf);
  }
}
