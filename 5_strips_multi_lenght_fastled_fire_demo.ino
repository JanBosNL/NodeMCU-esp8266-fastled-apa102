//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER // Only use when using nodeMCU pin numbers like D4,D5,D12,D15 etc...

#include <FastLED.h> // I am using the real GPIO numbers, not the D# nodeMCU numbers. If you want to use the NODEMCU specific numbers you need to define (#define FASTLED_ESP8266_NODEMCU_PIN_ORDER)
                     // Before including (#include <FastLED.h>)

#define LED_PIN_0     12 //288 Here I define the pins per ledstrip 
#define LED_PIN_1     13 //288 Not all pins are unused so be careful
#define LED_PIN_2     14 //288 to choose the ones that are availeable on the NODEMCU
#define LED_PIN_3     4  //30    
#define LED_PIN_4     5  //30    
#define CLOCK_PIN     15 //     Here I define the single clockpin So dont use a clockpin per ledstrip. Al the clock wires from all the strips use this single clockpin.
#define COLOR_ORDER BGR  //     Defining the colour order of the 3 colours the APA102 led has. Other choices can be BRG, RGB, RBG, GRB, GBR 
#define CHIPSET     APA102 // Defining the actual LED chip used that Fastled can recognise. 
#define NUM_LEDS_0    288 // Define nr of leds on the strip connected to pin 12 
#define NUM_LEDS_1    288 // Define nr of leds on the strip connected to pin 13
#define NUM_LEDS_2    288 // Define nr of leds on the strip connected to pin 14
#define NUM_LEDS_3    30  // Define nr of leds on the strip connected to pin 4
#define NUM_LEDS_4    30  // Define nr of leds on the strip connected to pin 5
#define NUM_STRIPS 5 // Total number of strips

#define MILLI_AMPS         30000 // IMPORTANT!!: set the max milli-Amps of your powersupply (30A = 30000mA) as my powersupply is rated up to 30A.
                                 // Fastled calculates power consumption by using known consumption specs per led and chipset, to lower brightness when 
                                 // you try and draw more power then your powersupply can handle. This to not damage your electronics.

#define BRIGHTNESS  180       // Overall Brightnes from 0 up to 255
#define FRAMES_PER_SECOND 120   // Speed of the frames. Higher number is more speed.
#define COOLING  40 // 20/100
#define SPARKING 25 // 50/200

bool gReverseDirection = false;

CRGB leds[NUM_LEDS_0 + NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4];  // Add all the leds together here

// Create my custom, 8 color gradient, Fire palette!
DEFINE_GRADIENT_PALETTE( myfire_gp ) {
  0,    0,0,0,            //BLACK
 36,    36,0,0,          //MAROON
 72,    51,26,0,         //Brown
108,    140,39,0,         //ORANGERED
144,    255,165,0,        //ORANGE
180,    255,215,30,        //AMBER
216,    255,255,0,        //YELLOW
255,    255,255,255 };    //WHITE
CRGBPalette16 myPal = myfire_gp;    

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN_0, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_0).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_1, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_1).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_2, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_2).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_3, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_3).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_4, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS_4).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );

}

//Below is code I used from FASTLED EXAMPLES
// Fire2012 with programmable Color Palette

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
random16_add_entropy( random(256));

  Fire2012WithPalette(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
void Fire2012WithPalette()
{ 
 // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS_0];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS_0; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_0) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS_0 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS_0; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( myPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS_0-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
      addGlitter(1);
    }
}

//Sparks at the top flying of in the sky
void addGlitter( fract8 chanceOfGlitter) 
{
if( random8() < chanceOfGlitter) {
leds[ random16(NUM_LEDS_0) ] += CRGB (140,39,0); //Orangered 
  }


}

