#include "mqtt.h"
#include "homeassistant_mqtt.h"
#include "pir.h"

String current_state = "off";

const uint16_t pirPin = D2;

String getPirState() {
	const bool new_state = digitalRead(pirPin) == 1;
	const String pir_state = new_state? "on" : "off";
 	digitalWrite(LED_BUILTIN, !new_state? HIGH : LOW);
	return pir_state;
}

void pushNewState(String new_state){
	state_homeassistant_mqtt("pir_sensor", new_state);
	current_state = new_state;
	mqtt_log("Pir state = " + new_state);
}

void pir_setup(){
	pushNewState(getPirState());
}

void pir_loop(){
	const String new_state = getPirState();


	if(new_state != current_state){
		pushNewState(new_state);
	}
}