#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "common.h"
#include "ac_send.h"

const uint16_t kIrLed = D1;

void AcCommand::setMode(String mode) {
  if(mode == "cool") {
    this->mode = kDaikinCool;
  } else if(mode == "fan_only") {
    this->mode = kDaikinFan;
  } else if(mode == "off") {
    this->on = false;
  }
}

void AcCommand::setFan(String fan) {
  if(fan == "auto") {
    this->fan = kDaikinFanAuto;
  } else {
    int fanMode = fan.toInt();
    if(fanMode >=0 && fanMode <= 5){
      this->fan = fanMode;
    }
  }
}

String AcCommand::toString() {
  return "Command : state on? = " + String(this->on) +\
         " temperature = " + String(this->temperature) +\
         " fan = " + String(this->fan) +\
         " mode = " + String(this->mode);
}

void sendDaikinOnOffCommand(struct AcCommand command){
  IRDaikinESP ac(kIrLed);
  ac.begin();

  if(command.on){
    ac.on();
  }else{
    ac.off();
  }
  ac.setFan(command.fan);
  ac.setMode(command.mode);
  ac.setTemp(command.temperature);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  ac.send();

  util_log("Sent " + command.toString());
}

void sendAcOn(String tempArg, String fanArg){
  AcCommand command;
  if(tempArg.length() > 0) command.temperature = tempArg.toInt();
  if(fanArg.length() > 0) command.fan = fanArg.toInt();
  command.on = true;

  sendDaikinOnOffCommand(command);
}

void sendAcOff(){
  AcCommand command;
  command.on = false;
  sendDaikinOnOffCommand(command);
}
