/* class_VEML6070-basics      
 *   2017-10-28  Zonker Harris
 * 
 * https://learn.adafruit.com/adafruit-si1145-breakout-board-uv-ir-visible-sensor/
 * https://github.com/adafruit/Adafruit_VEML6070
 * https://www.adafruit.com/product/992
 * 
 * i2c addresses are 0x38 and 0x39, and cannot be changed.  
 * Price was $6(US) as of this writing.
 * 
 * Pinouts:  VIN takes 3-5v   
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    ACK is the interrupt/alert output. You can set up the sensor to warn you 
 *    on overly high or low UV signal. Logic level of ACK is the same as whatever 
 *    you set Vin.
 *    
 * Serial Monitor and Serial Plotter cannot run at the same time. Pick only one!
 * 
 * The VEML6070 from Vishay has a true UV A light sensor. The Si1145 does 
 * UV Index approximations based on light level not true UV sensing.
 * The VEML6070 in contrast does have a real light sensor in the UV spectrum.
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include "Adafruit_VEML6070.h"

int UV = 0;
int measurement = 0;

//library_name  how_we_call_it
Adafruit_VEML6070 uv = Adafruit_VEML6070();

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.println("VEML6070 test, for UV intensity (not UV Index)");
  /* Next, you will call begin() in your setup procedure. There are 4 different 
   *  'integration' times used to calculate the intensity. The longer the 
   *  integration time, the more light is collected. Use shorter integration 
   *  times if you want to get measurements quickly with less precision. 
   *  Longer times will give you more precision, but of course, take longer!
        VEML6070_HALF_T  ~62.5ms
        VEML6070_1_T     ~125ms
        VEML6070_2_T     ~250ms
        VEML6070_4_T     ~500ms
   */
  uv.begin(VEML6070_4_T);
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
  // Read our sensor values   
  UV = uv.readUV();
   
  // and print the results on one line for graphing
  // Since we're using FLOATing point variables, print precision 
  //  to 2 decimal places is desired
  Serial.print ("Reading: "); Serial.print(measurement);
  Serial.print("\tUV: "); Serial.print(UV);
  Serial.println("\t(Note this is not UV index, its just UV A intensity!)"); 

  // Incerment our loop count (measurement). There are many ways to do this...
  // measurement = measurement + 1 (this method lets you count by twos, threes...)
  measurement++;   
  // The "++" means "increment by 1. Its why there is a C, and C++, but not C+.
  // you can read more at  https://www.arduino.cc/en/Reference/Arithmetic 
  
  // We add a pause of one second between readings.
  delay(1000);
  // You can make the delay shorter, if your reading will be changing quickly.
  //  (But doing fast measurements will quickly fill your log.)
  // If you want to graph the changes over a day, or week, consider a larger delay.
  //  (How many seconds in 5 minutes? How many in 30 minutes? How many in an hour?)
  // Remember that the delay function is in MILLISECONDS (1,000ths of a second).
}
