#include <RGB.h>

// Which pin on the Arduino is connected to the NeoPixels?
int pin         =  5; // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
int numPixels   = 12; // Popular NeoPixel ring size

// NeoPixel color format & data rate. See the strandtest example for
// information on possible values.
int pixelFormat = NEO_GRB + NEO_KHZ800;

// Rather than declaring the whole NeoPixel object here, we just create
// a pointer for one, which we'll then allocate later...
RGB *pixels;

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {  
  // Right about here is where we could read 'pin', 'numPixels' and/or
  // 'pixelFormat' from EEPROM or a file on SD or whatever. This is a simple
  // example and doesn't do that -- those variables are just set to fixed
  // values at the top of this code -- but this is where it would happen.

  // Then create a new NeoPixel object dynamically with these values:
  pixels = new RGB(numPixels, pin, pixelFormat);

  // Going forward from here, code works almost identically to any other
  // NeoPixel example, but instead of the dot operator on function calls
  // (e.g. pixels.begin()), we instead use pointer indirection (->) like so:
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // You'll see more of this in the loop() function below.
}

void loop() {
  pixels->clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<numPixels; i++) { // For each pixel...

    // pixels->Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels->setPixelColor(i, pixels->Color(0, 150, 0));

    pixels->show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}
