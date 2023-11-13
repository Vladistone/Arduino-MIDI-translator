// Korg EX/DW-8000 and DSS-1 MIDI Enhancer
// By Steve Baines 2016 mode by Vladistone 2023
// See:
// https://hackaday.io/

#include <MIDI.h>

// Create MIDI interface on main hardware serial port
// (Tx/Rx pins on Arduino board)
MIDI_CREATE_DEFAULT_INSTANCE();


// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// http://arduinomidilib.fortyseveneffects.com/a00022.html

const int ledPinOut = 13;

// 0 here is 1 on DW-8000 - need to set Param 83 to 1
const int dwChannel = 0; // mode from 0

// Also need to set Param 84 to 2, otherwise SYSEX messages are ignored.

void sendDw8000Param(byte channel, byte paramOffset, byte paramValue7Bit)
{
  const int sysexLen = 8;
  static byte sysexData[sysexLen] = { 
    0xf0,   // 0 Sysex start
    0x42,   // 1 Manufacturer ID: 42, Korg
    0x30,   // 2 Channel 1 - mode frome 0x31
    0x0b,   // 3 Device ID: 03, for EX/DW-8000; 0b - for DSS-1
    0x41,   // 4 Message: 41, Parameter change
    0x00,   // 5 Parameter Offset number (which parameter we want to change)
    0x00,   // 6 Parameter value
    0xf7    // 7 Sysex end
  };

  // paramValue7Bit is assumed to run from 0 to 127 in all cases, 
  paramValue7Bit &= 0x7f;


#if(0)
    case 0: // OSC 1 Octave (2b)
    case 1: // OSC 1 Waveform (4b)
    case 2: // OSC 1 Level (5b)
    case 3: // Auto Bend Select (2b)
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
    case 40: // Bend VCF (1b)
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

  // so here we rescale to fit to appropriate bit width for each parameter
  byte paramValueScaled = 0;
  switch (paramOffset)
  {
    case 4: // A. Bend Mode (1b)
    case 18: // Polarity (1b)
    case 40: // Bend VCF (1b)
      paramValueScaled = paramValue7Bit >> 6;
      break;

    case 0: // OSC 1 Octave (2b)
    case 3: // Auto Bend Select (2b)
    case 7: // OSC 2 Octave (2b)
    case 13: // Assign Mode (2b)
    case 17: // Keyboard Tracking (2b)
    case 34: // MG Waveform (2b)
    case 48: // Aftertouch OSC MG (2b)
    case 49: // Aftertouch VCF (2b)
    case 50: // Aftertouch VCA (2b)
      paramValueScaled = paramValue7Bit >> 5;
      break;

    case 10: // Interval (3b)
    case 11: // Detune (3b)
    case 26: // VCF Velocity Sensitivity (3b)
    case 33: // VCA Velocity Sensitivity (3b)
    case 41: // Delay Time (3b)
      paramValueScaled = paramValue7Bit >> 4;
      break;

    case 1: // OSC 1 Waveform (4b)
    case 8: // OSC 2 Waveform (4b)
    case 39: // Bend OSC (4b)
    case 42: // Delay Factor (4b)
    case 43: // Delay Feedback (4b)
    case 46: // Delay Effect Level (4b)
      paramValueScaled = paramValue7Bit >> 3;
      break;

    case 2: // OSC 1 Level (5b)
    case 5: // A. Bend Time (5b)
    case 6: // A. Bend Intensity (5b)
    case 9: // OSC 2 Level (5b)
    case 12: // Noise Level (5b)
    case 16: // Resonance (5b)
    case 19: // EG. Intensity (5b)
    case 20: // VCF Attack (5b)
    case 21: // VCF Decay (5b)
    case 22: // VCF Breakpoint (5b)
    case 23: // VCF Slope (5b)
    case 24: // VCF Sustain (5b)
    case 25: // VCF Release (5b)
    case 27: // VCA Attack (5b)
    case 28: // VCA Decay (5b)
    case 29: // VCA Breakpoint (5b)
    case 30: // VCA Slope (5b)
    case 31: // VCA Sustain (5b)
    case 32: // VCA Release (5b)
    case 35: // MG Frequency (5b)
    case 36: // MG Delay (5b)
    case 37: // MG OSC (5b)
    case 38: // MG VCF (5b)
    case 44: // Delay Frequency (5b)
    case 45: // Delay Intensity (5b)
    case 47: // Portamento (5b)
      paramValueScaled = paramValue7Bit >> 2;
      break;

    case 14: // Parameter No. Memory (6b)
    case 15: // Cutoff (6b)
      paramValueScaled = paramValue7Bit >> 1;
      break;

    default:
    return; // Unknown parameter - ignore
 }
  
  sysexData[2] = 0x30 | (channel & 0x0f); // Set channel number - mode from 0x30
  sysexData[5] = paramOffset;
  sysexData[6] = paramValueScaled;
  
  MIDI.sendSysEx(sysexLen, sysexData, true);
}



