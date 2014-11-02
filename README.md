SX-K250
=======

Hacked my old Technics SX-K250 keyboard (from ~1984 (!))

Removed all electical hardware except 
  - power supply to provide 12V voltage for Arduino
  - board holding the user interface hardware (buttons, LEDs, slider)
  - board with claviature encoding chip
  
Added Arduino and custom board.

Claviature is separated into eight groups with six keys and one single key. 
Group can be activated with a 3Bit code. The state of the any key in the 
active group can then be asked.
So groups are time multiplexed whereas in a group all six keys can be asked
simultaneously.

Four slides can be requested. State of one slider can be expressed by five
LEDs.

A MIDI interface is implemented by the serial output port of the Arduino.

Code can be found here as well as some circuit diagrams.
