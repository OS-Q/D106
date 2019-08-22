
#include <WS2812FX.h>

// include the custom effects
// #include "custom/BlockDissolve.h"
#include "custom/DualLarson.h"
// #include "custom/MultiComet.h"
// #include "custom/Oscillate.h"
// #include "custom/RainbowLarson.h"
// #include "custom/RandomChase.h"
// #include "custom/TriFade.h"
// #include "custom/VUMeter.h"

#define LED_COUNT 24
#define LED_PIN 5

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  
  ws2812fx.init();
  ws2812fx.setBrightness(255);

  // setup the custom effects
  uint8_t dualLarsonMode  = ws2812fx.setCustomMode(F("Dual Larson"), dualLarson);

  uint32_t colors[] = {RED, BLUE, WHITE};

  ws2812fx.setSegment(0, 0, LED_COUNT - 1, dualLarsonMode, colors, 2000, FADE_SLOW);

  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
}
