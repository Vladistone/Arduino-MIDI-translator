// By Steve Baines 2016; renovated by Vladistone, 2023
// idea see: https://hackaday.io/
// forum disscusion: https://gearspace.com/board/electronic-music-instruments-and-electronic-music-production/1418944-korg-dss-1-arduino-upgrade-project.html
// Arduino chipping to Korg DW/EX-8000 MIDI translator

// modules of preparation steps:
// Sysex_6 byte leight..................................................................[Unit_0] - here is absent
// Sysex_8 byte leight..................................................................[Unit_1] - Main Scaling
// Sysex_9 byte leight..................................................................[Unit_2] - here is absent
// Sysex_10 byte leight.................................................................[Unit_3] - here is absent
// Main CC# Mapping.....................................................................[Unit_4] - Main Mapping
// Additional processing................................................................[Unit_5]

#include <MIDI.h>

// Create MIDI interface on main hardware serial port
// (Tx/Rx pins on Arduino board)
MIDI_CREATE_DEFAULT_INSTANCE();
/*This function will be automatically called when a NoteOn is received.
  It must be a void-returning function with the correct parameters,
  see documentation here:
  http://arduinomidilib.fortyseveneffects.com/a00022.html
  https://github.com/FortySevenEffects/arduino_midi_library/wiki/Using-Callbacks */

const int ledPinOut = 13;
const int dwChannel = 0; // 0 here is 1 on DW-8000 - need to set Param 83 to 1

// Also need to set Param 84 to 2, otherwise SysEx messages are ignored.
// DW/EX-8000 Modes request by use CC#2Sysex converter:

  #if 0
    case 0: // OSC 1 Octave (2b)
    case 1: // OSC 1 Waveform (4b)
    case 2: // OSC 1 Level (5b)
    case 3: // A. Bend Select (2b)
    case 4: // A. Bend Mode (1b)
    case 5: // A. Bend Time (5b)
    case 6: // A. Bend Intensity (5b)
    case 7: // OSC 2 Octave (2b)
    case 8: // OSC 2 Waveform (4b)
    case 9: // OSC 2 Level (5b)
    case 10: // Interval (3b)
    case 11: // Detune (3b)
    case 12: // Noise Level (5b)
    case 13: // Assign Mode (2b)
    case 14: // Parameter No. Memory (6b)
    case 15: // Cutoff (6b)
    case 16: // Resonance (5b)
    case 17: // Keyboard Tracking (2b)
    case 18: // Polarity (1b)
    case 19: // EG. Intensity (5b)
    case 20: // VCF Attack (5b)
    case 21: // VCF Decay (5b)
    case 22: // VCF Breakpoint (5b)
    case 23: // VCF Slope (5b)
    case 24: // VCF Sustain (5b)
    case 25: // VCF Release (5b)
    case 26: // VCF Velocity Sensitivity (3b)
    case 27: // VCA Attack (5b)
    case 28: // VCA Decay (5b)
    case 29: // VCA Breakpoint (5b)
    case 30: // VCA Slope (5b)
    case 31: // VCA Sustain (5b)
    case 32: // VCA Release (5b)
    case 33: // VCA Velocity Sensitivity (3b)
    case 34: // MG Waveform (2b)
    case 35: // MG Frequency (5b)
    case 36: // MG Delay (5b)
    case 37: // MG OSC (5b)
    case 38: // MG VCF (5b)
    case 39: // Bend OSC (4b)
    case 40: // Joystk Bend VCF (1b)
    case 41: // Delay Time (3b)
    case 42: // Delay Factor (4b)
    case 43: // Delay Feedback (4b)
    case 44: // Delay Frequency (5b)
    case 45: // Delay Intensity (5b)
    case 46: // Delay Effect Level (4b)
    case 47: // Portamento (5b)
    case 48: // Aftertouch OSC MG (2b)
    case 49: // Aftertouch VCF (2b)
    case 50: // Aftertouch VCA (2b)
  #endif

