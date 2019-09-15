# heat-gun-firmware
A homebrew dual soldering station (soldering heat gun) with digital control.

For the schematics, see https://easyeda.com/barath.laci10/heat-gun

**Note: I am desigining HW AC parts with 240V@Hz in mind. Adjust them to yor needs if necessary!**


Project goals:
 - temperature control (thermocouple read & adjust output power via phase cutting)
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
 - writing this readme
 - doing a thousand other stuff, so expect slow progress!
 
# Modules:

## Main MCU(Mega328):
- xtal + 2 caps
- IC
- resistor pullup on reset
- (firmware)
- ISP header

## LCD
- LCD
- contrast pot
- current limit resistor

## Rotary switch 
- rotary encoder

## Fan speed control
- IRFZ44 fet
- 1N4007 as flyback
- gate pulldown resistor

## Temperature control

### Temperature reading
- MAX6675 breakout board via SPI

### Triac control
- header to external board - 1 pin + gnd
- external board

### ZCD (zero crossing detector)
- header to external board - 2 pin + gnd
- external board

## Reed switch
- header - 1 pin + gnd
