// C128Keyboard.h
//
// C128 keyboard scan code to USB keyboard HID adapter
// for Arduino Pro Micro (Leonardo/32U4)
//
// MIT LICENSE
// c128_keyscan
// Copyright 2023 by David Van Wagner dave@davevw.com
// www.github.com/davervw www.twitter.com/davervw
//
// Permission is hereby granted, free of charge, to any person 
// obtaining a copy of this software and associated documentation files 
// (the “Software”), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//

//----------------------------------------------------------------------------

// TODO: port to other platforms such as Teensy and ESP32

// wiring is DB25 to Arduino Pro Micro pins

#include <Keyboard.h>
#include "C128Keyboard.h"

// PORTA = C64/128 DC00
static const int PORTA0PIN=0; // A0: DB25-13
static const int PORTA1PIN=1; // A1: DB25-19
static const int PORTA2PIN=14; // A2: DB25-18
static const int PORTA3PIN=6; // A3: DB25-17
static const int PORTA4PIN=5; // A4: DB25-16
static const int PORTA5PIN=7; // A5: DB25-15
static const int PORTA6PIN=9; // A6: DB25-14
static const int PORTA7PIN=10; // A7: DB25-20

static const int EXT0PIN=11; // extended keyboard scan register D02F output bit 0 DB25-21
static const int EXT1PIN=12; // extended keyboard scan register D02F output bit 1 DB25-22
static const int EXT2PIN=13; // extended keyboard scan register D02F output bit 2 DB25-23

// PORTB = C88 DC01
static const int PORTB0PIN=A0; // DB25-12
static const int PORTB1PIN=A1; // DB25-11
static const int PORTB2PIN=A2; // DB25-10
static const int PORTB3PIN=A3; // DB25-5
static const int PORTB4PIN=A4; // DB25-8
static const int PORTB5PIN=A5; // DB25-7
static const int PORTB6PIN=15; // DB25-6
static const int PORTB7PIN=16; // DB25-9

// DB25-1 to GND
static const int NMIPIN=4; // DB25-3
static const int DISPLAY4080PIN=3; // DB25-24
static const int CAPSLOCKPIN=8; // DB25-25

// no connection DB25-2, DB25-4

static int last_scan = 88;
static int last_nmi = 0;
static int last_caps = 0;
static int last_disp = 0;
static bool old_pressed[88];
static bool new_pressed[88];

