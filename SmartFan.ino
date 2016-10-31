#include "DHT.h" //the dht library is an external library for use of the DHT
#define DHTPIN 13 //the dht is connected to pin 13
#include <LiquidCrystal.h> /*the liquidcrystal library is an external library for use of 
an lcd display*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); /*the lcd is connected to multiple pins - 12,11,
5,4,3 and 2*/
#define DHTTYPE DHT22 //there are multiple models of the DHT, this circuit uses DHT22
DHT dht(DHTPIN, DHTTYPE); //this creates a new dht object using the dht library

int humiditytrue; //this integer stores the value of humidity in the room 
int temperaturetrue; //this integer stores the value of temperature in the room
int humidityset; //this integer stores the value of humidity desired by the user
int temperatureset; //this integer stores the value of temperature desired by the user
int motorPin = 9; //digital pin. pin that motor is connected to.
int buttonPin = 10; //digital pin. pin that pushbutton is connected to.
int infrared = 2; //analog pin. pin that infrared is connected to.
int pressure = 3; //analog pin. pin that pressure sensor is connected to.
int redPin = 6; //digital pin. pin that red led of multicolor led is connected to.
int greenPin = 7; //digital pin. pin that green led of multicolor led is connected to.
int bluePin = 8; //digital pin. pin that blue led of multicolor led is connected to.

boolean person = true; //this boolean stores whether or not a person is in the room
boolean on = false; //this boolean stores whether or not the system is turned on
boolean detectedperson = false; /*this boolean stores whether or not a person has been 
detected in the past 5 seconds by the infrared sensor*/
boolean detectedpress = false; /*this boolean stores wheter or not the pushbutton has been 
pressed in the past 500 milliseconds*/
boolean detectedpressure = false; /*this boolean stores wheter or not the pressure sensor 
has been pressed in the past 500 milliseconds*/
unsigned long timerperson = 0; /*this long records the time at which the infrared sensor
last detected a person */
unsigned long timerpress = 0; /*this long records the time at which the pushbutton last
detected a push */
unsigned long timerpressure = 0; /*this long records the time at which the pressure sensor 
last detected a press */

