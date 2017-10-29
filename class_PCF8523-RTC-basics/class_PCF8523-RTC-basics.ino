/* class_PCF8523-RTC-basics      
 *   2017-10-28  Zonker Harris
 * 
 * https://learn.adafruit.com/adafruit-si1145-breakout-board-uv-ir-visible-sensor/
 * https://github.com/adafruit/RTClib
 * https://www.adafruit.com/product/3295
 * 
 * i2c address is 0x68, and cannot be changed. 
 * Price was $5(US) as of this writing.
 * You should also get 1-2 CR1220 3v lithium coin cell batteries (1 needed for the clock)
 * Accuracy is +/- 2 seconds per day. If you need more precision, consider the DS3231
 *  Precision RTC (https://www.adafruit.com/products/3013 or a ChronoDot 
 *  http://macetech.com/store/index.php?main_page=product_info&products_id=8
 * 
 * Pinouts:  Vin takes 3-5v   3o is the *output* of the 3.3v regulator on the board!
 *    GND goes to ground.  SDA is the i2c Data bus lead, SCL is the i2c Clock lead. 
 *    SQW is a Square-wave output which can be used as a timebase for other uses.
 */

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
/* for Zero, output on USB Serial console, remove line below if using programming 
 * port to program the Zero! */
//   #define Serial SerialUSB
#endif

//library_name  how_we_call_it
RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
                             "Friday", "Saturday"};

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // The Serial commands are Case-Sensitive, f you type it wrong, it won't work.
  // The commands can also do base-number conversions (DEC -> HEX, OCT -> BIN
  //  Read more about the commands here: https://www.arduino.cc/en/Serial/Print 
  Serial.begin(115200);
  Serial.println("PCF8523 Real-Time Clock (RTC) time display");

  if (! rtc.begin()) {
    Serial.println("Couldn't find our RTC, check the connections.");
    while (1);
  }

  if (! rtc.initialized()) {
    // This will detect an unintialized RTC board, and set it for the current
    //  time (derived from the computer used to upload the sketch). Once the RTC
    //  has it's time, the sketch will not run this portion of the sketch.
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" UNIXTIME = ");
    Serial.print(now.unixtime());
    Serial.print(" = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println(" days since midnight JAN 1st 1970.");
    
    // calculate a date which is 7 days and 30 seconds into the future
    /* 
    DateTime future (now + TimeSpan(7,12,30,6));
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println(); 
    */
    
    Serial.println();
    delay(3000);
}
