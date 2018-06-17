#include "I2ColorduinoMaster.h"

void setup()
{
  I2ColorduinoMaster.Init();
}


void loop()
{
  morphAndSendFrame();
}


void morphAndSendFrame() {
  I2ColorduinoMaster.plasma_morph();
  //I2ColorduinoMaster.showLogo();
  //byte * tmpp = (byte *)I2ColorduinoMaster.getDisplayBuffer();
  //memset(tmpp, 0, 6);
  I2ColorduinoMaster.sendNormalFrame();
}

