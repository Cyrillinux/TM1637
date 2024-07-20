#include <TM1637UniDevice.h>

const unsigned char PROGMEM TM1637_digit[] = {
    // XGFEDCBA
    0x3F,  // 0b00111111,    // 0
    0x06,  // 0b00000110,    // 1
    0x5B,  // 0b01011011,    // 2
    0x4F,  // 0b01001111,    // 3
    0x66,  // 0b01100110,    // 4
    0x6D,  // 0b01101101,    // 5
    0x7D,  // 0b01111101,    // 6
    0x07,  // 0b00000111,    // 7
    0x7F,  // 0b01111111,    // 8
    0x6F,  // 0b01101111,    // 9
    0x77,  // 0b01110111,    // A
    0x7C,  // 0b01111100,    // b
    0x39,  // 0b00111001,    // C
    0x5E,  // 0b01011110,    // d
    0x79,  // 0b01111001,    // E
    0x71,  // 0b01110001,    // F
    0x74,  // 0b01110100,    // h
    0x76,  // 0b01110110,    // H
    0x1E,  // 0b00011110,    // J
    0x38,  // 0b00111000,    // L
    0x54,  // 0b01010100,    // n
    0x5C,  // 0b01011100,    // o
    0x73,  // 0b01110011,    // p
    0x67,  // 0b01100111,    // q
    0x50,  // 0b01010000,    // r
    0x78,  // 0b01111000,    // t
    0x1C,  // 0b00011100,    // u
    0x3E,  // 0b00111110,    // U
    0x6E,  // 0b01101110,    // y
    0x00,  // 0b00000000,    // SPACE
    0x80,  // 0b10000000,    // DOT
    0x40,  // 0b01000000,    // -
    0x63,  // 0b01100011,    // °
    0x36,  // 0b01110110,    // ||
    0x48,  // 0b01001000     // =
    0x46   // 0b01000110,    // +
};

#if defined(ARDUINO_ARCH_8051)

// TM1637_start
void TM1637_start(TM1637Device display) {
    // TM1637_DIO = 1;
    pinMode(display->dio, OUTPUT);
    digitalWrite(display->dio, HIGH);
    // TM1637_CLK = 1;
    pinMode(display->clk, OUTPUT);
    digitalWrite(display->clk, HIGH);
    // TM1637_DIO = 0;
    digitalWrite(display->dio, LOW);
    // TM1637_CLK = 0;
    digitalWrite(display->clk, LOW);
}
// TM1637_stop
void TM1637_stop(TM1637Device display) {
    // TM1637_CLK = 0;
    digitalWrite(display->clk, LOW);
    // TM1637_DIO = 0;
    digitalWrite(display->dio, LOW);
    delay(10);
    // TM1637_CLK = 1;
    digitalWrite(display->clk, HIGH);
    // TM1637_DIO = 1;
    digitalWrite(display->dio, HIGH);
}
// TM1637_writeByte
void TM1637_writeByte(TM1637Device display, unsigned char b) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        // TM1637_CLK = 0;
        digitalWrite(display->clk, LOW);
        if(b & 0x01) {
            // TM1637_DIO = 1;
            digitalWrite(display->dio, HIGH);
        } else {
            // TM1637_DIO = 0;
            digitalWrite(display->dio, LOW);
        }
        b >>= 1;
        // TM1637_CLK = 1;
        digitalWrite(display->clk, HIGH);
    }
    // Wait for the ACK
    // TM1637_CLK = 0;
    digitalWrite(display->clk, LOW);
    // TM1637_DIO = 1;
    digitalWrite(display->dio, HIGH);
    // TM1637_CLK = 1;
    digitalWrite(display->clk, HIGH);
    while(/*TM1637_DIO*/digitalRead(display->dio)) {}
    // TM1637_CLK = 0;
    digitalWrite(display->clk, LOW);
}
// TM1637_subinit
void TM1637_subinit(TM1637Device display, unsigned char clk, unsigned char dio) {
    display->clk=clk;
    display->dio=dio;
    TM1637_setBrightness(display, 0x07);
}
#else
// TM1637_bitDelay
void TM1637_bitDelay(void)
{
  delayMicroseconds(100);
}
// TM1637_start
void TM1637_start(TM1637Device display)
{
  pinMode(display->dio, OUTPUT);
  TM1637_bitDelay();
}
// TM1637_stop
void TM1637_stop(TM1637Device display)
{
  pinMode(display->dio, OUTPUT);
  TM1637_bitDelay();
  pinMode(display->clk, INPUT);
  TM1637_bitDelay();
  pinMode(display->dio, INPUT);
  TM1637_bitDelay();
}
// TM1637_writeByte
bool TM1637_writeByte(TM1637Device display, uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinMode(display->clk, OUTPUT);
    TM1637_bitDelay();

  // Set data bit
    if (data & 0x01)
      pinMode(display->dio, INPUT);
    else
      pinMode(display->dio, OUTPUT);

    TM1637_bitDelay();

  // CLK high
    pinMode(display->clk, INPUT);
    TM1637_bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  pinMode(display->clk, OUTPUT);
  pinMode(display->dio, INPUT);
  TM1637_bitDelay();

  // CLK to high
  pinMode(display->clk, INPUT);
  TM1637_bitDelay();
  uint8_t ack = digitalRead(display->dio);
  if (ack == 0)
    pinMode(display->dio, OUTPUT);


  TM1637_bitDelay();
  pinMode(display->clk, OUTPUT);
  TM1637_bitDelay();

  return ack;
}
// TM1637_subinit
void TM1637_subinit(TM1637Device display, uint8_t pinClk, uint8_t pinDIO)
{
  // Copy the pin numbers
  display->clk = pinClk;
  display->dio = pinDIO;
  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
    pinMode(display->clk, INPUT);
    pinMode(display->dio,INPUT);
  digitalWrite(display->clk, LOW);
  digitalWrite(display->dio, LOW);
  TM1637_setBrightness(display, 0x07);
}
#endif

