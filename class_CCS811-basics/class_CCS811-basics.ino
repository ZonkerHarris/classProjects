/* class_CCS811-VOC-basics      
 *   2017-10-29  Zonker Harris
 * 
 * https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor 
 * https://www.adafruit.com/product/3566 
 * https://cdn-shop.adafruit.com/product-files/3566/3566_datasheet.pdf 
 * 
 * Please note, this sensor, like all VOC/gas sensors, has variability and to
 * get precise measurements you will want to calibrate it against known sources!
 * 
 * This sensor is not well supported on Raspberry Pi. This is because it uses 
 * I2C clock stretching which the Pi cannot do without drastically slowing down 
 * the I2C speed. CircuitPython and Arduino are supported.
 * 
 * i2c address is 0x5A, and cannot be changed. Price was $20(US) as of this writing.
 * 
 * Pinouts:  Vin takes 3-5v   3v3 is the *output* of the 3.3v regulator on the board!
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    WAKE must be pulled to ground in order to talk to the sensor!
 *    INT is an interrupt output, an alarm when a reading is too high/low.
 *    RST when pulled to ground, this will reset the sensor.
 *    
 * Serial Monitor and Serial Plotter cannot run at the same time. Pick only one!
 * 
 * This sensor from AMS is a gas sensor that can detect a wide range of Volatile 
 * Organic Compounds (VOCs) and is intended for indoor air quality monitoring. 
 * When connected to your microcontroller (running our library code) it will 
 * return a Total Volatile Organic Compound (TVOC) reading and an equivalent 
 * carbon dioxide reading (eCO2) over I2C. There is also an onboard thermistor 
 * that can be used to calculate the local ambient temperature.
 *
 * The CCS811 has a 'standard' hot-plate MOX sensor, as well as a small 
 * microcontroller that controls power to the plate, reads the analog voltage, 
 * and provides an I2C interface to read from.
 * 
 * This part will measure eCO2 (equivalent calculated carbon-dioxide) 
 * concentration within a range of 400 to 8192 parts per million (ppm), and 
 * TVOC (Total Volatile Organic Compound) concentration within a range of 
 * 0 to 1187 parts per billion (ppb). According to the fact sheet it can detect 
 * Alcohols, Aldehydes, Ketones, Organic Acids, Amines, Aliphatic and 
 * Aromatic Hydrocarbons. We include a 10K NTC thermistor with matching 
 * balancing resistor which can be read by the CCS811 to calculate approximate 
 * temperature.
 * 
 * AMS recommends that you run this sensor for 48 hours when you first receive 
 * it to "burn it in", and then 20 minutes in the desired mode every time the 
 * sensor is in use. This is because the sensitivity levels of the sensor will 
 * change during early use.
 * 
 * The CCS811 has a configurable interrupt pin that can fire when a conversion 
 * is ready and/or when a reading crosses a user-settable threshold. The CCS811 
 * supports multiple drive modes to take a measurement every 1 second, every 
 * 10 seconds, every 60 seconds, or every 250 milliseconds.
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include <Adafruit_CCS811.h>

int measurement = 0;
float tempC = 0;
float tempF = 0;
int CO2 = 0;
int TVOC = 0;

//library_name  how_we_call_it
Adafruit_CCS811 ccs;

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN)
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.println("CCS811 test");
  if(!ccs.begin()){
    Serial.println("Failed to start CCS811 sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float tempC = ccs.calculateTemperature();
  ccs.setTempOffset(tempC - 25.0);
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
   */

  if(ccs.available()){
    tempC = ccs.calculateTemperature();
    
    if(!ccs.readData()){

    // Get the current CO2 reading
    CO2 = ccs.geteCO2();
  
    // Get the current Total Volatile Outgas Components (TVOC)reading
    TVOC = ccs.getTVOC();
    }
    else{
      Serial.println("ERROR! CCS811 Sensor not responding, check your wiring.");
      while(1);
    }
  }
  
  // We can only get temperature in Celsius from this sensor.
  // Do some math on the Celsius reading to get the Fahrenheit value
  tempF = (tempC * 9.0 / 5.0) + 32.0;
  
  // and print the results on one line for graphing
  // Since we're using FLOATing point variables, print precision to 2 decimals
  Serial.print ("Reading: "); Serial.print(measurement);
  Serial.print("\tCO2 (ppm): "); Serial.print(CO2);
  Serial.print("\tTVOC (ppb): "); Serial.print(TVOC);
  Serial.print("\tTemp. C: "); Serial.print(tempC, 2);
  Serial.print("\tTemp. F: "); Serial.println(tempF, 2);
  // Try changing the precision number to 4, to see the difference.

  // Incerment our loop count (measurement). There are many ways to do this...
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
