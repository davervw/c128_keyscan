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
// TODO: manage multiple key presses
// TODO: manage state changes (keydown, keyup)
// TODO: HID keyboard driver
// TODO: port to Teensy and ESP32

// wiring is DB25 to Arduino Pro Micro pins
// and 74LS138N needed to expand bus pins

const int MUXA0PIN=9; // 3-to-8 MUX address 0 (74LS138) to PORTA0..A7 (see DB25 pins A0..A7)
const int MUXA1PIN=8; // 3-to-8 MUX address 1 (74LS138) to PORTA0..A7 (see DB25 pins A0..A7)
const int MUXA2PIN=7; // 3-to-8 MUX address 2 (74LS138) to PORTA0..A7 (see DB25 pins A0..A7)
const int MUXE3PIN=6; // 3-to-8 MUX enable high
// PORTA = C64/128 DC00
// A0: DB25-13
// A1: DB25-19
// A2: DB25-18
// A3: DB25-17
// A4: DB25-16
// A5: DB25-15
// A6: DB25-14
// A7: DB25-20

const int EXT0PIN=5; // extended keyboard scan register D02F output bit 0 DB25-21
const int EXT1PIN=4; // extended keyboard scan register D02F output bit 1 DB25-22
const int EXT2PIN=3; // extended keyboard scan register D02F output bit 2 DB25-23

// PORTB = C88 DC01
const int PORTB0PIN=A1; // DB25-12
const int PORTB1PIN=A3; // DB25-11
const int PORTB2PIN=A2; // DB25-10
const int PORTB3PIN=14; // DB25-5
const int PORTB4PIN=16; // DB25-8
const int PORTB5PIN=10; // DB25-7
const int PORTB6PIN=15; // DB25-6
const int PORTB7PIN=A0; // DB25-9

// DB25-1 to GND
const int NMIPIN=0; // DB25-3
const int DISPLAY4080PIN=1; // DB25-4
const int CAPSLOCKPIN=2; // DB25-25

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(15200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

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
  pinMode(MUXE3PIN, OUTPUT);
  digitalWrite(MUXE3PIN, LOW); // enable off
  pinMode(MUXA0PIN, OUTPUT);
  pinMode(MUXA1PIN, OUTPUT);
  pinMode(MUXA2PIN, OUTPUT);
  pinMode(EXT0PIN, OUTPUT);
  pinMode(EXT1PIN, OUTPUT);
  pinMode(EXT2PIN, OUTPUT);
  digitalWrite(EXT0PIN, HIGH);
  digitalWrite(EXT1PIN, HIGH);
  digitalWrite(EXT2PIN, HIGH);
}

static int last_scan = 88;
static int last_nmi = 0;
static int last_caps = 0;
static int last_disp = 0;

void loop() {
  int scan_code = 88;

  int scan_out;
  for (scan_out = 0; scan_out < 11; ++scan_out)
  {
    digitalWrite(MUXE3PIN, LOW); // enable off

    digitalWrite(MUXA0PIN, scan_out < 8 && (scan_out & 1) ? HIGH : LOW);
    digitalWrite(MUXA1PIN, scan_out < 8 && (scan_out & 2) ? HIGH : LOW);
    digitalWrite(MUXA2PIN, scan_out < 8 && (scan_out & 4) ? HIGH : LOW);
    digitalWrite(EXT0PIN, scan_out == 8 ? LOW : HIGH);
    digitalWrite(EXT1PIN, scan_out == 9 ? LOW : HIGH);
    digitalWrite(EXT2PIN, scan_out == 10 ? LOW : HIGH);

    digitalWrite(MUXE3PIN, HIGH); // enable off

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
      if ((scan_in & (1 << i)) == 0 && scan_code == 88)
        scan_code = new_scan_code;
    }
  }

  if (scan_code != last_scan) {
    last_scan = scan_code;
    char s[15];
    sprintf(s, "%d\n", scan_code);
    Serial.write(s);
  }

  int nmi = ~digitalRead(NMIPIN) & 1;
  int caps = ~digitalRead(CAPSLOCKPIN) & 1;
  int disp = ~digitalRead(DISPLAY4080PIN) & 1;

  if (nmi != last_nmi) {
    char s[20];
    sprintf(s, "RESTORE/NMI: %d\n", nmi);
    Serial.write(s);
    last_nmi = nmi;
  }

  if (caps != last_caps) {
    char s[15];
    sprintf(s, "CAPS: %d\n", caps);
    Serial.write(s);
    last_caps = caps;
  }

  if (disp != last_disp) {
    char s[15];
    sprintf(s, "DISP: %d\n", disp);
    Serial.write(s);
    last_disp = disp;
  }
}