// TM1637 Universal functions
#define TM1637_clear(x) TM1637_printString(x, "    ");
#define TM1637_init(x,y,z)  x = (TM1637Device) malloc(sizeof(__TM1637Device));TM1637_subinit(x, y, z)

// Function to get segment code for a given character
unsigned char TM1637_getDigit(char c) {
    if (c >= '0' && c <= '9') {
        return TM1637_digit[c - '0'];
    } else if (c >= 'A' && c <= 'F') {
        return TM1637_digit[10 + c - 'A'];
    } else if (c >= 'a' && c <= 'f') {
        return TM1637_digit[10 + c - 'a'];
    } else {
        switch (c) {
            case 'h': return TM1637_digit[16];
            case 'H': return TM1637_digit[17];
            case 'j': return TM1637_digit[18];
            case 'J': return TM1637_digit[18];
            case 'l': return TM1637_digit[19];
            case 'L': return TM1637_digit[19];
            case 'n': return TM1637_digit[20];
            case 'o': return TM1637_digit[21];
            case 'O': return TM1637_digit[21];
            case 'p': return TM1637_digit[22];
            case 'q': return TM1637_digit[23];
            case 'R': return TM1637_digit[24];
            case 'r': return TM1637_digit[24];
            case 't': return TM1637_digit[25];
            case 'u': return TM1637_digit[26];
            case 'U': return TM1637_digit[27];
            case 'y': return TM1637_digit[28];
            case ' ': return TM1637_digit[29];
            case '.': return TM1637_digit[30];
            case '-': return TM1637_digit[31];
            case '\xb0': return TM1637_digit[32];
            case '|': return TM1637_digit[33];
            case '=': return TM1637_digit[34];
            case '+': return TM1637_digit[35];
            default: return 0x00; // Default to all segments off for unrecognized characters
        }
    }
}
// TM1637_printByteByPos
void TM1637_printByteByPos(TM1637Device display, unsigned char pos, unsigned char data) {
    TM1637_start(display);
    TM1637_writeByte(display, 0x44);  // Command to set data
    TM1637_stop(display);

    TM1637_start(display);
    TM1637_writeByte(display, pos | 0xC0);  // Command to set address
    TM1637_writeByte(display, data);
    TM1637_stop(display);

    TM1637_start(display);
    TM1637_writeByte(display, 0x8F);  // Command to set display control (brightness)
    TM1637_stop(display);
}
// TM1637_setBrightness
void TM1637_setBrightness(TM1637Device display, unsigned char brightness) {
    display->brightness=brightness;
    TM1637_start(display);
    TM1637_writeByte(display, 0x88 | (display->brightness & 0x07));  // Command to set display control with brightness
    TM1637_stop(display);
}
// TM1637_printString
void TM1637_printString(TM1637Device display, char *str) {
   unsigned char pos = 0;
   unsigned char len = strlen(str);
   char i;
   for (int i=0; (i<len) && (pos<4); i++) {
      if (((str[i+1]=='.') || (str[i+1]==':')) && (str[i]!='.') && (str[i]!=':')) {
          TM1637_printByteByPos(display, pos, TM1637_getDigit(str[i]) | TM1637_DOT);i++;
        } else if (str[i]=='+') {
          TM1637_printByteByPos(display, pos, TM1637_getDigit(str[i]) | TM1637_DOT);
          pos++;
          TM1637_printByteByPos(display, pos, TM1637_getDigit('-') | TM1637_DOT);
        } else TM1637_printByteByPos(display, pos, TM1637_getDigit(str[i]));
      //#if !defined(ARDUINO_ARCH_8051)
       if (str[i+1]=='\xb0') pos--;
      //#endif
      pos++;
   }
    for (i=pos; i<4;i++) {
        TM1637_printByteByPos(display, i, TM1637_getDigit(' '));
    }
}