void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    // Do whatever you want when a note is pressed.
    digitalWrite(ledPinOut, HIGH);   // turn the LED on
    MIDI.sendNoteOn(pitch, velocity, channel);
}


void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    digitalWrite(ledPinOut, LOW);    // turn the LED off
    MIDI.sendNoteOff(pitch, velocity, channel);
}


void handleControlChange(byte channel, byte number, byte value)
{
  if (channel != dwChannel)
  { // If CC not sent to DW-8000 channel, then just pass them on
    MIDI.sendControlChange(number, value, channel);
    return;
  }

  // Map from received CC numbers to corresponding DW-8000 Parameter offset numbers
  switch(number)
  {
    //   CC number                    Param. Offset
    //   ||                           ||
    // Mappings to common CCs: (See http://nickfever.com/music/midi-cc-list)
    case  5: sendDw8000Param(channel, 47, value); break; // Portamento Time
    case 70: sendDw8000Param(channel,  1, value); break; // (Sound Variation) OSC1 Waveform
    case 71: sendDw8000Param(channel, 16, value); break; // Resonance
    case 72: sendDw8000Param(channel, 32, value); break; // VCA Release
    case 73: sendDw8000Param(channel, 27, value); break; // VCA Attack
    case 74: sendDw8000Param(channel, 15, value); break; // Cutoff Freq

    // Arbitrary mappings from generic CCs to remaining params
    case 75: sendDw8000Param(channel, 34, value); break; // MG Waveform (2b)
    case 76: sendDw8000Param(channel, 35, value); break; // MG Frequency (5b)
    case 77: sendDw8000Param(channel, 36, value); break; // MG Delay (5b)
    case 78: sendDw8000Param(channel, 37, value); break; // MG OSC (5b)
    case 79: sendDw8000Param(channel, 38, value); break; // MG VCF (5b)

    case 16: sendDw8000Param(channel,  3, value); break; // Auto Bend Select (2b)
    case 17: sendDw8000Param(channel,  4, value); break; // A. Bend Mode (1b)
    case 18: sendDw8000Param(channel,  5, value); break; // A. Bend Time (5b)
    case 19: sendDw8000Param(channel,  6, value); break; // A. Bend Intensity (5b)

    case 20: sendDw8000Param(channel, 20, value); break; // VCF Attack (5b)
    case 21: sendDw8000Param(channel, 21, value); break; // VCF Decay (5b)
    case 22: sendDw8000Param(channel, 22, value); break; // VCF Breakpoint (5b)
    case 23: sendDw8000Param(channel, 23, value); break; // VCF Slope (5b)
    case 24: sendDw8000Param(channel, 24, value); break; // VCF Sustain (5b)
    case 25: sendDw8000Param(channel, 25, value); break; // VCF Release (5b)
    case 26: sendDw8000Param(channel, 26, value); break; // VCF Velocity Sensitivity (3b)
    case 27: sendDw8000Param(channel, 28, value); break; // VCA Decay (5b)
    case 28: sendDw8000Param(channel, 29, value); break; // VCA Breakpoint (5b)
    case 29: sendDw8000Param(channel, 30, value); break; // VCA Slope (5b)
    case 30: sendDw8000Param(channel, 31, value); break; // VCA Sustain (5b)
    case 31: sendDw8000Param(channel, 33, value); break; // VCA Velocity Sensitivity (3b)

    case 85: sendDw8000Param(channel, 41, value); break; // Delay Time (3b)
    case 86: sendDw8000Param(channel, 42, value); break; // Delay Factor (4b)
    case 87: sendDw8000Param(channel, 43, value); break; // Delay Feedback (4b)
    case 88: sendDw8000Param(channel, 44, value); break; // Delay Frequency (5b)
    case 89: sendDw8000Param(channel, 45, value); break; // Delay Intensity (5b)
    case 90: sendDw8000Param(channel, 46, value); break; // Delay Effect Level (4b)
    case 102: sendDw8000Param(channel, 0, value); break; // OSC 1 Octave (2b)
    case 103: sendDw8000Param(channel, 2, value); break; // OSC 1 Level (5b)
    case 104: sendDw8000Param(channel, 7, value); break; // OSC 2 Octave (2b)
    case 105: sendDw8000Param(channel, 8, value); break; // OSC 2 Waveform (4b)
    case 106: sendDw8000Param(channel, 9, value); break; // OSC 2 Level (5b)
    case 107: sendDw8000Param(channel, 10, value); break; // Interval (3b)
    case  94: sendDw8000Param(channel, 11, value); break; // Detune (3b)
    case 109: sendDw8000Param(channel, 12, value); break; // Noise Level (5b)
    case 110: sendDw8000Param(channel, 13, value); break; // Assign Mode (2b)
    case 111: sendDw8000Param(channel, 14, value); break; // Parameter No. Memory (6b)
    case 112: sendDw8000Param(channel, 17, value); break; // Keyboard Tracking (2b)
    case 113: sendDw8000Param(channel, 18, value); break; // Polarity (1b)
    case 114: sendDw8000Param(channel, 19, value); break; // EG. Intensity (5b)
    case 115: sendDw8000Param(channel, 39, value); break; // Bend OSC (4b)
    case 116: sendDw8000Param(channel, 40, value); break; // Bend VCF (1b)
    case 117: sendDw8000Param(channel, 48, value); break; // Aftertouch OSC MG (2b)
    case 118: sendDw8000Param(channel, 49, value); break; // Aftertouch VCF (2b)
    case 119: sendDw8000Param(channel, 50, value); break; // Aftertouch VCA (2b)


#if(0)
CC       Param Offset
102    case 0: // OSC 1 Octave (2b)
70    case 1: // OSC 1 Waveform (4b)
103    case 2: // OSC 1 Level (5b)
16    case 3: // Auto Bend Select (2b)
17    case 4: // A. Bend Mode (1b)
18    case 5: // A. Bend Time (5b)
19    case 6: // A. Bend Intensity (5b)
104    case 7: // OSC 2 Octave (2b)
105    case 8: // OSC 2 Waveform (4b)
106    case 9: // OSC 2 Level (5b)
107    case 10: // Interval (3b)
94    case 11: // Detune (3b)
109    case 12: // Noise Level (5b)
110    case 13: // Assign Mode (2b)
111    case 14: // Parameter No. Memory (6b)
74    case 15: // Cutoff (6b)
71    case 16: // Resonance (5b)
112    case 17: // Keyboard Tracking (2b)
113    case 18: // Polarity (1b)
114    case 19: // EG. Intensity (5b)
20    case 20: // VCF Attack (5b)
21    case 21: // VCF Decay (5b)
22    case 22: // VCF Breakpoint (5b)
23    case 23: // VCF Slope (5b)
24    case 24: // VCF Sustain (5b)
25    case 25: // VCF Release (5b)
26    case 26: // VCF Velocity Sensitivity (3b)
73    case 27: // VCA Attack (5b)
27    case 28: // VCA Decay (5b)
28    case 29: // VCA Breakpoint (5b)
29    case 30: // VCA Slope (5b)
30    case 31: // VCA Sustain (5b)
72    case 32: // VCA Release (5b)
31    case 33: // VCA Velocity Sensitivity (3b)
75    case 34: // MG Waveform (2b)
76    case 35: // MG Frequency (5b)
77    case 36: // MG Delay (5b)
78    case 37: // MG OSC (5b)
79    case 38: // MG VCF (5b)
115    case 39: // Bend OSC (4b)
116    case 40: // Bend VCF (1b)
85    case 41: // Delay Time (3b)
86    case 42: // Delay Factor (4b)
87    case 43: // Delay Feedback (4b)
88    case 44: // Delay Frequency (5b)
89    case 45: // Delay Intensity (5b)
90    case 46: // Delay Effect Level (4b)
 5    case 47: // Portamento (5b)
117    case 48: // Aftertouch OSC MG (2b)
118    case 49: // Aftertouch VCF (2b)
119    case 50: // Aftertouch VCA (2b)
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
  MIDI.sendSysEx(arrayLen, arrayData, false);
}

// -----------------------------------------------------------------------------

void setup()
{
    // initialize digital pin with LED as an output.
    pinMode(ledPinOut, OUTPUT);
    digitalWrite(ledPinOut, LOW);    // turn the LED off by making the voltage LOW

    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);

    MIDI.setHandleControlChange(handleControlChange);

    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleSystemExclusive(handleSystemExclusive);
    
    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff(); // Prevent passthrough
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();

    // There is no need to check if there are messages incoming
    // if they are bound to a Callback function.
    // The attached method will be called automatically
    // when the corresponding message has been received.
}
