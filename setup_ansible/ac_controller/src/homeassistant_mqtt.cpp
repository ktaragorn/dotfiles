#include "homeassistant_mqtt.h"
#include "mqtt.h"
#include "common.h"

void trigger_homeassistant_mqtt(String trigger, String value="on"){
  String topic = "trigger/" + String(HOSTNAME) + "/" + trigger;
  mqtt_publish(topic, value);
}

void state_homeassistant_mqtt(String stateName, String value="on"){
  String topic = "state/" + String(HOSTNAME) + "/" + stateName;
  mqtt_publish(topic, value, true);
}