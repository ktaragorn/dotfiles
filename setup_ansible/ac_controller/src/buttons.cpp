#include "buttons.h"
#include <OneButton.h>
#include "common.h"
#include "homeassistant_mqtt_trigger.h"

const int buttonPin = D7;
OneButton button(buttonPin, false);

void on_click(){
   util_log("Button Pressed");
   trigger_homeassistant_mqtt("button_pressed", "on");
}

void on_double_click(){
   util_log("Button double Pressed");
   trigger_homeassistant_mqtt("button_double_pressed", "on");
}

void on_long_press(){
   util_log("Button long Pressed");
   trigger_homeassistant_mqtt("button_long_pressed", "on");
}

void buttons_setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Make it harder for a double click to be detected as a click
  // should be 1 second ish.
  button.setClickTicks(1000);
  button.setPressTicks(2000);// just in case it clashes with above

  button.attachClick(on_click);
  button.attachDoubleClick(on_double_click);
  button.attachLongPressStart(on_long_press);
}

void buttons_loop() {
  button.tick();
}