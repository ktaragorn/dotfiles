#include "homeassistant_webhook.h"
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

void trigger_homeassistant_webhook(String webhook, String value){
  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, "http://hass.middle.earth/api/webhook/" + webhook)) {  // HTTP
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.POST(value);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}