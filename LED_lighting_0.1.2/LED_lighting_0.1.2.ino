// Original code provided by Michael Schwindt and Kaylyn Wessel 
// Edited by Mike Schwindt 
// Version 0.1.2
// 8/21/18
// New power supply will be in this week needed more amps (had 2 will have 5)
// tried playing with  interrupts, decided its not worth it right now
// Decided best color loop was a loop that was very close to white but still hits all the colors as it goes around

// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Keypad.h>
#include <LiquidCrystal.h>

#define OE   9
#define LAT 10
#define CLK 11
#define A   A0
#define B   A1
#define C   A2
#define D   A3


//for keypad
const byte ROWS = 4;  //four rows 
const byte COLS = 3;  //three columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {7,2,3,5}; //connect to the pins of the board that correspond to rows
byte colPins[COLS] = {6,8,4}; //connect to the pins of the board that correspond to the columns

//initialize an instance of class NewKeypad 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
// initialize the library with the numbers of the interface pins on LCD screen
LiquidCrystal lcd(37,34,38,30,36,32);

//for LED
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// Globals for dim and bright
int BRIGHTNESS = 7;

// more globals
int GROWINGCYCLE = 0;
int HUMHIGH = 80;
int HUMLOW = 70;
int TEMPHIGH = 80;
int TEMPLOW = 68;
int red = BRIGHTNESS;
int blue = BRIGHTNESS;
int green = BRIGHTNESS;
int red2 = red;
int blue2 = blue;
int green2 = green;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long startMillis2;
unsigned long startMillis3;
unsigned long currentMillis;
unsigned long currentMillis2;
unsigned long currentMillis3;
unsigned long period1 = 1000*1;  // second number is the number of seconds
unsigned long period2 = 1000;

void messageCancel()
{
      lcd.clear();
      lcd.begin(16,2);
      lcd.print("MESSAGE CANCELED!");
      delay (2000);
      lcd.clear();
}

void greenAndHello()
{
   // fix the screen with green
    matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, BRIGHTNESS, 0));
    delay(2000);
  
    //Write HELLO across screen to get attention before message is displayed
     matrix.setCursor(3, 8);  //start at 8 pixels down and 3 pixels in
     matrix.setTextSize(2);   //size 2 == 16 pixels high
     matrix.setTextWrap(false);
     
   // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.print('H');
    delay(250);
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.print('E');
    delay(250);
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.print('L');
    delay(250);
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.print('L');
    delay(250);
    //This last one will determine color for message as well since we do not change it anywhere else
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.print('O');
    delay(2500);
    
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
}
// return 1/true for good
// return 0/false for cancel
bool checkForCancel(long delayTime)
{
  int count = 0;
  char customKey;
  Serial.print(delayTime);
  Serial.print(" ");
  Serial.print(count);
  Serial.print("\n");   
  while(count < delayTime)
  {
    customKey = customKeypad.getKey();  
    if(customKey == '*')
    {
      return false;
    }
    delay(1);
    count = count + 1;
  }
  return true;
}

bool lcdCountDown(bool shorter = false)
{
    long Time = 1000; // approximately 1 sec
    // this is a work around, function was being weird with first number being too big
    // so this is how I got it to work correctly
    lcd.setCursor(14,1);
    lcd.print(" ");
    Serial.print("Going to 5 \n");
    if(!checkForCancel(100))
    {
      return false;
    }
    // just shorten time if needed
    if(!shorter)
    {
      lcd.setCursor(14,1);
      lcd.print(" 5");
      Serial.print("Going to 4 \n");
      if(!checkForCancel(Time))
      {
        return false;
      }
      lcd.setCursor(14,1);
      lcd.print(" 4");
      Serial.print("Going to 3 \n");
      if(!checkForCancel(Time))
      {
        return false;
      }
    }
    lcd.setCursor(14,1);
    lcd.print(" 3");
    Serial.print("Going to 2 \n");
    if(!checkForCancel(Time))
    {
      return false;
    }
    lcd.setCursor(14,1);
    lcd.print(" 2");
    Serial.print("Going to 1 \n");
    if(!checkForCancel(Time))
    {
      return false;
    }
    lcd.setCursor(14,1);
    lcd.print(" 1");
    Serial.print("Going to 0 \n");
    if(!checkForCancel(Time))
    {
      return false;
    }
    return true;
}

