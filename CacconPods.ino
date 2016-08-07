/*
// Based on Adafruit Neopixel example https://github.com/adafruit/Adafruit_NeoPixel 

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

/*
State machine:
Pulsing - random period & duration every 30 sec
every 15 sec - random Start-End -> crawl there
*/


#include <Adafruit_NeoPixel.h>

//just for the beginning, later I'll might divide it to 4 pins so
//we won't have data wires between the led strips
#define PIN 23

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

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledsPerStrip, PIN, NEO_GRB + NEO_KHZ800);

//catterpilar dimensions
int min_cat = 10;
int max_cat = 80;

int max_led;
int min_led;

int dest_max_led;
int dest_min_led;

int periodePulse = random(3000,6000);
int displacePulse = random(1000,3000);     
long time=0;

uint32_t color_value;

unsigned long crawlTimeDelaysec = 10000; // 10 sec
unsigned long crawlCountTime = 0;

unsigned long stepTimeDelaysec = 25; //0.025 sec
unsigned long stepCountTime = 0;

unsigned long pulseTimeDelaysec = 10; //0.01 sec
unsigned long pulseCountTime = 0;

bool crawlMode = false;

void colorWipe(int startLED, int endLED, int color, int wait)
{
  //kill all the leds
  for (int i=0; i < ledsPerStrip; i++)
    strip.setPixelColor(i, BLCK);
  //light just one one we need - useful when crawling
  for (int i=startLED; i <= endLED; i++)
    strip.setPixelColor(i, color,0,0);
  strip.show();
  // I don't how smart the Arduino complier is...
  if (wait > 0)
    delayMicroseconds(wait); 
}


void setup() 
{
  
  //starting the program - kill all the lights - make them dark
  colorWipe(0,ledsPerStrip, BLCK, 0);

  for (int i=0; i < ledsPerStrip; i++)
    strip.setPixelColor(i, 0,0,0);
    
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  crawlMode = true;
  max_led = 50;
  min_led = 20;
}

void loop() 
{
  
  //2000ms between each cycle loop
  // change them all in 2 seconds
  int microsec = 2000000 / ledsPerStrip;

  //no delay mechanism for counting

  if ((abs(millis() - crawlCountTime) > crawlTimeDelaysec) and crawlMode == false)
  {
    //create random led cound to light up, must be bigger than 10
    //in order to create a feeling that there is something living inside the pods
    while (((dest_max_led - dest_min_led) < min_cat) or ((dest_max_led - dest_min_led) > max_cat))
    {
      dest_max_led = random(0,ledsPerStrip-1);
      dest_min_led = random(0,dest_max_led);
      crawlMode = true;
      
    }  
    crawlCountTime = millis();
    
  }    

  
  //if the Start-End chaned start to crwal.. go to crawl mode.
  if (crawlMode)
    if (abs(millis() - stepCountTime) > stepTimeDelaysec) {
      if (max_led < dest_max_led) 
        max_led++;
      if (max_led > dest_max_led) 
        max_led--;
      if (min_led < dest_min_led)
        min_led++;
      if (min_led > dest_min_led) 
        min_led--;

      //to restart the crawler -random mode
      if (max_led == dest_max_led and min_led == dest_min_led) 
      {
        crawlMode = false;
        dest_max_led = 0;
        dest_min_led = 0;
      }
      stepCountTime = millis();
    }

  
  //smart cos fade of the leds
  time=millis();
  color_value = 128+127*cos(2*PI/periodePulse*(displacePulse-time));
  
  //RED    0xFF0000 = 16711680d (255*256*256)
  colorWipe(min_led,max_led,255,0);    
  strip.setBrightness(color_value);
  strip.show();

  

}


