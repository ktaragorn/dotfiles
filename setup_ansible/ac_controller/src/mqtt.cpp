#include <MQTT.h>
#include <WiFiClient.h>
#include "ac_send.h"
#include "mqtt.h"

WiFiClient wifi;
MQTTClient client;
struct AcCommand command;

void mqtt_connect(){
	Serial.print("\nconnecting to mqtt...");
	// choosing not to loop since it is in the loop function anyway, and it isnt as importent yet.
	// while (!
	client.connect("nodemcu");
	// 	) {
	// 	Serial.print(".");
	// 	delay(1000);
	// }

	// moving this to the connect method since persistent sessions doesnt seem to work
	// and the lib demo does this anyway
  	client.subscribe("nodemcu/ac/+");
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
}

void mqtt_publish(String topic, String &message){
	client.publish(topic, message);
}

void mqtt_log(String &message) {
	mqtt_publish("log/nodemcu", message);
}

void mqtt_received(String &topic, String &payload){
	mqtt_log("Message Recieved on topic " + topic +", message - " + payload);
	if(topic.endsWith("power")) {
		command.on = payload == "ON";
		return; // we get this on top of another message
	}else if(topic.endsWith("mode")) {
		command.setMode(payload);
	}else if(topic.endsWith("fan")) {
		command.setFan(payload);
	}else if(topic.endsWith("temperature")) {
		command.temperature = payload.toInt();
	}else{
		mqtt_log("Unexpected topic " + topic);
		return;
	}
	sendDaikinOnOffCommand(command);
}