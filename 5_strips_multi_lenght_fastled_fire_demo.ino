#include <FastLED.h>

#define LED_PIN_0     12 //288
#define LED_PIN_1     14 //30
#define LED_PIN_2     13 //30
#define LED_PIN_3     4 //30
#define LED_PIN_4     5 //30
#define CLOCK_PIN   15
#define COLOR_ORDER BGR
#define CHIPSET     APA102
#define NUM_LEDS_0    288
#define NUM_LEDS_1    288
#define NUM_LEDS_2    288
#define NUM_LEDS_3    30
#define NUM_LEDS_4    30
#define NUM_STRIPS 5 // Total number of strips

#define BRIGHTNESS  50
#define FRAMES_PER_SECOND 67

bool gReverseDirection = false;

CRGB leds[NUM_LEDS_0 + NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4];  // Add all together here


uint8_t stripLength; // Used to store a strip length.

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN_0, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_0).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_1, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_1).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_2, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_2).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_3, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_3).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_4, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_4).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );
  
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
random16_add_entropy( random(256));

  Fire2012(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}



#define COOLING  55 // 20/100

#define SPARKING 15 // 50/200


void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS_0];
  

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS_0; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_0) + 2));
    }

// Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS_1; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_1) + 2));
    }
    
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS_0 - 1; k >= 3; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS_1 - 1; k >= 3; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(9);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

   
    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS_0; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS_0-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }

  // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS_1; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS_1-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }


    
}
