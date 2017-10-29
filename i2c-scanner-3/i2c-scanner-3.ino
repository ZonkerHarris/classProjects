// --------------------------------------
// i2c-scanner-3
//
//   Found at http://playground.arduino.cc/Main/I2cScanner?action=sourceblock&num=1
//     26 OCT 2017  -- Adding the addresses from the Adafruit list
//          https://learn.adafruit.com/i2c-addresses/the-list
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

/* Adafruit Trinket Pro Clues!!!
 *    https://www.adafruit.com/products/2000
 *    https://learn.adafruit.com/introducing-pro-trinket
 *    https://learn.adafruit.com/adafruit-arduino-ide-setup/windows-setup#arduino-gemma-driver-installation
 *  Here's some things you may have to consider when adapting Arduino sketches:
 *  Pins #2 and #7 are not available (they are exclusively for USB)
 *  The onboard 5V regulator can only provide 150mA output, not 800mA out
 *  You cannot plug shields directly into the Pro Trinket
 *  There is no Serial-to-USB chip onboard. This is to keep the Pro Trinket small and inexpensive, 
 *   you can use any FTDI cable to connect to the FTDI port for a Serial connection. 
 *   The USB connection is for uploading new code only.
 *  The bootloader on the Pro Trinket use 4KB of FLASH so the maximum sketch size is 28,672 bytes. 
 *  The bootloader does not affect RAM usage.
 *  
 * Here's some handy specifications:
 *  ATmega328P onboad chip in QFN package
 *  16MHz clock rate, 28K FLASH available
 *  USB bootloader with a nice LED indicator looks just like a USBtinyISP so you can program it 
 *  with AVRdude and/or the Arduino IDE (with a few simple config modifications).
 *  
 * Make sure you select the USBtinyISP programmer, and pick the correct type/port of trinket
 *  You can only expect the Pseudo-serial debugger with the FTDI port...
 */
 