void setup() 
{
  lcd.begin(16, 2); //this defines the lcd as one with 16 columns and 2 rows
  dht.begin(); //this begins communication with the dht sensor
  pinMode(motorPin,OUTPUT); //this defines the pin of the motor as an output
  pinMode(redPin,OUTPUT); //defines the red  pin of the multicolor led as an output
  pinMode(greenPin,OUTPUT); //defines the green  pin of the multicolor led as an output
  pinMode(bluePin,OUTPUT); //defines the blue led pin of the multicolor led as an output
  pinMode(buttonPin,INPUT); //this defines the pushbutton as an input
  digitalWrite(redPin, LOW); //this turns on the red led of the multicolor led
  /*due to the nature of the circuit, setting an LED as low turns it on, and setting it 
  as high turns it off*/
}
void printlcd()
{
  lcd.setCursor(0,0); //sets the cursor to the beginning of the first line
  lcd.print("Set: "); /*prints set to indicate that the following temperature
  is the user set temperature */
  lcd.print(temperatureset); //prints the user set temperature
  lcd.print("|Room: "); /*prints room to indicate that the following temperature is the 
  room temperature. (the line in the middle is to separate the two values)*/
  lcd.print(temperaturetrue); //prints the room temperature
  lcd.setCursor(0,1); //sets the cursor to the beginning of the second line
  lcd.print("Set: "); /* prints set to indicate that the following humidity is the
  user set humidity */
  lcd.print(humidityset); //prints the user set humidity
  lcd.print("|Room: ");/*prints room to indicate that the following humidity is the
  room humidity. (the line in the middle is to separate the two values)*/
  lcd.print(humiditytrue); //prints the room humidity
}
void checkvars()
{
  if (digitalRead(buttonPin) == LOW && detectedpress == false)   /*If a push is detected
  for the first time, a new timer will be started using the current time
   as a starting point, and detectedpress will be made true. */
  {
    timerpress = millis(); //sets current time as the starting time for the timer
    detectedpress = true; //sets detectedpress as true
  }
  if (digitalRead(buttonPin) == LOW && detectedpress == true && (millis()-timerpress>500UL)) 
  /*if detectedpress is true (meaning the timer has started), 
   checks whether five seconds have passed. If they have, and the press is still being detected,
   it switches the state of on and turns detectedpress to false again*/
  {
    on=!on; //switches the state of on
    detectedpress = false; //sets detectedpress to false
  }
  if (digitalRead(buttonPin) == HIGH && detectedpress == true && (millis()-timerpress>500UL)) 
  /*if detectedpress is true (meaning the timer has started), 
   checks whether five seconds have passed. If they have, and the press is not being detected,
   it keeps the state of on the same and turns detectedpress to false again*/
    detectedpress = false; //sets detectedpress to false
  if ((person ==  false && analogRead(infrared)>900) && !detectedperson) /*If a person is detected 
  for the first time and person is currently false, 
   a new timer will be started using the current time as a starting point, 
   and detectedperson will be made true. */
  {
    timerperson = millis(); //sets current time as the starting time for the timer
    detectedperson = true; //sets detectedperson as true
  }
  if ((person ==  false && analogRead(infrared)>900) && detectedperson && (millis()-timerperson>5000UL)) 
  /*if detectedperson is true (meaning the timer has started), 
   checks whether five seconds have passed. If they have, and the person is still being detected,
   it sets person to true and sets detectedperson to false again*/
  {
    detectedperson = false; //sets detectedperson to false 
    person = true; //sets person to true
  }
  if ((person ==  true && analogRead(infrared)<900) && !detectedperson)
  /*If a person is not detected time and person is currently true, 
   a new timer will be started using the current time as a starting point,
   and detectedperson will be made true. */
  {
    timerperson = millis(); //starts a new timer with the current time as a starting time
    detectedperson = true; //sets detectedperson to false
  }
  if ((person ==  true && analogRead(infrared)<900) && detectedperson && (millis()-timerperson>5000UL))
  /*if detectedperson is true (meaning the timer has started), 
   checks whether five seconds have passed. If they have, and the person is still not being detected, 
   it it sets detectedperson to false and person to false.*/
  {
    detectedperson = false; //sets detectedperson to false
    person = false; //sets person to false
  }
  if (millis()-timerperson>5000UL) /*five seconds after a timer has been started,
  this loop turns the timers off again. */
    detectedperson = false; //sets detectedperson to false
  if ((analogRead(pressure)>150) && detectedpressure == false) /* If the pressure sensor is pushed 
  (with threshhold 150), and it has not been pushed in the last 500 milliseconds, detectedpressure will
  be made true and a timer will be started with the current time as its starting time. */
  {
    timerpressure = millis(); //starts new timer
    detectedpressure = true; //sets detectedpressure to true
  }
  if ((analogRead(pressure)>150) && detectedpressure == true && (millis()-timerpressure>500UL))
  /*if .5 seconds later the push is still detected, person will
   change its state and detectedpressure will be made false again. Furthermore, detectedperson will
   be made false and timerperson reset. This prevents  the infrared sensor from changing the state 
   of person until at least 5 seconds after this loop has run.*/
  {
    person=!person; //switches state of person
    detectedpressure = false; //sets detectedpressure to false
  }
  if ((analogRead(pressure)<150) && detectedpressure == true && (millis()-timerpressure>500UL))
  /*after half a second, if no push is detected, the push is considered a
   false press. detectedpressure is made false again.*/ 
  detectedpressure = false; //sets detectedpressure to false
}
void loop() 
{
  while (person && on) /*if a person is detected and the system is on,
  turns the fan on/off to achieve desired temperature and humidity. */
  {
    humiditytrue = dht.readHumidity(); /* updates humiditytrue by reading the DHT's 
    humidity and storing it */
    temperaturetrue = dht.readTemperature(); /* updates humiditytrue by reading the 
    DHT's humidity and storing it */
    humidityset = 100 - (map(analogRead(0),0,1023,0,100)); /* maps the input of the top
    potentiometer to a humidity value between 40 to 60 */
    temperatureset = 50-(map(analogRead(1),0,1023,0,50)); /* maps the imput of the bottom 
    potentiometer to a temperature value between 16 and 32 */
    printlcd(); //calls a method that updates the values displayed on the lcd screen
    if (humiditytrue>humidityset||temperaturetrue>temperatureset) /*checks if either of 
    the set humidity and temperature values are less than the room temperature or humidity.
    If so, turns the motor on and sets the led to blue. */
    {
      digitalWrite(motorPin,HIGH); //turns the motor on
      digitalWrite(greenPin, HIGH); //turns the green led off to avoid mixing of colors
      digitalWrite(redPin, HIGH); //turns the red led off to avoid mixing of colors
      digitalWrite(bluePin, LOW); //turns the blue led on
    }
    else /* If the above conditions are not met, meaning the set temperature and humidity
    are greater than the room temperature and humidity, turns the motor off 
    and the green led on.*/
    {
      digitalWrite(motorPin,LOW); //turns the motor off
      digitalWrite(greenPin, LOW); //turns the green led on
      digitalWrite(redPin, HIGH); //turns the red led off to avoid mixing of colors
      digitalWrite(bluePin, HIGH); //turns the blue led off to avoid mixing of colors
    }
    checkvars(); //updates all variables using the checkvars method
  }
  while (!person||!on) /*if a person is not detected or if the system is off, prints
  a message to the user and sets the led to a certain color. */
  { 
    if (on == false) /* if the fan is turned off, turns the motor off, sets the led
    to red, and displays a message to the user */
    {
      digitalWrite(motorPin,LOW); //turns the motor off
      digitalWrite(greenPin, HIGH); //turns the green led off to avoid mixing of colors
      digitalWrite(redPin, LOW); //turns the red led on
      digitalWrite(bluePin, HIGH); //turns the blue led off to avoid mixing of colors
      lcd.setCursor(0,0); //sets the cursor to the beginning of the first line
      lcd.print("Smart Fan is off."); //prints smart fan is off
      lcd.setCursor(0,1); //sets the cursor to the beginning of the second line
      lcd.print("Press pushbutton."); //prints press pushbutton   
    }
    else /*If the above if statement is false, it means that the person variable is false. 
    This means a person was not detected by the system. Therefore, the system turns the 
    motor off, the led to white, and prints a message to the user*/
    {
      digitalWrite(motorPin,LOW); //turns the motor off
      digitalWrite(greenPin, LOW); /*turns the green led on, 
      along with red and blue, to make white */
      digitalWrite(redPin, LOW); /*turns the red led on, 
      along with green and blue, to make white */
      digitalWrite(bluePin, LOW); /*turns the blue led on, 
      along with red and green, to make white */
      lcd.setCursor(0,0); //sets the cursor to the beginning of the first line
      lcd.print("No person found."); //prints no person found
      lcd.setCursor(0,1); //sets the cursor to the beginning of the second line
      lcd.print("Fan is sleeping."); //prints fan is sleeping
    }
    checkvars(); //updates all variables using the checkvars method
  }
}

