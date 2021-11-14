/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// ST7735.h
// Runs on LM4F120/TM4C123
// Low level drivers for the ST7735 160x128 LCD based off of
// the file described above.
// Daniel Valvano
// September 12, 2013
// Augmented 3/3/2014 to have a simple graphics facility

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#ifndef _ST7735H_
#define _ST7735H_

#include <stdint.h>

struct ST7735 {
    struct SSI *ssi;
    struct GPIO_PIN *dat_com;
    struct GPIO_PIN *rst;
    uint8_t x_pos;
    uint8_t y_pos;
    uint16_t text_color;
};

extern struct ST7735 ST7735;

#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0xF800
#define ST7735_RED     0x001F
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0xFFE0
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0x07FF
#define ST7735_WHITE   0xFFFF

void ST7735_init(struct ST7735 *st7735);
void ST7735_DrawPixel(struct ST7735 *st7735, uint8_t x, uint8_t y, uint16_t color);
void ST7735_DrawFastVLine(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t h, uint16_t color);
void ST7735_DrawFastHLine(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t w, uint16_t color);
void ST7735_FillScreen(struct ST7735 *st7735, unsigned short color);
void ST7735_FillRect(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b);
uint16_t ST7735_SwapColor(uint16_t x);
void ST7735_SetTextColor(struct ST7735 *st7735, uint16_t color);
void ST7735_DrawCharS(struct ST7735 *st7735, uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size);
void ST7735_DrawChar(struct ST7735 *st7735, uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size);
void ST7735_OutString(struct ST7735 *st7735, char *ptr);
void ST7735_SetCursor(struct ST7735 *st7735, uint8_t new_x, uint8_t new_y);
void Output_Init(struct ST7735 *st7735);
void ST7735_OutChar(struct ST7735 *st7735, char ch);

#endif /* _ST7735H_ */
