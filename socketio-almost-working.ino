#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>

#include <SocketIoClient.h>

#include <SoftwareSerial.h>
SoftwareSerial s(13,15);
#define USE_SERIAL Serial

#define ledPin D2
#define ledPin_D1 D1

bool connectedToServer = false;

int moveFront = 10;
int moveStop = 30;
int moveBack = 50;
int moveLeft = 70;
int moveRight = 90;

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n", payload);
}


void setup() {
    s.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(2, OUTPUT);
    USE_SERIAL.begin(115200);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }
    
    //WiFiMulti.addAP("OPPO A53", "Password");
    WiFiMulti.addAP("UTB-Students.", NULL);



    while(WiFiMulti.run() != WL_CONNECTED) {
        digitalWrite(2, HIGH);
        delay(100);
    }

    webSocket.on("receive_message", hi);

    webSocket.on("connect", handler);
    webSocket.on("disconnect", handler_disconnected);

    webSocket.begin("socket-io-server-utb-tele-bot.herokuapp.com");
}

//char rdata;
String myString;
void loop() {
    webSocket.loop();

  if (WiFi.status() == WL_CONNECTED){
    if (connectedToServer){
      digitalWrite(2, LOW);
    }else {
      digitalWrite(2, LOW);   // Turn the LED on by making the voltage LOW
      delay(100);            // Wait for a second
      digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
      delay(100);            // Wait for two seconds 
    }
  }
}

// This runs when we connected to the server
void handler(const char * payload, size_t length) {
  connectedToServer = true;
}

// This runs when we are disconnected after being connected to the server
void handler_disconnected (const char * payload, size_t length) {
  connectedToServer = false;
}


void hi(const char* message, size_t length){

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);

  String message_2 = doc["message"];
  USE_SERIAL.println(message_2);

  if (message_2.equals("F")){
    s.write(moveFront);
  }
  if (message_2.equals("S")){
    s.write(moveStop);
  }
  if (message_2.equals("B")){
    s.write(moveBack);
  }
  if (message_2.equals("L")){
    s.write(moveLeft);
  }
  if (message_2.equals("R")){
    s.write(moveRight);
  }

}
