#include "I2ColorduinoMaster.h"


unsigned char I2ColorduinoMasterObject::addrs[I2C_DEVICE_COUNT_ACT] = {4, 5, 6, 7};

ColorRGB I2ColorduinoMasterObject::displayPixels[SCREENSIZE];

long I2ColorduinoMasterObject::paletteShift = 0;

unsigned char I2ColorduinoMasterObject::x_aaaaaa[32] = {
  0x43, 0x4D, 0x59, 0x61, 0x00, 0x7F, 0xFF, 0x3F,
  0x40, 0x4F, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
  0x00, 0x3F, 0xF0, 0xFF, 0x3E, 0x40, 0x40, 0x3F
};

unsigned char I2ColorduinoMasterObject::x_f29700[32] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41,
  0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char I2ColorduinoMasterObject::x_ed7700[32] = {
  0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1C, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00,
  0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char I2ColorduinoMasterObject::x_fcd00a[32] = {
  0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00,
  0x00, 0x00, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



ColorRGB * I2ColorduinoMasterObject::getDisplayBuffer() {
  return displayPixels;
}


void I2ColorduinoMasterObject::Init() {
  I2ColorduinoMaster.paletteShift = 128000;
  Wire.begin(DEST_I2C_ADDR_SELF); // join i2c bus (address optional for master)
}


void I2ColorduinoMasterObject::showLogo() {
  for (unsigned char i = 0; i < 32; i++) {
    for (unsigned char j = 0; j < 8; j++) {
      unsigned char _r = 0xff * (((x_aaaaaa[i] << j) & 0x80) >> 7) +  0xf2 * (((x_f29700[i] << j) & 0x80) >> 7) +  0xaa * (((x_ed7700[i] << j) & 0x80) >> 7) +  0xfc * (((x_fcd00a[i] << j) & 0x80) >> 7);
      unsigned char _g = 0xff * (((x_aaaaaa[i] << j) & 0x80) >> 7) +  0x97 * (((x_f29700[i] << j) & 0x80) >> 7) +  0x55 * (((x_ed7700[i] << j) & 0x80) >> 7) +  0xd0 * (((x_fcd00a[i] << j) & 0x80) >> 7);
      unsigned char _b = 0xff * (((x_aaaaaa[i] << j) & 0x80) >> 7) +  0x00 * (((x_f29700[i] << j) & 0x80) >> 7) +  0x00 * (((x_ed7700[i] << j) & 0x80) >> 7) +  0x10 * (((x_fcd00a[i] << j) & 0x80) >> 7);
      updateDisplayBuffer(i, j, _r, _g, _b);
    }
  }
}



//update display buffer using x,y,r,g,b format
void I2ColorduinoMasterObject::updateDisplayBuffer(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b) {
  unsigned char p = (y * SCREENSIZE_X) + x; //convert from x,y to pixel number in array
  displayPixels[p].r = r;
  displayPixels[p].g = g;
  displayPixels[p].b = b;
}


ColorRGB * getDisplayBuffer() {
  return I2ColorduinoMasterObject::displayPixels;
}


unsigned char * getDisplayBuffer_B() {
  return (unsigned char *)getDisplayBuffer;
}


void I2ColorduinoMasterObject::sendMessageViaIIC(unsigned char addr, unsigned char * info, unsigned char len) {
  Wire.beginTransmission(addr);
  Wire.write(START_OF_DATA);
  Wire.write(len);
  Wire.write(info, len - 1);
  Wire.write(END_OF_DATA);
  Wire.endTransmission();
}


void I2ColorduinoMasterObject::sendSlice(unsigned char _act, unsigned char _param, unsigned char _s, unsigned char _x, unsigned char _y) {
  /*
     _act = 0x01 means normally refreshing
     _s = No. of screens
     _x = starting point x
     _y = starting point y
  */
  unsigned char addr = addrs[_s];
  unsigned char tmpSlice[I2C_MESSAGE_LENGTH];
  unsigned char _p = 0;
  unsigned char _row = _y % PER_SCREENSIZE_Y;
  tmpSlice[_p++] = _act;
  tmpSlice[_p++] = _param | (_row << 4); //Add row number
  for (unsigned char _i = 0; _i < PER_SCREENSIZE_X; _i++) {
    tmpSlice[_p + _i * 3 + 0] = displayPixels[_x + _y * SCREENSIZE_X + _i].r;
    tmpSlice[_p + _i * 3 + 1] = displayPixels[_x + _y * SCREENSIZE_X + _i].g;
    tmpSlice[_p + _i * 3 + 2] = displayPixels[_x + _y * SCREENSIZE_X + _i].b;
  }
  sendMessageViaIIC(addr, tmpSlice, 30);
}


void I2ColorduinoMasterObject::sendNormalFrame() {
  unsigned char act = 0x01;
  unsigned char param = 0x83; //B10000011;
  unsigned char screen, x, y;
  //Slice matrix to 8-RGB-LEDs pieces
  for (unsigned char i = 0; i < SCREENCOUNT_Y; i++) {
    for (unsigned char j = 0; j < PER_SCREENSIZE_Y; j++) {
      for (unsigned char k = 0; k < SCREENCOUNT_X; k++) {
        sendSlice(act, param, i * SCREENCOUNT_X + k, k * PER_SCREENSIZE_X, i * PER_SCREENSIZE_Y + j);
      }
    }
  }
  unsigned char tmpSlice[I2C_MESSAGE_LENGTH] = {0x82};
  sendMessageViaIIC(0x00, tmpSlice, 2);
}

//plasma convert
//Converts an HSV color to RGB color
void I2ColorduinoMasterObject::HSVtoRGB(void *vRGB, void *vHSV)
{
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB = (ColorRGB *)vRGB;
  ColorHSV *colorHSV = (ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //if saturation is 0, the color is a shade of grey
  if (s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //if saturation > 0, more complex calculations are needed
  else
  {
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch (i)
    {
      case 0: r = v; g = t; b = p; break;
      case 1: r = q; g = v; b = p; break;
      case 2: r = p; g = v; b = t; break;
      case 3: r = p; g = q; b = v; break;
      case 4: r = t; g = p; b = v; break;
      case 5: r = v; g = p; b = q; break;
      default: r = g = b = 0; break;
    }
  }
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);
}


unsigned int I2ColorduinoMasterObject::RGBtoINT(void *vRGB)
{
  ColorRGB *colorRGB = (ColorRGB *)vRGB;

  return (((unsigned int)colorRGB->r) << 16) + (((unsigned int)colorRGB->g) << 8) + (unsigned int)colorRGB->b;
}


float I2ColorduinoMasterObject::dist(float a, float b, float c, float d)
{
  return sqrt((c - a) * (c - a) + (d - b) * (d - b));
}


void I2ColorduinoMasterObject::plasma_morph()
{
  unsigned char x, y;
  float value;
  ColorRGB colorRGB;
  ColorHSV colorHSV;

  for (y = 0; y < SCREENSIZE_Y; y++) {
    for (x = 0; x < SCREENSIZE_X; x++)
    {
      value = sin(dist(x + paletteShift, y, 128.0, 128.0) / 8.0)
              + sin(dist(x, y, 64.0, 64.0) / 8.0)
              + sin(dist(x, y + paletteShift / 7, 192.0, 64) / 7.0)
              + sin(dist(x, y, 192.0, 100.0) / 8.0);
      colorHSV.h = (unsigned char)((value) * 128) & 0xff;
      colorHSV.s = 255;
      colorHSV.v = 255;
      HSVtoRGB(&colorRGB, &colorHSV);
      //  Colorduino.SetPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
      updateDisplayBuffer(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
      //  displayPixels[y * 32 + x] = colorRGB;
    }
  }//end of for

  paletteShift++;
  // Colorduino.FlipPage(); // swap screen buffers to show it

  //update_display(DEST_I2C_ADDR);
  //update_display2(DEST_I2C_ADDR2);
  //update_display3(DEST_I2C_ADDR, DEST_I2C_ADDR2, DEST_I2C_ADDR3);
  //sendNormalFrame();
  //update_local_display();
}


// global instance
I2ColorduinoMasterObject I2ColorduinoMaster;
