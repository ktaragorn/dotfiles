#include "buttons.h"
#include <OneButton.h>
#include <pins_arduino.h>
#include "homeassistant_webhook.h"

const int buttonPin = D7;
OneButton button(buttonPin, false);

void on_click(){
   Serial.println("Button Pressed");
   trigger_homeassistant_webhook("button_pressed", "on");
}

void on_double_click(){
   Serial.println("Button double Pressed");
   trigger_homeassistant_webhook("button_double_pressed", "on");
}

void on_long_press(){
   Serial.println("Button long Pressed");
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