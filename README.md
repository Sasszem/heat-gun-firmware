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
 - opensource - mega328p + arduino code -> repairable
 
 Progress:
 - thing is built, firmware is working. Temperature control is working, albeit kinda shitty (simple on-off control), so improvements are possible.
 
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

## Heater ON

There are some inputs that control (enable or disable) the heating element:
- reed switch in handle
- swtich on front panel (or maybe a menu item?)

Heater is ON (controlled by feedback loop) if and only if reed detects picked up AND switch is on.
