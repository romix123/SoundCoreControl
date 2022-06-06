/*
 * Neopixel functions, taken from the Adafruit Neopixel library
 * 
 * 
 * Midi notes range from A0 - 21 to C8 - 108 (on an 88 key piano keyboard)
 *  to map those on the 255 range, we can do the following
 *  
 *  smaller keyboard range from C2 - 36 to B5 - 83
 *  Wheelpos = note * 3 - 21. This covers almost the entire range, In practice  
 * 
 */


 void initNeo(){
  pxl.begin();
  pxl.setBrightness(20);
  pxl.clear();
    pxl.setPixelColor(0, pxl.Color(10, 10, 10));
  pxl.show();
 }



 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte Note) {
  byte WheelPos = 0;
  WheelPos = 255 - (Note * 3 - 21);
  if (WheelPos < 0) WheelPos = 0;
  if(WheelPos < 85) {
    return pxl.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pxl.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pxl.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void BarUpdate(int led){
 // for(int i=1; i< 9; i++) {
    pxl.setPixelColor(led + 1, Wheel(PotVal[led] * 2));
 //  }
    pxl.show();
    updated = 0;
}


void Blink(void)
{
    if (ledOn)
    {
            pxl.setPixelColor(0, pxl.Color(0, 127, 127));
            pxl.show();   
            tm=millis();
    }
    else
    {
        pxl.setPixelColor(0, pxl.Color(0, 0, 0));
            pxl.show();  
         tm=millis();
    }
    ledOn = !ledOn;
}
