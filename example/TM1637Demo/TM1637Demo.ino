/*
 * TM1637 Demonstration
 * Using : TM1637UniDevice library
 * 4 x 7 segments display module HW-069
 * pin D2 Clock and pin D3 DataIO
 *
 * DISCLAIMER OF ALL WARRANTIES
 *
 */
#include <TM1637UniDevice.h>

TM1637Device display; // New 7 seg display

void setup() {
  // Init TM1637 Display : CLK = pin 2 / DIO = pin 3
  TM1637_init(display, 2, 3);
}

void loop() {
  TM1637_printString(display, "Arch");
  delay(1000);
#if defined(ARDUINO_ARCH_8051)
  TM1637_printString(display, "8051");
  delay(1000);
#else
  TM1637_printString(display, "ArDU");
  delay(1000);
#endif 
  TM1637_clear(display);
  delay(1000);
  TM1637_printString(display, "He|o");
  delay(4000);
  TM1637_clear(display);
  delay(1000);
  TM1637_printString(display, "....");
  delay(2000);
  TM1637_printString(display, "1.2.3.4");
  delay(2000);
  TM1637_printString(display, "00:00");
  delay(1000);
  TM1637_printString(display, "24:59");
  delay(1000);
#if defined(ARDUINO_ARCH_8051)
  TM1637_printString(display, "-1°");
#else
  TM1637_printString(display, "-1*");
#endif
  delay(1000);
  for(char b=7;b>0;b--) {
    TM1637_setBrightness(display, b);
    delay(200);
  }
  for(char b=0 ;b<8;b++) {
    TM1637_setBrightness(display, b);
    delay(200);
  }
  TM1637_printByteByPos(display, 0, TM1637_getDigit('0'));  // Display '0' at position 0
  delay(1000);
  TM1637_printByteByPos(display, 1, TM1637_getDigit('1') | TM1637_DOT);  // Display '1' at position 1 with DOT
  delay(1000);
  TM1637_printByteByPos(display, 2, TM1637_getDigit('2'));  // Display '2' at position 2
  delay(1000);
  TM1637_printByteByPos(display, 3, TM1637_getDigit('3'));  // Display 'y'  at position 3
  delay(1000);
  TM1637_printString(display, "done");
  delay(1000);
}
