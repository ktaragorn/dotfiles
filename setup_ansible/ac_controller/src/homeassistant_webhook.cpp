#include "homeassistant_webhook.h"
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "common.h"

void trigger_homeassistant_webhook(String webhook, String value){
  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, "http://hass.middle.earth/api/webhook/" + webhook)) {  // HTTP
    util_log("[HTTP] Webhook Start");
    // start connection and send HTTP header
    int httpCode = http.POST(value);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      util_log("[HTTP] POST... code: " + String(httpCode));

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      util_log("[HTTP] POST... failed, error: " + (String)http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    util_log("[HTTP] Unable to connect");
  }
}