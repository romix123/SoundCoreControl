/*
 * DISPLAY functions
*/

void displayLine2(String msg){
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
  display.setCursor(0,12);             // Start at top-left corner
  display.println(msg);
  display.display();
}

void displayLine3(String msg){
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
  display.setCursor(0,22);             // Start at top-left corner
  display.println(msg);
  display.display();
}

void displayStatus(String msg){
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
  display.setCursor(0,22);             // Start at top-left corner
 // display.print(msg);
  display.display();
}

void displayProgram(String prgm){
  display.setTextSize(3);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
  display.setCursor(103,0);             // Start at top-left corner
  display.print(prgm);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(96,25);             // Start at top-left corner
  display.print("voice");

  display.display();
  
}


// Draw the ADSR graph on screen
void drawADSR() {
  uint8_t dispAttack = map(PotVal[2], 0, MIDICONTROLMAXVAL, 0, 32);
  uint8_t dispDecay = map(PotVal[3], 0, MIDICONTROLMAXVAL, 0, 32);
  uint8_t dispSustain = map(PotVal[4], 0, MIDICONTROLMAXVAL, 0, 22);
  uint8_t dispRelease = map(PotVal[5], 0, MIDICONTROLMAXVAL, 0, 20);;
  // start by clearing the display space for the adsr
  display.fillRect(DISP_ADSR_WIDTH_OFFSET, 0, 88, 32, BLACK);
  // draw Attack line
  display.drawLine(0 + DISP_ADSR_WIDTH_OFFSET, SCREEN_HEIGHT, 
                      dispAttack + DISP_ADSR_WIDTH_OFFSET, DISP_ADSR_HEIGHT_OFFSET, WHITE);  
  // draw Decay line
  display.drawLine(DISP_ADSR_WIDTH_OFFSET + dispAttack, DISP_ADSR_HEIGHT_OFFSET, 
                      dispAttack + dispDecay + DISP_ADSR_WIDTH_OFFSET, DISP_ADSR_HEIGHT_OFFSET + (21 - dispSustain), WHITE);
  // draw Sustain line
  display.drawLine(DISP_ADSR_WIDTH_OFFSET + dispAttack + dispDecay, DISP_ADSR_HEIGHT_OFFSET + (21 - dispSustain), 
                      DISP_ADSR_WIDTH_OFFSET + dispAttack + dispDecay + dispRelease, DISP_ADSR_HEIGHT_OFFSET + (21 - dispSustain), WHITE);  
  // draw Release line
  display.drawLine(DISP_ADSR_WIDTH_OFFSET + dispAttack + dispDecay + dispRelease, DISP_ADSR_HEIGHT_OFFSET + (21 - dispSustain), 
                      SCREEN_WIDTH, SCREEN_HEIGHT , WHITE);
  display.display(); 
}
