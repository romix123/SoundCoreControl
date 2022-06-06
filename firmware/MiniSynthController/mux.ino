/*
 * This file contains the code for the ADC, aka the 8 pots
 */

//#include "avdweb_AnalogReadFast.h"


static const uint8_t Mux_Address[8][3] = {
    {0,0,0}, //channel 0
    {1,0,0}, //channel 1
    {0,1,0}, //channel 2
    {1,1,0}, //channel 3
    {0,0,1}, //channel 4
    {1,0,1}, //channel 5
    {0,1,1}, //channel 6
    {1,1,1}, //channel 7
 }; 

static const uint8_t Program[]{
  0, 1, 2, 3, 
  0, 4, 5, 6, 0
};


void MuxSetup(){
  pinMode(E, OUTPUT);
  digitalWrite(E, LOW);
  analogReadResolution(10);
  uint8_t i;
  for (i = 0; i < 3; i++){
    pinMode(Mux_Pin[i], OUTPUT); 
    digitalWrite(Mux_Pin[i],LOW);
  }
  //pinMode(swPin, INPUT);
}



byte MuxRead(){
int readings = 3;
   int val = 0; 
    for(int i=0; i<readings; i++)  {
    val += analogRead(adcPin);
  }
  return map(val/readings, 0, 1023, 0, 127);
}

void MuxArm(int ch){
  for (int i = 0; i < 3; i ++){
      digitalWrite(Mux_Pin[i], Mux_Address[ch][i]);
  }
 // analogRead(adcPin);
}


 void populatePotVals(){
  for (int i = 0 ; i < 8; i++) {
    MuxArm(i);
    PotVal[i] = MuxRead();
    HandlePot(i);
    BarUpdate(i);
  }
 }

 void HandlePot(int p){
   byte currentPotVal = MuxRead();
    if ( (currentPotVal > PotVal[p] + 3) || (currentPotVal < PotVal[p] - 3)){
       PotVal[p] = currentPotVal;
      displayLine2(String(currentPotVal));

       switch (p) {
        case 0:       //wave shape
          controlChange(0, POT1, currentPotVal);
          break;
        case 1:
          controlChange(0, POT2, currentPotVal);
          break;
        case 2: 
          controlChange(0, POT3, currentPotVal);
          //setDisplayAttack(currentPotVal); 
           break;
        case 3:
          controlChange(0, POT4, currentPotVal);
          //setDisplayDecay(currentPotVal); 
            break;
        case 4:
          controlChange(0, POT5, currentPotVal);
           //setDisplaySustain(currentPotVal); 
           break;
        case 5:
          controlChange(0, POT6, currentPotVal);
           // setDisplayRelease(currentPotVal); 
          break;
        case 6:
          controlChange(0, POT7, currentPotVal);

            break;
        case 7:
          controlChange(0, POT8, currentPotVal);
            break;
        default:
            break;            
        }
        BarUpdate(p);
    }      
}


boolean buttonDebounce(int p) {
  // read the state of the switch into a local variable:

  int real = analogRead(swPin);

  // check to see if you just pressed the button
  // The Mux output is connected to 3v3 through a 330 ohm resistor, hence pulled high. 
  // the gummy buttons have an on resistance of a bout 80 ohms. Pushing the button drops the rest signal from 1023 to around 800.


  // If the switch changed, due to noise or pressing:
  if (real  < 1000) {
    // reset the debouncing timer
   // lastDebounceTime = millis();
    delay(40);
    if (analogRead(swPin) < 1000) {
        return true;        
      }
    }
  return false;
 }
 
void HandleSwitch(int p){
   int real= analogRead(swPin);
   int prog ;
  // displayLine2(String(real));

  if (buttonDebounce(p)){
    if ((p != 0) && (p != 4)) {
    for (int i = 1; i < 4; i++){
      SwitchVal[i] = 0;
      SwitchVal[i+4] = 0;
    }    
      SwitchVal[p] = 1;
      prog = Program[p];
        programChange(0, prog - 1); 
        displayProgram(String(prog));
        displayLine2(progName[prog - 1]);
        removeTime=0;
    }
  }
    removeTime++;
    if (removeTime == 400) displayLine2("                 ");

}
