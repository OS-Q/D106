#include <WS2812FX.h>

#define LED_COUNT 24
#define LED_PIN   5

// include and config the VUMeter custom effect
#define NUM_BANDS 7
#define USE_RANDOM_DATA false
#include "custom/VUMeter.h"

// MSGEQ7 pin assignments

#define STROBE 12
#define RESET  13
#define OUT    14
#define MSGEQ7_DELAY 50

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  // initialize the MSGEQ7
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(RESET, LOW);
  digitalWrite(STROBE, HIGH);
  delay(1);

  // reset the MSGEQ7's output multiplexer
  digitalWrite(RESET, HIGH);
  delayMicroseconds(50);
  digitalWrite(RESET, LOW);
  delay(1);

  ws2812fx.init();
  ws2812fx.setBrightness(255);

  // setup the custom effect
  uint32_t colors[] = {GREEN, YELLOW, RED};
  uint8_t vuMeterMode = ws2812fx.setCustomMode(F("VU Meter"), vuMeter);
  ws2812fx.setSegment(0, 0, LED_COUNT-1, vuMeterMode, colors, MSGEQ7_DELAY, NO_OPTIONS);

  ws2812fx.start();
}

void loop() {
  static long then = 0;
  long now = millis();

  // read the MSGEQ7 and update the vuMeter custom effect's values
  // every MSGEQ7_DELAY ms
  if(now > then + MSGEQ7_DELAY) {
    for (uint8_t i=0; i < NUM_BANDS; i++) {
      digitalWrite(STROBE, LOW);
      delayMicroseconds(50);
      uint16_t value = analogRead(OUT); // read the MSGEQ7 value
      vuMeterBands[i] = value < 40 ? 0 : map(value, 40, 1023, 0, 255); // scale the value
      digitalWrite(STROBE, HIGH);
      delayMicroseconds(50);
    }
    then = now;
  }

  ws2812fx.service();
}
