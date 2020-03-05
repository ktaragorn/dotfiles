#include <IRremoteESP8266.h>
#include <ir_Daikin.h>
#include <IRsend.h>
#include "common.h"

const uint16_t kIrLed = D1;

struct AcCommand {
  bool on;
  int temperature = 25;
  int fan = kDaikinFanAuto;
};

void sendDaikinOnOffCommand(AcCommand command){
  IRDaikinESP ac(kIrLed);
  ac.begin();

  if(command.on){
    ac.on();
  }else{
    ac.off();
  }
  ac.setFan(command.fan);
  ac.setMode(kDaikinCool);
  ac.setTemp(command.temperature);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  ac.send();

  if(command.on) {
    util_log("Sent turn on command with temperature = " + String(command.temperature) + " and fan = " + String(command.fan));
  }else{
    util_log("Sent off command");
  }
}

void sendAcOn(String tempArg = "25", String fanArg = "3"){
  AcCommand command;
  if(tempArg.length() > 0) command.temperature = tempArg.toInt();
  if(fanArg.length() > 0) command.fan = fanArg.toInt();

  sendDaikinOnOffCommand(command);
}

void sendAcOff(){
  AcCommand command;
  command.on = false;
  sendDaikinOnOffCommand(command);
}
