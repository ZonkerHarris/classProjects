/* class_Si7021-basics      
 *   2017-10-27  Zonker Harris
 *   
 * https://learn.adafruit.com/adafruit-si7021-temperature-plus-humidity-sensor/overview 
 * https://github.com/adafruit/Adafruit_Si7021 
 * https://www.adafruit.com/products/3251 
 * 
 * i2c address is 0x40, and cannot be changed.  Price was $7(US) as of this writing.
 * 
 * Pinouts:  Vin takes 3-5v   3o is the *output* of the 3.3v regulator on the board!
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    
 * Serial Monitor and Serial Plotter cannot run at the same time. Pick only one!
 * 
 * This is from the Adafruit example sketch. It doesn't read the sensor into a variable,
 *  but rather it simply prints the variable.
 *  
 Serial.print("Humidity:    "); Serial.print(sensor.readHumidity(), 2);
 Serial.print("\tTemperature C: "); Serial.print(sensor.readTemperature(), 2);
 * 
 * Since I wanted to also show the Fahrenheit value, I need to get the C value and calculate.
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include "Adafruit_Si7021.h"

float humidity = 0;
float tempC = 0;
float tempF = 0;
int measurement = 0;
uint32_t sernum_a, sernum_b;

Adafruit_Si7021 sensor = Adafruit_Si7021();

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.print("Si7021 test");
  sensor.begin();
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
  // Incerment our loop count (measurement). There are many ways to do this...
  // measurement = measurement + 1  (this lets you count by twos, threes, Xs)
  measurement++;   
  // The "++" means "increment by 1. Its why there is a C, and C++, but not C+.
  // you can read more at  https://www.arduino.cc/en/Reference/Arithmetic 
  
  // Read our sensor values
  /*  When you call a function you can also send it info when you call...
   *   that is why you have the parenthesis. In these cases, we are not
   *   sending any data. 
   *  Every function will return some value. If nothing else, it will return
   *   a "true" if it ran without an error. In these cases, calling each 
   *   variable will return a floating point number representing the desired
   *   environmental reading.
   */  
  humidity = sensor.readHumidity();
  tempC = sensor.readTemperature();
    
  // We can only get temperature in Celsius from thsi sensor.
  // Do some math on the Celsius reading to get the Fahrenheit value
  tempF = (tempC * 9.0 / 5.0) + 32.0;
  
  // and print the results on one line for graphing
  // Since we're using FLOATing point variables, print precision to 2 decimal places
  Serial.print ("Reading: "); Serial.print(measurement);
  Serial.print("\tHumidity: \t"); Serial.print(humidity, 2);
  Serial.print("\tTemperature C:\t"); Serial.print(tempC, 2);
  Serial.print("\tTemperature F:\t"); Serial.println(tempF, 2);
  // Try changing the precision number to 5, to see the difference

  // We add a pause of one second between readings
  delay(1000);
  // You can make the delay shorter, if your reading will be changing quickly.
  //  (But doing fast measurements will quickly fill your log.)
  // If you want to graph the changes over a day, or a week, consider a much larger delay.
  //  (How many seconds in 5 minutes? How many in 30 minutes? How many in an hour?)
  // Remember that the delay function is in MILLISECONDS (1,000ths of a second).
}
