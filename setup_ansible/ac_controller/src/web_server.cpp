#if defined(ESP8266)
#include <ESP8266WebServer.h>
#endif  // ESP8266
#if defined(ESP32)
#include <WebServer.h>
#endif  // ESP32
#include "common.h"
#include "ac_send.h"

#if defined(ESP8266)
ESP8266WebServer server(80);
#endif  // ESP8266
#if defined(ESP32)
WebServer server(80);
#endif  // ESP32

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


void handleAcOn(){
  sendAcOn(server.arg("temperature"), server.arg("fan"));
  handleRoot();
}

void handleAcOff(){
  sendAcOff();
  handleRoot();
}

String state = "false";
void handleHomeAssistantAcState(){
  server.send(200, "text/plain", state);
}

void handleHomeAssistantAc(){
  util_log("Received AC command from HA with " + server.argName(0) + ": " + server.arg(0));
  state = server.arg("plain");
  if(state == "ON"){
    sendAcOn();
  }else{
    sendAcOff();
  }
  handleHomeAssistantAcState();
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
}

void web_server_loop() {
  server.handleClient();
}