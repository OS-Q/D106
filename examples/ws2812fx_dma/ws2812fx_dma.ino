#include <WS2812FX.h>
#include <NeoPixelBus.h>

#define LED_PIN    5  // digital pin used to drive the LED strip, (for ESP8266 DMA, must use GPIO3/RX/D9)
#define LED_COUNT 24  // number of LEDs on the strip

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// create a DMA instance
// first parameter is number of LEDs, second paramter is number of bytes per LED (3 for RGB, 4 for RGBW)
NeoEsp8266Dma800KbpsMethod dma = NeoEsp8266Dma800KbpsMethod(LED_COUNT, 3);

void setup() {
  Serial.begin(115200);

  ws2812fx.init();

  // MUST init dma after ws2812fx.init(), so GPIO3 is initalized properly
  dma.Initialize();

  // set the custom show function
  ws2812fx.setCustomShow(myCustomShow);

  ws2812fx.setBrightness(255);
  const uint32_t colors[] = {RED, BLACK, BLACK};
  ws2812fx.setSegment(0, 0, LED_COUNT-1, FX_MODE_COMET, colors, 2000, NO_OPTIONS);

  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
}

void myCustomShow(void) {
  if(dma.IsReadyToUpdate()) {
    // copy the ws2812fx pixel data to the dma pixel data
    memcpy(dma.getPixels(), ws2812fx.getPixels(), dma.getPixelsSize());
    dma.Update();
  }
}
