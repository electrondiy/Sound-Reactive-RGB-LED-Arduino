#include <Adafruit_NeoPixel.h>

#define RING_PIN 6
#define RING_COUNT 12

#define STRIP_PIN 7
#define STRIP_COUNT 8

#define MIC_PIN A0

Adafruit_NeoPixel ring(RING_COUNT, RING_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(STRIP_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800);

int noiseSmooth = 0;
int rainbowOffset = 0;
int stripOffset = 0;

void setup() {
  ring.begin();
  strip.begin();

  ring.show();
  strip.show();

  ring.setBrightness(150);
  strip.setBrightness(150);
}

void loop() {
  // Read microphone
  int micValue = analogRead(MIC_PIN);
  noiseSmooth = (noiseSmooth * 0.8) + (micValue * 0.2);

  // Brightness for ring based on sound
  int brightness = map(noiseSmooth, 200, 700, 10, 255);
  brightness = constrain(brightness, 5, 255);
  ring.setBrightness(brightness);

  // ------------------------------------
  //  Rainbow effect for 12-LED ring
  // ------------------------------------
  for (int i = 0; i < RING_COUNT; i++) {
    int colorIndex = (i * 256 / RING_COUNT + rainbowOffset) & 255;
    ring.setPixelColor(i, WheelRing(colorIndex));
  }
  ring.show();
  rainbowOffset += 3;

  // ------------------------------------
  //  Wave effect for 8-LED strip (Rainbow)
  // ------------------------------------
  int waveLen = map(noiseSmooth, 200, 700, 1, STRIP_COUNT);
  waveLen = constrain(waveLen, 1, STRIP_COUNT);

  strip.clear();

  for (int i = 0; i < waveLen; i++) {

    int colorIndex = (i * 256 / STRIP_COUNT + stripOffset) & 255;
    uint32_t color = WheelStrip(colorIndex);

    // Fade tail of wave
    int fade = map(i, 0, waveLen - 1, 255, 50);

    uint8_t r = ((color >> 16) & 0xFF) * fade / 255;
    uint8_t g = ((color >> 8) & 0xFF) * fade / 255;
    uint8_t b = (color & 0xFF) * fade / 255;

    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  strip.show();
  stripOffset += 4;

  delay(20);
}

// ------------------------------------
//  Rainbow generator for ring
// ------------------------------------
uint32_t WheelRing(byte WheelPos) {
  if (WheelPos < 85) {
    return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// ------------------------------------
//  Rainbow generator for strip
// ------------------------------------
uint32_t WheelStrip(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
