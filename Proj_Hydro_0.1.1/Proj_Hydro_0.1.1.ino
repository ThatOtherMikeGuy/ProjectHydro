#include "DHT.h"
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
#define TOOHOT 9
#define TOOCOLD 10
#define TOOHUMID 8
#define NOTHUMID 7
#define MOTORSIG 6
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE);

void checkTmpHum()
{
  int tooHot = 79;
  int tooCold = 68;
  int tooHumid = 80;
  int notHumid = 30;
  
  //Check and see if the hot or cold signal is detected
  if(digitalRead(11) == HIGH || digitalRead(12) == HIGH)
  {
    //Turn motor on
    digitalWrite(6,LOW);
  }
  // if hot signal input no longer detected
  else if(digitalRead(11) == LOW)
  {
    //turn fan off
    digitalWrite(6,HIGH);
  }
  //Future stuff
  // too cold, too humid, etc.
  else //
  {
    
  }
}

void setTempSig(float temp)
{
  if(temp >= 79)
  {
    digitalWrite(TOOHOT, HIGH);
  }
  else if(temp < 70)
  {
    digitalWrite(TOOCOLD, HIGH);
  }
  else
  {
    digitalWrite(TOOHOT, LOW);
    digitalWrite(TOOCOLD, LOW);
  }
}

void setHumSig(float humid)
{
  int growingStage = 1; // just thinking about a variable here that will allow easy adjustability 
  // based on what growing stage the plants are in
  // variable above will likely be in charge of changing the two variables below since
  // humidity levels need to be different in diferent stages of growing
  int humidRangeLow = 30;
  int humidRangeHigh = 80;
  if(humid < humidRangeLow)
  {
    digitalWrite(NOTHUMID, HIGH);
  }
  else if(humid > humidRangeHigh)
  {
    digitalWrite(TOOHUMID, HIGH);
  }
  else if(humid > humidRangeLow && humid < humidRangeHigh)
  {
    digitalWrite(TOOHUMID, LOW);
    digitalWrite(NOTHUMID, LOW);
  }
}

void setup() 
{
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, INPUT); // input from temp/humidity sensor
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
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  setTempSig(f);
  setHumSig(h);
  
  /*
  // testing the outputs to make sure they are sending correct signals
  //digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  check();
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  check();
  delay(1000);                       // wait for a second
  */
}
