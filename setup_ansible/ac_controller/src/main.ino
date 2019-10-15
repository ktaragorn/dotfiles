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
#include <ESP8266WebServer.h>
#endif  // ESP8266
#if defined(ESP32)
#include <WebServer.h>
#endif  // ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <ir_Daikin.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <pins_arduino.h>
#include "buttons.h"
#include "wifi.h"
#include "homeassistant_webhook.h"

#if defined(ESP8266)
ESP8266WebServer server(80);
#endif  // ESP8266
#if defined(ESP32)
WebServer server(80);
#endif  // ESP32

const uint16_t kIrLed = D8; // actually D3.. dunno why the pinouts are wrong..
const uint16_t kRecvPin = D5;

IRrecv irrecv(kRecvPin);

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

void ir_receive(){
  decode_results results;

  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    trigger_homeassistant_webhook("tv_remote_detected", uint64ToString(results.value, HEX));
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}

//https://lowvoltage.github.io/2017/07/09/Onboard-LEDs-NodeMCU-Got-Two
void blink_led(){
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
}

void setup(void) {
  Serial.begin(115200);
  wifi_setup();

  irrecv.enableIRIn();  // Start the receiver

  web_server_setup();

  buttons_setup();

  blink_led();
}

void loop(void) {
  buttons_loop();
  ir_receive();
  server.handleClient();
}
