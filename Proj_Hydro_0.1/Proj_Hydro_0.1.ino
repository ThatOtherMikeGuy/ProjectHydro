#include "pins_arduino.h"

// Starting code provided by arduino
// Ver 0.1 
// Modified by: Mike Schwindt on 7/23/18
/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
//#define HOT 9
//#define COLD 10

// the setup function runs once when you press reset or power the board

void check()
{
  //Check and see if the hot or cold signal is detected
  if(digitalRead(11) == HIGH || digitalRead(12) == HIGH)
  {
    //Move motor and signal testing light that it worked
    digitalWrite(6,LOW);
    digitalWrite(8,HIGH);
  }
  // if hot signal is put no longer detected
  else if(digitalRead(11) == LOW) // && digitalRead(12) == LOW)
  {
    //turn fan off and turn off testing LED
    digitalWrite(6,HIGH);
    digitalWrite(8,LOW);
  }
  //Future stuff
  // too cold, too humid, etc.
  else //
  {
    
  }
}

void setup() 
{
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT); // motor output (drive low to move motor)
  pinMode(8, OUTPUT); // Tetsing pin with (white) led
  pinMode(9, OUTPUT); // Red LED/too hot signal
  pinMode(10, OUTPUT); // Bkue LED/too cold signal
  pinMode(11,INPUT); // Too hot signal input (not needed)
  pinMode(12, INPUT); // Too cold signal input (not needed)
}

// the loop function runs over and over again forever
void loop() 
{
  //digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  check();
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  check();
  delay(1000);                       // wait for a second

  
}
