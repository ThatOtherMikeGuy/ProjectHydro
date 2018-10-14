#include "DHT.h"
#include "pins_arduino.h"

// Starting code provided by arduino
// Ver 0.2.1 
// Modified by: Mike Schwindt on 8/1/18
// Added a few more comments and a new set up light scheme
// Modified a few other lines for readability and freed up a few pins
/*
  Blink
  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#define TOOHOT 9        // Digital pin used for 'tooHot' signal
#define TOOCOLD 10      // Digital pin used for 'tooCold' signal
#define TOOHUMID 8      // Digital pin used for 'tooHumid' signal
#define NOTHUMID 7      // Digital pin used for 'notHumid' signal
#define MOTORSIG 6      // Digital pin used for motor signal (HIGH = off, LOW = on)
#define DHTPIN 2        // Digital pin used for thermostat/humid sensor
#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE);

void checkTmpHum(int muchHot, int muchCold, int muchHumid, int noHumid)
{
  // get input values from function
  int tooHot = muchHot;
  int tooCold = muchCold;
  int tooHumid = muchHumid;
  int notHumid = noHumid;
  
  //Check and see if the hot or tooHumid signal is detected
  if(digitalRead(TOOHOT) == HIGH || digitalRead(TOOHUMID) == HIGH)
  {
    //Turn motor on
    digitalWrite(MOTORSIG,LOW);
  }
  // if hot signal input no longer detected
  else if(digitalRead(TOOHOT) == LOW)
  {
    //turn fan off
    digitalWrite(MOTORSIG,HIGH);
  }
  //Future stuff
  // too cold, too humid, etc.
  else //
  {
    
  }
}

void setTempSig(float temp, int muchHot, int muchCold)
{
  int tooHot = muchHot;
  int tooCold = muchCold;

  if(temp >= tooHot)
  {
    digitalWrite(TOOHOT, HIGH);
  }
  else if(temp < tooCold)
  {
    digitalWrite(TOOCOLD, HIGH);
  }
  else
  {
    digitalWrite(TOOHOT, LOW);
    digitalWrite(TOOCOLD, LOW);
  }
}

void setHumSig(float humid, int muchHumid, int noHumid)
{
  int growingStage = 1; // just thinking about a variable here that will allow easy adjustability 
  // based on what growing stage the plants are in
  // variable above will likely be in charge of changing the two variables below since
  // humidity levels need to be different in diferent stages of growing
  
  int humidRangeLow = noHumid;
  int humidRangeHigh = muchHumid;

  
  if(humid < humidRangeLow)
  {
    digitalWrite(NOTHUMID, HIGH);
  }
  else if(humid > humidRangeHigh)
  {
    digitalWrite(TOOHUMID, HIGH);
  }
  else //if(humid > humidRangeLow && humid < humidRangeHigh)
  {
    digitalWrite(TOOHUMID, LOW);
    digitalWrite(NOTHUMID, LOW);
  }
}

void setup() 
{
  Serial.begin(9600);
  pinMode(2, INPUT); // input from temp/humidity sensor
  pinMode(6, OUTPUT); // motor output (drive low to move motor)
  pinMode(8, OUTPUT); // Tetsing pin with (white) led
  pinMode(9, OUTPUT); // Red LED/too hot signal
  pinMode(10, OUTPUT); // Bkue LED/too cold signal
  
  //want to make sure the motor is off on start up
  digitalWrite(MOTORSIG,HIGH);
  
  //blink colors alternatlely to indicate setup done
  for(int i = 0; i < 5; i++)
  {
    if(i%2 == 0)
    {
      digitalWrite(TOOHOT, HIGH);
      digitalWrite(TOOCOLD, LOW);   
    }
    else
    {
      digitalWrite(TOOHOT, LOW);
      digitalWrite(TOOCOLD, HIGH);
    }
    delay(1000);
  }
  //off
  digitalWrite(TOOHOT, LOW);
  digitalWrite(TOOCOLD, LOW);
}

// the loop function runs over and over again forever
void loop() 
{
  // initialize all values to be passed by functions
  int muchHot = 79;
  int muchCold = 72;
  int muchHum = 80;
  int noHum = 30;
  
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
    return;
  }
  
  setTempSig(f,muchHot,muchCold);
  setHumSig(h,muchHum,noHum);
  checkTmpHum(muchHot,muchCold,muchHum, noHum);
  // Wait a few seconds between measurements.
  delay(2000);
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
