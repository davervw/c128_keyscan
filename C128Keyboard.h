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

class C128Keyboard {

public:
C128Keyboard();
void poll();

private:
static unsigned char C128Keyboard::keyboard_map[2][88];
int scanKeys();
void sendHIDKeys();
bool C128Keyboard::isChanged(int scan_code);
void checkChange(int shift, int scan_code);
bool mustSuppressShift(int shift);
void checkSuppressShift(int shift);
void checkToggleKey(int shift, int scan_code);

C128Keyboard(const C128Keyboard&); // disallow, not implemented
};
