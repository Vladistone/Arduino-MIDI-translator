[#Back-to-Top""]
= Arduino MIDI translator
Vladistone <vl2guitar@gmail.com>
:toc:
:toclevels: 3
:doctype: book
:sectnums:
:partnums:
:sectnumlevels: 3
:experimental:
:source-highlighter: pygments
:source-language: cpp
:tip-caption: pass:[&#128161;]
:warning-caption: pass:[&#9888;]
:note-caption: pass:[&#128204;]
:caution-caption: pass:[&#8252;]
:synth_upgrade_instruction: to be created as soos as possible
:code_implement_instruction: to be created as soos as possible
:readme_content: at this moment I’m just gathering my thoughts and the description of README2 is still in its starting state, as soon as possible I will add blocks and pictures of examples of both synthesizers and analysis of the code and its internal blocks with explanations

|===
|Last update of text: |`2023-12-09 (v0.01.0)`
|Last update of relevant screenshots: |`2023-12-09 (v0.01.0)`
|===

kbd:[<<Back-to-Top>>]

= Quick start

Here's a step-by-step guide to help you get started with Arduino and a MIDI controller:

. Download & Start Arduino IDE.
. If you haven't already done it, https://www.arduino.cc/en/software[install Arduino IDE].
. Make sure your MIDI_library.h is enabled in _Sketch → Include Library… → Manage Librarys ⇧⌘i →  download or upgrade the MIDI_library.h if you need...
  * For the Arduino board device, tick _for your name device_
  * and tick your _Port for connection_.

kbd:[<<Back-to-Top>>]

[colophon]
= Introduction and a bit history

[TIP]
The great idea is to organize a repository for codes of `Arduino MIDI translator` in one place, which to accumulate projects related to the rare synths from 80-90 y., by Arduino upgrading to facilitate focused share knowledge, and foster collaboration among musicians & coder enthusiasts. May be this dedicated space will serve as a hub for users to exchange code mapping, ideas, troubleshoot, and showcase their projects or branching related to the any synths upgrade by Arduino,.. Time will show...


:MIDI_code_preset_content: There are many controllers projects for synth manipulation by Arduino chipping.
And although technically possible in exactly the same way as with controller presets for factory MIDI devices,
Synth map layouts and using controllers to combine with synths are very custom, so I'm no sure yet if it would make much sense to create map presets of code... but the base approach of the system for developing code for `Arduino MIDI translator` and consistency in adapting MIDI implementation has already been launched and this requires publicity...
*show must go on*!

.MIDI implementaion developing
====
The Korg <<DW-8000>>; <<EX8000>> and <<DSS-1>> (and other early hybrid synthesisers) was created during the early days of MIDI, before things like `CC#` messages were standardised.  As a result, it uses proprietary `SysEx` commands to change parameters over MIDI. Most modern MIDI controllers use `CC#` messages instead, and very few are flexible enough to be able to send the appropriate `SysEx` messages. Those similar project uses an embedded Arduino Nano (or clone) mounted inside the synth body, which is used to intercept the MIDI IN data just before it reaches the CPU of synth. It replaces specific `CC#` messages with `SysEx` messages that the sysnthesizer understands them.
====

The project start from 5th July 2016 and based on a hack https://hackaday.io/project/12541-korg-dw-8000-midi-enhancer[Korg DW8000 midi enhancer] publication by Steve Baines https://github.com/sjbaines/Korg_DW-8000_Midi_Enhancer[and his code repo]. As His discribed His code was tested and may have been stable, but by the time November 2023, when I discovered this project on the network - I could not immediately run it on the EX8000.
It turned out to be non-working and required debugging. The pull request were added after fixing, but I did not find any activity from the project's master.
So therefore, I independently continued the path of adapting the project for Korg DSS-1, thanks to mentoring from experienced Arduino C++ coder. I achieved the first results literally a week after [the analysis differences between DW8000 vs DSS-1 of MIDI implementation charts]<<Nuances>>.

kbd:[<<Back-to-Top>>]

=== What is Arduino MIDI-translator?

Probably you know already that Arduino is a sort ofimprovement or upgrade over Synth's built-in MIDI interface circuit diagram. But what is it exactly? Let me put it this way:

imag::under_controller.jpg
____

Arduino is an IC board which will allow you to take whatever MIDI controller you have, be it a keyboard or some fader box, plug it in and  tune changing of the parameter of synth…
regardless of whether your oldschool synthesizer can read `CC#` commands or not.

And for such pleasures as independence from expensive specialized controllers or the not need to be chained to a computer for software MIDI translation, you can, together with other performance equipment give a new life to old synthesizers from the 80s such as the Korg DSS-1  using Arduino and with programming skills!!
____


This Simple Arduino project to remap MIDI `CC#` messages into `SysEx` messages that Korg DSS-1 Synth expects.


The practical upshot of this is that the Synth Program parameters can be controlled by standard `CC#` messages, rather than obscure `SysEx` messages.
MIDI THRU is unaffected by the changes, as the Nano is be spliced in just before the MIDI IN reaches the CPU, AFTER the IN has been echoed back out of the THRU port.
Certain `CC#` messages are replaced with `SysEx` parameter control messages, but all other MIDI messages are passed unchanged. This means that the `SysEx` messages can still be used.
kbd:[<<Back-to-Top>>]

[#Synth part]
= Synth review
This section is dedicated to the internal features and details when implementing Arduino on a synthesizer board (I will try to describe how it worked out for me and give links to the experience of other experimenters with their upgrage...
Here is my small list of synthesizers that achieved results with the Arduino translator!
Korg DSS-1 - the flagship for upgrades! The following events were held for it:
 * Cleared, Adjusted, silent and soft touched keys board;
 * new micro buttons on the front panel;
 * a modern 3-pin AC-Plug with line-filter;
 * a Gotek 1,4M & flashflopy emulato;r
 * a new blue LCD;
 * a built-in arduino nano for MIDI translator CC2SysEx;

Korg DW8000 with Arduino chipping;
Korg EX8000 with the same upgrade;

[#EX8000]
== Korg EX8000
The first my project point it was with Korg EX8000: For full project details and step by step instructions discribed at https://hackaday.io/project/12541-korg-dw-8000-midi-enhancer

[#DW8000]
== Korg DW8000

[#DSS-1]
== Korg DSS-1
Mod for Korg DSS-1 was done a little differently which EX8000: by attaching an arduino board to a MIDI socket board - a KLM-788 Jack panel. See circuit diagram below...
 * Power is taken from the closest points to the CN26B connector (pin #5 and #6 ).
 * The power plug for the arduino is also placed on thermo-plastic, and the arduino is already fixed on it, at 90 degrees/"edge" to the KLM-788 board.
 * To connect RX and TX using an additional connecting plug, I made a break in the wire from pin#2 of the same CN26B socket, going to the main board KLM-781 CPU-II - HD63B03X.

====
The nuance is that you don’t confuse where to connect the ends of Tx and Rx:
*Rx* end - connect to the end going to the socket pin # 2 CN26B,
*Tx* end - to the trim going to the main board of the KLM-781 processor HD63B03Xb designated in circuit diagram as IC11
====

The design turned out to be more clear when opening the top cover of the synth and an easy
ability the Arduino unit can be removed for reprogramming of the code.

kbd:[<<Back-to-Top>>]

[#Controller part]
= Controller part review

[#Nucleus]
== SSL Nucleus
:imagesdir: IMAGES/Controllers/SSL Nucleus/
.example: MCU & XT Projections
|===
|image:projection_MCU.svg[Projection MCU,470,380] |image:projection_XT.svg[Projection XT,260,380]
2+|image:SSL_Nucleus_Mk2.svg[SSL_Nucleus_Mk2,700,350]
|===

[#JP800]
== Roland JP8000

kbd:[<<Back-to-Top>>]

[#code discribe]
= CODE Discribtion:
== Preparation for renovation of code for DSS-1:
There are base difference in the description of the implementation chart for DW8000 vs DSS-1:

 .the First:
====
- DW8000 parameters range from 0 to 64 (or upto 6b of SysEx length) in 7 categories:
- DSS-1 parameters vary from 0 to 127 and even upto 500 points; it increase the range categoties up to 12 (it mean SysEx length to 7b and upto 10b and more if you wish manupulating mod request parameter of synth)
Therefore, it was necessary to analyze and remake the CC Value transformation system to the additional parameters of the DSS1 regulation range that appeared.
====

 .the Second:
====
The parameter correspondence table in code for DW8000 isn't suitable for DSS-1 mapping, so I had to compeared and refer to the https://glenstegner.com/dss1/home.html[service manual Korg DSS-1] and the https://gearspace.com/board/electronic-music-instruments-and-electronic-music-production/1418944-korg-dss-1-arduino-upgrade-project.html#:~:text=DSS%2D1_SysEx%20analytical%20table[DSS-1_SysEx excel table], which is available on the https://glenstegner.com/dss1/home.html[Glen Stegner]and this site author very deeply described all the possibilities in the work of DSS-1. Therefore, I also pay tribute and respect to Glen Stegner in his preparation of the site.
Including there are certain nuances when implementing Sisex commands, which I left for later and placed them in the last section <<nuancesand and troubleshooting>>:

image:IMAGES/COM_IMG/GlenSiegner.png[GlenStegner]

====

 .The Third:
====
- the parameters `DDL 1&2 Time` have control range from *0~500*, and it was necessary to create additional processing to transform the CC# range (0-127) into the expected one, implement 9-byte SysEx distribution over MSB LSB nibbles.
====

 .The Four:
====
- the parameters `DDL 1&2 Time` have control range from *0~500*, and it was necessary to create additional processing to transform the CC# range (0-127) into the expected one, implement 9-byte SysEx distribution over MSB LSB nibbles.
====
On this basis, a new table of correspondence between DDS-1 parameters and application of parameters to CC# control messages from the controller.

== Main block discribtion of coding text:
Unfortunately, I’m not a professional coder and apparently you noticed my lack of professionalism even earlier. Therefore, I describe the process of creating code as a self-taught person. having experience in coding in basic and fortran when computer centers were relevant and a personal computer was like a spaceship - unattainable. And the current Windows wasn't even 3.1

So, code distributed across 5 different units
for preparing and processing MIDI data arriving at the Rx Arduino input
SysEx byte preparation blocks:
[,cpp,%linenums]
----
 - Sysex_6 byte leight...[Unit_0]
 - Sysex_8 byte leight...[Unit_1]
 - Sysex_9 byte leight...[Unit_2]
 - Sysex_10 byte leight..[Unit_3]
 - Main CC# Mapping......[Unit_4]
 - setup & loop..........[Unit_5]
----

=== unrealized ideas for processing
At the time of publication of the code, there were unrealized ideas for processing the remaining SysEx commands located in the section [Unit_0] as:
[example%collapsible]
[,cpp,%linenums]
----
 Sysex hex[4]:    funcID inDec# [id Eqiualent] name		action		status:
	case 0x10: // functionID 16 [id10] Program Param	Request		in progress:
	case 0x11: // functionID 17 [id11] Write			Request		--
	case 0x12: // functionID 18 [id12] Mode				Request		done
	case 0x13: // functionID 19 [id13] Play mode 		Request		done
	case 0x14: // functionID 20 [id14] PCM data			Request		researching
	case 0x15: // functionID 21 [id15] MSound parameter	Request		--
	case 0x16: // functionID 22 [id16] MSound list		Request		in progress
	case 0x17: // functionID 23 [id17] Program nameList	Request		in progress
	case 0x40: // functionID 64 [id40] Program paramDump Tx/Rx		in progress
	case 0x41: // functionID 65 [id41] Program parameter Change		done
	case 0x42: // functionID 66 [id43] Mode data		Tx only		---
	case 0x43: // functionID 67 [id43] PCM data Dump	Tx/Rx		researching
	case 0x44: // functionID 68 [id44] MSound param Dump Tx/Rx		researching
	case 0x45: // functionID 69 [id45] MSound list		Tx/Rx		in progress
	case 0x46: // functionID 70 [id46] Program nameList	Tx only		in progress
----

=== Discribtion [Unit_0]
the first block provides preparation of the length Sysex_6 bytes for executing only request commands for modifying the Korg DSS-1 synthesizer without providing transformation of the value byte. This was implemented at the very last moment and represents the most interesting section for further development in case of interest from both the code supporter and the performer himself . Therefore, any help and cooperation is accepted!
Also in the interblock space there is a complete list of parameters of the DSS-1 program with hex and decimal data correspondence for ease of programming.
====
From my little programming experience, I can say that I used Pocket MIDI software to monitor MIDI messages and because of it I got into trouble when compiling the CC#2SysEx correspondence table due to combining data in decimal and hexadecimal formats in one IN / OUT monitor window, which led to a fatal error at the last stage of the redesign.
TIP when working with correspondence tables, use Excel tables and convert hex or DEC data into a single, readable format. but taking into account how SysEx and CC# will be presented when debugging with a MIDI monitor
====

Function ID [Fxx] - a convenient parameter when focusing on the built-in interface of Korg DSS-1, which is indicated on the surface and LCD of the synthesizer and was ignored by the developers in previous versions of the code and Excel tables from GlenStegner.

I’ll immediately voice some notes present in the code as an addition:

[#NOTE] within parentheses wouldn't work if use off/on SW

My Arduino project had limitations when designing the lookup table/template:
- *Nucleus2 SSL controller*
I was limited by the controller's capabilities due to its tight binding of CC# to such interface surface elements - as a result of which there a exception list upto 24 unused elements from 64 was formed, that unsuitable be mapping with the desired DSS-1 parameters due to their limitation of the on/off-state only. In a situation where more targes level of change are required (at least 4 or 16 levels)... Moreover, the SSL engineers decision was somehow strange to software exclude the central group of CC# (from 32 to 63) from the control list and continue further from 64 to 95 of CC#?!

and because of this limitation, I had to try to at least somehow use the SW buttons on program parameters that require more than 3-4 switching values. Because of this nuance, only the minimum and maximum values assigned to the parameter are switched by CC# on / off. such as:

`Osc MG ModSel	Off (Osc1, Osc2) Both`

where in brackets are the values that are unattainable when using the button 0-127

if You wish remapping CC#2SysEx use the program parameter number (but not param.Offset).
Refer: implementation chart [6]DSS-1 Programm parameter map, column "2", at pages 7-8:
https://dn790004.ca.archive.org/0/items/sm_DSS-1ServiceManual/DSS-1ServiceManual.pdf)

.Korg DSS-1 Parameter list (sorted by Function ID, then by paramNumber
[example%collapsible]
[,cpp,%linenums]
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

=== Discribtion [Unit_1]

kbd:[<<Back-to-Top>>]


[#MIDI Table]
= MIDI implementation charts

kbd:[<<Back-to-Top>>]

[#nuances and troubleshooting]
= Nuances and troubleshooting
on Glen Stegner's website there is a section on https://glenstegner.com/dss1/home.html[Tips and Trics], which describes very interesting things that are implemented by the manufacturer, but which are evaluated and interpretating by the musicians in two ways.
I would say - based on their preferences of each performer at Korg DSS.
  .here is one of them:

- parameters `OSC 1 Level` and `OSC 2 Level`: which are marked on the operating system of the synthesizer as *[F14]* as `OSC Mix ratio` or *X-fader* with manipulation by one fade from the synth surface.
This parameter is also described in the Program Parameter table [3] and has a *note2* below:
image:IMAGES/COM_IMG/LSB_MSB_data_format.jpeg

+
[NOTE]
====
_Must be set for both oscilator so that OSC1 + OSC2 = 100_
====

which is not mandatory for the performing musician. because you can manage the parameters separately via Sysex:

 - `Osc 1 level: [F0 42 30 0B 41 00 xx F7]`
 - `Osc 2 level: [F0 42 30 0B 41 01 xx F7]`

or 2nd way:

 - `Osc Mix ratio: [F0 42 30 0B 41 00 xx 00 yy F7]`

My original code version is implemented as 2 (see Unit_3)

[example%collapsible]
[,cpp,%linenums]
----
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
    sysexData[5] = paramNumber; // master must be (master+slave) == 100%:
    sysexData[6] = paramValueScaled & 0x7f; // master value
    sysexData[7] = (paramNumber +1); // slave
    sysexData[8] = (100 - paramValueScaled) & 0x7f; // slave value
    MIDI.sendSysEx(sysexLen, sysexData, true);
}
----

====
And this may be a controversial decision, since according to the reasoning on the https://glenstegner.com/dss1/home.html[Glen website]:
- he prefers an independent control option for the `OSC 1&2` parameter with the ability to raise the level of both oscillators up to 100%, while training the powerful OUTPUT from the synthesizer.
====
My reason was to create authenticity to
- the "DSS like" control method and
- the ability to save the fader control on the SSL Nucleus controller. Plus,
- the argument for *X-fader* control is that it is possible to achieve overload at the DAC output when playing particularly loud waves with distortion.
====
How to avoid this and whether this is considered an artistic decision by the performer is up to everyone to decide for themselves!
image:IMAGES/LSB_MSB data format.jpeg

kbd:[<<Back-to-Top>>]

