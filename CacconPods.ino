/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/

/*
State machine:
Pulsing - random period & duration every 30 sec
every 15 sec - random Start-End -> crawl there
*/

#include <OctoWS2811.h>

#define RED    0xFF0000
#define BLCK   0x000000
#define PUR    0xFF00FF
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE1 0xFFFFFF

//number of LED per srip is 180 - 
// 30 LED per meter, each strip is 1.5 meters, 4 strips in total = 6 meters = 180 LEDs
const int ledsPerStrip = 180;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int max_led;
int min_led;

int dest_max_led;
int desl_min_led;

int periodePulse = random(3000,6000);
int displacePulse = random(1000,3000);     
long time=0;

int color_value;

long crawlTimeDelaysec = 15000; // 15 sec
long crawlCountTime = 0;

long steplTimeDelaysec = 500; //0.5 sec
long steplCountTime = 0;

bool crawlMode;

void setup() {
  leds.begin();
  leds.show();
  //starting the program - kill all the lights - make them dark
  colorWipe(BLCK, microsec);
}

void loop() {
  //2000ms between each cycle loop
  // change them all in 2 seconds
  int microsec = 2000000 / leds.numPixels();  

  //no delay mechanism for counting

  if (abs(millis() - crawlCountTime) > crawlTimeDelaysec) {
  {
    //create random led cound to light up, must be bigger than 10
    //in order to create a feeling that there is something living inside the pods
    while (max_led - min_led < 10)
    {
      max_led = random(0,ledsPerStrip-1);
      min_led = random(0,max_led);
    }  
    crawlCountTime = millis();
    crawlMode = true;
  }    

  
  //if the Start-End chaned start to crwal.. go to crawl mode.
  if (crawlMode)
    if (abs(millis() - steplCountTime) > steplTimeDelaysec) {
      if (max_led < dest_max_led) 
        max_led++;
       if (min_led > dest_min_led)
        min_led--;
      steplCountTime = millis();
    }


  time = millis();
  //smart cos fade of the leds
  color_value = 128+127*cos(2*PI/periodePulse*(displacePulse-time));

  //RED    0xFF0000 = 16711680d (255*256*256)

  colorWipe(1,30,(color_value*256*256),0);

}

void colorWipe(int startLED, int endLED, int color, int wait)
{
  //kill all the leds
  for (int i=0; i < ledsPerStrip; i++)
    leds.setPixel(i, BLCK);
  //light just one one we need - useful when crawling
  for (int i=startLED; i =< endLED; i++)
    leds.setPixel(i, color);
  leds.show();
  // I don't how smart the Arduino complier is...
  if (wait > 0)
    delayMicroseconds(wait); 
}