static unsigned char C128Keyboard::keyboard_map[2][88] = {
  {
  KEY_BACKSPACE, // 0
  KEY_RETURN, // 1
  KEY_RIGHT_ARROW, // 2
  KEY_F7, // 3
  KEY_F1, // 4
  KEY_F3, // 5
  KEY_F5, // 6
  KEY_DOWN_ARROW, // 7
  '3', // 8
  'w', // 9
  'a', // 10
  '4', // 11
  'z', // 12
  's', // 13
  'e', // 14
  KEY_LEFT_SHIFT, // 15
  '5', // 16
  'r', // 17
  'd', // 18
  '6', // 19
  'c', // 20
  'f', // 21
  't', // 22
  'x', // 23
  '7', // 24
  'y', // 25
  'g', // 26
  '8', // 27
  'b', // 28
  'h', // 29
  'u', // 30
  'v', // 31
  '9', // 32
  'i', // 33
  'j', // 34
  '0', // 35
  'm', // 36
  'k', // 37
  'o', // 38
  'n', // 39
  '+', // 40
  'p', // 41
  'l', // 42
  '-', // 43
  '.', // 44
  ':', // 45
  '@', // 46
  ',', // 47
  '\\', // 48
  '*', // 49
  ';', // 50
  KEY_HOME, // 51
  KEY_RIGHT_SHIFT, // 52
  '=', // 53
  '^', // 54
  '/', // 55
  '1', // 56
  '_', // 57
  KEY_LEFT_CTRL, // 58
  '2', // 59
  ' ', // 60
  KEY_LEFT_GUI, // 61
  'q', // 62
  KEY_PAUSE, // 63
  KEY_F1, // HELP 64
  KEY_KP_8, // 65
  KEY_KP_5, // 66
  KEY_TAB, // 67
  KEY_KP_2, // 68
  KEY_KP_4, // 69
  KEY_KP_7, // 70
  KEY_KP_1, // 71
  KEY_ESC, // 72
  KEY_KP_PLUS, // 73
  KEY_KP_MINUS, // 74
  KEY_NUM_LOCK, // LINE FEED 75
  KEY_KP_ENTER, // 76
  KEY_KP_6, // 77
  KEY_KP_9, // 78
  KEY_KP_3, // 79
  KEY_LEFT_ALT, // 80
  KEY_KP_0, // 81
  KEY_KP_DOT, // 82
  KEY_UP_ARROW, // 83
  KEY_DOWN_ARROW, // 84
  KEY_LEFT_ARROW, // 85
  KEY_RIGHT_ARROW, // 86
  KEY_SCROLL_LOCK, // 87
  },
  {
  KEY_INSERT, // 0
  KEY_RETURN, // 1
  KEY_LEFT_ARROW, // 2
  KEY_F8, // 3
  KEY_F2, // 4
  KEY_F4, // 5
  KEY_F6, // 6
  KEY_UP_ARROW, // 7 
  '#', // 8
  'W', // 9
  'A', // 10
  '$', // 11
  'Z', // 12
  'S', // 13
  'E', // 14
  KEY_LEFT_SHIFT, // 15
  '%', // 16
  'R', // 17
  'D', // 18
  '&', // 19
  'C', // 20
  'F', // 21
  'T', // 22
  'X', // 23
  '\'', // 24
  'Y', // 25
  'G', // 26
  '(', // 27
  'B', // 28
  'H', // 29
  'U', // 30
  'V', // 31
  ')', // 32
  'I', // 33
  'J', // 34
  0, // 0 35
  'M', // 36
  'K', // 37
  'O', // 38
  'N', // 39
  '+', // 40
  'P', // 41
  'L', // 42
  '-', // 43
  '>', // 44
  '[', // 45
  '@', // 46
  '<', // 47
  '\\', // 48
  '*', // 49
  ']', // 50
  KEY_HOME, // 51
  KEY_RIGHT_SHIFT, // 52
  '=', // 53
  '^', // 54
  '/', // 55
  '!', // 56
  '_', // 57
  KEY_LEFT_CTRL, // 58
  '"', // 59
  ' ', // 60
  KEY_LEFT_GUI, // 61
  'Q', // 62
  KEY_PAUSE, // 63
  KEY_F1, // HELP 64
  KEY_KP_8, // 65
  KEY_KP_5, // 66
  KEY_TAB, // 67
  KEY_KP_2, // 68
  KEY_KP_4, // 69
  KEY_KP_7, // 70
  KEY_KP_1, // 71
  KEY_ESC, // 72
  KEY_KP_PLUS, // 73
  KEY_KP_MINUS, // 74
  KEY_NUM_LOCK, // LINE FEED 75
  KEY_KP_ENTER, // 76
  KEY_KP_6, // 77
  KEY_KP_9, // 78
  KEY_KP_3, // 79
  KEY_LEFT_ALT, // 80
  KEY_KP_0, // 81     
  KEY_KP_DOT, // 82
  KEY_UP_ARROW, // 83
  KEY_DOWN_ARROW, // 84
  KEY_LEFT_ARROW, // 85
  KEY_RIGHT_ARROW, // 86
  KEY_SCROLL_LOCK, // 87
  },
};

C128Keyboard::C128Keyboard() {
  Keyboard.begin();

  pinMode(NMIPIN, INPUT_PULLUP);
  pinMode(DISPLAY4080PIN, INPUT_PULLUP);
  pinMode(CAPSLOCKPIN, INPUT_PULLUP);
  pinMode(PORTB0PIN, INPUT_PULLUP);
  pinMode(PORTB1PIN, INPUT_PULLUP);
  pinMode(PORTB2PIN, INPUT_PULLUP);
  pinMode(PORTB3PIN, INPUT_PULLUP);
  pinMode(PORTB4PIN, INPUT_PULLUP);
  pinMode(PORTB5PIN, INPUT_PULLUP);
  pinMode(PORTB6PIN, INPUT_PULLUP);
  pinMode(PORTB7PIN, INPUT_PULLUP);
  pinMode(PORTA0PIN, INPUT); // HI-Z
  pinMode(PORTA1PIN, INPUT); // HI-Z
  pinMode(PORTA2PIN, INPUT); // HI-Z
  pinMode(PORTA3PIN, INPUT); // HI-Z
  pinMode(PORTA4PIN, INPUT); // HI-Z
  pinMode(PORTA5PIN, INPUT); // HI-Z
  pinMode(PORTA6PIN, INPUT); // HI-Z
  pinMode(PORTA7PIN, INPUT); // HI-Z
}

void C128Keyboard::poll()
{
  scanKeys();
  sendHIDKeys();
}

