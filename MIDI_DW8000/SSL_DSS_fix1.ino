// Korg DSS-1 MIDI Enhancer
// By Steve Baines 2016; moded by Vladistone, 2023
// See: https://hackaday.io/
// preparation steps:
// Unit_0 Sysex_6 byte leight
// Unit_1 Sysex_8 byte leight
// Unit_2 Sysex_9 byte leight
// Unit_3 Sysex_10 byte leight
// Unit_4 mapping
// void setup ()
// void loop ()
#include <MIDI.h>

// Create MIDI interface on main hardware serial port
// (Tx/Rx pins on Arduino board)
MIDI_CREATE_DEFAULT_INSTANCE();
/*This function will be automatically called when a NoteOn is received.
  It must be a void-returning function with the correct parameters,
  see documentation here:
  http://arduinomidilib.fortyseveneffects.com/a00022.html */

const int ledPinOut = 13;
const int dssChannel = 0; // 0 here is 1 on DSS-1
/*So necessary to check&set MIDI parameters below:
  MIDI F1 Channel Select: 1
  MIDI F2 Program Change Mode: MODE1 or MODE2; MODE3
  MIDI F2 Modulation Mode: ON
  MIDI F2 After-touch Mode: ON
  MIDI F3 OMNY Mode: ON
The first thing you should check of synth is in performance mode,
not edit, or any other mode! Otherwise the SysEx messages will ignored.
*/
// DDS1 Modes request by use CC#2Sysex converter:
void sendParam_6(byte channel, byte functionID)	// Sysex_6 byte leight Unit_0
{
	const int sysexLen = 6;
	static byte sysexData[sysexLen] = {
		0xF0, // 0 Sysex start
		0x42, // 1 Manufacturer ID: 42, Korg
		0x30, // 2 Channel 1
		0x0B, // 3 Device ID: 0b, DSS-1
		0x00, // 4 Function ID
		0xF7  // 5 EOX
	};
	sysexData[2] = 0x30 | ((channel - 1) & 0x0f);// Set channel#
	sysexData[4] = functionID;// see implement chart discribtion below:
	MIDI.sendSysEx(sysexLen, sysexData, true);
}
	
#if 0
 Sysex hex[4]:    funcID inDec# [id Eqiualent] name		action:
	case 0x10: // functionID 16 [id10] Program Param	Request
	case 0x11: // functionID 17 [id11] Write			Request
	case 0x12: // functionID 18 [id12] Mode				Request
	case 0x13: // functionID 19 [id13] Play mode 		Request
	case 0x14: // functionID 20 [id14] PCM data			Request
	case 0x15: // functionID 21 [id15] MSound parameter	Request	
	case 0x16: // functionID 22 [id16] MSound list		Request
	case 0x17: // functionID 23 [id17] Program nameList	Request
	case 0x40: // functionID 64 [id40] Program paramDump Tx/Rx
	case 0x41: // functionID 65 [id41] Program parameter Change
	case 0x42: // functionID 66 [id43] Mode data		Tx only
	case 0x43: // functionID 67 [id43] PCM data Dump	Tx/Rx
	case 0x44: // functionID 68 [id44] MSound param Dump Tx/Rx
	case 0x45: // functionID 69 [id45] MSound list		Tx/Rx
	case 0x46: // functionID 70 [id46] Program nameList	Tx only

if You wish remapping CC#2SysEx use the program parameter number (but not param.Offset).
Refer: implementation chart [6]DSS-1 Programm parameter map, column "2", at pages 7-8:
https://dn790004.ca.archive.org/0/items/sm_DSS-1ServiceManual/DSS-1ServiceManual.pdf)

	Korg DSS-1 Parameter list (sorted by Function ID, then by paramNumb)
