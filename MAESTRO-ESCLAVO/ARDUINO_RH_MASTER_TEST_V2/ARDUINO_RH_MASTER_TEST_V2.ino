// RHMASTER
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer

#define MAESTRO 10

RH_ASK driver(1200, 5, 4);
RBD::Timer timer;

struct dataStruct {
  byte tipo;
  byte to;
  byte from;
  float dato;
  unsigned long contador;
} DatosTipo1;

byte buf[RH_ASK_MAX_MESSAGE_LEN];
byte buflen = sizeof(buf);
byte tx_buf[RH_ASK_MAX_MESSAGE_LEN] = {0};

boolean dump = false;
boolean interroga = true;
int maxesclavo = 3;
byte esclavo = 1;

String salida = "";

void setup()
{
  Serial.begin(115200);   // Debugging only

  Serial.println(F("RH MASTER"));
  Serial.println(RH_ASK_MAX_MESSAGE_LEN);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  if (!driver.init())
  {
    Serial.println(F("FALLA INICIO DRIVER"));
    digitalWrite(13, HIGH);
  }
  timer.setTimeout(1000);
  timer.restart();
  //  tiempo = millis();
}

void loop()
{

  //Comandos via puerto serie

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
          break;
        }
      case 'i': //interroga
        {
          interroga = !interroga;
          if (interroga) {
            timer.stop();
          } else {
            timer.restart();
          }
          break;
        }
      //nro de esclavos
      case 'e':
        {
          maxesclavo = Serial.parseInt();
          if (maxesclavo <= 0 || maxesclavo > 8) {
            maxesclavo = 1;
          }
          break;
        }
      //reset
      case 'r':
        {

          break;
        }
      //help
      case '?':
        {
          Serial.println(F("\n\rs: estado y parametros\n\rk: clave TS\n\rn: #canal TS\n\ri: nombre WiFi\n\rp: clave WiFi"));
          Serial.println(F("w: conecta WiFi\n\ra: desconecta y lista redes\n\ru: activa/des upload\n\rc: #campo TS (1-7)\n\rv: valor en campo TS (float)"));
          Serial.println(F("r: reset\n\rf: guarda config.json\n\rd: activa/des debug\n\r"));
          break;
        }
    }
  }

  buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  { // Mensaje con checksum correcto recibido.

    // Verifica tipo de estructura.
    if (buf[0] == 1) {
      // Copia buffer en estructura de datos
      memcpy(&DatosTipo1, buf, sizeof(DatosTipo1));

      if (DatosTipo1.to == MAESTRO) { // Destinatario ok
        salida += (String)DatosTipo1.from + "," + (String)DatosTipo1.dato + ",";
      }
    }

    if (dump) {
      Serial.println("***");
      driver.printBuffer("Vuelco:", buf, buflen);
      Serial.println(DatosTipo1.tipo);
      Serial.println(DatosTipo1.to);
      Serial.println(DatosTipo1.from);
      Serial.println(DatosTipo1.dato);
      Serial.println(DatosTipo1.contador);
      Serial.println("***");
    }
  }

  if (timer.onRestart()) {

    // Envia mensaje de interrogacion
    DatosTipo1.tipo = 1;
    DatosTipo1.to = esclavo;
    DatosTipo1.from = MAESTRO;
    DatosTipo1.dato = 0.0;
    DatosTipo1.contador = millis();

    memcpy(tx_buf, &DatosTipo1, sizeof(DatosTipo1) );
    driver.send(tx_buf, sizeof(DatosTipo1));
    driver.waitPacketSent();

    if (dump) {
      Serial.println("-> " + (String)esclavo);
    }

    esclavo++;

    if (esclavo > maxesclavo) {
      esclavo = 1;
      if (salida != "") {
        salida.setCharAt(salida.length() - 1, ' ');
        Serial.println(salida);
        salida = "";
      } else {
        if (dump) {
          Serial.println(F("Sin respuesta"));
        }
      }
    }
  }
}
