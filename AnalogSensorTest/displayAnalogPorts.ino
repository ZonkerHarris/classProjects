// displayAnalogPorts  v0.1   2017-10-19 Happy Diwali!
//  we have 6 analog ports, lets see what values they are
//    Add to the Preferences, to get the Adafruit boards...
//    https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
//   Others https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls#list-of-3rd-party-boards-support-urls 

// create an integer for the pin number for the analog port
int a0Pin = 14;
int a1Pin = 15;
int a2Pin = 16;
int a3Pin = 17;
int a4Pin = 18;
int a5Pin = 19;

// set the initial value, to establish the integer variables
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // set up for the pseudo-serial monitor
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

  //  We'll print them on the same line, with tabs in between...
  //  We can put more than one command on each line, with semi-colons
  //    https://www.arduino.cc/en/Serial/Print 
  Serial.print(val1); Serial.print ("\t");
  Serial.print(val2); Serial.print ("\t");
  Serial.print(val3); Serial.print ("\t");
  Serial.print(val4); Serial.print ("\t");
  Serial.print(val5); Serial.print ("\t");
  Serial.println(val6);  // add the linefeed after the last variable, no tab

  //  If it's scrolling by too fast, add a delay here...
  // delay(100);   // the delay value is in milliseconds.
}
