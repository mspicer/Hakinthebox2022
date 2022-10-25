//SAINTCON 2022 Hack in the Box UT Badge Code
//Written by -Weston Shakespear-

// #define DEBUG 1
// #define TEST 1

#define PIN_UART_TX 0
#define PIN_LED_DOUT 1


#define NUM_PIXELS 12

#include <tinyNeoPixel.h>


void HSVtoRGB(int pixel, float H, float S,float V);

#if defined(DEBUG)

  #include <SendOnlySoftwareSerial.h>
  SendOnlySoftwareSerial mySerial(PIN_UART_TX);

#endif


tinyNeoPixel pixels = tinyNeoPixel(NUM_PIXELS, PIN_LED_DOUT, NEO_GRB + NEO_KHZ800);

#define FRAMES 9
const int frames[FRAMES][12] = {
  {1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1}, //0left
  {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0}, //1mid left
  {0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0}, //2right
  {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1}, //3front
  {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, //4mid front
  {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0}, //5back
  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //6base
  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}, //7mid
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, //8top
};

#define ANIMS 4
const int anims[ANIMS][4] = {
                            {0, 5, 2, 3},
                            {0, 2, 0, 2},
                            {3, 5, 3, 5},
                            {6, 7, 8, 7}
                          };

void setup() {
  pixels.begin();
  pixels.setBrightness(100);

  #if defined(DEBUG)

    mySerial.begin(9600);

  #endif

}

void loop() {

  #if defined(TEST)


  int H = 180 + (180 * sin(millis()/100));
  int S = 50 + (50 * sin(millis()/150));
  int V = 50 + (50 * sin(millis()/20));
  for (int i = 0; i < NUM_PIXELS; i++) {
    HSVtoRGB(i, H, S, V);
    // pixels.setPixelColor(i, pixels.Color(40, 0, 0));

    if (i != 0) {
      pixels.setPixelColor(i-4, pixels.Color(0, 0, 0));
    } else {
      pixels.setPixelColor(NUM_PIXELS - 1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(NUM_PIXELS - 2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(NUM_PIXELS - 3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(NUM_PIXELS - 4, pixels.Color(0, 0, 0));
    }

    pixels.show();
    delay(100);
  }
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(20);
  }



  #else

  int MIN_RANDOM = -1 * random(0, 4);
  int CHANCE_ANIM = random(0, 6);

  int CHANCE_WIPE_NO = -1 * random(0, 2);
  int CHANCE_WIPE_YES = random(0, 2);

  int UPDATE_H_Y = -1 * random(0, 4);
  int UPDATE_H_N = random(0, 4);

  int UPDATE_S_Y = -1 * random(0, 4);
  int UPDATE_S_N = random(0, 4);

  int UPDATE_V_Y = -1 * random(0, 4);
  int UPDATE_V_N = random(0, 4);

  int UPDATE_DELAY_CHANCE = random(0, 8);

  int anim = -1;
  int frame = 0;
  int wipe = 0;
  int custom[4] = {1, 5, 3, 2};

  int H = 180 + (180 * sin(millis()/100));
  int S = 75 + (25 * sin(millis()/150));
  int V = 50 + (50 * sin(millis()/20));

  byte delayTime = 100;

  while (true) {
    if (random(UPDATE_H_Y, UPDATE_H_N) < 0) {
      H = 180 + (180 * sin(millis()/100));
    }

    if (random(UPDATE_S_Y, UPDATE_S_N) < 0) {
      S = 75 + (25 * sin(millis()/150));
    }

    if (random(UPDATE_V_Y, UPDATE_V_N) < 0) {
      V = 50 + (50 * sin(millis()/20));
    }

    if (random(0, CHANCE_ANIM) == 1) {
      anim = random(MIN_RANDOM, ANIMS);
    }

    wipe = random(CHANCE_WIPE_NO, CHANCE_WIPE_YES);


    if (anim < 0) {
      for (int i = 0; i < 4; i++) {
        custom[i] = random(0, FRAMES);
      }
    }

    for (int section = 0; section < 4; section++) {
      if (anim < 0) {
        frame = custom[section];
      } else {
        frame = anims[anim][section];
      }


      for (int led = 0; led < 12; led++) {
        if (frames[frame][led] == 1) {
          HSVtoRGB(led, H, S, V);
        } else {
          if (wipe > 0) {
            pixels.setPixelColor(led, pixels.Color(0, 0, 0));
          }

        }
      }
      pixels.show();
      if (random(0, UPDATE_DELAY_CHANCE) == 0) {
        delayTime = 5 * random(10, 100);
      }
      delay(delayTime);

    }
  }



  #endif

  #if defined(DEBUG)

    mySerial.println("hello from debug stuff");

  #endif

}

void HSVtoRGB(int pixel, float H, float S,float V) {
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    pixels.setPixelColor(pixel, pixels.Color(R, G, B));

}
