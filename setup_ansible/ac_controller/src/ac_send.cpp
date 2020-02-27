#include <IRremoteESP8266.h>
#include <ir_Daikin.h>
#include <IRsend.h>
#include "common.h"

const uint16_t kIrLed = D1;

void sendDaikinOnOffCommand(bool on, int temperature = 25, int fan = kDaikinFanAuto){
  IRDaikinESP ac(kIrLed);
  ac.begin();

  if(on){
    ac.on();
  }else{
    ac.off();
  }
  ac.setFan(fan);
  ac.setMode(kDaikinCool);
  ac.setTemp(temperature);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  ac.send();
}

void sendAcOn(String tempArg = "25", String fanArg = "3"){
  const String temp = tempArg ? tempArg : "25";
  const String fan = fanArg ? fanArg : "3";

  sendDaikinOnOffCommand(true, temp.toInt(), fan.toInt());
  util_log("Sent turn on command with temperature = " + temp + " and fan = " + fan);
}

void sendAcOff(){
  sendDaikinOnOffCommand(false);
  util_log("Sent turn off command");
}
