// displayTMP36values  v0.1   2017-10-19 Happy Diwali!
//  Read in all 6 analog values, covert to Temperature, display the values
//    Add to the Preferences, to get the Adafruit boards...
//    https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
//   Others https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls#list-of-3rd-party-boards-support-urls 
// 
// https://www.adafruit.com/product/165   $1.50 each, reads Celsius natively.
// https://learn.adafruit.com/tmp36-temperature-sensor/overview 
// http://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf 
// http://www.analog.com/en/products/analog-to-digital-converters/integrated-special-purpose-converters/digital-temperature-sensors/tmp36.html

// create an integer for the pin number for the analog port
int a0Pin = 14;
int a1Pin = 15;
int a2Pin = 16;
int a3Pin = 17;
int a4Pin = 18;
int a5Pin = 19;

// set the initial value, to establish the integer variables
//  The analog inputs read out in integer values, so use int variables...
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set up for the pseudo-serial monitor
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
  // put your main code here, to run repeatedly:
  // Read in the values, as close to the same time as posible
  //  the values will be stored in the integers to display soon
  val1 = analogRead(a0Pin); // read the first Analog input pin
  val2 = analogRead(a1Pin); // read the second Analog input pin
  val3 = analogRead(a2Pin); // read the third Analog input pin
  val4 = analogRead(a3Pin); // read the fourth Analog input pin
  val5 = analogRead(a4Pin); // read the fifth Analog input pin
  val6 = analogRead(a5Pin); // read the sixth Analog input pin
  /* These inputs are "high impedance". If your readings seem unstable
   * (the is, not stable/repeatable), it could be due to the impedance
   * changes from rapid queries. Try reading twice, after a short delay,
   * using the second reading.
   */

  // Convert the reading into voltage... This depends on the CPU voltage
  // We need small precision, so we'll use FLOATing point variables
  //   https://www.arduino.cc/en/Reference/Float 
  // Adalogger M0 is a 3.3v CPU, so we give the sensors 3.3 volts...
  // We then divide the result by 1024 steps of our A to D converter
  float tempV1 = val1 * 3.3; tempV1 /= 1024.0;
  float tempV2 = val2 * 3.3; tempV2 /= 1024.0;
  float tempV3 = val3 * 3.3; tempV3 /= 1024.0;
  float tempV4 = val4 * 3.3; tempV4 /= 1024.0;
  float tempV5 = val5 * 3.3; tempV5 /= 1024.0;
  float tempV6 = val6 * 3.3; tempV6 /= 1024.0;

  //  For Centigrade, [(analog voltage in mV) - 0.5] * 100
  float tempC1 = (tempV1 - 0.5) * 100;
  float tempC2 = (tempV2 - 0.5) * 100;
  float tempC3 = (tempV3 - 0.5) * 100;
  float tempC4 = (tempV4 - 0.5) * 100;
  float tempC5 = (tempV5 - 0.5) * 100;
  float tempC6 = (tempV6 - 0.5) * 100;

  // for Farenheit, (Temperature C * 9.0 / 5.0) + 32.0
  // The TMP35 and TMP37 are designed for Farenheit reading output
  // The TMP36 is designed for Centigrade reading, and we convert to F
  float tempF1 = (tempC1 * 9.0 / 5.0) + 32.0;
  float tempF2 = (tempC2 * 9.0 / 5.0) + 32.0;
  float tempF3 = (tempC3 * 9.0 / 5.0) + 32.0;
  float tempF4 = (tempC4 * 9.0 / 5.0) + 32.0;
  float tempF5 = (tempC5 * 9.0 / 5.0) + 32.0;
  float tempF6 = (tempC6 * 9.0 / 5.0) + 32.0;


  //  We'll print similar readings on the same line, with tabs in between...
  //  We can put more than one command on each line, with semi-colons
  //    https://www.arduino.cc/en/Serial/Print 
  Serial.println (); Serial.print ("Volts     ");
  Serial.print(tempV1); Serial.print ("\t ");
  Serial.print(tempV2); Serial.print ("\t ");
  Serial.print(tempV3); Serial.print ("\t ");
  Serial.print(tempV4); Serial.print ("\t ");
  Serial.print(tempV5); Serial.print ("\t ");
  Serial.println(tempV6);  // add the linefeed after the last variable, no tab

  Serial.print ("Temp (C) ");
  Serial.print(tempC1); Serial.print ("\t");
  Serial.print(tempC2); Serial.print ("\t");
  Serial.print(tempC3); Serial.print ("\t");
  Serial.print(tempC4); Serial.print ("\t");
  Serial.print(tempC5); Serial.print ("\t");
  Serial.println(tempC6);  // add the linefeed after the last variable, no tab

  Serial.print ("Temp (F) ");
  Serial.print(tempF1); Serial.print ("\t");
  Serial.print(tempF2); Serial.print ("\t");
  Serial.print(tempF3); Serial.print ("\t");
  Serial.print(tempF4); Serial.print ("\t");
  Serial.print(tempF5); Serial.print ("\t");
  Serial.println(tempF6);  // add the linefeed after the last variable, no tab

  //  If it's scrolling by too fast, add a delay here...
  delay(1000);   // the delay value is in milliseconds.
  // You can make the delay shorter, if your reading will be changing quickly.
  //  (But doing fast measurements will quickly fill your log.)
  // If you want to graph the changes over a day, or a week, consider a larger delay.
  //  (How many seconds in 5 minutes? How many in 30 minutes? How many in an hour?)
  // Remember that the delay function is in MILLISECONDS (1,000ths of a second).
}
