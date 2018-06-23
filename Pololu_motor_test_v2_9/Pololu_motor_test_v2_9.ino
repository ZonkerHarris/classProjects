/*   Zonker's Pololu MC33926 Motor Shield demo  v2.9  3 JAN 2015
  - Moving the Arduino Mega 2560 for the Hidden Harry Potter Train display
    because the motor controller and the LCD library didn't play well on a mini.
       http://ltcarduino.pbworks.com/Hidden-HP-Train  (browse the wiki)
  - Using the MC33926, for the higher current (than L293D or L298N) drivers
    as well as the fault detection and the current feedback!
  - Using a modified, smaller 5-button analog switch interface.
 - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - |
*/

/* Pins used
SDA, SCL leads available for I2C (maybe LCD, or Chrono-dot clock chip?)
Saving PWM outputs (0-13) for Servo use and possible display dimming.

== LCD pins ==
23  RS
25  Enable
26  Data 5
27  Data 4
28  Data 7
29  Data 6

== Train Position Sensing Inputs ==
36  HHPT in the public siding
37  Train approaching public siding
38  Train clear of public siding
39  HHPT in the hidden siding
40  Train approaching hidden siding
41  Train clear of hidden siding

== Outputs ==
50  
51  
52  M1 Stopped
53  M2 Stopped

== Motor pins ==
8   M1PWM  Motor 1 speed input (HIGH = motor is on, LOW = BRAKE) 20 kHz MAX
9   M2PWM  
30  M1!D2  will tri-state the outputs if this is held LOW
    (use PWM on this pin, if you want drive/coast, versus drive/brake!)
31  M2!D2  will tri-state the outputs if this is held LOW
    (use PWM on this pin, if you want drive/coast, versus drive/brake!)
32  M1DIR  Motor 1 Direction
33  M2DIR  Motor 2 Direction
34  M1!SF  Status Flag (LOW indicates a FAULT (short, over-current, etc.)
35  M2!SF  Status Flag (LOW indicates a FAULT (short, over-current, etc.)
    (!SF affects !D2, and will cause motor outputs to be disabled.)
    (this lead controls both motors, can be broken into seperate leads!) 
    (When used seperately, extra resistors need to be added to !SF2 and !D2)
    
== Analog pins ==
A0  Analog in for pushbutton switch sensing.  
    Parallel momentary switches tap intoa string of resistors in series
    between 5v and ground. Each switch has a reading in a tight range.
A1  1/4 of the motor supply voltage (2.2v input ~ 12 VDC motor power)
A2  M1FB current load feedback, ~.525mv per amp drawn
A3  M2FB current load feedback, ~.525mv per amp drawn
A4  From current sensor, Logos Electronics MIN-ANA-POW1B-1
      http://www.logos-electro.com/min-ana-pow1b-1
*/

#include "DualMC33926MotorShield.h"

//DualMC33926MotorShield2(M1dir,M1pwm,M1fb,M2dir,M2pwm,M2fb,M1D2,M2D2,M1SF,M2SF)
//DualMC33926MotorShield(M1dir,M1pwm,M1fb,M2dir,M2pwm,M2fb,nD2,nSF)
//  (32,8,2,33,9,3,30,34)
DualMC33926MotorShield md(32,9,2,33,10,3,30,34);

// This is using the initial code from the LCD Library page at arduino.cc
// include the library code:
#include <LiquidCrystal.h>

/* initialize the library with the numbers of the interface pins
   The format is;  lcd(RS, EN, D4, D5, D6, D7)  */
LiquidCrystal lcd(23, 25, 27, 26, 29, 28);

//  We will use a single analog input, to read multiple pushbuttons...
int controlPin = A0;  // Initialize our analog-switch input
int control = 0;   // this is the variable for the switch setting

int motorVoltsPin = A1;  // 1/4 of the Motor Supply voltage
int voltsIn = 0;  // measuring the 12v input, through resistor-divider
//  +12v in ----(15k)---< A1 >---(4.7k)--- gnd

int M1fbPin = A2;  // Motor 1 Current Feedback
int M2fbPin = A3;  // Motor 2 Current Feedback
int M1fb = 0;
int M2fb = 0;

int currentInPin = A4;  // measuring the 12v input current
int currentIn = 0;  // reading from the analog in
float current = 0;  // calculated from the resistor divider

//Define our global variables
//The MC33926 shield demo had max speeds of -44 thru 400. Too fast for LEGO.
//Using arrays... index 0 is unused, indexes 1 & 2 represent both motors.
int Mspeed[] = { 0, 0, 0 };  // Motor speed, should be -175 thru 175.
int Mtarget[] = { 0, 0, 0 }; // Motor target speed.
int Mincrement[] = { 0, 1, 1 };   // speed change increment for each Motor.
int Mrunlevel[] = { 0, 8, 8 };   // speed change increment for each Motor.
int runlevel[] = { -225, -210, -195, -175, -140, -125, -110, -100, 0, 
  100, 110, 125, 140, 175, 195, 210, 225 }; // speed setting for each Motor.
int motorMax = 225;
int motorMin = -225;

// - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - + - - - - |

void setup()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(40, 2);
  lcd.clear();      // start with a blank screen, cursor at the upper-left.
  // Print a message to the LCD.
  lcd.setCursor(0, 0);   // set cursor to column 0, row 0 (the first row)
  lcd.print("Dual MC33926 Motor Shield on Mega  v2.9");   
  lcd.setCursor(0, 1);     // set cursor to column 0, row 1
  lcd.print("Zonker Harris   JAN 2015");  

  Serial.begin(115200);  // initialize the psuedo-serial console port
  Serial.println("Dual MC33926 Motor Shield v2.9");
  md.init();
  
  pinMode(controlPin, INPUT);  // This is our analog control line for throttles.
  digitalWrite(controlPin, LOW);  // set pulldown on the control pin.
  pinMode(motorVoltsPin, INPUT);  // This is 1/4 of the Motor Power Supply.
  pinMode(M1fbPin, INPUT);  // This is current feedback for Motor 1.
  pinMode(M2fbPin, INPUT);  // This is current feedback for Motor 2.
  pinMode(currentInPin, INPUT);  // This is the (1/4) 12v input current

  delay (2000);
  lcd.clear();   // start with a blank screen, cursor at the upper-left.
  }

void loop()
{
  // Serial.println("loop-starts");
  int motorVolts = analogRead(motorVoltsPin); // read the Motor Power input pin
  float voltage = (motorVolts / 49.3);  // mapping the reading to human-scale volts
  lcd.setCursor(33, 0);  // set cursor to column 35, row 1 (first row)
  lcd.print("V:    ");
  lcd.setCursor(35, 0);  // set cursor to column 35, row 0 (first row)
  lcd.print(voltage);
  int controlIn = analogRead(controlPin);  // read the control input pin
  Serial.println(" .");
  Serial.print(controlIn);
  Serial.print("\t");
  lcd.setCursor(33, 1);     // set cursor to column 35, row 1 (second row)
  lcd.print("Ctl    ");
  lcd.setCursor(37, 1);     // set cursor to column 35, row 1 (second row)
  lcd.print(controlIn);
 
  /*  We will use a single analog input, to read multiple pushbuttons...
 *  
 *  A0 In |------------------------------------------------------.
 *                   |          |          |          |          |
 *                   o          o          o          o          o
 *                    | 1        | 2        | 3        | 4        | 5
 *                   o          o          o          o          o
 *                   |          |          |          |          |
 *    vcc |--'\/\/`--+--'\/\/`--+--'\/\/`--+--'\/\/`--+--'\/\/`--+--'\/\/`--> ground
 *                 1k         470        810       1.2k       3.3k        1k
 *    
 *  As you press (m)any button(s), you will get a different reading
 *  (within a narrow range, since resistors age, and vary a bit with heat).
 *  Test with your switches, find the ranges, tune your code accordingly.
 *  Try different resistors, but you should have more than 5k from vcc to ground.
 */
// If a pushbutton is pressed, note which one. (chording does noting)
  if (controlIn > 260 && controlIn < 280) {  // if A0 reads between 260-280...
    control = 1;  // M1+ (Inner Loop)
    if ((Mtarget[1] <= 210) && (Mrunlevel[1] < 16)) { 
      Mrunlevel[1] = Mrunlevel[1]++; 
      Mtarget[1] = runlevel[Mrunlevel[1]];
      Mincrement[1] = 2;
    }
  }
  
  if (controlIn > 520 && controlIn < 540) { 
    control = 2;  // M1- (Inner Loop)
    if ((Mtarget[1] >= -210) && (Mrunlevel[1] > 0)) {
      Mrunlevel[1] = (Mrunlevel[1] - 1); 
      Mtarget[1] = runlevel[Mrunlevel[1]];
      Mincrement[1] = 2;
    }
  }
 
  if (controlIn > 340 && controlIn < 360) { 
    control = 3;  // M2+  (Outer Loop)
    if ((Mtarget[2] <= 210)  && (Mrunlevel[2] < 16)) {
      Mrunlevel[2] = Mrunlevel[2]++; 
      Mtarget[2] = runlevel[Mrunlevel[2]];
      Mincrement[2] = 2;
    }
  }

  if (controlIn > 425 && controlIn < 445) { 
    control = 4;  // M2-  (Outer Loop)
    if ((Mtarget[2] >= -210)  && (Mrunlevel[2] > 0)) { 
      Mrunlevel[2] = (Mrunlevel[2] - 1); 
      Mtarget[2] = runlevel[Mrunlevel[2]];
      Mincrement[2] = 2;
    }
  }
  
  if (controlIn > 180 && controlIn < 200) { 
    control = 5;  // Stop!
    Mrunlevel[1] = 8;
    Mrunlevel[2] = 8;
    Mtarget[1] = runlevel[Mrunlevel[1]];
    Mtarget[2] = runlevel[Mrunlevel[2]];
    Mincrement[1] = 10;
    Mincrement[2] = 10;
  }

  // run the motor control and information display routine.
  M1fb = analogRead(M1fbPin);
  M2fb = analogRead(M2fbPin);
  currentIn = analogRead(currentInPin);
  current = (currentIn - 517);
  if (current < 0) { current = 0; }
  current = (current * 0.05);  // Each 185 mv = 1 amp of input current
  Serial.print(" /t ");
  Serial.print(M1fb);
  Serial.print("  /t ");
  Serial.print(M2fb);
  Serial.print(" /t ");
  Serial.print(current);
  lcd.setCursor(28, 0);  // set cursor to column 28, row 0 (first row)
  lcd.print("     ");
  lcd.setCursor(28, 0);  // set cursor to column 28, row 0 (first row)
  lcd.print(current);
  lcd.setCursor(28, 1);  // set cursor to column 28, row 1 (second row)
  lcd.print("   ");
  lcd.setCursor(28, 1);  // set cursor to column 28, row 1 (second row)
  lcd.print(M1fb);
  lcd.setCursor(30, 1);  // set cursor to column 30, row 1 (second row)
  lcd.print("   ");
  lcd.setCursor(30, 1);  // set cursor to column 30, row 1 (second row)
  lcd.print(M2fb);
  for (int motor = 1; motor <= 2; motor++)  { checkMotors(motor);}
  delay(150);
}

