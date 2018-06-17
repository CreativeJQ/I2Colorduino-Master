#ifndef _COLORDUINO_I2C_M_
#define _COLORDUINO_I2C_M_

#include <EEPROM.h>
#include <Wire.h>
#include <math.h>

#define START_OF_DATA 0x10       //data markers
#define END_OF_DATA 0x20         //data markers
#define DEST_I2C_ADDR_SELF 1     //set destination I2C address (must match firmware in Colorduino module)

#define PER_SCREENSIZE_X 8        //num of ROW_LEDs on single screen
#define PER_SCREENSIZE_Y 8        //num of COLUMN_LEDs on single screen
#define PER_SCREENSIZE   PER_SCREENSIZE_X * PER_SCREENSIZE_Y        //num of COLUMN_LEDs on single screen
#define SCREENCOUNT_X    4        //num of ROW_LED_SCREENs
#define SCREENCOUNT_Y    1        //num of COLUMN_LED_SCREENs
#define SCREENCOUNT      SCREENCOUNT_X * SCREENCOUNT_Y        //num of LED_SCREENs
#define SCREENSIZE_X     SCREENCOUNT_X * PER_SCREENSIZE_X       //num of ROW_LEDs
#define SCREENSIZE_Y     SCREENCOUNT_Y * PER_SCREENSIZE_Y       //num of COLUMN_LEDs
#define SCREENSIZE       SCREENCOUNT * PER_SCREENSIZE      //num of LED_LIGHTs

#define I2C_MESSAGE_LENGTH    32      //I2C message buffer = 32byte
#define I2C_DEVICE_COUNT_MAX  120     //I2C message buffer = 32byte
#define I2C_DEVICE_COUNT_ACT  SCREENCOUNT     //I2C message buffer = 32byte

//setup for plasma
typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

class I2ColorduinoMasterObject {
  public:

    static unsigned char addrs[];

    static ColorRGB displayPixels[];

    static unsigned char x_aaaaaa[];

    static unsigned char x_f29700[];

    static unsigned char x_ed7700[];

    static unsigned char x_fcd00a[];

    static long paletteShift;

    static ColorRGB * getDisplayBuffer();
    static unsigned char * getDisplayBuffer_B();

    void Init();

    void showLogo();

    //update display buffer using x,y,r,g,b format
    void updateDisplayBuffer(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);

    void sendMessageViaIIC(unsigned char addr, unsigned char * info, unsigned char len);


    void sendSlice(unsigned char _act, unsigned char _param, unsigned char _s, unsigned char _x, unsigned char _y);


    void sendNormalFrame();

    //plasma convert
    //Converts an HSV color to RGB color
    void HSVtoRGB(void *vRGB, void *vHSV);

    unsigned int RGBtoINT(void *);

    float dist(float a, float b, float c, float d);

    void plasma_morph();
};

extern I2ColorduinoMasterObject I2ColorduinoMaster;



#endif // _COLORDUINO_I2C_M_
