// RHSLAVE
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

#define MIID 2 //CAMBIAR SEGUN ASIGNACION DE DIRECCION

#define MAESTRO 0

RH_ASK driver(2400, 5, 4);

struct dataStruct {
  byte tipo; //1=interroga 2=responde 3= ping 4=pong
  byte to;
  byte from;
  byte maxnode;
  byte hope;
  float dato;
  unsigned long contador;
} DatosTipo;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);
byte tx_buf[RH_ASK_MAX_MESSAGE_LEN + 100] = {0};
boolean dump = true;
unsigned long tiempo = 0;

void setup()
{
  Serial.begin(115200);   // Debugging only

  Serial.print("RH SLAVE #"); Serial.println(MIID);
  //  Serial.println(RH_ASK_MAX_MESSAGE_LEN);

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
  if ( Serial.available()) {

    char ch = Serial.read();
    switch (ch) {
      //    case '0'...'9':
      //      //v = v * 10 + ch - '0';
      //      v = ch - '0';
      //      break;
      case 'd': //vuelco
        {
          dump = !dump;
          if (dump)
          Serial.print("VUELCO "); Serial.println(dump ? "SI" : "NO");
          break;
        }

      case 'p': //transmite PING
        {
          int destino = Serial.parseInt();
          DatosTipo.tipo = 3;
          DatosTipo.to = (byte)destino;
          DatosTipo.from = MIID;
          if (destino > MIID)
          {
            DatosTipo.hope = MIID + 1;
          }
          else
          {
            DatosTipo.hope = MIID - 1;
          }
          DatosTipo.contador = millis();
          tiempo = millis();
          SendDatos();
          break;
        }
      //help
      case '?':
        {
          Serial.println(F("\n\rpnn: PING\n\rd: DUMP"));
          break;
        }
    }// swith
  }

  buflen = RH_ASK_MAX_MESSAGE_LEN;
  
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    //Mensaje con checksum recibido.
    Serial.println(F("MENSAJE RECIBIDO")); 
    memcpy(&DatosTipo, buf, sizeof(DatosTipo));

    if (dump) {
      driver.printBuffer("RECIBIDO:", buf, buflen);
    }

    delay(200); //espero para que todos pasen al modo recepcion

    /*
       Verifica tipo de estructura ruta y destinatario final.
    */

    if (DatosTipo.hope == MIID) //HOPE
    {
      if (DatosTipo.to == MIID ) //PARA MI
      {

        if (DatosTipo.tipo == 1) //INTERROGA
        {
          if (dump)
            Serial.println(F("INGRESA INTERROGA - ENVIA RESPUESTA"));

          //INTERROGA DESTINO FINAL -> para mi

          SendMISDatos(); //envia mis datos a nodo (hope) anterior

        }//INTERROGA

        if (DatosTipo.tipo == 3) //INGRESA PING
        {
          if (dump)
            Serial.println(F("INGRESA PING - RETORNA PONG"));
          DatosTipo.tipo = 4; //Retorno un PONG
          //Permuto destino
          DatosTipo.to = DatosTipo.from;
          //RUTEA PING
          if (DatosTipo.to > MIID)
          {
            DatosTipo.hope = MIID + 1;
          }
          else
          {
            DatosTipo.hope = MIID - 1;
          }
          //Ajusto origen
          DatosTipo.from = MIID;

          SendDatos();

        }//PING

        if (DatosTipo.tipo == 4)  //INGRESA PONG
        {
          if (dump)
            Serial.println(F("INGRESA PONG"));
          //Computar cuanto dura el proceso
          sprintf(tx_buf, "PONG desde #%d. Demora=%d", DatosTipo.from, millis() - DatosTipo.contador);
          Serial.println((char *)tx_buf);
        }//PONG

      }//PARA MI
      else //NO ES PARA MI -> RUTEO
      {

        if (DatosTipo.tipo == 1) //RUTEA INTERROGA
        {
          if (dump)
            Serial.println(F("RUTEA INTERROGA"));
          DatosTipo.hope += 1;
          SendDatos();
        }//RUTEA INTERROGA

        if (DatosTipo.tipo == 2) //RUTEA RESPUESTA
        {
          if (dump)
            Serial.println(F("RUTEA RESPUESTA"));
          DatosTipo.hope -= 1;
          SendDatos();
        }//RUTEA RESPUESTA

        if (DatosTipo.tipo == 3 || DatosTipo.tipo == 4) //RUTEA PINGPONG
        {
          if (dump)
            Serial.println(F("RUTEA PINGPONG"));
          if (DatosTipo.to > MIID)
          {
            DatosTipo.hope = MIID + 1;
          }
          else
          {
            DatosTipo.hope = MIID - 1;
          }

          SendDatos(); //RUTEA PINGPONG
        }//RUTEA PINGPONG

      }//RUTEO
    }//HOPE
  }//RECV
}//LOOP
/*
   RUTINAS
*/
void SendMISDatos() {

  DatosTipo.tipo = 2;
  DatosTipo.to = MAESTRO;
  DatosTipo.from = MIID;
  DatosTipo.hope -= 1;

  int digi = analogRead(A0);

  DatosTipo.dato = (float)(digi * 5.0 / 1023.0);
//  DatosTipo.contador = millis();

  //  memcpy(tx_buf, &DatosTipo, sizeof(DatosTipo));
  //  driver.send(tx_buf, sizeof(DatosTipo));
  //  driver.waitPacketSent();
  //  Serial.print(F("Respuesta al MAESTRO via # ")); Serial.println(DatosTipo.hope);

  SendDatos();
}

void SendDatos() {

  sprintf(tx_buf, "ENVIA MENSAJE tipo=%d origen=%d destino=%d via=%d", DatosTipo.tipo, DatosTipo.from, DatosTipo.to, DatosTipo.hope);
  Serial.println((char *)tx_buf);

  memcpy(tx_buf, &DatosTipo, sizeof(DatosTipo));
  driver.send(tx_buf, sizeof(DatosTipo));
  driver.waitPacketSent();

}

