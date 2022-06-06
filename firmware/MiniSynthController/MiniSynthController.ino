/**
 * MiniSynthConbtroller
 * using SoundCoreKnobs
 * 
 *  by Ronald Leenes, romix@macuser.nl
 *  version 0.1, 29 May 2022
 *  
 * 
 *  It handles USB midi in, reads 8 pots and can display messages on a 128x32 oled screen. Each pot has a neopixel, finally there is a NeoPixel that 
 *  can be used as a status display or whatever.
 *  
 *  The SAMD21 code requires slightly modified Adafruit SAMD QT PY configuration files.
 *    In your Arduino15/packages/adafruit/hardware/samd/1.7.3/variants/qtpy_m0 change the following files:
 *      variant.cpp
 *        after the definition of the USB Pins add these two lines:
           { PORTA, 14, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // D22 Neopix
           { PORTA, 27, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // E
 * 
 *      in variant.h
 *        change the pin def to
 *        // Number of pins defined in PinDescription array
            #define PINS_COUNT           (23u)
            #define NUM_DIGITAL_PINS     (23u)
         in // LEDS 
          add
            #define PIN_NEOIXEL2     (22u)

          The modded files are in this github repository

          To put the SAMD21 in programming mode, connect to the board using a usb cable in the keyboard/host port and press the reset button above the 
          Neopixel twice. The NeoPixel will light up green if it is in programming mode.(it turns red if something is not ok).
          The board registers as cu.usbmodem14401 on my machine.

          In Music operation, connect an OTG cable to the keyboard port, plug in a MIDI keyboard in this cable and connect usb power to the power in port.
          
 *        
 * TODO:
 *        allow for a flexible pot to midi CC code mapping
 *        clean up code
 *        fix MIDI keyboard recognition

 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "MIDIUSB.h"


/*
 *     inline void sendProgramChange(DataByte inProgramNumber,
                                  Channel inChannel);

    inline void sendControlChange(DataByte inControlNumber,
                                  DataByte inControlValue,
                                  Channel inChannel);
 */


#include "config.h"


 
   // setup Adafruit Neopixel and Oled display lib on QT Py board M0
#include <Adafruit_NeoPixel.h>
#define NEOPIXEL      11

Adafruit_NeoPixel  pxl(9, NEOPIXEL, NEO_GRB + NEO_KHZ800);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include "splash.h"


 
 uint32_t PollPot = 0;

const unsigned int NeoPeriod = 200;



#if DEBUG == 1
#define debug(x) Serial1.print(x)
#define debugln(x) Serial1.println(x)
#define debugh(x,y) Serial1.println(x,y)
#else
#define debug(x)
#define debugln(x)
#define debugh(x,y)
#endif
/*
 * DEFINITIONS AND VARIABLES
 */

// Button debounce
int buttonState = 1;             // the current reading from the input pin
int lastButtonState = 1;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// button debounce


  int roundrobbin = 0;
    static int curChan = 0;  //which pot is in focus?
    
 int tm;
 int pot = 0;
 int currentPotVal;
 int pixelOff = 0;
 boolean updated = 0;   //do we have an updated potval?
int pauze = 50;
 bool ledOn = true;
 bool MIDIon = false;
 int removeTime;    // timer for programname removal

// display
//uint8_t dispAttack, dispDecay, dispSustain, dispRelease =0;
#define MIDICONTROLMAXVAL 127
#define DISP_HEIGHT 32
#define DISP_WIDTH  88 // 128- 40
#define DISP_ADSR_HEIGHT_OFFSET 10     // height offset for ADSR graph, to account for menu name
#define DISP_ADSR_WIDTH_OFFSET  40    // width offset for ADSR graph to account for left hand status

//------------- MIDI ---------------


 /*
  * Mux stuff for the pots
  */
 

 uint8_t PotVal[8] =    {0,0,0,0,0,0,0,0};
uint8_t SwitchVal[8] =  {0,0,0,0,0,0,0,0};




// Things to do at bootup
void setup() {
   
   delay(200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
 delay(pauze);
 
 display.clearDisplay();
display.drawBitmap(0, 0, splash1_data, splash1_width, splash1_height, 1);
display.display();
 delay(2000);
display.clearDisplay();
display.display();
  displayLine2("SoundCoreControl v1");
  
      delay(2000);
      display.clearDisplay();

      displayLine2("Init: Neo");
  initNeo();

      delay(pauze);
      displayLine2("Init: Mux");
   MuxSetup();

      delay(pauze);
      displayLine2("Init: Pots");
  populatePotVals();

      delay(pauze);
      displayLine2("Init: MIDI");
    //  usb_midi.setStringDescriptor("SoundCore Control");
   // Initialize MIDI, and listen to all MIDI channels
  // This will also call usb_midi's begin()
      noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
       delay(500);
      noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
        
  delay(2000);
      delay(pauze);
      displayLine2("Here we go.");
      display.display();
      delay(pauze);
   
  display.clearDisplay();
  displayProgram("0");
  //drawADSR();
  
}


void loop() 
{

         MuxArm(curChan);
         HandlePot(curChan);
         HandleSwitch(curChan);
         curChan++;
         if (curChan == 8) curChan = 0;
                
}
