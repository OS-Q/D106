#include <WS2812FX.h>
#include "ESP32_RMT_Driver.h"

// The ESP32's RMT hardware supports up to 8 channels, so it
// can drive up to 8 independent WS2812FX instances. We'll use 2.
WS2812FX ws2812fx1 = WS2812FX(24, 5, NEO_GRB  + NEO_KHZ800); // 144 RGB LEDs driven by GPIO_12
WS2812FX ws2812fx2 = WS2812FX(60, 13, NEO_GRBW + NEO_KHZ800); //  60 RGBW LEDs driven by GPIO_13

void setup() {
  Serial.begin(115200);
  delay(500);

  ws2812fx1.init(); // initialize the ws2812fx instances
  ws2812fx2.init();

  ws2812fx1.setBrightness(64); // set the overall LED brightnesses
  ws2812fx2.setBrightness(32);

  rmt_tx_int(RMT_CHANNEL_0, ws2812fx1.getPin()); // assign ws2812fx1 to RMT channel 0
  rmt_tx_int(RMT_CHANNEL_1, ws2812fx2.getPin()); // assign ws2812fx2 to RMT channel 1

  ws2812fx1.setCustomShow(myCustomShow1); // set the custom show function to forgo the NeoPixel
  ws2812fx2.setCustomShow(myCustomShow2); // bit-bang method and instead use the RMT hardware

  // parameters: seg_index, start, stop, mode, color, speed, options
  ws2812fx1.setSegment(0, 0, 144-1, FX_MODE_COMET, GREEN, 1000, NO_OPTIONS); // setup each ws2812fx's effect
  ws2812fx2.setSegment(0, 0,  60-1, FX_MODE_COMET, BLUE,  1000, NO_OPTIONS);

  ws2812fx1.start(); // start'em up
  ws2812fx2.start();
}

void loop() {
  ws2812fx1.service(); // service each ws2812fx instance
  ws2812fx2.service();
}

// Custom show functions which will use the RMT hardware to drive the LEDs.
// Need a separate function for each ws2812fx instance.
void myCustomShow1(void) {
  uint8_t *pixels = ws2812fx1.getPixels();
  // numBytes is one more then the size of the ws2812fx's *pixels array.
  // the extra byte is used by the driver to insert the LED reset pulse at the end.
  uint16_t numBytes = ws2812fx1.getNumBytes() + 1;
  rmt_write_sample(RMT_CHANNEL_0, pixels, numBytes, false); // channel 0
}

void myCustomShow2(void) {
  uint8_t *pixels = ws2812fx2.getPixels();
  // numBytes is one more then the size of the *pixels array.
  // the extra byte is used by the driver to insert the LED reset pulse at the end.
  uint16_t numBytes = ws2812fx2.getNumBytes() + 1;
  rmt_write_sample(RMT_CHANNEL_1, pixels, numBytes, false); // channel 1
}
