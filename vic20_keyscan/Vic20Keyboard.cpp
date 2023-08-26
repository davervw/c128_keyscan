// Vic20Keyboard.h
//
// Vic20 keyboard scan code to USB keyboard HID adapter
// for Arduino Pro Micro (Leonardo/32U4)
//
// MIT LICENSE
// Vic20_keyscan
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

// wiring is DB25 to Adafruit ItsyBitsy (small factor Arduino Leonardo clone)

//#include <Keyboard.h>
#include "Vic20Keyboard.h"

// PORTB = VIC-20 input
static const int PORTB0PIN=A5; // A7: KBD20
static const int PORTB1PIN=A4; // A1: KBD19
static const int PORTB2PIN=A0; // A2: KBD18
static const int PORTB3PIN=A3; // A3: KBD17
static const int PORTB4PIN=A2; // A4: KBD16
static const int PORTB5PIN=A1; // A5: KBD15
static const int PORTB6PIN=12; // A6: KBD14
static const int PORTB7PIN=0; // A0: KBD13

// PORTA = VIC-20 output
static const int PORTA0PIN=1; // KBD12
static const int PORTA1PIN=6; // KBD11
static const int PORTA2PIN=4; // KBD10
static const int PORTA3PIN=5; // KBD9
static const int PORTA4PIN=7; // KBD8
static const int PORTA5PIN=9; // KBD7
static const int PORTA6PIN=10; // KBD6
static const int PORTA7PIN=11; // KBD5

// KBD1 to GND
static const int NMIPIN=13; // DB25-3
// no connection KBD2, KBD4

static int last_scan = 64;
static int last_nmi = 0;
static bool old_pressed[64];
static bool new_pressed[64];

Vic20Keyboard::Vic20Keyboard() {
  pinMode(NMIPIN, INPUT_PULLUP);
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

void Vic20Keyboard::poll()
{
  scanKeys();

  int nmi = ~digitalRead(NMIPIN) & 1;

  if (memcmp(old_pressed, new_pressed, 64) != 0 || last_nmi != nmi) {
    last_nmi = nmi;
    sendKeys();
    memcpy(old_pressed, new_pressed, 64);
  }
}

int Vic20Keyboard::scanKeys()
{
  int scan_code = 64;
  int new_scan_code = 0;

  int scan_out;
  for (scan_out = 0; scan_out < 8; ++scan_out)
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
      int first_pressed = (pressed && scan_code == 64);
      if (first_pressed) 
        scan_code = new_scan_code;
      ++new_scan_code;
    }
  }

  return scan_code;
}

void Vic20Keyboard::sendKeys()
{
  static char keyString[128];
  static char num[6];

  keyString[0] = 0;

  int i;
  for (i=0; i<64; ++i) {
    if (new_pressed[i]) {
      if (strlen(keyString) > 0)
        strncat(keyString, ",", sizeof(keyString));
      strncat(keyString, itoa(i, num, 10), sizeof(keyString));
    }
  } 

  if (last_nmi == 1) {
    if (strlen(keyString) > 0)
      strncat(keyString, ",", sizeof(keyString));
    strncat(keyString, "1024", sizeof(keyString));
  }

  if (keyString[0] == 0)
    Serial.println("64");
  else
    Serial.println(keyString);
}
