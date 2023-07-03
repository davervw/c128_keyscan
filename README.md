## C128D to USB keyboard HID driver ##

This hardware and software allows a Commodore 128D keyboard to be used as an almost(*) standard USB keyboard. (*keys like braces, tilde, back quote, F9-F12 are missing)

    LINE FEED = NUM LOCK
    HELP = F1
    Numeric keypad can double as cursor keys, home/end, page up/down when NUM LOCK is toggled off
    C= = WINDOWS KEY
    STOP = PAUSE/BREAK
    NO SCROLL = SCROLL LOCK

Status: there are some multiple key presses (same row) issues.  Looks like a keyboard issue not pulling down low enough, too much resistance.  Adding another line driver circuit (74AHC125) on row inputs from DB25-9 and DB25-10 clear up some of the problems.  The row for DB25-9 includes the left shift with bottom row letters which are all working in combination except comma and slash keys not working with left shift, and intermitent issues with left shift cursor up.  The additional line driver for the row connected to DB-10 also corrected the Ctrl A key combination. Further investigation and corrections (clean that keyboard again?) should be possible with additional effort.  It would probably make a good blog post or video.

Previously shows latest scan code 0..88 plus Caps/Display/Restore to USB serial port via Arudino Pro Micro (3U4) and 74LS138.  Code in Arduino IDE.
(Note: previous version omits 74LS138 to only do 64 keys.)

DB-25 to Pro Micro wiring instructions are in source code.  74LS138 wiring instructions can be inferred from source code and searching for datasheet.

![prototype2.jpg](prototype2.jpg)

![serialout.jpg](serialout.jpg)

[Reference C128 Keyboard @ zimmers](http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c128/servicemanuals/manual/51.gif)
