#include <Arduino.h>
#include <stdlib.h>

#ifndef _TM1637_UNIDEVICE_
#define _TM1637_UNIDEVICE_

#define TM1637_DOT 0x80 // Define DOT

typedef struct _TM1637Device {
  unsigned char clk;
  unsigned char dio;
  unsigned char brightness;
} __TM1637Device, *TM1637Device;

extern const unsigned char TM1637_digit[];

#if defined(ARDUINO_ARCH_8051)
// TM1637_start
void TM1637_start(TM1637Device display);
// TM1637_stop
void TM1637_stop(TM1637Device display);
// TM1637_writeByte
void TM1637_writeByte(TM1637Device display, unsigned char b);
// TM1637_subinit
void TM1637_subinit(TM1637Device display, unsigned char clk, unsigned char dio);
#else
// TM1637_bitDelay
void TM1637_bitDelay(void);
// TM1637_start
void TM1637_start(TM1637Device display);
// TM1637_stop
void TM1637_stop(TM1637Device display);
// TM1637_writeByte
bool TM1637_writeByte(TM1637Device display, uint8_t b);
// TM1637_subinit
void TM1637_subinit(TM1637Device display, uint8_t pinClk, uint8_t pinDIO);
#endif

// TM1637 Universal functions
#define TM1637_clear(x) TM1637_printString(x, "    ");
#define TM1637_init(x,y,z)  x = (TM1637Device) malloc(sizeof(__TM1637Device));TM1637_subinit(x, y, z)

// Function to get segment code for a given character
unsigned char TM1637_getDigit(char c);
// TM1637_printByteByPos
void TM1637_printByteByPos(TM1637Device display, unsigned char pos, unsigned char data);
// TM1637_setBrightness
void TM1637_setBrightness(TM1637Device display, unsigned char brightness);
// TM1637_printString
void TM1637_printString(TM1637Device display, char *str);

#endif
