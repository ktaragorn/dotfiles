#include "mqtt"
#include <MQTT.h>
#include <WiFiClient.h>

WiFiClient wifi;
MQTTClient client;

void mqtt_connect(){
	Serial.print("\nconnecting to mqtt...");
	// choosing not to loop since it is in the loop function anyway, and it isnt as importent yet.
	// while (!
	client.connect("nodemcu")
	// 	) {
	// 	Serial.print(".");
	// 	delay(1000);
	// }
}

void mqtt_setup(){
  	client.begin("pine64.middle.earth", 1883, wifi);
  	client.onMessage(mqtt_received);

 	mqtt_connect();
}

void mqtt_loop() {
  	client.loop();
  	if(!client.connected()) mqtt_connect();
}

void mqtt_publish(String &topic, String &message){
	client.publish(topic, message);
}

void mqtt_log(String &message) {
	mqtt_publish("/log/nodemcu", message);
}

void mqtt_received(String &topic, String &payload){

}