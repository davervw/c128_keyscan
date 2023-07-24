# C64/C128 to USB keyboard HID driver #

## [Adafruit ItsyBitsy](https://learn.adafruit.com/introducting-itsy-bitsy-32u4/) version ##

This hardware and software allows a Commodore 64 or Commodore 128 keyboard to be used as a USB HID keyboard.  The use of ItsyBitsy allowed all the extra lines necessary to support the extra keys added for C128.  ItsyBitsy is an alternate form factor related to [Arduino Leonardo](https://docs.arduino.cc/hardware/leonardo) with all the extra lines of the 32U4 present.  The [ATmega32U4](https://www.microchip.com/en-us/product/atmega32u4) has the ability to directly act as a USB HID device in conjuction with the Arduino keyboard library. 

Status: Working 88+ key solution. Multiple keys pressed on same row issue has been resolved (only one column output must be active at one time, otherwise change column to a HIGH-Z input to avoid competing low and high outputs). 

Utilizing a DB-25 to standard 0.1" dual row pin socket adapter for the PCB

    RESTORE key mapping not included in this iteration.

    WARNING!!! wiring has changed between various commits. Always review DB-25 to Pro Micro wiring instructions which are in source code comments.  

![pcb.jpg](pcb.jpg)

![prototype.jpg](prototype.jpg)

Links

* [Reference C128 Keyboard @ zimmers](http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c128/servicemanuals/manual/51.gif)
* [Blog: Extremely small emulated C64](https://techwithdave.davevw.com/2023/06/extremely-small-emulated-c64.html)
