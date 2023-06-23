## C128D to USB driver prototype ##

HID Keyboard driver prototype implemented, with some multiple key presses (same row) issues.  Maybe an electrical pull-up or sink current issue?

Previously shows latest scan code 0..88 plus Caps/Display/Restore to USB serial port via Arudino Pro Micro (3U4) and 74LS138.  Code in Arduino IDE.
(Note: previous version omits 74LS138 to only do 64 keys.)

DB-25 to Pro Micro wiring instructions are in source code.  74LS138 wiring instructions can be inferred from source code and searching for datasheet.

![prototype2.jpg](prototype2.jpg)

![serialout.jpg](serialout.jpg)

[Reference C128 Keyboard @ zimmers](http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c128/servicemanuals/manual/51.gif)