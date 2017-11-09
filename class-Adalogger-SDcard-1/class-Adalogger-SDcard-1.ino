//   class-Adalogger-SDcard-1
//     2017-11-04   Zonker Harris
//
// This is a simple sketch to test reading and writing to the SDcard socket.
//  I've also added pin assignments, so students know which pins are in use.
//    https://learn.adafruit.com/adafruit-feather-m0-adalogger 
//    https://learn.adafruit.com/assets/46243  (Annotated pinouts)
//    https://learn.adafruit.com/adafruit-feather-m0-adalogger/faq  (debugging)
//    https://learn.adafruit.com/adafruit-feather-m0-adalogger/using-the-sd-card

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
/*
CPU: ATSAMD21G18 ARM Cortex M0 
Clock: 48 MHz 
Logic level: 3.3V  Onboard regulator up to 500 mA peak current)
Memory:  256K of FLASH (program storage), and 32K of RAM (for vafriables)
LiPo:  The JST connector polarity is matched to Adafruit LiPoly batteries. 
      NOTE: Using wrong polarity batteries can destroy your Feather!
  You can also connect a 4.2/3.7V Lithium Polymer (Lipo/Lipoly) or 
  Lithium Ion (LiIon) battery to the JST jack. This will let the Feather 
  run on a rechargable battery. When the USB power is powered, it will 
  automatically switch over to USB for power, as well as start charging 
  the battery (if attached) at 100mA per hour.
  The CHG LED will light if the battery is connected and charging.
  
PIN  Use
  4  SD Card "CS" (Chip Select)
  7  SD Card "CD" (Card Detect) If configured for Pullup, LOW = empty slot.
  8  Green LED near SD Card
  9  GPIO, but also serves as Analog 7 (You can only use A7 -OR- D9!)
      (connected to a 2 * 100k voltage divider for measuring the LiPo voltage)
 13  Red LED for blink, and for upload status.
 20  SDA data for i2c bus
 21  SCL clock for i2c bus
 22  MISO (SPI "Master In, Slave Out")
 23  MOSI (SPI "Master Out, Slave In")
 24  SCK (SPI "Serial Clock")
 A0  Analog INPUT 0, but can also be True Analog OUTPUT!
 
 */ 

#include <SPI.h>    // (needed for the SD Card)
#include <SD.h>     // (the SD Card commands)
#include <Wire.h>   // (initialize the i2c bus)
#include "RTClib.h" // (needed to talk to the i2c Real-Time Clock)

// Set the pins used by the SD card, battery voltage monitor
#define cardSelect 4
#define VBATPIN A7

File logfile;  // for the SD card

int tempPin = 14;   // for the TMP36, on Analog input 0
int TempV = 0;      // for measuring the temperature
float TempC = 0;
float TempF = 0;

//library_name  how_we_call_it
RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
                             "Friday", "Saturday"};

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
// This line is not needed if you have Adafruit SAMD board package 1.6.2+
//   #define Serial SerialUSB

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void setup() {
  // connect at 115200 so we can print data fast without dropping characters
  // also spit it out
  Serial.begin(115200);
  Serial.println("\r\nAnalog temperature logger test");
  pinMode(13, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15];
  strcpy(filename, "ANALOG00.TXT"); // <-- The name of the NEW file on the SD Card.
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);

  Serial.println("Initializing PCF8523 Real-Time Clock (RTC) module...");

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
  //rtc.adjust(DateTime(2017, 11, 8, 23, 33, 0)); //uncomment this and upload to set
  Serial.println("Ready!");
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
void loop() {
  // turn off the red LED to save power...
  digitalWrite(13, LOW);
  
  /* This will turn on the green LED near the SD Card slot, reads the Analog 7
  *  pin and writed the result to the SD Card, then reads Analog 7 again, and
  *  prints the result to the Serial Monitor/Grapher, then turns off the LED.
  *  Then it waits one second. See how fast the LED blinks on and off?
  *  
  *  The data for the SD card is buffered which means that whenever we reach 
  *  512 bytes of log file that needs to be written, the SD card will actually 
  *  write the data. When this happens you'll see a 50mA pulse for 10ms. If 
  *  you use flush() to write the log file, this pulse will be much longer, 
  *  as you have to write the file as well as the file table sectors. Your 
  *  50mA spike could end up being 500ms or longer. So basically, keep your 
  *  file writes to a minimum if you can avoid it!  */
  
  digitalWrite(8, HIGH);

  // Measure the battery voltage... 
  //  When it's plugged into USB, the voltage is about 4.25v
  //  But, when you unplug it from USB, the battery voltage will be read
  //  You can use this to estimate how long your sketch may be able to run 
  //  on battery power (or see the improvements when you try Low Power fixes.
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  // Measure the temperature
  TempV = analogRead(tempPin);
  float voltage = TempV * 3.3;
  voltage /= 1024.0;
  
  // Convert from 10 mv per degree with 500 mV offset to degrees...
  TempC = (voltage - 0.5) * 100 ;  //  ((volatge - 500mV) times 100)

  // now convert to Fahrenheit
  float TempF = (TempC * 9.0 / 5.0) + 32.0;

  // now let's get the current time from the RTC
  DateTime now = rtc.now();
  // Start the logging with the time in the format below;
  //   2017-11-04 22:08:15, PT day

  logfile.print(now.year(), DEC);
  logfile.print('-');
  logfile.print(now.month(), DEC);
  logfile.print('-');
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(':');
  logfile.print(now.minute(), DEC);
  logfile.print(':');
  logfile.print(now.second(), DEC);
  logfile.print(", PT, ");
  logfile.print(daysOfTheWeek[now.dayOfTheWeek()]);
  //logfile.print("A0, "); logfile.print(TempV);
  logfile.print(", TempC, "); logfile.print(TempC);
  logfile.print(", TempF, "); logfile.print(TempF);
  logfile.print(", Batt, "); logfile.println(measuredvbat);
  Serial.print(now.year(), DEC);
  Serial.print('-');
  Serial.print(now.month(), DEC);
  Serial.print('-');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(", PT, ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
   //Serial.print(", A0, "); Serial.print(TempV);
  Serial.print("; TempC, "); Serial.print(TempC);
  Serial.print("; TempF, "); Serial.print(TempF);
  Serial.print("; Batt, "); Serial.println(measuredvbat);
  digitalWrite(8, LOW);
  
  delay(30000);
}

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - +
/*  https://learn.adafruit.com/adafruit-feather-m0-adalogger/power-management 
#define VBATPIN A7
   
float measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage
Serial.print("VBat: " ); Serial.println(measuredvbat);
*/
