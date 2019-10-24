#include "common.h"

void mqtt_setup();
void mqtt_loop();
void mqtt_publish(String &topic, String &message);
void mqtt_received(String &topic, String &payload);