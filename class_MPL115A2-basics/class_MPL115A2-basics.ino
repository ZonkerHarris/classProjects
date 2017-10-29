/* class_MPL115A2-basics      
 *   2017-10-28  Zonker Harris
 * 
 * https://github.com/adafruit/Adafruit_MPL115A2
 * https://www.adafruit.com/product/992
 * https://cdn-shop.adafruit.com/datasheets/MPL115A2.pdf
 *   (Read the datasheet for more info about what the chip can do.)
 * 
 * At 1.5 hPa resolution, the MPL115A2 is not as precise as the BMP085, 
 * which has up to 0.03 hPa resolution so we don't suggest the MPL115A2 
 * as a precision altimeter.
 * 
 * i2c address is 0x60, and cannot be changed.  
 * Price was $8(US) as of this writing.
 * 
 * Pinouts:  Vin takes 3-5v   
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    SDWN will Shutdown (disable) the board when you connect it to ground
 *      (You can use this to selectively enable different boards on one address.)
 *    RST when pulled to Ground, will disable the I2C communications.
 *    
 * Serial Monitor and Serial Plotter cannot run at the same time. Pick only one!
 * 
 * This is from Adafruit's example sketch. It doesn't read the sensor into a variable,
 *  but rather it simply prints the variable. Notice it calls the funtion, and tells
 *  which variable will receive the first and second results!
mpl115a2.getPT(&pressureKPA,&temperatureC);
Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.print(" kPa  ");
Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C both measured together");
 * 
 * Since I wanted to also show the Fahrenheit value, I needed to get the Celsius 
 * value and calculate.
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include "Adafruit_MPL115A2.h"

float pressureKPA = 0;
float pressureHG = 0;
float tempC = 0;
float tempF = 0;
int measurement = 0;

//library_name  how_we_call_it
Adafruit_MPL115A2 mpl115a2;

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.print("MPL115A2 test, Getting the Barometric Pressure and Temp");
  mpl115a2.begin();
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
  // Read our sensor values
  /*  When you call a function you can also send it info when you call...
   *   that is why you have the parenthesis. In these cases, we are not
   *   sending any data. 
   *  Every function will return some value. If nothing else, it will return
   *   a "true" if it ran without an error. In these cases, calling each 
   *   variable will return a floating point number representing the desired
   *   environmental reading.
   *   
   * The Barometric Pressure is read out in KPA (KiloPascals)
   *  The pascal (symbol Pa) is the SI unit of pressure. It is equivalent to 
   *  one newton per square metre. The unit is named after Blaise Pascal, the 
   *  eminent French mathematician, physicist and philosopher.
   * 
   * You can find a conversion chart here, to other pressure scales...
   * http://www.windandweather.com/mplace_assets/images/pdfs/barometric_pressure_kilopascals.pdf 
   * 
   * In the US, we're often used to weather reports in "Inches of Mercury"...
   *  Inches of mercury or inHg is a non-SI unit for pressure. It is still widely 
   *  used for barometric pressure in weather reports and aviation in the U.S., 
   *  but is considered somewhat outdated elsewhere.
   * It is defined as the pressure exerted by a column of mercury of 1 inch in 
   *  height at 32 °F (0 °C) at the standard acceleration of gravity.
   *
   *    1 inHg = 3,386.389 pascals at 0 °C.
   *    1 pascal is equal to 0.001 kpa, or 0.00029529983071445 inhg.
   *    
   * Aircraft operating at higher altitudes (above 18,000 feet) set their 
   * barometric altimeters to a standard pressure of 29.92 inHg or 1,013.2 hPa 
   * (1 hPa = 1 mbar) regardless of the actual sea level pressure, with 
   * inches of mercury used in the U.S. and Canada. The resulting altimeter 
   * readings are known as flight levels.
   */  
  pressureKPA = mpl115a2.getPressure(); 
  tempC = mpl115a2.getTemperature();

  // We can only get pressure in KiloPascals, but want Inches Mercury
  pressureHG = (pressureKPA * 0.29529983071445);
  
  // We can only get temperature in Celsius from this sensor.
  // Do some math on the Celsius reading to get the Fahrenheit value
  tempF = (tempC * 9.0 / 5.0) + 32.0;
  
  // and print the results on one line for graphing
  // Since we're using FLOATing point variables, print precision to 2 decimal places
  Serial.print ("Reading: "); Serial.print(measurement);
  Serial.print("\tPressure (kPa): "); Serial.print(pressureKPA, 2);
  Serial.print("\t(InHg): "); Serial.print(pressureHG, 2);
  Serial.print("\tTemperature C: "); Serial.print(tempC, 2);
  Serial.print("\tTemperature F: "); Serial.println(tempF, 2);
  // Try changing the precision number to 4, to see the difference.

  // Increment our loop count (measurement). There are many ways to do this...
  // measurement = measurement + 1  (this lets you count by twos, threes, Xs)
  measurement++;   
  // The "++" means "increment by 1. Its why there is a C, and C++, but not C+.
  // you can read more at  https://www.arduino.cc/en/Reference/Arithmetic 
  
  // We add a pause of one second between readings
  delay(1000);
  // You can make the delay shorter, if your reading will be changing quickly.
  //  (But doing fast measurements will quickly fill your log.)
  // If you want to graph the changes over a day, or a week, consider a much larger delay.
  //  (How many seconds in 5 minutes? How many in 30 minutes? How many in an hour?)
  // Remember that the delay function is in MILLISECONDS (1,000ths of a second).
}
