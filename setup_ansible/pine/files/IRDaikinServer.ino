/*
 * IRremoteESP8266: IRServer - demonstrates sending IR codes controlled from a webserver
 * Version 0.3 May, 2019
 * Version 0.2 June, 2017
 * Copyright 2015 Mark Szabo
 * Copyright 2019 David Conran
 *
 * An IR LED circuit *MUST* be connected to the ESP on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif  // ESP8266
#if defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#endif  // ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <ir_Daikin.h>

const char* kSsid = "ironforge";
const char* kPassword = "speakfriendandenter";
MDNSResponder mdns;

#if defined(ESP8266)
ESP8266WebServer server(80);
#undef HOSTNAME
#define HOSTNAME "nodemcu"
#endif  // ESP8266
#if defined(ESP32)
WebServer server(80);
#undef HOSTNAME
#define HOSTNAME "esp32"
#endif  // ESP32

const uint16_t kIrLed = 4;  // ESP GPIO pin to use. Recommended: 4 (D2).

void sendDaikinOnOffCommand(bool on, int temperature = 25, int fan = 3){
  IRDaikinESP ac(kIrLed);
  ac.begin();

  if(on){
    ac.on();
  }else{
    ac.off();
  }
  ac.setFan(fan);
  ac.setMode(kDaikinCool);
  ac.setTemp(temperature);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  ac.send();
}

void sendAcOn(){
  const String temp = server.hasArg("temperature") ? server.arg("temperature") : "25";
  const String fan = server.hasArg("fan") ? server.arg("fan") : "3";

  sendDaikinOnOffCommand(true, temp.toInt(), fan.toInt());
  Serial.println("Sent turn on command with temperature = " + temp + " and fan = " + fan);
}

void sendAcOff(){
  sendDaikinOnOffCommand(false);
  Serial.println("Sent turn off command");
}

void handleAcOn(){
  sendAcOn();
  handleRoot();
}

void handleAcOff(){
  sendAcOff();
  handleRoot();
}

String state = "false";
void handleHomeAssistantAc(){
  Serial.println("Received AC command from HA with " + server.argName(0) + ": " + server.arg(0));
  state = server.arg("plain");
  if(state == "ON"){
    sendAcOn();
  }else{
    sendAcOff();
  }
  handleHomeAssistantAcState();
}

void handleHomeAssistantAcState(){
  server.send(200, "text/plain", state);
}

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>" HOSTNAME " Demo</title></head>" \
                "<body>" \
                  "<h1>Hello from " HOSTNAME ", you can send Daikin IR" \
                      "signals from here!</h1>" \
                  "<p><a href=\"/ac/on\">Turn on AC</a></p>" \
                  "<p><a href=\"/ac/off\">Turn off AC</a></p>" \
                "</body>" \
              "</html>");
}

//
//void handleIr() {
//  for (uint8_t i = 0; i < server.args(); i++) {
//    if (server.argName(i) == "code") {
//      uint32_t code = strtoul(server.arg(i).c_str(), NULL, 10);
//#if SEND_NEC
//      irsend.sendNEC(code, 32);
//#endif  // SEND_NEC
//    }
//  }
//  handleRoot();
//}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void wifi_setup(void){
  WiFi.begin(kSsid, kPassword);
  WiFi.hostname(HOSTNAME);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(kSsid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());

#if defined(ESP8266)
  if (mdns.begin(HOSTNAME, WiFi.localIP())) {
#else  // ESP8266
  if (mdns.begin(HOSTNAME)) {
#endif  // ESP8266
    Serial.println("MDNS responder started");
  }
}

void web_server_setup(){
  server.on("/", handleRoot);
  server.on("/ac", HTTP_GET, handleHomeAssistantAcState);
  server.on("/ac", HTTP_POST, handleHomeAssistantAc);
  server.on("/ac/on", handleAcOn);
  server.on("/ac/off", handleAcOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup(void) {
  Serial.begin(115200);

  wifi_setup();

  web_server_setup();
}

void loop(void) {
  server.handleClient();
}