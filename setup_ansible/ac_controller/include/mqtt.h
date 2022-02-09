#include "common.h"

void mqtt_setup();
void mqtt_loop();
void mqtt_publish(String &topic, String &message, bool retained = false);
void mqtt_received(String &topic, String &payload);
void mqtt_log(String message);