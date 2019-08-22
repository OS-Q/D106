
#include <WS2812FX.h>

#define LED_PIN    5  // digital pin used to drive the LED strip
#define LED_COUNT 24  // number of LEDs on the strip

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  ws2812fx.init();
  ws2812fx.setBrightness(128);

  // parameters: index, start, stop, mode, color, speed, reverse
  ws2812fx.setSegment(0,  0,  9, FX_MODE_BLINK, 0xFF0000, 1000, false); // segment 0 is leds 0 - 9
  ws2812fx.setSegment(1, 10, 19, FX_MODE_SCAN,  0x00FF00, 1000, false); // segment 1 is leds 10 - 19
  ws2812fx.setSegment(2, 20, 29, FX_MODE_COMET, 0x0000FF, 1000, true);  // segment 2 is leds 20 - 29

  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
}
