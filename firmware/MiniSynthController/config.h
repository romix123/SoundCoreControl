#define DEBUG 0

#if BEGUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

#define E       22 // Enable pin of the 4051
#define adcPin  A0 // Pot 4051
#define swPin   A6  // switch 4051 
 int Mux_Pin[3] = {A1,A2,A3};  // pins 4, 5, 6 


#define POT0 0 // nothing yet

#define POT1 73 // Controls the “Attack’ of a sound. The attack is the amount of time it takes for the sound to reach maximum amplitude. 
                // EGVCFAttack=73 
#define POT2 80 // Decay Generic or on/off switch 
                // EGVCFDecay=80
#define POT3 64 // On/off switch that controls sustain pedal. Nearly every synth will react to CC 64. (See also Sostenuto CC 66)
                // EGVCFSustain=64
#define POT4 69 // Another way to “hold notes” (see MIDI CC 64 and MIDI CC 66). However notes fade out according to their release parameter rather than when the pedal is released 
                // EGVCFRelease=69
#define POT5 94 //  VCODetune=94 
#define POT6  71  // Allows shaping the Voltage Controlled Filter (VCF). Default = Resonance also (Timbre or Harmonics)
#define POT7  74  // Controls VCFs cutoff frequency of the filter.
#define POT8  7   // mastervolume   


 String progName[] = {
  "Digi-piano",
  "Chiptune 80",
  "Poly-brass",
  "Pipe-organ",
  "String-machine",
  "Space-pad"
};




 
