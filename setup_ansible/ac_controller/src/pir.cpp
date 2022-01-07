#include "mqtt.h"
#include "homeassistant_mqtt_trigger.h"
#include "pir.h"

bool current_state = false;

const uint16_t pirPin = D2;

void pir_setup(){

}

void pir_loop(){
	bool new_state = digitalRead(pirPin) == 1;

	if(new_state != current_state){
		const String pir_state = new_state? "on" : "off";
		trigger_homeassistant_mqtt("pir_sensor", pir_state);
		mqtt_log("Pir state = " + pir_state);
		current_state = new_state;
	}
}