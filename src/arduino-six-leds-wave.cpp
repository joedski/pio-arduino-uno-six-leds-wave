#include <Arduino.h>
#include <FastLED.h>

#define _GAMMA_ 2.6
const int _GBASE_ = __COUNTER__ + 1; // Index of 1st __COUNTER__ ref below
#define _G1_ (uint8_t)(pow((__COUNTER__ - _GBASE_) / 255.0, _GAMMA_) * 255.0 + 0.5),
#define _G2_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_        // Expands to 8 items
#define _G3_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_        // Expands to 64 items
const uint8_t PROGMEM gammaTable[] = {_G3_ _G3_ _G3_ _G3_}; // 256 items

struct LEDString
{
  int pin;
  uint8_t offset;

  uint8_t animationAt(uint8_t progress8)
  {
    uint8_t offsetProgress = progress8 + offset;
    if (offsetProgress < 128)
    {
      // Start at 0, go up to 255, then back down to 0.
      return 255 - cos8(offsetProgress * 2);
    }
    return 0;
  }
};

LEDString ledStrings[6] = {
    {.pin = 3, .offset = 0},
    {.pin = 5, .offset = (uint8_t)(255.0 * 1.0 / 6.0)},
    {.pin = 6, .offset = (uint8_t)(255.0 * 2.0 / 6.0)},
    {.pin = 9, .offset = (uint8_t)(255.0 * 3.0 / 6.0)},
    {.pin = 10, .offset = (uint8_t)(255.0 * 4.0 / 6.0)},
    {.pin = 11, .offset = (uint8_t)(255.0 * 5.0 / 6.0)},
};

// Sometimes I only need a couple of the strips
// This provides a convenient obviously-named way to change that.
// Anything more complex, just change the ledStrings items.
#define MAX_STRIP_TO_ANIMATE 6

void setup()
{
  for (int i = 0; i < MAX_STRIP_TO_ANIMATE; ++i)
  {
    pinMode(ledStrings[i].pin, OUTPUT);
  }
}

uint8_t animationProgress = 0;
uint8_t animationIncrement = 1;
// 15 ms gives a nice lazy approx 3.8s loop.
// 4 ms gives about 1s loop. (approx 1000ms / 256 = 4ms)
uint8_t animationDelay = 4;

uint8_t gammaCorrect(uint8_t value)
{
  return pgm_read_byte(&gammaTable[value]);
}

void loop()
{
  delay(animationDelay);

  for (int i = 0; i < MAX_STRIP_TO_ANIMATE; ++i)
  {
    analogWrite(ledStrings[i].pin, gammaCorrect(ledStrings[i].animationAt(animationProgress)));
  }

  animationProgress += animationIncrement;
}
