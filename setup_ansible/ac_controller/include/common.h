#include <Arduino.h>
#include <pins_arduino.h>
#undef HOSTNAME
#define HOSTNAME "nodemcu"

void log(String &log_message);

#define LED_BUILTIN 2