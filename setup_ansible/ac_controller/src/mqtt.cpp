#include <MQTT.h>
#include <WiFiClient.h>
#include "ac_send.h"
#include "mqtt.h"

WiFiClient wifi;
MQTTClient client;
struct AcCommand command;
unsigned long syncAt = 0;

void mqtt_connect(){
	Serial.print("\nconnecting to mqtt...");
	// choosing not to loop since it is in the loop function anyway, and it isnt as importent yet.
	// while (!
	client.connect(HOSTNAME);
	// 	) {
	// 	Serial.print(".");
	// 	delay(1000);
	// }

	// moving this to the connect method since persistent sessions doesnt seem to work
	// and the lib demo does this anyway
  	client.subscribe(HOSTNAME + String("/ac/+"));
}

void mqtt_setup(){
  	client.begin("pine64.middle.earth", 1883, wifi);
  	client.onMessage(mqtt_received);
  	// TODO should be available in the next version after 2.4.7
  	// client.setCleanSession(false);

  	client.setOptions(10, false, 1000);

 	mqtt_connect();
}

void mqtt_loop() {
  	client.loop();
  	if(!client.connected()) mqtt_connect();
  	if(syncAt != 0 && millis() > syncAt) {
  		sendDaikinOnOffCommand(command);
  		syncAt = 0;
  	}
}

void mqtt_publish(String &topic, String &message, bool retained){
	client.publish(topic, message, retained, 0);
}

void mqtt_log(String message) {
	String topic = "log/" + String(HOSTNAME);
	mqtt_publish(topic, message);
}

void mqtt_received(String &topic, String &payload){
	mqtt_log("Message Recieved on topic " + topic +", message - " + payload);
	if(topic.endsWith("power")) {
		command.on = payload == "ON";
	}else if(topic.endsWith("mode")) {
		command.setMode(payload);
	}else if(topic.endsWith("fan")) {
		command.setFan(payload);
	}else if(topic.endsWith("temperature")) {
		command.temperature = payload.toInt();
	}else if(topic.endsWith("swing")) {
		command.setSwing(payload);
	}else{
		mqtt_log("Unexpected topic " + topic);
		return;
	}
	if(syncAt == 0){
		syncAt = millis() + 500;
	}
}