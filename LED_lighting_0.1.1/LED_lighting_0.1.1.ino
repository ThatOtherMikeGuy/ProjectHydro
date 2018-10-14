// Original code provided by Michael Schwindt and Kaylyn Wessel 
// Edited by Mike Schwindt 
// Version 0.1.1
// 8/10/18
// Have new power supply so these edits are for testing the entire board lighting up
// left all previous code because I plan to impliment a keypad and LCD into the project. I'm not exactly sure how
// yet but will implpiment testing when power source for LED board comes in. This is very basic protyping for testing 
// basic color rotation on individual lines since I will need to have a dominant range of red and blue for the LED's 
// since that is what growing light LED colors need to be based on research. Some greens and yellows may be implimented
// in the near future. 
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
int red = BRIGHTNESS;
int blue = BRIGHTNESS;
int green = BRIGHTNESS;
int red2 = red;
int blue2 = blue;

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

// this is where we will add future messages
void messages(char *key, bool testing = false)
{
  int displayTime = 25000;
  if(testing)
  {
    displayTime = displayTime/4;
  }
  
  if (key[0] == '0' && key[1] == '1')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();    
    matrix.begin();
    
    greenAndHello();
          
    matrix.setTextSize(1);   //size 1 == 8 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(20, 3); 
    matrix.println("Your");
    matrix.setCursor(11, 12);
    matrix.println("brights");
    matrix.setCursor(14, 20);  
    matrix.println("are on");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0)); 
  }

  else if (key[0] == '0' && key[1] == '2')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();
    matrix.begin();
  
    greenAndHello();
        
    matrix.setTextSize(1);   //size 1 == 8 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(12, 3);  //start 12 pixels in
    matrix.println("CAUTION");
    matrix.setCursor(2, 12);  //start 2 pixels in
    matrix.println("Animals on");
    matrix.setCursor(8, 20);  //start 8 pixels in
    matrix.println("the road");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
    else if (key[0] == '0' && key[1] == '3')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();
    matrix.begin();
  
    greenAndHello();
        
    matrix.setTextSize(2);   //size 2 == 16 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(9, 0);  //start 9 pixels in
    matrix.println("Pass");
    matrix.setCursor(20, 17);  //start 20 pixels in
    matrix.println("Me");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
  else if (key[0] == '0' && key[1] == '4')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();
    matrix.begin();
  
    greenAndHello();
        
    matrix.setTextSize(1);   //size 1 == 8 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(12, 3);  //12 pixels in
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,0));
    matrix.println("CAUTION");
    matrix.setCursor(7, 12);  //7 pixels in
    matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
    matrix.println("Road Work");
    matrix.setCursor(17, 20); 
    matrix.println("ahead");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
  else if (key[0] == '0' && key[1] == '5')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();
    matrix.begin();
  
    greenAndHello();
        
    matrix.setTextSize(1);  
    matrix.setTextWrap(false);
    matrix.setCursor(12, 3);  
    matrix.println("Road is");
    matrix.setTextSize(2);   
    matrix.setCursor(13, 12);  
    matrix.setTextColor(matrix.Color333(0,BRIGHTNESS-1,BRIGHTNESS));
    matrix.println("ICY");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
    else if (key[0] == '0' && key[1] == '9')
  {
    // run the method and
    // if this return false that means the user canceled so leave this function without printing anything
    if(!lcdMessages(key))
    {
      messageCancel();
      return;
    }
    displaying();
    matrix.begin();
  
    greenAndHello();
        
    matrix.setTextSize(2);   //size 1 == 8 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(8, 0);  //start at the top and 2 pixels in
    matrix.println("FUCK");
    matrix.setCursor(15, 17);  //start at the top and 2 pixels in
    matrix.println("YOU");
    matrix.setCursor(8, 20);  //start at the top and 2 pixels in
    //matrix.println("the road");
    delay(displayTime);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
  else if (key[0] == '5' && key[1] == '0')
  {    
    displaying();
    matrix.begin();
  
    //greenAndHello();
        
    matrix.setTextSize(2);   //size 1 == 8 pixels high
    matrix.setTextWrap(false);
    matrix.setCursor(15, 0);  //start at the top and 2 pixels in
    matrix.println("COP");
    matrix.setCursor(3, 17);  //start at the top and 2 pixels in
    matrix.println("AHEAD");
    delay(displayTime/2);
    // fill the screen with 'black'
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
  // button to change brightness
  else if(key[0] == '0' && key[1] == '#')
  {
    lcdMessages(key);
    return;    
  }
  // say this key isnt recognized
  else
  {
    lcdMessages(key);
    return;
  }
}

char getKeyVal(int timer = 1500)
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
  lcd.begin(8,2);
  lcd.print("Growing stage: ");  
  lcd.print(GROWINGCYCLE);
  lcd.setCursor(1,1);
  lcd.print("Enter Command");
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
      matrix.drawLine(0,i,64,i,matrix.Color333(red,0,blue));
    }
    else
    {
      // make these the blue lines
      matrix.drawLine(0,i,64,i,matrix.Color333(red2,0,blue2));
    }
  }
  
  blue = blue - 1;
  red2 = red2 - 1;
    
  if(red2 < 0)
  {
    red2 = red;
  }
  if(blue < 0)
  {
    blue = red;
  }
}

void colorloop2()
{
  int r = 6;
  int b = 7;
  int g = 3;
  for(int i = 0; i < 80; i++)
  {
    matrix.fillScreen(matrix.Color333(r,g,b));
    delay(500);
    if(r < 7 && r > -1)
    {
      r--;
    }
    else if(r == 0)
    {
      r =7;
    }
    else
    {
      b--;
    }
    if(b == 0)
    {
      r = 6;
      b = 7;
    }
  }
}

void setup() 
{  
  Serial.begin(9600);   //is for the keypad
  matrix.begin();
  fillPink();
}

void loop() 
{
  // this delay sets the time for color loop (1000 = 1 sec)
  LCDSetup();
  delay(30000);
  //colorloop2();
  
  colorLoop();


  // old project code
//  Serial.println("here");
//  LCDSetup();
//  char totalKey[2];
//  bool testing = true;
//  totalKey[0] = getKeyVal(0);
//  // give feedback to the user
//  lcd.begin(8,2);
//  lcd.print(totalKey[0]);
//  totalKey[1] = getKeyVal();
//  if(totalKey[1] == '?')
//  {
//    char tmp = totalKey[0];
//    totalKey[0] = '0';
//    totalKey[1] = tmp;
//  }
//  // show the user their command briefly before moving on
//  lcd.begin(8,2);
//  lcd.print(totalKey[0]);
//  lcd.print(totalKey[1]);
//  delay(500);
//  messages(totalKey, testing);
//  lcd.clear();
//  lcd.begin(8,2);
//  lcd.print("OK to send again");
//  delay(1000);
//  matrix.setTextColor(matrix.Color333(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS));
}


