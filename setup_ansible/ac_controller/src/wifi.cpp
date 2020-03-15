#include "wifi.h"
#include "common.h"
#include "secrets.h"
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#endif  // ESP8266
#if defined(ESP32)
  #include <WiFi.h>
  #include <ESPmDNS.h>
#endif  // ESP32

MDNSResponder mdns;
void wifi_setup(void){
  WiFi.disconnect();
  WiFi.begin(wifi_ssid, wifi_password);
  WiFi.hostname(HOSTNAME);
  WiFi.setAutoConnect ( true );
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifi_ssid);
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

void wifi_loop(){
  if (!wifi_connected())
  {
    delay(1000);
    ESP.restart();
    // wifi_connect();
    // WiFi.reconnect();
  }
}

bool wifi_connected(){
  return WiFi.status() == WL_CONNECTED;
}