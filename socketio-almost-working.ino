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
//    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(2, OUTPUT);
    USE_SERIAL.begin(115200);
//    USE_SERIAL.begin(9600);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("Safudil", "0580502070");

    while(WiFiMulti.run() != WL_CONNECTED) {
        digitalWrite(2, HIGH);
        delay(100);
    }

    webSocket.on("receive_message", hi);
//    webSocket.begin("server-test-socketio.herokuapp.com");
//    webSocket.begin("http://socketio-server-telebot.onrender.com", 10000);
//    webSocket.beginSSL("socketio-server-telebot.onrender.com", 443, "/socket.io/?transport=websocket", "68 53 03 7A 27 63 D1 D6 12 36 C1 5F D4 E4 11 DB 65 8B B9 87");
//    webSocket.beginSSL("https://socket-io-server-robot-control-production.up.railway.app", 443);
//    webSocket.beginSSL("https://socket-io-server-robot-control-production.up.railway.app", 443, "/socket.io/?transport=websocket", "14 8B 97 9B 8D 33 14 33 C6 9A 4A CA 24 AD AA B9 8F 74 3A F2");
//    

    webSocket.on("connect", handler);
    webSocket.on("disconnect", handler_disconnected);

    webSocket.beginSSL("socket-io-server-utb-tele-bot.herokuapp.com", 443, "/socket.io/?transport=websocket", "11 58 B1 D9 1F 55 63 DB F0 52 63 17 82 6B 2A F2 E4 2B 7A 40");
//    webSocket.begin("socket-io-server-utb-tele-bot.herokuapp.com");
//    webSocket.begin("192.168.100.9", 3001);
    // use HTTP Basic Authorization this is optional remove if not needed
//    webSocket.setAuthorization("username", "password");
}

//char rdata;
String myString;
void loop() {
    webSocket.loop();
//    USE_SERIAL.println("Do we come here??");
//    rdata = Serial.read(); 
//    myString = myString+ rdata; 
    
//    delay(1000);

//    digitalWrite(ledPin_D1, HIGH);

  if (WL_CONNECTED){
//    USE_SERIAL.println("WIFI Connected");
//      digitalWrite(LED_BUILTIN, HIGH);
//      digitalWrite(2, LOW);
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

//  USE_SERIAL.println(message);

//  Serial.write("Hi Arduino");

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);

  String message_2 = doc["message"];
  USE_SERIAL.println(message_2);

  if (message_2.equals("F")){
    s.write(moveFront);
    digitalWrite(ledPin, HIGH);
  }
  if (message_2.equals("S")){
    s.write(moveStop);
    digitalWrite(ledPin, LOW);
  }
  if (message_2.equals("B")){
    s.write(moveBack);
    digitalWrite(ledPin, LOW);
  }
  if (message_2.equals("L")){
    s.write(moveLeft);
    digitalWrite(ledPin, HIGH);
  }
  if (message_2.equals("R")){
    s.write(moveRight);
    digitalWrite(ledPin, HIGH);
  }
  
  
  
  else{
    //digitalWrite(ledPin, LOW);
  
//    webSocket.emit("send_message", "\"sendThis\"");
//    webSocket.emit("plainString", "\"this is a plain string\"");
//    webSocket.emit("send_message", sendThis);
//    webSocket.emit("send_this", "\"this is a plain string\"");
//      int number = 10;
//      String thisBoy = buf; 
//      String sendThis = "\"" + thisBoy + "\"";
//    webSocket.emit("send_this", "{\"message\":\"Not Forward!!\"}");
//    String json= "{\"message\":" + "\"" + "The val is:" number + "\"" + "}";  

    //    String sendThis = "\"Hello There\"";

    
    //Working Well TO EMIT
//    int number = 55;
//    String value = String(number);
//    String sendThis = "\"" + value + "\"";
//    String json= "{\"message\":" + sendThis + "}";
//    webSocket.emit("send_this", json.c_str());
    //Working Well TO EMIT

    
//      webSocket.emit("send_this", "{\"message\":sendThis}");
//      webSocket.emit("send_this", "{\"message\":%sendThis}");
// https://stackoverflow.com/questions/74724916/arduino-nodejs-socket-io-send-json-format-data-not-getting-the-value-of-variable
//  Google: https://www.google.com/search?q=socketio+arduino+send+variable&client=safari&rls=en&sxsrf=ALiCzsaGPbghSUuBPYefzy8icWmFZj8AwQ%3A1672126976795&ei=AKKqY_GNMJqygQbVrr7ABQ&oq=socketio+arduino+send+variabl&gs_lcp=Cgxnd3Mtd2l6LXNlcnAQAxgAMgcIIRCgARAKOgcIIxCwAxAnOgoIABBHENYEELADOgQIIxAnOgYIABAWEB46CQgAEBYQHhDxBDoICAAQFhAeEAo6CwgAEAgQHhDxBBANOgoIABAWEB4QDxAKOg0IABAWEB4QDxDxBBAKOggIIRAWEB4QHUoECEEYAEoECEYYAFCHBVi-IGDtJmgBcAF4AIABtAGIAd4QkgEEMC4xM5gBAKABAcgBCcABAQ&sclient=gws-wiz-serp
//https://stackoverflow.com/questions/74724916/arduino-nodejs-socket-io-send-json-format-data-not-getting-the-value-of-variable
//  https://github.com/timum-viw/socket.io-client/issues/22
  }


}