int C128Keyboard::scanKeys()
{
  int scan_code = 88;
  int new_scan_code = 0;

  int scan_out;
  for (scan_out = 0; scan_out < 11; ++scan_out)
  {
    // write one column of keyboard matrix LOW
    int output_pin = -1;
    switch (scan_out) 
    {
      case 0: output_pin = PORTA0PIN; break;
      case 1: output_pin = PORTA1PIN; break;
      case 2: output_pin = PORTA2PIN; break;
      case 3: output_pin = PORTA3PIN; break;
      case 4: output_pin = PORTA4PIN; break;
      case 5: output_pin = PORTA5PIN; break;
      case 6: output_pin = PORTA6PIN; break;
      case 7: output_pin = PORTA7PIN; break;
      case 8: output_pin = EXT0PIN; break;
      case 9: output_pin = EXT1PIN; break;
      case 10: output_pin = EXT2PIN; break;
    }
    pinMode(output_pin, OUTPUT);
    digitalWrite(output_pin, LOW);

    // read in eight bits, 1 bit of each row
    int scan_in = (digitalRead(PORTB0PIN) << 0) 
                | (digitalRead(PORTB1PIN) << 1) 
                | (digitalRead(PORTB2PIN) << 2) 
                | (digitalRead(PORTB3PIN) << 3) 
                | (digitalRead(PORTB4PIN) << 4) 
                | (digitalRead(PORTB5PIN) << 5) 
                | (digitalRead(PORTB6PIN) << 6) 
                | (digitalRead(PORTB7PIN) << 7);

    pinMode(output_pin, INPUT); // HI-Z

    int i;
    for (i=0; i<=7; ++i) {
      int pressed = ((scan_in & (1 << i)) == 0);
      new_pressed[new_scan_code] = pressed;
      int first_pressed = (pressed && scan_code == 88);
      if (first_pressed) 
        scan_code = new_scan_code;
      ++new_scan_code;
    }
  }

  // if (scan_code != last_scan) {
  //   last_scan = scan_code;
  //   char s[20];
  //   sprintf(s, "scan: %d\n", scan_code);
  //   Serial.write(s);
  // }
  
  return scan_code;
}

void C128Keyboard::sendHIDKeys()
{
  int nmi = ~digitalRead(NMIPIN) & 1;
  int caps = ~digitalRead(CAPSLOCKPIN) & 1;
  int disp = ~digitalRead(DISPLAY4080PIN) & 1;

  if (nmi != last_nmi) {
    // char s[20];
    // sprintf(s, "RESTORE/NMI: %d\n", nmi);
    // Serial.write(s);
    last_nmi = nmi;
  }

  if (caps != last_caps) {
    // char s[15];
    // sprintf(s, "CAPS: %d\n", caps);
    // Serial.write(s);
    Keyboard.press(KEY_CAPS_LOCK);
    Keyboard.release(KEY_CAPS_LOCK);
    last_caps = caps;
  }

  if (disp != last_disp) {
    // char s[15];
    // sprintf(s, "DISP: %d\n", disp);
    // Serial.write(s);
    Keyboard.press(KEY_NUM_LOCK);
    Keyboard.release(KEY_NUM_LOCK);
    last_disp = disp;
  }

  int i;
  int shift = new_pressed[15] || new_pressed[52]; // LSHIFT or RSHIFT
  
  checkSuppressShift(shift);

  checkChange(shift, 15); // LSHIFT
  checkChange(shift, 52); // RSHIFT

  // if (memcmp(new_pressed, old_pressed, 88) != 0) {
  //   for (i = 0; i < 88; ++i)
  //     Serial.write('0' + new_pressed[i]);
  //   Serial.write('\n');
  // }

  for (i = 0; i < 88; ++i) {
    if (i==15 || i==52) // already handled
      continue;
    checkChange(shift, i);
  }

  memcpy(old_pressed, new_pressed, 88);  
}

bool C128Keyboard::isChanged(int scan_code)
{
  return (old_pressed[scan_code] != new_pressed[scan_code]);
}

void C128Keyboard::checkChange(int shift, int scan_code)
{
  if (!isChanged(scan_code))
    return;
  int hid_key = keyboard_map[shift][scan_code];
  if (new_pressed[scan_code])
    Keyboard.press(hid_key);
  else
    Keyboard.release(hid_key);
}

void C128Keyboard::checkSuppressShift(int shift)
{
  static bool last_suppress = false;
  if (mustSuppressShift(shift)) {
    if (!last_suppress) {
      checkToggleKey(0, 2);    
      checkToggleKey(0, 7);
      checkToggleKey(0, 45);    
      checkToggleKey(0, 50);
      checkToggleKey(0, 24);
    }

    // suppress state change this time
    new_pressed[15] = 0;
    new_pressed[52] = 0;

    last_suppress = true;
  } 
  else if (last_suppress) {
    checkToggleKey(1, 2);    
    checkToggleKey(1, 7);    
    checkToggleKey(1, 45);    
    checkToggleKey(1, 50);
    checkToggleKey(1, 24);
    last_suppress = false;
  }
}

bool C128Keyboard::mustSuppressShift(int shift)
{
  if (!shift)
    return false;
  return new_pressed[2] // LEFT CURSOR
    || new_pressed[7] // UP CURSOR
    || new_pressed[45] // [
    || new_pressed[50] // ]
    || new_pressed[24]; // ' single quote
}

void C128Keyboard::checkToggleKey(int shift, int scan_code)
{
    // check for switching from UP to DOWN, or LEFT to RIGHT, or vice versa on same key
    if (new_pressed[scan_code] && old_pressed[scan_code]) {
      Keyboard.release(keyboard_map[shift][scan_code]);
      Keyboard.press(keyboard_map[1-shift][scan_code]);
    }
}
