/* class_Si1145-basics      
 *   2017-10-28  Zonker Harris
 * 
 * https://learn.adafruit.com/adafruit-si1145-breakout-board-uv-ir-visible-sensor/
 * https://github.com/adafruit/Adafruit_MPL115A2
 * https://www.adafruit.com/product/992
 * 
 * i2c address is 0x60, and cannot be changed.  
 * Price was $10(US) as of this writing.
 * 
 * Pinouts:  Vin takes 3-5v   3o is the *output* of the 3.3v regulator on the board!
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    LED is an open-collector input, which will briefly light an off-board IR LED,
 *      which can be used for proximity detection!  Serial.println(uv.readProx())
 *    INT is a signal for your controller to say "I have a new reading for you", 
 *      but the Adafruit library doesn't (yet) support that feature.
 *    
 * The Si1145 does UV Index approximations based on light level not true UV sensing.
 *  (The VEML6070 in contrast does have a real light sensor in the UV spectrum.)
 * Serial Monitor and Serial Plotter cannot run at the same time. Pick only one!
 * 
 * This is from the Adafruit example sketch. It doesn't read the sensor into a 
 *  variable, but rather it simply prints the variable. 
Serial.print("Vis: "); Serial.println(uv.readVisible());
Serial.print("IR: "); Serial.println(uv.readIR());
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include "Adafruit_SI1145.h"

float UVindex = 0;
int visual = 0;
int IR = 0;
int UV = 0;
int measurement = 0;

//library_name  how_we_call_it
Adafruit_SI1145 SI1145 = Adafruit_SI1145();

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.println("SI1145 test, for UV Index, Visual spectrum, and IR light");
  SI1145.begin();
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
   */  
   
  UV = SI1145.readUV();
  visual = SI1145.readVisible();
  IR = SI1145.readIR();
  // the UV index is multiplied by 100, so to get the
  // integer index, divide by 100!
  UVindex = UV / 100.0;  
  
  // and print the results on one line for graphing
  // Since we're using FLOATing point variables, print precision 
  //  to 2 decimal places is desired
  Serial.print ("Reading: "); Serial.print(measurement);
  Serial.print("\tVisual: "); Serial.print(visual);
  Serial.print("\tIR: "); Serial.print(IR);
  Serial.print("\t  UV: "); Serial.print(UV);
  Serial.print("\t  UV Index: "); Serial.println(UVindex, 2);

  /* The UV Index number 
   *  0 to 2: Low
        A UV Index reading of 0 to 2 means low danger from the sun's 
        UV rays for the average person. Wear sunglasses on bright days.
        If you burn easily, cover up and use broad spectrum SPF 30+ sunscreen.
        Watch out for bright surfaces, like sand, water and snow, which 
        reflect UV and increase exposure.

   * 3 to 5: Moderate
        A UV Index reading of 3 to 5 means moderate risk of harm from 
        unprotected sun exposure. Stay in shade near midday when the 
        sun is strongest.
        If outdoors, wear protective clothing, a wide-brimmed hat, 
        and UV-blocking sunglasses.
        Generously apply broad spectrum SPF 30+ sunscreen every 2 hours, 
        even on cloudy days, and after swimming or sweating. 
        Watch out for bright surfaces, like sand, water and snow, 
        which reflect UV and increase exposure.

   * 6 to 7: High
        A UV Index reading of 6 to 7 means high risk of harm from 
        unprotected sun exposure. Protection against skin and eye 
        damage is needed. Reduce time in the sun btwn 10 a.m. and 4 p.m.
        If outdoors, seek shade and wear protective clothing, a 
        wide-brimmed hat, and UV-blocking sunglasses.
        Generously apply broad spectrum SPF 30+ sunscreen every 
        2 hours, even on cloudy days, and after swimming or sweating. 
        Watch out for bright surfaces, like sand, water and snow, 
        which reflect UV and increase exposure.

   * 8 to 10: Very High
        A UV Index reading of 8 to 10 means very high risk of harm 
        from unprotected sun exposure. Take extra precautions because 
        unprotected skin and eyes will be damaged and can burn quickly.
        Minimize sun exposure between 10 a.m. and 4 p.m.
        If outdoors, seek shade and wear protective clothing, a 
        wide-brimmed hat, and UV-blocking sunglasses.
        Generously apply broad spectrum SPF 30+ sunscreen every 
        2 hours, even on cloudy days, and after swimming or sweating. 
        Watch out for bright surfaces, like sand, water and snow, 
        which reflect UV and increase exposure.

   * 11 or more: Extreme
        A UV Index reading of 11 or more means extreme risk of harm 
        from unprotected sun exposure. Take all precautions because 
        unprotected skin and eyes can burn in minutes. 
  */
  
  // Increment our loop count "measurement". There are many ways to do this...
  // measurement = measurement + 1 (this method lets you count by twos, threes...)
  measurement++;   
  // The "++" means "increment by 1. Its why there is a C, and C++, but not C+.
  // you can read more at  https://www.arduino.cc/en/Reference/Arithmetic 
  
  // We add a pause of one second between readings
  delay(1000);
  // You can make the delay shorter, if your reading will be changing quickly.
  //  (But doing fast measurements will quickly fill your log.)
  // If you want to graph the changes over a day, or a week, consider a larger delay.
  //  (How many seconds in 5 minutes? How many in 30 minutes? How many in an hour?)
  // Remember that the delay function is in MILLISECONDS (1,000ths of a second).
}
