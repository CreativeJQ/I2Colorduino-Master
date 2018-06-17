# I2Colorduino-Master
## How to use
### Overall
- Copy I2Colorduino-Master.ino, I2ColorduinoMaster.h, I2ColorduinoMaster.cpp to folder I2Colorduino-Master
- Open I2Colorduino-Master.ino with Arduino
- Connect your Arduino like device with usb cable
- Upload sketch
- Pin up I2C slave devices(Colorduino like devices, with I2Colorduino-Slave DEMO running) to see demo
### Protocol Explain (Master -> Slave)
- I2C_START
- 0x10 // START_OF_DATA
- 0xZZ // ZZ stands for LENGTH of MSG body, including this byte itself
- INFO[0xZZ - 1] // MSG - INFO body
- 0x20 // END_OF_DATA
### INFO Explain
| byte 0 | byte 1 | byte 2 ... |
| :----- | :----- | :--------- |
| Action | Param  | Pixels[8] R-G-B |
## Basic configuration
### EEPROM
- currently no need to operate EEPROM
### I2C
- Master GND - Slave GND
- Master SCL - Slave SCL
- Master SDA - Slave SDA
- Master 5V \ Slave 5V (can be supplied separately)