void preset()
{
  lcd.begin(16,2);
  lcd.print("Preset:");
}
// message for the LCS display
// return a 1/true for 'good to go'
// return a 0/false for 'cancel'
bool lcdMessages(char *num)
{
  if(num[0] == '0' && num[1] == '1')
  {
    preset();
    lcd.print(" Brights");
  }
  else if(num[0] == '0' && num[1] == '2')
  {
    preset();
    lcd.print(" Animals");
  }
  else if(num[0] == '0' && num[1] == '3')
  {
    preset();
    lcd.print(" Pass");
  }
    else if(num[0] == '0' && num[1] == '4')
  {
    preset();
    lcd.print("Road Work");
  }
    else if(num[0] == '0' && num[1] == '5')
  {
    preset();
    lcd.print(" ICE");
  }
    else if(num[0] == '0' && num[1] == '9')
  {
    preset();
    lcd.print(" F.U.");
  }
  else if(num[0] == '0' && num[1] == '#')
  {
    if(BRIGHTNESS == 7)
    {
      lcd.begin(16,2);
      lcd.print("Brightness");
      lcd.setCursor(0,1);
      lcd.print("turned DOWN");
      delay (1000);
      lcd.begin(16,2);
      lcd.print(" ");
      BRIGHTNESS = 3;
    }
    else if(BRIGHTNESS == 3)
    {
      lcd.begin(16,2);
      lcd.print("Brightness");
      lcd.setCursor(0,1);
      lcd.print("turned UP");
      delay (1000);
      lcd.begin(16,2);
      lcd.print(" ");
      BRIGHTNESS = 7;      
    }
    return 0;
  }
  else
  {
    lcd.begin(16,2);
    lcd.print("No Preset for ");
    lcd.print(num[0]);
    lcd.print(num[1]);
    lcd.setCursor(0,1);
    lcd.print("Exiting");
    // use the short lcd countdown
    lcdCountDown(true);
    return 0;    
  }
  
  lcd.setCursor(0,1);
  lcd.print("Displaying in:");
  if(!lcdCountDown())
  {
    return 0;
  }
  lcd.begin(16,2);
  lcd.print("message sent");
  delay (1000);
  lcd.begin(16,2);
  lcd.print(" ");
  return 1;
}

char getKeyVal(int timer = 500)
{
  int count = 0;
  while(count <= timer)
  {
    char customKey = customKeypad.getKey();
    if(customKey != NULL)
    {
      return customKey;     
    }
    if(timer != 0)
    {
      delay(1);
      count++;
    }
    else
    {
      count = 0;
    }
  }
  // if it leaves the while loop based on the timer return the char '?'
  return '?';
}

// this is where we will add future messages
void messages(char *key, bool testing = false)
{
  int displayTime = 25000;
  if(testing)
  {
    displayTime = displayTime/4;
  }
  // 01 change growing stage
  // 02 change HUMHIGH
  // 03 change HUMLOW
  // 04 change TEMPHIGH
  // 05 change TEMPLOW
  if (key[0] == '0' && key[1] == '1')
  {
    lcd.begin(16,2);
    lcd.print("Current GS: ");
    lcd.print(GROWINGCYCLE);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current GROWINGSTAGE: ");
      lcd.print(GROWINGCYCLE);
      incrDecr(GROWINGCYCLE, key);
    } 
  }
  
  if (key[0] == '0' && key[1] == '1')
  {
    lcd.begin(16,2);
    lcd.print("Current HUMHIGH: ");
    lcd.print(HUMHIGH);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current HUMHIGH: ");
      lcd.print(HUMHIGH);
      incrDecr(HUMHIGH, key);
    } 
  }
  else if (key[0] == '0' && key[1] == '2')
  {
    lcd.begin(16,2);
    lcd.print("Current HUMLOW: ");
    lcd.print(HUMLOW);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current HUMLOW: ");
      lcd.print(HUMLOW);
      incrDecr(HUMLOW, key);
    } 
  }
  else if (key[0] == '0' && key[1] == '3')
  {
    lcd.begin(16,2);
    lcd.print("Current TEMPLOW: ");
    lcd.print(TEMPLOW);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current TEMPLOW: ");
      lcd.print(TEMPLOW);
      incrDecr(TEMPLOW, key);
    } 
  }

  else if (key[0] == '0' && key[1] == '4')
  {
    lcd.begin(16,2);
    lcd.print("Current HUMLOW: ");
    lcd.print(TEMPHIGH);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current TEMPHIGH: ");
      lcd.print(TEMPHIGH);
      incrDecr(TEMPHIGH, key);
    } 
  }

  else if (key[0] == '0' && key[1] == '5')
  {
    lcd.begin(16,2);
    lcd.print("Current HUMLOW: ");
    lcd.print(HUMLOW);
    char key = getKeyVal(0);
    while(key != "*")
    {
      lcd.begin(16,2);
      lcd.print("Current HUMHIGH: ");
      lcd.print(HUMLOW);
      incrDecr(HUMLOW, key);
    } 
  }
}


