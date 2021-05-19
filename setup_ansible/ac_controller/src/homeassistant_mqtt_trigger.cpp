#include "homeassistant_mqtt_trigger.h"
#include "mqtt.h"
#include "common.h"

void trigger_homeassistant_mqtt(String trigger, String value="on"){
  String topic = "trigger/" + String(HOSTNAME) + "/" + trigger;
  mqtt_publish(topic, value);
}