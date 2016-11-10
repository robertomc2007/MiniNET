// RHMASTER
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer

#define MAESTRO 0

RH_ASK driver(2400, 5, 4);
RBD::Timer timer, timersend;

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

boolean dump = false;
boolean interroga = false;
boolean muestra = false;
int maxesclavo = 5;
byte esclavo = 1;
unsigned long tiempo = 0;
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
  timer.setTimeout(3500);
  timersend.setTimeout(20000);
  timer.stop();
  timersend.stop();
  Serial.println(F("DETIENE TIMERS"));
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
      case 'm': //muestra preliminares
        {
          muestra = !muestra;
          break;
        }
      case 't': //timers interroga
        {
          interroga = !interroga;
          if (interroga) {
            timer.stop();
            timersend.stop();
            Serial.println(F("DETIENE TIMERS"));
          } else {
            timer.restart();
            Serial.println(F("INICIA TIMERS"));
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
      //interroga
      case 'i':
        {
          int destino = Serial.parseInt();
          DatosTipo.tipo = 1; // Mensaje de interrogacion
          DatosTipo.to = (byte)destino;
          DatosTipo.contador = millis();
          SendDatos();
          break;
        }

      //ping
      case 'p':
        {
          int destino = Serial.parseInt();
          DatosTipo.tipo = 3;
          DatosTipo.to = (byte)destino;
          DatosTipo.contador = millis();
          SendDatos();
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

    // Mensaje con checksum recibido.
    memcpy(&DatosTipo, buf, sizeof(DatosTipo));

    if (dump) {
      Serial.println("***");
      driver.printBuffer("RECIBIDO:", buf, buflen);
      Serial.println(DatosTipo.tipo);
      Serial.println(DatosTipo.to);
      Serial.println(DatosTipo.from);
      Serial.println(DatosTipo.maxnode);
      Serial.println(DatosTipo.hope);
      Serial.println(DatosTipo.dato);
      Serial.println(DatosTipo.contador);
      Serial.println("***");
    }

    delay(200); //espero para que todos pasen al modo recepcion

    if (DatosTipo.hope == MAESTRO && DatosTipo.to == MAESTRO) // Destinatario ok
    {
      if (DatosTipo.tipo == 2) //RESPUESTA
      {
        if (muestra)
        {
          //Computar cuanto dura el proceso
          sprintf(tx_buf, "RESP desde #%d. Demora=%d", DatosTipo.from, millis() - DatosTipo.contador);
          Serial.println((char *)tx_buf);
        }
        
        salida += (String)DatosTipo.from + "," + (String)DatosTipo.dato + ",";

        if (muestra)
          Serial.println(salida);
      }
      else if (DatosTipo.tipo == 4) //PONG
      {
        //Computar cuanto dura el proceso
        sprintf(tx_buf, "PONG desde #%d. Demora=%d", DatosTipo.from, millis() - DatosTipo.contador );
        Serial.println((char *)tx_buf);
      }
      else if (DatosTipo.tipo == 3) //PING
      {
        sprintf(tx_buf, "PING desde #%d", DatosTipo.from);
        Serial.println((char *)tx_buf);
        DatosTipo.tipo = 4; //Retorno un PONG
        //Permuto destino
        DatosTipo.to = DatosTipo.from;
        SendDatos();

      }
    }
  }

  if (timersend.onExpired()) {
    // code only runs once per event
    if (dump) {
      Serial.println("Timer Send");
    }
    timer.restart();
  }

  if (timer.onRestart()) {
    // Envia mensaje de interrogacion
    DatosTipo.tipo = 1;
    DatosTipo.to = esclavo;
    DatosTipo.contador = millis();
    
    SendDatos();

    if (muestra) {
      Serial.println("-> " + (String)esclavo);
    }

    esclavo++;

    if (esclavo > maxesclavo) {
      esclavo = 1;
      timer.stop();
      timersend.restart();
      if (salida != "") {
        salida.setCharAt(salida.length() - 1, ' ');
        Serial.println(salida); //Envia datos al GATEWAY
        salida = "";
      } else {
        if (dump) {
          Serial.println(F("Sin respuesta"));
        }
      }
    }
  }
}

void SendDatos()
{
  DatosTipo.hope = 1;
  DatosTipo.maxnode = maxesclavo;
  DatosTipo.from = MAESTRO;
  DatosTipo.dato = 0.0;
  //DatosTipo.contador = millis();

  if (muestra)
  {
    sprintf(tx_buf, "ENVIA MENSAJE tipo=%d destino=%d via=%d", DatosTipo.tipo, DatosTipo.to, DatosTipo.hope);
    Serial.println((char *)tx_buf);

  }

  memcpy(tx_buf, &DatosTipo, sizeof(DatosTipo) );
  driver.send(tx_buf, sizeof(DatosTipo));
  driver.waitPacketSent();
}

