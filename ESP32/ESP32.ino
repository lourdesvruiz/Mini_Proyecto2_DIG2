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
#define led 2
#define RX2 16
#define TX2 17
int G = 0;
int Y = 0;
int R = 0;
char bandera;

char *semaforo;



// set up the 'counter' feed
AdafruitIO_Feed *sem = io.feed("semaforo");
AdafruitIO_Feed *green = io.feed("verde");
AdafruitIO_Feed *yellow = io.feed("amarillo");
AdafruitIO_Feed *red = io.feed("rojo");

void setup() {


  // start the serial connection
  //Serial.begin(115200);
  Serial.begin(9600);
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
  sem->get();
  green->get();
  yellow->get();
  red->get();

  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  /*
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(count);
  counter->save(count);

  // increment the count by 1
  count++;
  */
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  //delay(3000);

}

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


  
