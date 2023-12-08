[#Back-to-Top""]
= # Arduino2Korg_DSS1 MIDI translator
:toc:
:toclevels: 3
:doctype: book
:sectnums:
:partnums:
:sectnumlevels: 3
:experimental:
:tip-caption: pass:[&#128161;]
:warning-caption: pass:[&#9888;]
:note-caption: pass:[&#128204;]
:caution-caption: pass:[&#8252;]

// Reusable text snippets
:MIDI_preset_content: There are many Arduino chipping controllers projects for synth manipulation. And although technically possible in exactly the same way as with controller presets for factory MIDI devices, Arduino map layouts are very custom, so I'm no sure if it would make much sense to create presets of map. Time will show...

|===
|Last update of text: |`2023-12-08 (v0.01.0)`
|Last update of relevant screenshots: |`2023-12-08 (v0.01.0)`
|===

kbd:[<<Back-to-Top>>]

= Quick start

Here's a step-by-step guide to help you get started with Arduino and a MIDI controller:

. Download & Start Arduino IDE.
. If you haven't already done it, https://www.arduino.cc/en/software[install Arduino].
. Make sure your MIDI_library.h is enabled in _Sketch → Include Library… → Manage Librarys ⇧⌘i →  download the MIDI_library.h if you need...
  * For the Arduino board device, tick _for your name device_ and tick
 your _Port for connection_.

Simple Arduino project to remap MIDI CC# messages into SysEx messages that Korg DSS-1 Synth expects.
This is project start from 5th July 2016 by point what I done with Korg EX8000 based on a hack “Korg DW8000 midi enhancer” by Steve Baines
The Korg DW-8000 (and other early synthesisers) was created during the early days of MIDI, before things like CC messages were standardised.  As a result, it uses proprietary SYSEX commands to change parameters over MIDI.  Most modern MIDI controllers use CC messages instead, and very few are flexible enough to be able to send the appropriate SYSEX messages.

This project uses an embedded Arduino Nano (or clone) mounted inside the DW-8000, which is used to intercept the MIDI IN data just before it reaches the DW-8000 CPU. It replaces specific CC messages with SYSEX messages that the DW-8000 understands them.

The practical upshot of this is that the DW-8000 parameters can be controlled by standard CC messages, rather than obscure SYSEX messages.

MIDI THRU is unaffected by the changes, as the Nano is be spliced in just before the MIDI IN reaches the CPU, AFTER the IN has been echoed back out of the THRU port.

Certain CC messages are replaced with SYSEX parameter control messages, but all other MIDI messages are passed unchanged. This means that the SYSEX messages can still be used.

For full project details and step by step instructions:
https://hackaday.io/project/12541-korg-dw-8000-midi-enhancer