// Here are the subroutines...
void checkMotors(int thisMotor)
{
  /* Subroutine, inherit motor number
  check target and speed, increment if needed, and set speed
  update LCD information for that motor
  Using arrays... index 0 is unused, indexes 1 & 2 represent both motors.
   int Mspeed[] = Motor speed, should be -175 thru 175.
   int Mtarget[] = Motor target speed.
   int Mincrement[] = { 0, 1, 2 }; // speed change increment for each Motor. */

  stopIfFault();
  
  Serial.print(Mrunlevel[thisMotor]);
  Serial.print("\t");
  Serial.print(Mspeed[thisMotor]);
  Serial.print("\t");
  Serial.print(Mtarget[thisMotor]);
  Serial.print("\t");
  lcd.setCursor(0, (thisMotor-1));  // set cursor to column 0, on the correct row
  lcd.print("  Run    PWM      Goal     ");
  lcd.setCursor(0, (thisMotor-1));  // set cursor to column 0, on the correct row
  lcd.print(thisMotor);
  lcd.setCursor(6, (thisMotor-1));  // set cursor to column 6, on the correct row
  lcd.print((Mrunlevel[thisMotor]));
  lcd.setCursor(13, (thisMotor-1));  // set cursor to column 13, on the correct row
  lcd.print((Mspeed[thisMotor]));
  lcd.setCursor(23, (thisMotor-1));  // set cursor to column 23, on the correct row
  lcd.print((Mtarget[thisMotor]));

  if (Mspeed[thisMotor] < Mtarget[thisMotor]) {   // speed will be increased.
    int diff = ( Mtarget[thisMotor] - Mspeed[thisMotor] );
    if (diff > Mincrement[thisMotor]) {  // we are still ramping up...
      Mspeed[thisMotor] = (Mspeed[thisMotor] + Mincrement[thisMotor]);
      if ((Mspeed[thisMotor] > 10) && (Mspeed[thisMotor] < 95)) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] + 9); }
      if ((Mspeed[thisMotor] > -95) && (Mspeed[thisMotor] < -10)) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] + 9); }
      if (Mtarget[thisMotor] - Mspeed[thisMotor] < 9) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] + 1); }
      if (thisMotor == 1) { md.setM1Speed(Mspeed[thisMotor]); }
      if (thisMotor == 2) { md.setM2Speed(Mspeed[thisMotor]); }
    }
    else
    { Mspeed[thisMotor] = (Mspeed[thisMotor] + diff); }
  }
  
  if (Mspeed[thisMotor] > Mtarget[thisMotor]) {   // speed will be decreased.
    int diff = ( Mtarget[thisMotor] - Mspeed[thisMotor] );
    if (diff < Mincrement[thisMotor]) {  // we are still ramping down...
      Mspeed[thisMotor] = (Mspeed[thisMotor] - Mincrement[thisMotor]);
      if ((Mspeed[thisMotor] < 100) && (Mspeed[thisMotor] > 16)) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] - 9); }
      if ((Mspeed[thisMotor] < -10) && (Mspeed[thisMotor] > -85)) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] - 9); }
      if ( Mspeed[thisMotor] - Mtarget[thisMotor] < 9) 
        { Mspeed[thisMotor] = (Mspeed[thisMotor] - 1); }
      if (thisMotor == 1) { md.setM1Speed(Mspeed[thisMotor]); }
      if (thisMotor == 2) { md.setM2Speed(Mspeed[thisMotor]); }
    }
    else
    { Mspeed[thisMotor] = (Mspeed[thisMotor] - diff); }
  }
}

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    lcd.clear();      // start with a blank screen, cursor at the upper-left.
    lcd.setCursor(2, 0);     // set cursor to column 2, row 0 (second row)
    lcd.print("              ");
    lcd.setCursor(2, 0);     // set cursor to column 35, row 1 (second row)
    lcd.print("Circuit Fault ");
    while(1);
  }
}

