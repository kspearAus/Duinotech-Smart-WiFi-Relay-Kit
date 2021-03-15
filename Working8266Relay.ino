// File: Working8266Relay
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

ESP8266WebServer server(80);

 DNSServer dns;
 IPAddress apIP(192, 168, 1, 1);
 const byte cmdOFF[] = {0xA0, 0x01, 0x00, 0xA1};
 const byte cmdON[] = {0xA0, 0x01, 0x01, 0xA2};
 
void setup() { 
 pinMode(0, OUTPUT);
 Serial.begin(9600);
 WiFi.mode(WIFI_AP);
 dns.setTTL(300);
 dns.setErrorReplyCode(DNSReplyCode::ServerFailure);
 dns.start(53, "relay.net", apIP);
 Serial.println();
 Serial.println("Duinotech WiFi Relay");
 Serial.println("Setting up Soft-AP..");
 WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
 if (WiFi.softAP("Duinotech WiFi Relay")) {
 Serial.println("Success!");
 Serial.println("----------------");
 Serial.println("Connect to the WiFi network called");
 Serial.println("Duinotech WiFi Relay");
 Serial.println("and navigate to relay.net");
 } 
 else { Serial.println("Failed! Resetting...");
 //hang, and causes a reset;
 for (;;) ;
 } 
 server.on("/", sendIndex);
 server.on("/open", openRelay);
 server.on("/close", closeRelay);
 server.onNotFound(sendNotFound);
 server.begin();
 } 

void loop() { dns.processNextRequest();
 server.handleClient();
 } 

void sendIndex() { 
server.send(200, "text/html", "<meta name=viewport content= width=device-width, initial-scale=1> </head>"
"<h2> Duinotech WiFi Relay </h2>"
"<h5> Think possible. </h5>"
"<a href='/open'>Open Relay</a>"
"<p> <a href='/close'>Close Relay</a>");
}

void openRelay() { 
 Serial.write(cmdON, 4);
 //server.send(200,"text/plain", "sent off command");   
 } 

void closeRelay(){ 
 Serial.write(cmdOFF, 4);
 //server.send(200, "text/plain", "sent on command");  
 } 

void sendNotFound() { server.send(404, "text/plain", "404: Not Found");
 } 
