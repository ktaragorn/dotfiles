#include "ir_receive.h"
#include "homeassistant_webhook.h"
#include <pins_arduino.h>
#include <IRrecv.h>
#include <IRutils.h>
const uint16_t kRecvPin = D5;

IRrecv irrecv(kRecvPin);

void ir_receive_setup() {
  irrecv.enableIRIn();  // Start the receiver
}

void ir_receive_loop(){
  decode_results results;

  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    trigger_homeassistant_webhook("tv_remote_detected", uint64ToString(results.value, HEX));
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
