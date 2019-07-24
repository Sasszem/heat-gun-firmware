# heat-gun-firmware
Arduino (ATMEGA328P) firmware for ~my heat gun~ soldering station project

For the schematics, see https://easyeda.com/barath.laci10/heat-gun

A homebrew dual soldering station (soldering iron + soldering heat gun) with digital control.


Project goals:
 - 2 channel temperature control (thermocouple read & adjust output power via phase cutting)
 - 858 handle fan speed control & reed switch reading
 - simple to use interface using 16x2 alphanumeric LCD & rotary encoder
 - safe - no danger of electric shock
 - simple to build - 1 layered PCB to make DIY etching possible
 - cheap - at most half the prize of an already alaviable one
 - opensource - mega328p + arduino code
 
 Progress:
  - schematic is 85% done
     - zero crossing detector: 100%
     - main logic PCB: 85%
     - triac control PCBs: 20%
     - connected parts: 10%
  - code is 20% done 
     - menu: 90%
     - rotary: 100%
     - LCD: 100%
     - phase cutting: 0%
     - fan speed control: 90%
     - temperature reading: 0%
 - PCB desing is 10% done
     - ZCD module: 100%
     - main logic pcb: 0%
     - triac control: 0%
- HW is 10% built
     - ZCD module: 100%
     - main logic PCB: 0%
     - triac control PCBs: 0%
     - connected parts: 0%
     - case: 0%
     
Currently I am:
 - breadboard prototyping the circuit
 - writing the firmware
 - desigining the schematic
     
     
     
