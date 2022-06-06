/*
 * Wired
 * Contains all wired/i2C related functions
 * Please note that this is all designed for the SoundCore peripheral boards
*/

#include <Wire.h>

void initWired(){
    Wire.begin();        // join i2c bus (address optional for master)
}

void pollTheWire(){
  
}

void readTheWire(uint8_t device, uint8_t reg){
  Wire.requestFrom(2, 6);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
}
