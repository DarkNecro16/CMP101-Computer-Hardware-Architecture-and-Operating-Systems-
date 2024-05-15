#include <TM1638plus.h>          // these are the libraries i included for my alarm clock the TM libraries are for the 7 seg expansion board DS3231 for RTC Adafruit for OLED , Servo for the servo arm
#include <TM1638.h>             // please note i have listed hardware connections throughout the commenting on the code 
#include <DS3231.h>
#include <Streaming.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>



#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(-1);


#define STROBE_TM D5 // strobe = GPIO connected to strobe line of module
#define CLOCK_TM D6 // clock = GPIO connected to clock line of module
#define DIO_TM D7 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true.
// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);
DS3231 rtc;
bool century = false;
bool h12Flag;   //used to distingusih between 12 hour time format and 24 hour 
bool pmFlag;

// variables for my alarm clock the detect os for the sensor the buzzer is connected to the D4 pin aswell as being grounded to the breadboard 
//the sensor is running from the D3 pin and is grounded to the breadboard and the VCC to the 5v input on wemos board
int alarmhour = 0;
int alarmmin = 0;
int detect = D3;
int Buzzer = D4;

Servo myservo;  // create servo object to control a servo this is connected to the second D5/SCK pin, its grounded and also VCC connected to the breadboard 



void setup() {
 pinMode (Buzzer, OUTPUT);    //the setup section is for initializing the components for example declaring here that the Buzzer varaible i created above will be recognised as an output and the detect variable recognised as an input
 pinMode (detect,INPUT);      // so i can declare for example if ( detect == low or detect == high ) will be used as conditions. Serial is for initializing communications from the serial monitor and this can be written to and from.
tm.displayBegin();
Wire.begin();
Serial.begin(115200);
myservo.attach(D5, 500, 2400);


   // -- OLED setup
 display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);  // this is for setting up the OLED here you can set the text size and colour
 display.display();
 delay(2000);
 display.clearDisplay();
 display.setCursor(0,0);
 display.setTextSize(2.5); // 
 display.setTextColor(WHITE);

}



