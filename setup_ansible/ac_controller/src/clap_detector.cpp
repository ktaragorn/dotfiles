#include "clap_detector.h"
#include "mqtt.h"
#include <VolAnalyzer.h>
#include <Clap.h>

VolAnalyzer vol(A0);
Clap clap;

void clap_setup() {

}

void clap_loop() {
  vol.tick();
  clap.tick(vol.getRawMax());
  mqtt_log("Analog: "+ String(analogRead(A0))+", analyser: " + vol.getRawMax());
  if (clap.isClap()) mqtt_log("clap");
  if (clap.hasClaps(1)) mqtt_log("1 clap");
  if (clap.hasClaps(2)) mqtt_log("2 clap");
}