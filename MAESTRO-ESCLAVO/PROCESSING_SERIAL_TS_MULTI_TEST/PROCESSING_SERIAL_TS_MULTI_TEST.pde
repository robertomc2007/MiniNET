/**
 * Tests the thinkspeak channel by sending any numeric 
 * using the specified APIKEY and FIELD
 */

import processing.serial.*;
import processing.net.*;
import static javax.swing.JOptionPane.*;

//CONFIGURATION
String APIKEY = "XXXXXXXXXXXX"; //your api key
int PORTNUM = 0; //Indice del listado de puertos arduino
//END CONFIGURATION

Serial arduino;
Client c;
String data;
//float valor=0; //dato a cargar en TS
//int campo=0; //campo del canal en TS

void setup() {
  size(600, 400);
  try {
    //setup the serial port
    // Lista los puerto
    println(Serial.list());
    //Conecta puerto serial
    arduino = new Serial(this, Serial.list()[PORTNUM], 115200);
  }
  catch (Exception e)
  { //Print the type of error
    showMessageDialog(frame, "COM no disponible");
    println("Error:", e);
    exit();
  }
}

void draw() {
  background(50);
  fill(255);
  text("ThinkSpeak Processor", 10, 20);

  fill(0, 255, 0);
  //text("Campo:"+campo+"  Valor:" + valor, 10, 40);

  if ( data != null ) {
    fill(0, 255, 0);
    text("Respuesta del servidor:", 10, 60);
    fill(200);
    text(data, 10, 80);
  }
  if (c != null) {
    if (c.available() > 0) { // Muestra la respuesta del servidor...
      data = c.readString();
      println(data);
    }
  }

  //if we have a new line from our arduino, then send it to the server
  String ln;
  if ( (ln = arduino.readStringUntil('\n')) != null) {
    println("Ingresa: "+ln);
    try {
      // Separa campos
      String[] val = splitTokens(ln, ",");
      if (val != null)
      {
        //valor = new Float(trim(val[1]));
        //campo = new Integer(trim(val[0]));
        String salida="";
        for (int i=0; i<val.length; i+=2) {
          salida += "&field"+trim(val[i])+"="+trim(val[i+1]);
        }
        println(salida);

        sendNumber(salida);
      }
    }
    catch(Exception ex) {
      println("ERROR: "+ex);
    }
  }
}

void sendNumber(String compuesto) {
  c = new Client(this, "api.thingspeak.com", 80); // Connect to server on port 80
  c.write("GET /update?key=" + APIKEY + compuesto + " HTTP/1.1\n");
  c.write("Host: my_domain_name.com\n\n"); // Be polite and say who we are
}