// Main DW/EX-8000: Scaling SysEx_8 leight for paramNumber of [5]
void sendDw8000Param(byte channel, byte paramOffset, byte paramValue7Bit) // this is    Unit_1
{
  const int sysexLen = 8;
  static byte sysexData[sysexLen] = { 
    0xf0,   // 0 Sysex start
    0x42,   // 1 Manufacturer ID: 42, Korg
    0x30,   // 2 Channel 1
    0x03,   // 3 Device ID: 03, DW-8000 or EX-8000
    0x41,   // 4 Message: 41, Parameter change
    0x00,   // 5 Parameter number (which parameter we want to change)
    0x00,   // 6 Parameter value
    0xf7    // 7 EOX
  };
  // paramValue7Bit is assumed to run from 0 to 127 in all cases, 
  paramValue7Bit &= 0x7f;
  // so here we rescale to fit to appropriate bit width for each parameter
  byte paramValueScaled = 0;
  switch (paramOffset) {
    case 4: // [F15] A. Bend Mode (1b)
    case 18: // [F34] Polarity (1b)
    case 40: // [F67] Joystk Bend VCF (1b)
      paramValueScaled = paramValue7Bit >> 6; break;
    case 0: // [F11] OSC 1 Octave (2b)
    case 3: // [F14] A. Bend Select (2b)
    case 7: // [F21] OSC 2 Octave (2b)
    case 13: // [F -] Assign Mode (2b)
    case 17: // [F33] Keyboard Tracking (2b)
    case 34: // [F61] MG Waveform (2b)
    case 48: // [F81] Aftertouch OSC MG (2b)
    case 49: // [F82] Aftertouch VCF (2b)
    case 50: // [F83] Aftertouch VCA (2b)
      paramValueScaled = paramValue7Bit >> 5; break;
    case 10: // [F24] Interval (3b)
    case 11: // [F25] Detune (3b)
    case 26: // [F47] VCF Velocity Sensitivity (3b)
    case 33: // [F57] VCA Velocity Sensitivity (3b)
    case 41: // [F71] Delay Time (3b)
      paramValueScaled = paramValue7Bit >> 4; break;
    case 1: // [F12] OSC 1 Waveform (4b)
    case 8: // [F22] OSC 2 Waveform (4b)
    case 39: // [F66] Pitch Bend OSC (4b)
    case 42: // [F72] Delay Factor (4b)
    case 43: // [F73] Delay Feedback (4b)
    case 46: // [F76] Delay Effect Level (4b)
      paramValueScaled = paramValue7Bit >> 3; break;
    case 2: // [F13] OSC 1 Level (5b)
    case 5: // [F16] A. Bend Time (5b)
    case 6: // [F17] A. Bend Intensity (5b)
    case 9: // [F23] OSC 2 Level (5b)
    case 12: // [F26] Noise Level (5b)
    case 16: // [F32] Resonance (5b)
    case 19: // [F35] EG. Intensity (5b)
    case 20: // [F41] VCF Attack (5b)
    case 21: // [F42] VCF Decay (5b)
    case 22: // [F43] VCF Breakpoint (5b)
    case 23: // [F44] VCF Slope (5b)
    case 24: // [F45] VCF Sustain (5b)
    case 25: // [F46] VCF Release (5b)
    case 27: // [F51] VCA Attack (5b)
    case 28: // [F52] VCA Decay (5b)
    case 29: // [F53] VCA Breakpoint (5b)
    case 30: // [F54] VCA Slope (5b)
    case 31: // [F55] VCA Sustain (5b)
    case 32: // [F56] VCA Release (5b)
    case 35: // [F62] MG Frequency (5b)
    case 36: // [F63] MG Delay (5b)
    case 37: // [F64] MG OSC (5b)
    case 38: // [F65] MG VCF (5b)
    case 44: // [F74] Delay Frequency (5b)
    case 45: // [F75] Delay Intensity (5b)
    case 47: // [F77] Portamento (5b)
      paramValueScaled = paramValue7Bit >> 2; break;
    case 14: // [F--] Parameter No. Memory (6b)
    case 15: // [F31] Cutoff (6b)
      paramValueScaled = paramValue7Bit >> 1; break;
    default:
    return; // Unknown parameter - ignore
 }
  sysexData[2] = 0x30 | (channel & 0x0f); // Set channel number
  sysexData[5] = paramOffset;
  sysexData[6] = paramValueScaled;
  MIDI.sendSysEx(sysexLen, sysexData, true);
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{// Do whatever you want when a note is pressed.
    digitalWrite(ledPinOut, HIGH);   // turn the LED on
    MIDI.sendNoteOn(pitch, velocity, channel);
}
void handleNoteOff(byte channel, byte pitch, byte velocity)
{// Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    digitalWrite(ledPinOut, LOW);    // turn the LED off
    MIDI.sendNoteOff(pitch, velocity, channel);
}
void handleControlChange(byte channel, byte number, byte value)
{
  if (--channel != dwChannel) //INT dwChannel's value is always differ and less than the BYTE channel value by 1
  { // If CC not sent to DW-8000 channel, then just pass them on
    MIDI.sendControlChange(number, value, ++channel); // return the correction back
    return;
  }
    // Mapping received CC# to corresponding DSS-1 Parameter numbers                    Unit_4
    // NOTE: It's personal decision for each one for individual controller
  switch(number)  // refer http://nickfever.com/music/midi-cc-list
  {
    //  CC#                   ParamOffset                  [FunID]  Param.name    Notes
    //       SSL Nucleus Fader group L
    case 0: sendDw8000Param(channel, 12, value); break; // [F26] Noise Level (5b)
    case 1: sendDw8000Param(channel, 47, value); break; // [F77] Portamento time (5b)
    case 2: sendDw8000Param(channel, 2, value); break; // [F13] OSC 1 Level (5b)
    case 3: sendDw8000Param(channel, 9, value); break; // [F23] OSC 2 Level (5b)
    case 4: sendDw8000Param(channel, 33, value); break; // [F57] VCA Velocity Sensitivity (3b)
    case 5: sendDw8000Param(channel, 6, value); break; // [F17] A. Bend Intensity (5b)
    case 6: sendDw8000Param(channel, 44, value); break; // [F74] Delay Frequency (5b)
    case 7: sendDw8000Param(channel, 45, value); break; // [F75] Delay Intensity (5b)
    //       SSL Nucleus Fader group R    
    case 8: sendDw8000Param(channel, 35, value); break; // [F62] MG Frequency (5b)
    case 9: sendDw8000Param(channel, 19, value); break; // [F35] EG. Intensity (5b)
    case 10: sendDw8000Param(channel, 27, value); break; // [F51] VCA Attack (5b)
    case 11: sendDw8000Param(channel, 28, value); break; // [F52] VCA Decay (5b)
    case 12: sendDw8000Param(channel, 29, value); break; // [F53] VCA Breakpoint (5b)
    case 13: sendDw8000Param(channel, 30, value); break; // [F54] VCA Slope (5b)
    case 14: sendDw8000Param(channel, 31, value); break; // [F55] VCA Sustain (5b)
    case 15: sendDw8000Param(channel, 32, value); break; // [F56] VCA Release (5b)
    //       SSL Nucleus V-Coder group L
    case 16: sendDw8000Param(channel, 26, value); break; // [F47] VCF Velocity Sensitivity (3b)
    case 17: sendDw8000Param(channel, 10, value); break; // [F24] Interval (3b)
    case 18: sendDw8000Param(channel, 1, value); break; // [F12] OSC 1 Waveform (4b)
    case 19: sendDw8000Param(channel, 8, value); break; // [F22] OSC 2 Waveform (4b)
    case 20: sendDw8000Param(channel, 11, value); break; // [F25] Detune (3b)
    case 21: sendDw8000Param(channel, 5, value); break; // [F16] A. Bend Time (5b)
    case 22: sendDw8000Param(channel, 41, value); break; // [F71] Delay Time (3b)
    case 23: sendDw8000Param(channel, 43, value); break; // [F73] Delay Feedback (4b)
    //       SSL Nucleus V-Coder group R
    case 24: sendDw8000Param(channel, 15, value); break; // [F31] Cutoff (6b)
    case 25: sendDw8000Param(channel, 16, value); break; // [F32] Resonance (5b)
    case 26: sendDw8000Param(channel, 20, value); break; // [F41] VCF Attack (5b)
    case 27: sendDw8000Param(channel, 21, value); break; // [F42] VCF Decay (5b)
    case 28: sendDw8000Param(channel, 22, value); break; // [F43] VCF Breakpoint (5b)
    case 29: sendDw8000Param(channel, 23, value); break; // [F44] VCF Slope (5b)
    case 30: sendDw8000Param(channel, 24, value); break; // [F45] VCF Sustain (5b)
    case 31: sendDw8000Param(channel, 25, value); break; // [F46] VCF Release (5b)
    //       SSL Nucleus Select SW group L
    //case 64: sendDw8000Param(channel, , value); break; // 
    //case 65: sendDw8000Param(channel, , value); break; // 
    case 66: sendDw8000Param(channel, 48, value); break; // [F81] Aftertouch OSC MG (2b)
    //case 67: sendDw8000Param(channel, , value); break; // 
    case 68: sendDw8000Param(channel, 49, value); break; // [F82] Aftertouch VCF (2b)
    //case 69: sendDw8000Param(channel, , value); break; // 
    case 70: sendDw8000Param(channel, 50, value); break; // [F83] Aftertouch VCA (2b)
    //case 71: sendDw8000Param(channel, , value); break; // 
    //       SSL Nucleus Select SW group R    
    case 72: sendDw8000Param(channel, 17, value); break; // [F33] Keyboard Tracking (2b)
    case 73: sendDw8000Param(channel, 18, value); break; // [F34] Polarity (1b)
    case 74: sendDw8000Param(channel, 34, value); break; // [F61] MG Waveform (2b)
    //case 75: sendDw8000Param(channel, , value); break; // 
    //case 76: sendDw8000Param(channel, , value); break; // 
    //case 77: sendDw8000Param(channel, , value); break; // 
    //case 78: sendDw8000Param(channel, , value); break; // 
    case 79: sendDw8000Param(channel, 40, value); break; // [F67] Joystk Bend VCF (1b)
    //       SSL Nucleus V-Coder Select group L
    case 80: sendDw8000Param(channel, 0, value); break; // [F11] OSC 1 Octave (2b)
    //case 81: sendDw8000Param(channel, , value); break; // 
    case 82: sendDw8000Param(channel, 7, value); break; // [F21] OSC 2 Octave (2b)
    //case 83: sendDw8000Param(channel, , value); break; // 
    //case 84: sendDw8000Param(channel, , value); break; // 
    case 85: sendDw8000Param(channel, 3, value); break; // [F14] A. Bend Select (2b)
    //case 86: sendDw8000Param(channel, , value); break; // 
    case 87: sendDw8000Param(channel, 4, value); break; // [F15] A. Bend Mode (1b)
    //       SSL Nucleus V-Coder Select group R    
    //case 88: sendDw8000Param(channel, , value); break; // 
    //case 89: sendDw8000Param(channel, , value); break; // 
    //case 90: sendDw8000Param(channel, , value); break; // 
    //case 91: sendDw8000Param(channel, , value); break; // 
    //case 92: sendDw8000Param(channel, , value); break; // 
    //case 93: sendDw8000Param(channel, , value); break; // 
    //case 94: sendDw8000Param(channel, , value); break; // 
    //case 95: sendDw8000Param(channel, , value); break; // 

//  CC# list to matching the map of DW/EX-8000 Parameter for SSL Nucleus2 CC numbers (CC#SysEx for Nucleus2DW/EX8000)
#if 0
CC#    Param.#
  80 case 0   // [F11] OSC 1 Octave (2b)
  18 case 1   // [F12] OSC 1 Waveform (4b)
   2 case 2   // [F13] OSC 1 Level (5b)
  85 case 3   // [F14] A. Bend Select (2b)
  87 case 4   // [F15] A. Bend Mode (1b)
  21 case 5   // [F16] A. Bend Time (5b)
   5 case 6   // [F17] A. Bend Intensity (5b)
  82 case 7   // [F21] OSC 2 Octave (2b)
  19 case 8   // [F22] OSC 2 Waveform (4b)
   3 case 9   // [F23] OSC 2 Level (5b)
  17 case 10  // [F24] Interval (3b)
  20 case 11  // [F25] Detune (3b)
   0 case 12  // [F26] Noise Level (5b)
  24 case 15  // [F31] Cutoff (6b)
  25 case 16  // [F32] Resonance (5b)
  72 case 17  // [F33] Keyboard Tracking (2b)
  73 case 18  // [F34] Polarity (1b)
   9 case 19  // [F35] EG. Intensity (5b)
  26 case 20  // [F41] VCF Attack (5b)
  27 case 21  // [F42] VCF Decay (5b)
  28 case 22  // [F43] VCF Breakpoint (5b)
  29 case 23  // [F44] VCF Slope (5b)
  30 case 24  // [F45] VCF Sustain (5b)
  31 case 25  // [F46] VCF Release (5b)
  16 case 26  // [F47] VCF Velocity Sensitivity (3b)
  10 case 27  // [F51] VCA Attack (5b)
  11 case 28  // [F52] VCA Decay (5b)
  12 case 29  // [F53] VCA Breakpoint (5b)
  13 case 30  // [F54] VCA Slope (5b)
  14 case 31  // [F55] VCA Sustain (5b)
  15 case 32  // [F56] VCA Release (5b)
   4 case 33  // [F57] VCA Velocity Sensitivity (3b)
  74 case 34  // [F61] MG Waveform (2b)
   8 case 35  // [F62] MG Frequency (5b)
  79 case 40  // [F67] Joystk Bend VCF (1b)
  22 case 41  // [F71] Delay Time (3b)
  23 case 43  // [F73] Delay Feedback (4b)
   6 case 44  // [F74] Delay Frequency (5b)
   7 case 45  // [F75] Delay Intensity (5b)
   1 case 47  // [F77] Portamento time (5b)
  66 case 48  // [F81] Aftertouch OSC MG (2b)
  68 case 49  // [F82] Aftertouch VCF (2b)
  70 case 50  // [F83] Aftertouch VCA (2b)
// Unmapped controller items - due to SSL Nucleus 2 parameter list is unpossible
// with same specs of DW 8000 parameters (switch VS fader properties):
  64 case     // 
  65 case     // 
  67 case     // 
  69 case     // 
  71 case     // 
  75 case     // 
  76 case     // 
  77 case     // 
  78 case     // 
  81 case     // 
  83 case     // 
  84 case     // 
  86 case     // 
  88 case     // 
  89 case     // 
  90 case     // 
  91 case     // 
  92 case     // 
  93 case     // 
  94 case     // 
  95 case     // 
// The function below is currently being explored for management options:
   case : // [F84] MIDI channel
   case : // [F85] MIDI Enable
   case : // [F86] MIDI OMNI
   case : // [F87] Arpeggio clock
#endif

    default:
      // If not explicitly remapped, pass on
      MIDI.sendControlChange(number, value, channel);
      break;      
  }
}

void handleProgramChange(byte channel, byte number)
{
  MIDI.sendProgramChange(number, channel);  
}
void handleAfterTouchChannel(byte channel, byte pressure)
{
  MIDI.sendAfterTouch(pressure, channel);  
}
void handlePitchBend(byte channel, int bend)
{
  MIDI.sendPitchBend(bend, channel);  
}
void handleSystemExclusive(byte* arrayData, unsigned arrayLen)
{
  MIDI.sendSysEx(arrayLen, arrayData, true); //true - must to insert 'F0' and 'F7' Sysex status bytes
}

void setup()
{
    // additional processing module [Unit_5]
    pinMode(ledPinOut, OUTPUT);         // initialize digital pin with LED as an output.
    digitalWrite(ledPinOut, LOW);       // turn the LED off by making the voltage LOW
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn); // Put only the name of the function
    MIDI.setHandleNoteOff(handleNoteOff);// Do the same for NoteOffs
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleSystemExclusive(handleSystemExclusive);
    MIDI.begin(MIDI_CHANNEL_OMNI);      // Initiate MIDI communications, listen to all channels
    MIDI.turnThruOff();                 // Prevent passthrough
}
void loop()
  // delay(300);  // this string for the debuging only
{ MIDI.read(); }  /* Call MIDI.read the fastest you can for real-time performance.
    There is no need to check if there are messages incoming if they are bound to a Callback function.
    The attached method will be called automatically when the corresponding message has been received.
    */