№	FunID ParamNumb hex[5]	# (range)	name			NOTE:  SysEx[5] hex eqiualent to paramNumber[#] inDec.
1	[F01] paramNumb			--   (-) Inicalize params
2	[F02] paramNumb			--   (-) Write/Rename
3	[F11] paramNumb 0x3F	63   (3) Osc 1 Octave		16 (8) 4 within parenthese wouldnt work if use off/on SW
 SysEx [5] hex: param inDec[#] Eqiualent
  case 0x00: // paramNumber 0 Osc 1 Level (101)
  case 0x01: // paramNumber 1 Osc 2 Level (101)
  case 0x02: // paramNumber 2 A.Bend Intesity/ Portameto mix (128)
  case 0x03: // paramNumber 3 Noise Level (64)
  case 0x04: // paramNumber 4 VCF Mode/Slope (2)
  case 0x05: // paramNumber 5 VCF EG Polarity (2)
  case 0x06: // paramNumber 6 VCF Cutoff (128)
  case 0x07: // paramNumber 7 VCF EG Intensity (64)
  case 0x08: // paramNumber 8 VCF Resonance (64)
  case 0x09: // paramNumber 9 VCF Kbd Track (64)
  case 0x0A: // paramNumber 10 VCF MG Frequency (64)
  case 0x0B: // paramNumber 11 VCF MG Delay (64)
  case 0x0C: // paramNumber 12 VCF MG Intensity (64)
  case 0x0D: // paramNumber 13 VCF EG Attack (64)
  case 0x0E: // paramNumber 14 VCF EG Decay (64)
  case 0x0F: // paramNumber 15 VCF EG Breakpoint (64)
  case 0x10: // paramNumber 16 VCF EG Slope (64)
  case 0x11: // paramNumber 17 VCF EG Sustain (64)
  case 0x12: // paramNumber 18 VCF EG Release (64)
  case 0x13: // paramNumber 19 VCA Kbd Decay (128)
  case 0x14: // paramNumber 20 VCA Level (64)
  case 0x15: // paramNumber 21 VCA EG Attack (64)
  case 0x16: // paramNumber 22 VCA EG Decay (64)
  case 0x17: // paramNumber 23 VCA EG Breakpoint (64)
  case 0x18: // paramNumber 24 VCA EG Slope (64)
  case 0x19: // paramNumber 25 VCA EG Sustain (64)
  case 0x1A: // paramNumber 26 VCA EG Release (64)
  case 0x1B: // paramNumber 27 Veloc ABend Intens (64)
  case 0x1C: // paramNumber 28 Veloc VCF Cutoff (16)
  case 0x1D: // paramNumber 29 Veloc VCF EG Attack (64)
  case 0x1E: // paramNumber 30 Veloc VCF EG Decay (64)
  case 0x1F: // paramNumber 31 Veloc VCF EG Slope (64)
  case 0x20: // paramNumber 32 VCA EG Intensity (64)
  case 0x21: // paramNumber 33 Veloc VCA EG Attack (64)
  case 0x22: // paramNumber 34 Veloc VCA EG Decay (64)
  case 0x23: // paramNumber 35 Veloc VCA EG Slope (64)
  case 0x24: // paramNumber 36 ATch Osc MG Intens (16)
  case 0x25: // paramNumber 37 ATch VCF Level (16)
  case 0x26: // paramNumber 38 ATch VCF Mode (2)
  case 0x27: // paramNumber 39 ATch VCA Level (16)
  case 0x28: // paramNumber 40 JStck PBend Range (13)
  case 0x29: // paramNumber 41 JStck VCF Mode (2)
  case 0x2A: // paramNumber 42 EQ Bass (13)
  case 0x2B: // paramNumber 43 EQ Treble (13)
  case 0x2C: // paramNumber 44 DDL MG-A Freq (64)
  case 0x2D: // paramNumber 45 DDL MG-B Freq (64)
  case 0x2E: // paramNumber 46 DDL-1 Time (a) (128)
  case 0x2E: // paramNumber 46 DDL-1 Time (b) (128)
  case 0x2E: // paramNumber 46 DDL-1 Time (c) (128)
  case 0x2E: // paramNumber 46 DDL-1 Time (d) (117)
  case 0x2F: // paramNumber 47 DDL-1 Feedback (16)
  case 0x30: // paramNumber 48 DDL-1 Effect Level (16)
  case 0x31: // paramNumber 49 DDL-1 MG-A Intens (64)
  case 0x32: // paramNumber 50 DDL-1 MG-B Intens (64)
  case 0x33: // paramNumber 51 DDL-2 Input Select (2)
  case 0x34: // paramNumber 52 DDL-2 Time (a) (128)
  case 0x34: // paramNumber 52 DDL-2 Time (b) (128)
  case 0x34: // paramNumber 52 DDL-2 Time (c) (128)
  case 0x34: // paramNumber 52 DDL-2 Time (d) (117)
  case 0x35: // paramNumber 53 DDL-2 Feedback (16)
  case 0x36: // paramNumber 54 DDL-2 Effect Level (16)
  case 0x37: // paramNumber 55 DDL-2 MG-A Intens (64)
  case 0x38: // paramNumber 56 DDL-2 MG-B Intens (64)
  case 0x39: // paramNumber 57 DDL-2 Mod Invert (2)
  case 0x3A: // paramNumber 58 Osc 1 Multisound (16)
  case 0x3B: // paramNumber 59 Osc 2 Multisound (16)
  case 0x3C: // paramNumber 60 Max OSC Band Range (13)
  case 0x3D: // paramNumber 61 Sync Mode (2)
  case 0x3E: // paramNumber 62 Bit D A Resolution (5)
  case 0x3F: // paramNumber 63 Osc 1 Octave (3)
  case 0x40: // paramNumber 64 Osc 2 Octave (3)
  case 0x41: // paramNumber 65 Osc 2 Detune (64)
  case 0x42: // paramNumber 66 Osc 2 Interval (12)
  case 0x43: // paramNumber 67 Osc MG Select (4)
  case 0x44: // paramNumber 68 Osc MG Frequency (32)
  case 0x45: // paramNumber 69 Osc MG Intensity (64)
  case 0x46: // paramNumber 70 Osc MG Delay (16)
  case 0x47: // paramNumber 71 A.Bend Select (4)
  case 0x48: // paramNumber 72 A.Bend Polarity Mode (2)
  case 0x49: // paramNumber 73 A.Bend Time/ Portamento time (32)
  case 0x4A: // paramNumber 74 Unison Detune (8)
  case 0x4B: // paramNumber 75 Veloc Osc X-Switch (32)
  case 0x4C: // paramNumber 76 Key Assign mode (3)
  case 0x4D: // paramNumber 77 Unison Voices (4)
#endif

// Main DSS-1: Scaling SysEx_8 leight for paramNumber of [5]
void sendParam_8(byte channel, byte paramNumber, byte paramValue7Bit)// Unit_1
{
  const int sysexLen = 8;
  static byte sysexData[sysexLen] = {
	0xF0, // 0 Sysex start
	0x42, // 1 Manufacturer ID: 42, Korg
	0x30, // 2 Channel 1
	0x0B, // 3 Device ID: 0b, DSS-1
	0x41, // 4 Function ID: 41, Program Parameter change
	0x00, // 5 Parameter number (which parameter we want to change)
	0x00, // 6 Parameter value
	0xF7  // 7 EOX
  };
  // paramValue7Bit is assumed to run from 0 to 127 in all cases,
  paramValue7Bit &= 0x7f;
  // so here we rescale to fit to appropriate bit width for each parameter
  byte paramValueScaled = 0;
  switch (paramNumber) {
  case 0x04: // paramNumber 4  (2) VCF Mode/Slope
  case 0x05: // paramNumber 5  (2) VCF EG Polarity
  case 0x26: // paramNumber 38 (2) ATch VCF Mode
  case 0x29: // paramNumber 41 (2) JStck VCF Mode
  case 0x33: // paramNumber 51 (2) DDL-2 In Sel
  case 0x39: // paramNumber 57 (2) DDL-2 Mod Invert
  case 0x3D: // paramNumber 61 (2) Sync Mode
  case 0x48: // paramNumber 72 (2) A.Bend Polar.Mode
    paramValueScaled = paramValue7Bit >> 6; break;
  case 0x43: // paramNumber 67 (4) Osc MG Select
  case 0x47: // paramNumber 71 (4) A.Bend Select
  case 0x4D: // paramNumber 77 (4) Unison Voices
    paramValueScaled = paramValue7Bit >> 5; break;
  case 0x4A: // paramNumber 74 (8) Unison Detune
    paramValueScaled = paramValue7Bit >> 4; break;
  case 0x1C: // paramNumber 28 (16) Veloc VCF Cutoff
  case 0x24: // paramNumber 36 (16) ATch OSC MG Intens
  case 0x25: // paramNumber 37 (16) ATch VCF Level
  case 0x27: // paramNumber 39 (16) ATch VCA Level
  case 0x2F: // paramNumber 47 (16) DDL-1 Feedback
  case 0x30: // paramNumber 48 (16) DDL-1 FX Level
  case 0x35: // paramNumber 53 (16) DDL-2 Feedback
  case 0x36: // paramNumber 54 (16) DDL-2 FX Level
  case 0x3A: // paramNumber 58 (16) Osc 1 MSound
  case 0x3B: // paramNumber 59 (16) Osc 2 MSound
  case 0x46: // paramNumber 70 (16) OSC MG Delay
    paramValueScaled = paramValue7Bit >> 3; break;
  case 0x44: // paramNumber 68 (32) OSC MG Freq.
  case 0x49: // paramNumber 73 (32) A.Bend /Porta.Time
  case 0x4B: // paramNumber 75 (32) Veloc OSC X-Switch
    paramValueScaled = paramValue7Bit >> 2; break;
  case 0x03: // paramNumber 3  (64) Noise Level
  case 0x07: // paramNumber 7  (64) VCF EG Intens
  case 0x08: // paramNumber 8  (64) VCF Resonance
  case 0x09: // paramNumber 9  (64) VCF Kbd Track
  case 0x0A: // paramNumber 10 (64) VCF MG Freq.
  case 0x0B: // paramNumber 11 (64) VCF MG Delay
  case 0x0C: // paramNumber 12 (64) VCF MG Intens
  case 0x0D: // paramNumber 13 (64) VCF EG Attack
  case 0x0E: // paramNumber 14 (64) VCF EG Decay
  case 0x0F: // paramNumber 15 (64) VCF EG BrPoint
  case 0x10: // paramNumber 16 (64) VCF EG Slope
  case 0x11: // paramNumber 17 (64) VCF EG Sustain
  case 0x12: // paramNumber 18 (64) VCF EG Release
  case 0x14: // paramNumber 20 (64) VCA Level
  case 0x15: // paramNumber 21 (64) VCA EG Attack
  case 0x16: // paramNumber 22 (64) VCA EG Decay
  case 0x17: // paramNumber 23 (64) VCA EG BrPoint
  case 0x18: // paramNumber 24 (64) VCA EG Slope
  case 0x19: // paramNumber 25 (64) VCA EG Sustain
  case 0x1A: // paramNumber 26 (64) VCA EG Release
  case 0x1B: // paramNumber 27 (64) VelSns ABend Intens
  case 0x1D: // paramNumber 29 (64) VelSns VCF EG Attack
  case 0x1E: // paramNumber 30 (64) VelSns VCF EG Decay
  case 0x1F: // paramNumber 31 (64) VelSns VCF EG Slope
  case 0x20: // paramNumber 32 (64) VCA EG Intensity
  case 0x21: // paramNumber 33 (64) VelSns VCA EG Attack
  case 0x22: // paramNumber 34 (64) VelSns VCA EG Decay
  case 0x23: // paramNumber 35 (64) VelSns VCA EG Slope
  case 0x2C: // paramNumber 44 (64) DDL MG-A Freq
  case 0x2D: // paramNumber 45 (64) DDL MG-B Freq
  case 0x31: // paramNumber 49 (64) DDL-1 MG-A Intens
  case 0x32: // paramNumber 50 (64) DDL-1 MG-B Intens
  case 0x37: // paramNumber 55 (64) DDL-2 MG-A Intens
  case 0x38: // paramNumber 56 (64) DDL-2 MG-B Intens
  case 0x41: // paramNumber 65 (64) Osc 2 Detune
  case 0x45: // paramNumber 69 (64) OSC MG Intensity
    paramValueScaled = paramValue7Bit >> 1; break;
  case 0x02: // paramNumber 2 (128) A.Bend Intes/PortaMix
  case 0x06: // paramNumber 6 (128) VCF Cutoff
  case 0x13: // paramNumber 19(128) VCA Kbd Decay
    paramValueScaled = paramValue7Bit; break;
  // for this param below we are use ariphmetic ops:
  case 0x3F: // paramNumber 63  (3) Osc 1 Octave
  case 0x40: // paramNumber 64  (3) Osc 2 Octave
  case 0x4C: // paramNumber 76  (3) Key Assign mode
    paramValueScaled = paramValue7Bit/43; break;
  case 0x3E: // paramNumber 62  (5) Bit DA Resolution
    paramValueScaled = paramValue7Bit/26; break;
  case 0x42: // paramNumber 66 (12) Osc 2 Interval
    paramValueScaled = paramValue7Bit*24/256; break;
  case 0x28: // paramNumber 40 (13) JStck PBend Range
  case 0x2A: // paramNumber 42 (13) EQ Bass
  case 0x2B: // paramNumber 43 (13) EQ Treble
  case 0x3C: // paramNumber 60 (13) Max OSC Band Range
    paramValueScaled = paramValue7Bit*26/256; break;
  case 0x00: // paramNumber 0 (101) Osc 1 Level
  case 0x01: // paramNumber 1 (101) Osc 2 Level
    paramValueScaled = paramValue7Bit*203/256; break;
  default:
  return;	// unknown parameter - ignore
  }
  sysexData[2] = 0x30 | ((channel - 1) & 0x0f);
  sysexData[5] = paramNumber;
  sysexData[6] = paramValueScaled;
  MIDI.sendSysEx(sysexLen, sysexData, true);
}

// Scaling SyxEx_9 leight of DDL1&2 Time, LSB&MSB bytes only:
void sendParam_9(byte channel, byte paramNumber, byte paramValue7Bit) // Unit_2
{
  const int sysexLen = 9;
  static byte sysexData[sysexLen] =
  {
    0xF0, // 0 SOX
    0x42, // 1 Manufacturer ID: 42, Korg
    0x30, // 2 Channel 1
    0x0B, // 3 Device ID: 0b, DSS-1
    0x41, // 4 Function ID: 41, Program Parameter change
    0x00, // 5 Parameter number (which parameter we want to change)
    0x00, // 6 Parameter low byte
    0x00, // 7 Parameter high byte
    0xF7  // 8 EOX
  };
  paramValue7Bit &= 0x7f;
  int paramValueScaled = 0;
  switch (paramNumber) // So here we rescale to fit to range:
  {
	  case 0x2E: // [F81] paramNumb 46 DDL-1 Time (500)
	  case 0x34: // [F92] paramNumb 52 DDL-2 Time (500)
	  paramValueScaled = paramValue7Bit*252/64; break;
	  default: return;	// unknown parameter - ignore
  }
  sysexData[2] = 0x30 | ((channel - 1) & 0x0f);	// Set channel number
  sysexData[5] = paramNumber;
  sysexData[6] = paramValueScaled & 0x7f;		// LSB of 14-bit value
  sysexData[7] = (paramValueScaled >> 7) & 0x03;// MSB
  MIDI.sendSysEx(sysexLen, sysexData, true);
}

// Scaling SyxEx_10 leight of OSC mix ratio using by one CC# source:
void sendParam_10(byte channel, byte paramNumber, byte paramValue7Bit) { // Unit_3
  const int sysexLen = 10;
  static byte sysexData[sysexLen] = {
    0xF0, // 0 SOX
    0x42, // 1 Manufacturer ID: 42, Korg
    0x30, // 2 Channel 1
    0x0B, // 3 Device ID: 0b, DSS-1
    0x41, // 4 Message: 41, Parameter change (it not the parameter DUMP)
    0x00, // 5 Parameter number (1-st param witch we going to change)
    0x00, // 6 Parameter master value
    0x00, // 7 Parameter number (be as slave)
    0x00, // 8 Parameter slave value
    0xF7  // 9 EOX
    };

    paramValue7Bit &= 0x7f;
    int paramValueScaled;
    switch (paramNumber) { // so here we rescale to fit to rang:
    case 0x00: // [F14] paramNumber 0 OSC1 level Mix ratio (master)
	case 0x01: // [F14] paramNumber 1 OSC2 level Mix ratio (slave)
	paramValueScaled = paramValue7Bit*203/256; break;
    default:
    return;	// unknown parameter - ignore
   }
    sysexData[2] = 0x30 | ((channel - 1) & 0x0f);// Set channel number
    sysexData[5] = paramNumber; // master must be (Mster+Slave) == 100%:
    sysexData[6] = paramValueScaled & 0x7f; // master value
    sysexData[7] = (paramNumber +1); // slave
    sysexData[8] = (100 - paramValueScaled) & 0x7f; // slave value
    MIDI.sendSysEx(sysexLen, sysexData, true);
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{// Do whatever you want when a note is pressed.
	digitalWrite(ledPinOut, HIGH);	// turn the LED on
	MIDI.sendNoteOn(pitch, velocity, channel);
}
void handleNoteOff(byte channel, byte pitch, byte velocity)
{// Do something when the note is released.
	// Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
	digitalWrite(ledPinOut, LOW);	// turn the LED off
	MIDI.sendNoteOff(pitch, velocity, channel);
}
void handleControlChange(byte channel, byte number, byte value)
{
  if (channel != dssChannel + 1)
  {// If CC not sent to DSS-1 channel, then just pass them on
	MIDI.sendControlChange(number, value, channel);
	return;
  }
  // Map from received CC numbers to corresponding DSS-1 ParaNumbers. Unit_4
  // personal desigion for each one controllers
  switch(number) // refer http://nickfever.com/music/midi-cc-list
  {
    //		SSL Nucleus Fader group L (Mappings to myown CCs desigion)
    case 0: sendParam_8(channel, 32, value); break;  // [F44] VCA EG Intens
    //case 1: sendParam_8(channel, 69, value); break;// [F17] OSC MG intens		reserved CC#1
    //case 2: sendParam_8(channel, 12, value); break;// [F34] VCF MG intens		reserved CC#2
    case 3: sendParam_8(channel, 48, value); break;  // [F83] DDL-1 FX Level
    case 4: sendParam_8(channel, 54, value); break;  // [F94] DDL-2 FX Level
    case 5: sendParam_8(channel, 2, value); break;   // [F19] AB Intesity
    case 6: sendParam_8(channel, 3, value); break;   // [F21] Noise level
    //case 7: sendParam_8(channel, 20, value); break;// [F36] VCA TotalLevel	reserved CC#7
    //		SSL Nucleus Fader group R
    case 8: sendParam_8(channel, 42, value); break;  // [F65] EQ Bass
    case 9: sendParam_8(channel, 43, value); break;  // [F65] EQ Treble
    case 10: sendParam_8(channel, 21, value); break; // [F38] VCA EG Attack
    case 11: sendParam_8(channel, 22, value); break; // [F38] VCA EG Decay
    case 12: sendParam_8(channel, 23, value); break; // [F38] VCA EG BrPoint
    case 13: sendParam_8(channel, 24, value); break; // [F38] VCA EG Slope
    case 14: sendParam_8(channel, 25, value); break; // [F38] VCA EG Sustain
    case 15: sendParam_8(channel, 26, value); break; // [F38] VCA EG Release
    //		SSL Nucleus V-Coder group L
    case 16: sendParam_8(channel, 7, value); break;  // [F11] VCF EG Intens
    case 17: sendParam_8(channel, 68, value); break; // [F17] OSC MG Mod Freq
    case 18: sendParam_8(channel, 75, value); break; // [F46] Veloc OSC X-Sw
    case 19: sendParam_9(channel, 46, value); break; // [F81] DDL1 Time
    case 20: sendParam_9(channel, 52, value); break; // [F92] DDL2 Time
    case 21: sendParam_8(channel, 73, value); break; // [F19] AB time/Portam Time
    case 22: sendParam_8(channel, 74, value); break; // [F64] Unison Detune
    case 23: sendParam_10(channel, 0, value); break; // [F14] OSC Mix ratio		+[F14] 01 OSC2 Level
    //		SSL Nucleus V-Coder group R
    case 24: sendParam_8(channel, 6, value); break;  // [F32] VCF Cutoff
    case 25: sendParam_8(channel, 8, value); break;  // [F33] VCF Resonance
    case 26: sendParam_8(channel, 13, value); break; // [F35] VCF EG Attack
    case 27: sendParam_8(channel, 14, value); break; // [F35] VCF EG Decay
    case 28: sendParam_8(channel, 15, value); break; // [F35] VCF EG BrPoint
    case 29: sendParam_8(channel, 16, value); break; // [F35] VCF EG Slope
    case 30: sendParam_8(channel, 17, value); break; // [F35] VCF EG Sustain
    case 31: sendParam_8(channel, 18, value); break; // [F35] VCF EG Release
    //		SSL Nucleus Select SW group L
    case 64: sendParam_8(channel, 5, value); break;  // [F18] VCF EG Polar	reserved CC#64
    case 65: sendParam_8(channel, 41, value); break; // [F62] JStck VCF Sweep Mode
    case 66: sendParam_8(channel, 4, value); break;  // [F31] VCF Mode
    case 67: sendParam_8(channel, 51, value); break; // [F91] DDL2 Input Sel
    case 68: sendParam_8(channel, 57, value); break; // [F96] DDL2 Mod Invert
    case 69: sendParam_8(channel, 72, value); break; // [F18] AB Polar Mode
    case 70: sendParam_8(channel, 38, value); break; // [F52] ATch VCF Mode
    case 71: sendParam_6(channel, 19); break;		 // [ID13] Req. PlayMod [F0 42 30 0B 13 F7]
    //		SSL Nucleus Select SW group R
    case 72: sendParam_8(channel, 63, value); break; // [F11] Osc 1 Octave	16 (8) 4
    case 73: sendParam_8(channel, 64, value); break; // [F11] Osc 2 Octave	16 (8) 4
    case 74: sendParam_8(channel, 67, value); break; // [F17] Osc MG ModSel	Off (Osc1, Osc2) Both
    case 75: sendParam_8(channel, 62, value); break; // [F16] Bit Resolutn	6 (7, 8, 10) 12
    case 76: sendParam_8(channel, 76, value); break; // [F63] Key AssignMod	Poly2 (Poly1) Unison
    case 77: sendParam_8(channel, 71, value); break; // [F18] A.Bend Select	Off (Osc1, Osc2) Both
    case 78: sendParam_8(channel, 77, value); break; // [F64] Unison Voices	2 (4, 6) 8
    case 79: sendParam_8(channel, 61, value); break; // [F16] Sync Mode Sw.	Off/On
    //		SSL Nucleus V-Coder Select group L
    case 80: sendParam_6(channel, 18); break;		 // [id12] Request: Mode condition
    case 81: sendParam_6(channel, 22); break;		 // [id16] Request: MSound list
    case 82: sendParam_6(channel, 23); break;		 // [id17] Request: Program nameList
    /* case 83: sendParam_6(channel,); break;
    case 84: sendParam_7(channel, 21, value); break; // [id15] Request: MSound Param
    case 85: sendParam_7(channel, 16, value); break; // [id10] Request: Program Param
    case 86: sendParam_7(channel, 17, value); break; // [id11] Request: Write
    case 87: sendParam_18(channel,20,A,B,C,D,E,F,U,V,W,X,Y,Z); break; // [id14] Request: PCMdata
    		SSL Nucleus V-Coder Select group R
    case 88: sendDSS1Param_x(channel,, value); break; // Set System mode	in project...
    case 89: sendDSS1Param_x(channel,, value); break; // Set Disk Utility
    case 90: sendDSS1Param_x(channel,, value); break; // Set Program param.
    case 91: sendDSS1Param_x(channel,, value); break; // Set Sample
    case 92: sendDSS1Param_x(channel,, value); break; // Set Edit sample
    case 93: sendDSS1Param_x(channel,, value); break; // Set Creat Waveform
    case 94: sendDSS1Param_x(channel,, value); break; // Set MultiSound
    case 95: sendDSS1Param_x(channel,, value); break; // Set MIDI mode
	*/
#if 0
//  CC# to corresponding DSS-1 Parameter numbers map:
	CC# [FID] ## hex[5] Parameter	 Name
-------|-----|--|------|------------|---------------
	SSL Nucleus Fader group L
case 0:	[F44] 32 (0x20) paramNumber: VCA EG Intensity
case 1:	[F17] 69 (0x45) paramNumber: OSC MG Mod Intens	reserved CC#1
case 2:	[F34] 12 (0x0C) paramNumber: VCF MG Mod Intens	reserved CC#2
case 3:	[F83] 48 (0x30) paramNumber: DDL-1 FX Level
case 4:	[F94] 54 (0x36) paramNumber: DDL-2 FX Level
case 5:	[F19] 2  (0x02) paramNumber: A.Bend Intesity
case 6:	[F21] 3  (0x03) paramNumber: Noise Level
case 7:	[F36] 20 (0x14) paramNumber: VCA Total Level	reserved CC#7
	SSL Nucleus Fader group R
case 8:	[F65] 42 (0x2A) paramNumber: EQ Bass
case 9:	[F65] 43 (0x2B) paramNumber: EQ Treble
case 10:[F38] 21 (0x15) paramNumber: VCA EG Attack
case 11:[F38] 22 (0x16) paramNumber: VCA EG Decay
case 12:[F38] 23 (0x17) paramNumber: VCA EG BrPoint
case 13:[F38] 24 (0x18) paramNumber: VCA EG Slope
case 14:[F38] 25 (0x19) paramNumber: VCA EG Sustain
case 15:[F38] 26 (0x1A) paramNumber: VCA EG Release
	SSL Nucleus V-Coder group L
case 16:[F32] 7  (0x07) paramNumber: VCF EG Intensity
case 17:[F17] 68 (0x44) paramNumber: OSC MG Mod Freq
case 18:[F46] 75 (0x4B) paramNumber: VelSns OSC X-Sw.
case 19:[F81] 46 (0x2E) paramNumber: DDL-1 Time
case 20:[F92] 52 (0x34) paramNumber: DDL-2 Time
case 21:[F19] 73 (0x49) paramNumber: A.Bend Time
case 22:[F64] 74 (0x4A) paramNumber: Unison Detune
case 23:[F14] 0  (0x00) paramNumber: Osc1 Lev/MixRatio	Master
case 23:[F14] 1  (0x01) paramNumber: Osc2 Lev/MixRatio 	Slave
	SSL Nucleus V-Coder group R
case 24:[F32] 6  (0x06) paramNumber: VCF Cutoff
case 25:[F33] 8  (0x08) paramNumber: VCF Resonance
case 26:[F35] 13 (0x0D) paramNumber: VCF EG Attack
case 27:[F35] 14 (0x0E) paramNumber: VCF EG Decay
case 28:[F35] 15 (0x0F) paramNumber: VCF EG BrPoint
case 29:[F35] 16 (0x10) paramNumber: VCF EG Slope
case 30:[F35] 17 (0x11) paramNumber: VCF EG Sustain
case 31:[F35] 18 (0x12) paramNumber: VCF EG Release
	SSL Nucleus Select SW group L
case 64:[F31] 5  (0x05) paramNumber: VCF EG Polarity	reserved CC#64
case 64:[F--] 64 (0x40) paramNumber: Dumper pedal
case 65:[F62] 41 (0x29) paramNumber: JStck VCF Mode
case 66:[F31] 4  (0x04) paramNumber: VCF Mode/Slope
case 67:[F91] 51 (0x33) paramNumber: DDL-2 In. Select
case 68:[F96] 57 (0x39) paramNumber: DDL-2 Mod Invert
case 69:[F18] 72 (0x48) paramNumber: A.Bend Polar Mode
case 70:[F52] 38 (0x26) paramNumber: ATch VCF Mode
case 71:[ID13]18 (0x0D) Function ID: Play mode			Request
	SSL Nucleus Select SW group R
case 72:[F11] 63 (0x3F) paramNumber: Osc 1 Octave
case 73:[F11] 64 (0x40) paramNumber: Osc 2 Octave
case 74:[F17] 67 (0x43) paramNumber: Osc MG Mod Sel
case 75:[F16] 62 (0x3E) paramNumber: Bit Resolution
case 76:[F63] 76 (0x4C) paramNumber: Key Assign Mode
case 77:[F18] 71 (0x47) paramNumber: A.Bend Select
case 78:[F64] 77 (0x4D) paramNumber: Unison Voices
case 79:[F16] 61 (0x3D) paramNumber: Sync Mode
	SSL Nucleus V-Coder Select group L
case 80:[id12]18 (0x12) Function ID: Mode condition		Request
case 81:[id16]22 (0x16) Function ID: MSound list		Request
case 82:[id17]23 (0x23) Function ID: Program name list	Request 
	
#endif
	default: // If not explicitly remapped, pass on
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

void setup()
{
	// initialize digital pin with LED as an output.
	pinMode(ledPinOut, OUTPUT);
	digitalWrite(ledPinOut, LOW);	// turn the LED off by making the voltage LOW
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
{ MIDI.read(); }
	/*Call MIDI.read the fastest you can for real-time performance.
	There is no need to check if there are messages incoming
	if they are bound to a Callback function.
	The attached method will be called automatically
	when the corresponding message has been received
	*/