void incrDecr(int *value, char *key)
{
    lcd.begin(0,1);
    lcd.print("2=+1 8=-1 *=cancel");
    key = getKeyVal(0);
    if(key == "2")
    {
      value = value+1;
    }
    else if(key == "8")
    {
      value = value-1;
    }
}

void displaying()
{
  lcd.clear();
  lcd.begin(8,2);
  lcd.print("Displaying");
  lcd.setCursor(8,1);
  lcd.print("message");
}

void LCDSetup()
{
  lcd.clear();
  lcd.begin(16,2);
  lcd.print("GS: ");  
  lcd.print(GROWINGCYCLE);
  lcd.print(" Temp:");
  lcd.print(TEMPLOW);
  lcd.print("-");
  lcd.print(TEMPHIGH);
  lcd.setCursor(2,1);  
  lcd.print(" Hum: ");
  lcd.print(HUMLOW);
  lcd.print("-");
  lcd.print(HUMHIGH);
}

void fillPink()
{ 
  matrix.fillScreen(matrix.Color333(red,0,blue));
}

void colorLoop()
{
  // Testing color loop for red and blue will add green and yelow later
  // Will likely add green and yellows when a certian amount of loops have ran
  // The green and yellows wont be very many lines
  for(int i = 0; i < 64; i++)
  {
    if(i%2 == 0)
    {
      // make these the red lines
      matrix.drawLine(0,i,64,i,matrix.Color333(red,green,blue));
    }
    else
    {
      // make these the blue lines
      matrix.drawLine(0,i,64,i,matrix.Color333(red2,green,blue2));
    }
  }
  
  blue = blue - 1;
  green = green - 1;
  red2 = red2 - 1;
  
  if(red2 < 2)
  {
    red2 = red;
    green = red;
  }
  if(blue < 2)
  {
    blue = red;
    green = red;
  }
}

void colorLoop2()
{
  int low = 4;
  int high = 7;
  if(blue == low)
  {
    blue = high;
    if(green == low)
    {
      green = high;
      if(red == low)
      {
        red = high;
      }
      else
      {
        red--;
      }
    }
    else
    {
      green--;
    }
  }
  else
  {
    blue--;
  }
  matrix.fillScreen(matrix.Color333(red,green,blue));
}

void colorLoop3()
{
  //red full others empty
  //fill green
  int high = 7;
  int low = 4;
  if(red == high && blue == low && green < high)
  {
    green++;
  }
  //reduce red
  else if(green == high && blue == low && red > low)
  {
    red--;
  }
  //fill blue
  else if(green == high && red == low && blue < high)
  {
    blue++;
  }
  //reduce green
  else if(red == low && blue == high && green > low)
  {
    green--;
  }
  //fill red
  else if(blue == high && green == low && red < high)
  {
    red++;
  }
  //reduce blue
  else if(red == high && green == low && blue > low)
  {
    blue--;
  }
  matrix.fillScreen(matrix.Color333(red,green,blue));
}

void setup() 
{  
  Serial.begin(9600);   //is for the keypad
  red = 7;
  blue = 4;
  green = 4;
  period1 = 1000*60*50; // milliseconds*seconds*minutes*hours*days

  matrix.begin();
  fillPink();
  LCDSetup();
  startMillis = millis();
  colorLoop3(); 
}

void loop() 
{
  //first timer for color loop
  currentMillis = millis();  //get the current time
  if (currentMillis - startMillis >= period1)  //test whether the period has elapsed
  {
      colorLoop3();
      // restart loop
      startMillis = millis();
  }
}


