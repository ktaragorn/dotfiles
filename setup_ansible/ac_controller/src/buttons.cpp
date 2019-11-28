#include "buttons.h"
#include <OneButton.h>
#include "common.h"
#include "homeassistant_webhook.h"

const int buttonPin = D7;
OneButton button(buttonPin, false);

void on_click(){
   util_log("Button Pressed");
   trigger_homeassistant_webhook("button_pressed", "on");
}

void on_double_click(){
   util_log("Button double Pressed");
   trigger_homeassistant_webhook("button_double_pressed", "on");
}

void on_long_press(){
   util_log("Button long Pressed");
   trigger_homeassistant_webhook("button_long_pressed", "on");
}

void buttons_setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  button.attachClick(on_click);
  button.attachDoubleClick(on_double_click);
  button.attachLongPressStop(on_long_press);
}

void buttons_loop() {
  button.tick();
}