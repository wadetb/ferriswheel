#include <bluefruit.h>
#include <Nffs.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(174, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(145, 11, NEO_GRB + NEO_KHZ800);

#define NACTS 2

struct Conf 
{
  int act;
  char pad[36];  
};

Conf conf;

void setup() 
{
  Serial.begin(115200);
  
  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();

#if 1
  Bluefruit.begin();
  Nffs.begin();

  {
    Serial.println("reading config");
    NffsFile inFile("/ferriswheel2.dat", FS_ACCESS_READ);
    if (inFile.exists())
    {
      int readb = inFile.read((unsigned char*)&conf, sizeof(Conf));
      Serial.printf("config read %d\n", readb);
      inFile.close();
    }
  }

  Serial.printf("act %d\n", conf.act);

  {
    Serial.println("writing next config");
    NffsFile outFile("/ferriswheel2.dat", FS_ACCESS_WRITE);
    if (outFile.exists())
    {
      Conf nextConf = conf;
      nextConf.act = (nextConf.act + 1) % NACTS;
      Serial.printf("nextact %d %d\n", nextConf.act, sizeof(Conf));
      int wrote = outFile.write((unsigned char*)&nextConf, sizeof(Conf));
//      file.write(CONTENTS, strlen(CONTENTS));
      Serial.printf("config written %d %d\n", wrote, outFile.errnum);
      outFile.close();
    }
  }
#endif
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int t = 0;

void loop() 
{
  Serial.printf("loop\n");

  if (conf.act == 0)
  {
    t ++;

   // Serial.printf("t %d\n", t);

    for (int i=3-t%3; i < strip1.numPixels(); i+=3)
      strip1.setPixelColor(i, Wheel((t*3+i) % 255));
    for (int i=3-t%3; i < strip2.numPixels(); i+=3)
      strip2.setPixelColor(i, Wheel((t*3+i) % 255));
      
    strip1.show();
    strip2.show();

    delay(250);

    for (int i=3-t%3; i < strip1.numPixels(); i+=3)
      strip1.setPixelColor(i, 0);
    for (int i=3-t%3; i < strip2.numPixels(); i+=3)
      strip2.setPixelColor(i, 0);
}
  else if (conf.act == 1)
  {
    for (uint16_t i=95; i < strip1.numPixels()-30; i=i+3)
      strip1.setPixelColor(i, Wheel(i % 255));
    for (uint16_t i=85; i < strip2.numPixels(); i=i+3)
      strip2.setPixelColor(i, Wheel(i % 255));
      
    strip1.show();
    strip2.show();
  }
}

#if 0
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip1.show();

      for (uint16_t i=0; i < strip2.numPixels(); i=i+3) {
        strip2.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip2.show();

      delay(wait);

      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (uint16_t i=0; i < strip2.numPixels(); i=i+3) {
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
#endif

