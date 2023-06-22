// C128 keyboard scan code driver
//
// for Arduino Pro Micro (Leonardo/32U4)
// displays scan code to USB Serial port
//
// MIT LICENSE
// C128 keyboard scan code driver
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
// TODO: wire RESTORE pin switch DB25-3, DB25-1 GND
// TODO: wire additional pins for C128 extra keys
//   DB25-21 D02F scan output bit 0
//   DB25-22 D02F scan output bit 1
//   DB25-23 D02F scan output bit 2
//   DB25-24 40/80 display
//   DB25-25 CAPS LOCK
//   DB25-2, DB25-4 No Connection
// TODO: translate to HID keys
// TODO: show multiple key presses
// TODO: show state changes (keydown, keyup)

// wiring is DB25 to Arduino Pro Micro pins
// PORTA = C64 DC00
const int PORTA0PIN=9; // DB25-13
const int PORTA1PIN=8; // DB25-19
const int PORTA2PIN=7; // DB25-18
const int PORTA3PIN=6; // DB25-17
const int PORTA4PIN=5; // DB25-16
const int PORTA5PIN=4; // DB25-15
const int PORTA6PIN=3; // DB25-14
const int PORTA7PIN=2; // DB25-20

// PORTB = C64 DC01
const int PORTB0PIN=A1; // DB25-12
const int PORTB1PIN=A3; // DB25-11
const int PORTB2PIN=A2; // DB25-10
const int PORTB3PIN=14; // DB25-5
const int PORTB4PIN=16; // DB25-8
const int PORTB5PIN=10; // DB25-7
const int PORTB6PIN=15; // DB25-6
const int PORTB7PIN=A0; // DB25-9

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(15200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(PORTA0PIN, OUTPUT);
  pinMode(PORTA1PIN, OUTPUT);
  pinMode(PORTA2PIN, OUTPUT);
  pinMode(PORTA3PIN, OUTPUT);
  pinMode(PORTA4PIN, OUTPUT);
  pinMode(PORTA5PIN, OUTPUT);
  pinMode(PORTA6PIN, OUTPUT);
  pinMode(PORTA7PIN, OUTPUT);
  digitalWrite(PORTA0PIN, HIGH); // enable off
  digitalWrite(PORTA1PIN, HIGH); // enable off
  digitalWrite(PORTA2PIN, HIGH); // enable off
  digitalWrite(PORTA3PIN, HIGH); // enable off
  digitalWrite(PORTA4PIN, HIGH); // enable off
  digitalWrite(PORTA5PIN, HIGH); // enable off
  digitalWrite(PORTA6PIN, HIGH); // enable off
  digitalWrite(PORTA7PIN, HIGH); // enable off
  pinMode(PORTB0PIN, INPUT_PULLUP);
  pinMode(PORTB1PIN, INPUT_PULLUP);
  pinMode(PORTB2PIN, INPUT_PULLUP);
  pinMode(PORTB3PIN, INPUT_PULLUP);
  pinMode(PORTB4PIN, INPUT_PULLUP);
  pinMode(PORTB5PIN, INPUT_PULLUP);
  pinMode(PORTB6PIN, INPUT_PULLUP);
  pinMode(PORTB7PIN, INPUT_PULLUP);
}

static int last_scan = 64;

void loop() {
  int scan_code = 64;

  int scan_out;
  for (scan_out = 0; scan_out < 8; ++scan_out)
  {
    // turn off all buffer enables
    digitalWrite(PORTA0PIN, HIGH);
    digitalWrite(PORTA1PIN, HIGH);
    digitalWrite(PORTA2PIN, HIGH);
    digitalWrite(PORTA3PIN, HIGH);
    digitalWrite(PORTA4PIN, HIGH);
    digitalWrite(PORTA5PIN, HIGH);
    digitalWrite(PORTA6PIN, HIGH);
    digitalWrite(PORTA7PIN, HIGH);

    // enable only one
    switch(scan_out) {
      case 0: digitalWrite(PORTA0PIN, LOW); break;
      case 1: digitalWrite(PORTA1PIN, LOW); break;
      case 2: digitalWrite(PORTA2PIN, LOW); break;
      case 3: digitalWrite(PORTA3PIN, LOW); break;
      case 4: digitalWrite(PORTA4PIN, LOW); break;
      case 5: digitalWrite(PORTA5PIN, LOW); break;
      case 6: digitalWrite(PORTA6PIN, LOW); break;
      case 7: digitalWrite(PORTA7PIN, LOW); break;
      default: Serial.write("internal error");
    }

    // read the bus lines
    int scan0 = digitalRead(PORTB0PIN) & 1;
    int scan1 = digitalRead(PORTB1PIN) & 1;
    int scan2 = digitalRead(PORTB2PIN) & 1;
    int scan3 = digitalRead(PORTB3PIN) & 1;
    int scan4 = digitalRead(PORTB4PIN) & 1;
    int scan5 = digitalRead(PORTB5PIN) & 1;
    int scan6 = digitalRead(PORTB6PIN) & 1;
    int scan7 = digitalRead(PORTB7PIN) & 1;

    int scan_in = (scan0 << 0) 
                | (scan1 << 1) 
                | (scan2 << 2) 
                | (scan3 << 3) 
                | (scan4 << 4) 
                | (scan5 << 5) 
                | (scan6 << 6) 
                | (scan7 << 7);

    int i;
    for (i=0; i<=7; ++i) {
      int new_scan_code = scan_out*8 + i;
      if ((scan_in & (1 << i)) == 0 && scan_code == 64)
        scan_code = new_scan_code;
    }
  }

  if (scan_code != last_scan) {
    last_scan = scan_code;
    char s[15];
    sprintf(s, "%d\n", scan_code);
    Serial.write(s);
  }
}