void loop() {

// these variables store the information of the date and time and allow me to ammend them and write to and ammend the values stored on the RTC clock

int second = rtc.getSecond();
int minute = rtc.getMinute();
int hour = rtc.getHour(h12Flag, pmFlag);
int date = rtc.getDate();
int month = rtc.getMonth(century);
int year = rtc.getYear();
int buttons = tm.readButtons(); // this creates a variable allowing me to reference button pushes 
int detect = digitalRead(D3);  // the creates a variable for my sensor
int pos;                      // creates a variable for the servo arm 



// this first section allows me to set the hours on the 7 seg. once button 1 is pressed it will clear the screen and allow me to change the hours in string format it will then read that information to the RTC 
// if button 1 is pressed it will increase until 23 then reset to 0 and loop
// if button 2 is pressed it will decrease until 0 then read 0 and loop 
// if button 6 is pressed it will set that value to the rtc value of hours
// if button 8 is pressed it will break the loop and return to the time as it was before
// the buttons are recognised in bit format so button 1 = 1 bit, button 2 = 2 bit , 3 = 4 bit and then doubles from there 

if (buttons == 1){
  int hourset = 0;
  
  while(true){
    String s1 = String (hourset);
    tm.reset();
    tm.displayText(s1.c_str());
    buttons = tm.readButtons();
    
    if (buttons == 1){
      hourset++;
      if (hourset > 23) hourset = 0;
    }
    
    if (buttons == 2){
      hourset--;
      if ( hourset < 0 ) hourset =23;
    }
    if (buttons == 64){ //save
      //set hours
      rtc.setHour(hourset);
      break;
    }
    
    if (buttons == 128) break; //exit
    delay(100);
  }
}

// this first section allows me to set the minutes on the 7 seg. once button 2 is pressed it will clear the screen and allow me to change the minutes in string format it will then read that information to the RTC 
// if button 1 is pressed it will increase until 59 then reset to 0 and loop
// if button 2 is pressed it will decrease until 0 then read 59 and loop 
// if button 6 is pressed it will set that value to the rtc value of minutes
// if button 8 is pressed it will break the loop and return to the time as it was before


if (buttons == 2){
  ;
  int minset=0;
  
while(true){
    String s1 = String (minset);
    tm.reset();
    tm.displayText(s1.c_str());
    buttons = tm.readButtons();
    
    if (buttons == 1){
      minset++;
      if (minset > 59) minset = 0;
    }
    
    if (buttons == 2){
      minset--;
      if ( minset < 0 ) minset =59;
    }
    if (buttons == 64){ //save
      //set hours
      rtc.setMinute(minset);
      break;
    }
    if (buttons == 128) break; //exit
    delay(100);
  }
}


// this first section allows me to set the seconds on the 7 seg. once button 3 is pressed it will clear the screen and allow me to change the minutes in string format it will then read that information to the RTC 
// if button 1 is pressed it will increase until 59 then reset to 0 and loop
// if button 2 is pressed it will decrease until 0 then read 59 and loop 
// if button 6 is pressed it will set that value to the rtc value of seconds
// if button 8 is pressed it will break the loop and return to the time as it was before

if (buttons == 4){
  ;
  int secset=0;
  
while(true){
    String s1 = String (secset);
    tm.reset();
    tm.displayText(s1.c_str());
    buttons = tm.readButtons();
    
    if (buttons == 1){
      secset++;
      if (secset > 59) secset = 0;
    }
    
    if (buttons == 2){
      secset--;
      if ( secset < 0 ) secset =59;
    }
    if (buttons == 64){ //save
      //set hours
      rtc.setSecond(secset);
      break;
    }
    if (buttons == 128) break; //exit
    delay(100);
  }
}


// so this is the functonal part of my alarm clock. if button 4 is pressed remembering that buttons are in bit format then the screen will clear and allow me to set the hour of the alarm i wish to choose in string format.
// if button 1 is pressed it will increase until 23 then reset to 0 and loop
// if button 2 is pressed it will decrease until 0 then read 23 and loop 
// if button 6 is pressed it will set that value to the variable named alarmhour 
// if button 8 is pressed it will break the loop and return to the time as it was before

if (buttons == 8){
  ;
  int alarm1=0;
  
while(true){
    String s1 = String (alarm1);
    tm.reset();
    tm.displayText(s1.c_str());
    buttons = tm.readButtons();
    
    if (buttons == 1){
      alarm1++;
      if (alarm1 > 23) alarm1 = 0;
    }
    
    if (buttons == 2){
      alarm1--;
      if ( alarm1 < 0 ) alarm1 =23;
    }
    if (buttons == 64){ //save
      //set hours
      alarmhour=alarm1;
      break;
    }
    if (buttons == 128) break; //exit
    delay(100);
  }
}


// so this is the functonal part of my alarm clock. if button 5 is pressed remembering that buttons are in bit format then the screen will clear and allow me to set the minutes of the alarm i wish to choose in string format
// if button 1 is pressed it will increase until 59 then reset to 0 and loop
// if button 2 is pressed it will decrease until 0 then read 59 and loop 
// if button 6 is pressed it will set that value to the variable named alarmmin 
// if button 8 is pressed it will break the loop and return to the time as it was before

if (buttons == 16){
  ;
  int alarm2=0;
  
while(true){
    String s1 = String (alarm2);
    tm.reset();
    tm.displayText(s1.c_str());
    buttons = tm.readButtons();
    
    if (buttons == 1){
      alarm2++;
      if (alarm2 > 59) alarm2 = 0;
    }
    
    if (buttons == 2){
      alarm2--;
      if ( alarm2 < 0 ) alarm2 =59;
    }
    if (buttons == 64){ //save
      //set hours
      alarmmin=alarm2;
      break;
    }
    if (buttons == 128) break; //exit
    delay(100);
  }
}


// this creates a condition for displaytime, if the hours minutes or seconds are less than 10 ( single digits ) it will + a 0 to the value of hour,minute,second. it will also add a . at the end of hours and minutes so it can 
// be displayed in the correct format the else statements ensure that 0 is not added to the value to numbers greater than single digits but still adds the .

String displaytime1;
if (hour < 10) displaytime1 += "0" + String(hour) + ".";
else displaytime1 += String(hour) + ".";

if (minute < 10) displaytime1 += "0" + String(minute) + ".";
else displaytime1 += String(minute) + ".";

if (second < 10) displaytime1 += "0" + String(second);
else displaytime1 += String(second);



// displays the date and time on the oled screen using the RTC.
 display.clearDisplay();
 display.setCursor(0,0);
 display <<date << "/" << month << "/" << year << endl;
 display <<""<< endl;
  display <<hour << ":" << minute << ":" << second << endl;
 display.display();


// this displays the time on the 7 seg in string format with the LEDs counting up to 128 in 1 second so is effectivly a visual second counter 

 for (int i = 0; i < 257; i++)
 {
  tm.setLEDs(i*128);

tm.displayText(displaytime1.c_str());     // the TM1638 connections are, DI0 - D7, CLK - D6, STB - D5, VCC - positive on breadboard, GND - negataive on breadboard.
  delay(1);
}

  
  
// this is how my alarm works. if the minute or hour i have set on the RTC is = to the minute or hour i set as the alarm time on the TM1638 the alarm will trigger 

  if(  minute == alarmmin&& hour==alarmhour)
  {
 analogWrite(Buzzer,200);
 delay(100);
 analogWrite(Buzzer,100);
 
 }


 // once Infared beam on the sensor is broken AND the alarm is sounding this will trigger sleep mode and display " get up " on the TM1638 and " wakey wakey " on the OLED 
// the OLED connections are as followed, SDA - D2, SCL - D1, VCC - positive on breadboard, GRND - negative on breadboard.
  if (detect == LOW && minute == alarmmin && hour==alarmhour)
{

for (int i = 0; i < 7; i++)
{
   display.setTextSize(2);
   display.clearDisplay();
 display.setCursor(0,0);
 display << " WAKEY!! " << endl;
  display << " WAKEY!! " << endl;
  display.display();
  
tm.displayText ("GET UP");
    analogWrite(Buzzer,0);
    delay(1000);
    
    
// this will rotate the servo 180 degrees and back to 0 for 1 minute while the alarm is in sleep mode 

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  }
}



  

// this is the information from the RTC so it can keep track of the current date and time 
// the RTC connections are SCL - D1, SDA - D2, VCC - positive breadboard, GRND - negative breadboard
Serial << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << " " ;
Serial << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" << 
rtc.getSecond() << endl; 
  
}
