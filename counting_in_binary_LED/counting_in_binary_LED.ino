#include <TM1638plus.h> //include the library
#define STROBE_TM D5 // strobe = GPIO connected to strobe line of module
#define CLOCK_TM D6 // clock = GPIO connected to clock line of module
#define DIO_TM D7 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true.
// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);
byte buttons;
void setup() {
 tm.displayBegin();
 tm.reset();
// set up a pattern on the LEDs
 tm.setLEDs(0x0100); //The MSB of this 16 bit value controls which LEDS light up
}
void loop() {
 buttons = tm.readButtons(); // read which buttons are pressed
 tm.displayIntNum(buttons); // output the corresponding decimal number represented by the


for (int i = 0;  i < 257; i++ )

{
  if ( buttons == i ) 
  {
    tm.setLEDs(256*i);
  }
}
}



 
