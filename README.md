# C64/C128 to USB keyboard HID driver #

## Simple 32u4 Pro Micro version - C128 extra keys not supported. ##

This hardware and software allows a Commodore 64 or Commodore 128 keyboard to be used as a USB HID keyboard (minus keys not present on C64).

Status: Working 64 key solution.  Multiple keys pressed on same row issue has been resolved (only one column output must be active at one time, otherwise change column to a HIGH-Z input to avoid low and high outputs to compete). 

    RESTORE key mapping not included in this iteration.

    WARNING!!! wiring has changed since photo was taken. 
    
DB-25 to Pro Micro wiring instructions are in source code.  

![prototype.jpg](prototype.jpg)

![serialout.jpg](serialout.jpg)

[Reference C128 Keyboard @ zimmers](http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c128/servicemanuals/manual/51.gif)
