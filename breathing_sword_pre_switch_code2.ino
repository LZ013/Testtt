#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 
#define SLOW_SWEEP_SPEED            100
#define USER_COLOR      0x008000 //default color, red
#define ENABLE_COLOR_KNOB   1  
#define POT_PIN             A0  

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

uint32_t colorselected = USER_COLOR;
const int buttonPin = 4; // I chose pin 4 for the switch
int buttonState = 0; // I copied this as well as it's required for the switch to work
int lastButtonState = 0; //test

  void setup() {
  
     // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
      if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code
  
    pinMode(buttonPin, INPUT_PULLUP);
    
    
    //Get user selected color color on startup
    update_color_and_delay(0);
    strip.begin();
    strip.setBrightness(300);  
    strip.show(); // Initialize all pixels to 'off'

  }

  void loop() {
  
  //Loop definitions
  int TOTAL_LEDS = 32;
  float MaximumBrightness = 200;
  float SpeedFactor = 0.007; // I don't actually know what would look good
  float StepDelay = 4; // ms for a step delay on the lights
  
  //slow sweep
  colorWipe(SLOW_SWEEP_SPEED);
  
  //Read how the switch is set 
  buttonState = digitalRead(buttonPin);  

  // Make the lights breathe
  if (buttonState == HIGH) { // If switch is right
  
    for (int i = 0; i < 65535; i++) {
       
    // Intensity will go from 10 - MaximumBrightness in a "breathing" manner
    float intensity = MaximumBrightness /2.0 * (1.0 + sin(SpeedFactor * i));
    strip.setBrightness(intensity);
    
    // Set every LED to that color
    for (int ledNumber=0; ledNumber<TOTAL_LEDS; ledNumber++) {
    strip.setPixelColor(ledNumber, 0, 255, 0);

    }

    strip.show();
    //Wait a bit before continuing to breathe
    delay(StepDelay);
    }

  } else { // If switch is left  
      strip.setBrightness(250);
  }
    } 


// Fill the dots one after the other with a color - startup colour
  void colorWipe(uint8_t wait) {
    
    for(uint16_t i = 0; i < strip.numPixels(); i++)
    {
      
      strip.setPixelColor(i, colorselected);
      strip.setBrightness(150);
      strip.show();
      update_color_and_delay(wait);
      
    }
  }

//Update strip colour
  void update_color_and_delay(uint16_t wait) {
  
  int analog = 0;
  uint32_t colorwheel[] =
  {

    0x008000, //13 green
  };

  if(ENABLE_COLOR_KNOB)
  {
    //read potentiometer and use a map lookup to update global color
    analog = analogRead(POT_PIN);
    colorselected = colorwheel[map(analog, 0, 1024, 0, ( sizeof( colorwheel ) / sizeof( uint32_t ) ) )];
  }

  delay(60);
}
