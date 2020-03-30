#include <ir_Daikin.h>
struct AcCommand {
  bool on = false;
  int temperature = 25;
  int fan = kDaikinFanAuto;
  int mode = kDaikinCool;
  int swing = kDaikinSwingOff;

  void setSwing(String swing);
  void setMode(String mode);
  void setFan(String fan);
  String toString();
};

void sendDaikinOnOffCommand(struct AcCommand command);
void sendAcOn(String tempArg = "25", String fanArg = "3");
void sendAcOff();