#include <Wire.h>

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("\nI2C Scanner v3");
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of the 
    //  Wire.endTransmission()  call to see if a device
    //  did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
      // Serial.print(address);  If needed, print the address in decimal
      //
      // Now, detail sensors that we know about or expect...
      if (address == 3) 
      {
        // DEC 3 = 0x03 HEX = AS3935 Lightning Sensor
        Serial.print(" - AS3935 Lightning Sensor");
      }
      if (address == 56) 
      {
        // DEC 56 = 0x38 HEX = VEML6070 UV Sensor
        Serial.print(" - VEML6070 UV Sensor");
      }
      if (address == 57) 
      {
        // DEC 57 = 0x39 HEX = VEML6070 UV Sensor
        Serial.print(" - VEML6070 UV Sensor");
      }
      if (address == 64) 
      {
        // DEC 64 = 0x40 HEX = HTU21D  Humidity and Temp Sensor
        Serial.print(" - HTU21D or Si7021 Humidity and Temp Sensor");
      }
      if (address == 96) 
      {
        // DEC 104 = 0x60 HEX = Si1145 Digital UV Index/IR/Visible Light Sensor
        Serial.print(" - Si1145 Digital UV Index/IR/Visible Light OR Baro Pressure/Temp Sensor");
      }
      if (address == 104) 
      {
        // DEC 104 = 0x68 HEX = DS1307 (Chrono-Dot?) RTC
        Serial.print(" - DS1307 RTC (Chrono-Dot?) or PCF8523 RTC");
      }
      if (address == 119) 
      {
        // DEC 119 = 0x77 HEX = BMP180 Barometric Pressure and Tem Sensor
        Serial.print(" - BMP180 Barometric Pressure and Tem Sensor");
      }
      Serial.println(" ");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
            if (address == 3) 
      {
        // DEC 3 = 0x03 HEX = AS3935 Lightning Sensor
        Serial.print(" - AS3935 Lightning Sensor");
      }
      if (address == 64) 
      {
        // DEC 64 = 0x40 HEX = HTU21D  Humidity and Temp Sensor
        Serial.print(" - HTU21D Humidity and Temp Sensor");
      }
      if (address == 104) 
      {
        // DEC 104 = 0x68 HEX = DS1307 (Chrono-Dot?) RTC
        Serial.print(" - DS1307 RTC (Chrono-Dot?)");
      }
      if (address == 119) 
      {
        // DEC 119 = 0x77 HEX = BMP180 Barometric Pressure and Tem Sensor
        Serial.print(" - BMP180 Barometric Pressure and Tem Sensor");
      }
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
/* Scanning...
 * I2C device found at address 0x03 - AS3935 Lightning Sensor 
 * I2C device found at address 0x40 - HTU21D Humidity and Temp Sensor 
 * I2C device found at address 0x68 - DS1307 RTC (Chrono-Dot?) 
 * I2C device found at address 0x77 - BMP180 Barometric Pressure and Tem Sensor 
 * done
 */

 /*
Special cases:
PCA9685 can be used by any address between 0x40 to 0x7F
0x00 - 0x07 and 0x78 - 0x7F are reserved I2C addresses

0x00 - 0x0F

0x00 - Reserved - General Call Address

0x01 - Reserved for CBUS Compatibility

0x02 - Reserved for I2C-compatible Bus Variants

0x03 - Reserved for Future Use

0x04, 0x05, 0x06, 0x07 - Reserved for Hs-mode Master

0x0E
MAG3110 3-Axis Magnetometer (0x0E only)

0x10 - 0x1F

0x11
Si4713 FM Transmitter with RDS (0x11 or 0x63)

0x13
VCNL40x0 proximity sensor (0x13 only)

0x18
MCP9808 temp sensor (0x18 - 0x1F)
LIS3DH 3-axis accelerometer (0x18 or 0x19)
LSM303 Accelerometer & Magnetometer (0x19 for accelerometer and 0x1E for magnetometer)

0x19
MCP9808 temp sensor (0x18 - 0x1F)
LIS3DH 3-axis accelerometer (0x18 or 0x19)

0x1A
MCP9808 temp sensor (0x18 - 0x1F)

0x1B
MCP9808 temp sensor (0x18 - 0x1F)

0x1C
MCP9808 temp sensor (0x18 - 0x1F)
MMA845x 3-axis Accelerometer (0x1C or 0x1D)
FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)
MMA7455L (0x1C or 0x1D)

0x1D
MCP9808 temp sensor (0x18 - 0x1F)
MMA845x 3-axis Accelerometer (0x1C or 0x1D)
FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)
LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)
ADXL345 3-axis accelerometer (0x1D or 0x53)
MMA7455L (0x1C or 0x1D)

0x1E
MCP9808 temp sensor (0x18 - 0x1F)
FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)
LSM303 Accelerometer & Magnetometer (0x19 for accelerometer and 0x1E for magnetometer)
LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)
HMC5883 Magnetometer (0x1E only)

0x1F
MCP9808 temp sensor (0x18 - 0x1F)
FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)

0x20-0x2F

0x20
FXAS21002 Gyroscope (0x20 or 0x21)
Chirp! Water sensor (0x20)
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x21
FXAS21002 Gyroscope (0x20 or 0x21)
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x22
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x23
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x24
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x25
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x26
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x27
MCP23008 I2C GPIO expander (0x20 - 0x27)
MCP23017 I2C GPIO expander (0x20 - 0x27)

0x28
BNO055 IMU (0x28 or 0x29)
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x29
BNO055 IMU (0x28 or 0x29)
TCS34725 color sensor (0x29 only)
TSL2561 light sensor (0x29, 0x39 or 0x49)
TSL2591 light sensor (0x29 only)
VL53L0x ToF distance (0x29, software selectable)
VL6180X ToF sensor (0x29)
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x2A
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x2B
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x2C
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x2D
CAP1188 8-channel Capacitive Touch (0x28 - 0x2D )

0x30 - 0x3F

0x34

0x38
VEML6070 UV Index (0x38 and 0x39)
FT6x06 Capacitive Touch Driver (0x38 only)

0x39
TSL2561 light sensor (0x29, 0x39 or 0x49)
VEML6070 UV Index (0x38 and 0x39)
APDS-9960 IR/Color/Proximity Sensor (0x39 only)

0x3C
SSD1305 monochrome OLED (0x3C or 0x3D, hardware selectable on some displays with a solder connection)
SSD1306 monochrome OLED (0x3C or 0x3D, hardware selectable on some displays with a solder connection)

0x3D
SSD1305 monochrome OLED (0x3C or 0x3D, hardware selectable on some displays with a solder connection)
SSD1306 monochrome OLED (0x3C or 0x3D, hardware selectable on some displays with a solder connection)

0x40 - 0x4F

0x40
Si7021 Humidity/Temp sensor (0x40 only)
HTU21D-F Humidity/Temp Sensor (0x40 only)
HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
PCA9685 16-channel PWM driver default address (0x40 - 0x7F)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x41
HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)
STMPE610/STMPE811 Resistive Touch controller (0x41 or 0x44)

0x42
HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x43
HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x44
SHT31 Humidity/Temp sensor (0x44 or 0x45 selectable)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
ISL29125 Color Sensor (0x44 only)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)
STMPE610/STMPE811 Resistive Touch controller (0x41 or 0x44)

0x45
SHT31 Humidity/Temp sensor (0x44 or 0x45 selectable)
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x46
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x47
TMP007 IR Temperature sensor (0x40 - 0x47)
TMP006 IR Temperature sensor (0x40 - 0x47)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x48
TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)
PN532 NFC/RFID reader (0x48 only)
ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x49
TSL2561 light sensor (0x29, 0x39 or 0x49)
TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)
ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4A
TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)
ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4B
TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)
ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4C
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4D
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4E
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x4F
INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)

0x50 - 0x5F

0x50
MB85RC I2C FRAM (0x50 - 0x57)

0x51
MB85RC I2C FRAM (0x50 - 0x57)

0x52
MB85RC I2C FRAM (0x50 - 0x57)
Nintendo Nunchuck controller (0x52 only)

0x53
ADXL345 3-axis accelerometer (0x1D or 0x53)
MB85RC I2C FRAM (0x50 - 0x57)

0x54
MB85RC I2C FRAM (0x50 - 0x57)

0x55
MB85RC I2C FRAM (0x50 - 0x57)

0x56
MB85RC I2C FRAM (0x50 - 0x57)

0x57
MB85RC I2C FRAM (0x50 - 0x57)
MAX3010x Pulse & Oximetry sensor (0x57)

0x58
TPA2016 I2C-controlled Amplifier (0x58 only)

0x5A
MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)
CCS811 VOC sensor (0x5A or 0x5B)
MLX9061x IR temperature sensor (0x5A only)
DRV2605 Haptic Motor Driver (0x5A only)

0x5B
MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)
CCS811 VOC sensor (0x5A or 0x5B)

0x5C
AM2315 Humidity/Temp sensor (0x5C only)
MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)

0x5D
MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)

0x60 - 0x6F

0x60
MPL115A2 Barometric Pressure (0x60 only)
MPL3115A2 Barometric Pressure (0x60 only)
Si5351A Clock Generator (0x60 or 0x61)
Si1145 Light/IR Sensor (0x60 only)
MCP4725A0 12-bit DAC (0x60 or 0x61)
TEA5767 Radio receiver (0x60 only)

0x61
Si5351A Clock Generator (0x60 or 0x61)
MCP4725A0 12-bit DAC (0x60 or 0x61)

0x62
MCP4725A1 12-bit DAC (0x62 or 0x63)

0x63
MCP4725A1 12-bit DAC (0x62 or 0x63)
Si4713 FM Transmitter with RDS (0x11 or 0x63)

0x64
MCP4725A2 12-bit DAC (0x64 or 0x65)

0x65
MCP4725A2 12-bit DAC (0x64 or 0x65)

0x66
MCP4725A3 12-bit DAC (0x66 or 0x67)

0x67
MCP4725A3 12-bit DAC (0x66 or 0x67)

0x68
This address is really popular with real time clocks, almost all of them use 0x68!
AMG8833 IR Thermal Camera Breakout (0x68 or 0x69)
DS1307 RTC (0x68 only)
PCF8523 RTC (0x68 only)
DS3231 RTC (0x68 only)
MPU-9250 9-DoF IMU (0x68 or 0x69)
MPU-60X0 Accel+Gyro (0x68 or 0x69)
ITG3200 Gyro (0x68 or 0x69)

0x69
AMG8833 IR Thermal Camera Breakout (0x68 or 0x69)
MPU-9250 (0x68 or 0x69)
MPU-60X0 Accel+Gyro (0x68 or 0x69)
ITG3200 Gyro (0x68 or 0x69)

0x6A
L3GD20H gyroscope (0x6A or 0x6B)
LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)

0x6B
L3GD20H gyroscope (0x6A or 0x6B)
LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)

0x70 - 0x7F

0x70
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x71
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x72
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x73
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x74
IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x75
IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)

0x76
BME280 Temp/Barometric/Humidity (0x76 or 0x77)
BMP280 Temp/Barometric (0x76 or 0x77)
IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)
MS5607/MS5611 Barometric Pressure (0x76 or 0x77)

0x77
BME280 Temp/Barometric/Humidity (0x76 or 0x77)
BMP280 Temp/Barometric (0x76 or 0x77)
BMP180 Temp/Barometric (0x77 only)
BMP085 Temp/Barometric (0x77 only)
TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)
IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)
HT16K33 LED Matrix Driver (0x70 - 0x77)
BMA180 Accelerometer (0x77 only)
MS5607/MS5611 Barometric Pressure (0x76 or 0x77)

0x78 0x79 0x7A 0x7B - Reserved for 10-bit I2C Addressing

0x7C 0x7D 0x7E 0x7F - Reserved for Future Purposes
  */