/*
The following segment of code was created to scroll text on the arduino, but its use created a problem:
 while using this method to scroll text, the calling method was paused. This meant the only time the fan
 would update its variables was the instant the text stopped scrolling. Hence, if either the pushbutton
 or pressure sensor detected someone while the text was scrolling, the code would not detect it.
 */
/*
void scroll( char *text) //recieves character array as input
{
  int length = strlen(text);  //creates length variable with length of array
  if(length < 16) //if the text can fit without scrolling, prints the text
    lcd.print(text); //prints the text
  else //if text does not fit, scrolls text
  {
    int pos; //creates integer pos
    for( pos = 0; pos < 16; pos++) //iterates pos from 0 to 16
      lcd.print(text[pos]); //prints the letter at position pos on the array text
 //what the above for loop basically does is print the first 16 letters of the array text on the screen
    delay(1000); //waits for 1 second so user can read this text
    pos=1; //sets pos to 1
    while(pos <= length - 16) //prints out the rest of the array text
    {
      lcd.setCursor(0,0); //sets cursor back to beginning of line one
      for( int i=0; i < 16; i++) //iterates i from 0 to 16
        lcd.print(text[pos+i]); //prints the letter at position pos + i in the array text
 /*what the above for loop basically does is print 16 letters from the array text starting
 with the letter at position pos */
      /* delay(300); //waits for .3 seconds so user can read the new character
      pos++; //increments pos. this means that the next 16 letters printed will be the same as the last 16 
       //except with the first letter taken away and all the other letters moved one to the left. 
       the last letter will then become the next letter in the array text. over time,
       this will scroll the whole of array text on the lcd. *//*
    }
  }
}
*/


