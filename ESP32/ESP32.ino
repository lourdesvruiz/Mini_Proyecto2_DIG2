// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
#define IO_LOOP_DELAY 5000

#define led 2
#define RX2 16  //Pines GPIO
#define TX2 17
unsigned long lastUpdate = 0;
int G = 0;
int Y = 0;
int R = 0;
char bandera;

char *semaforo;
int sensor;
//String sensor_gyro;
char gyro_s[18];


// set up the feed
AdafruitIO_Feed *sem = io.feed("semaforo");
AdafruitIO_Feed *gyro = io.feed("sensor");

void setup() {

  //Comunicacion Arduino y ESP32
  Serial.begin(9600);
  //Comunicacion USART ESP32 y PIC 
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
 
  delay(1000);

  // wait for serial monitor to open
  while(! Serial);
  pinMode(led, OUTPUT);
  
  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

   // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  sem->onMessage(handleMessage);
  //yellow->onMessage(handleMessage2);
  //red->onMessage(handleMessage3);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  // we are connected
  // Recibir datos de los feeds
  sem->get();
  ;

  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  //Chequea si se está recibiendo datos del PIC
  if (Serial2.available()){
    
    //Se recibe una cadena de texto y lo almacena en el buffer 
    //También se define un buffer (en este caso, de 18 espacios) 
    //Para al llegar al carácter nulo 
    char sensor_gyro = Serial2.readBytesUntil('\n', gyro_s, 18);

    Serial.print(gyro_s);
    Serial.println(" ");
  }
  
  //Se usa millis como un medidor de tiempo 
  //Espera que pase un tiempo (delay) desde el primer impulso, para luego proceder a seguir ejecutando (la recepción de datos del PIC) 
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    gyro->save(gyro_s);     //se guarda el valor del sensor en el feed de sensor

    lastUpdate = millis();
  }
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(500);

}

//Funcion para mandarle el valor adecuado del semáforo a el PIC maestro 
void handleMessage(AdafruitIO_Data *data) {

  //Serial.print("received <- ");
  //Serial.println(data->value());
  semaforo = data->value();
  if (*semaforo == '0'){
    digitalWrite(led, HIGH);
    Serial.println("led encendido");
    bandera = 1;
    Serial.println(1);
    Serial2.write(1);
    
  }

  else if (*semaforo == '1') {
    digitalWrite(led, LOW);
    Serial.println("led apagado");
    bandera = 2;
    Serial.println(2);
    Serial2.write(2);
  }

  else if (*semaforo == '2') {
    bandera = 3;
    Serial.println(3);
    Serial2.write(3);
    
  }
}


  
