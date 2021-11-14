/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT displayun
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

// ST7735.c
// Runs on LM4F120/TM4C123
// Low level drivers for the ST7735 160x128 LCD based off of
// the file described above.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// March 30, 2015
// Augmented 7/17/2014 to have a simple graphics facility
// Tested with LaunchPadDLL.dll simulator 9/2/2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

// **********wide.hk ST7735R*******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********ADXL335 3-axis ST7735R*******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// X– (NC) analog input X-axis from ADXL335 accelerometer
// Y– (NC) analog input Y-axis from ADXL335 accelerometer
// Z– (NC) analog input Z-axis from ADXL335 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

#include <stdint.h>
#include "st7735.h"
#include "ssi_module.h"
#include "gpio_module.h"

/**
 * 16 rows (0 to 15) and 21 characters (0 to 20)
 *Requires (11 + size*size*6*8) bytes of transmission for each character
 */

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define DELAY 0x80

/**
 * ST7735
 * Backlight (pin 10) connected to +3.3 V
 * MISO (pin 9) unconnected
 * SCK (pin 8) connected to PA2 (SSI0Clk)
 * MOSI (pin 7) connected to PA5 (SSI0Tx)
 * TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 * CARD_CS (pin 5) unconnected
 * Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
 * RESET (pin 3) connected to PA7 (GPIO)
 * VCC (pin 2) connected to +3.3 V
 * Gnd (pin 1) connected to ground
 */
struct ST7735 ST7735 = {
    &SSI_0,
    &PORTA_PIN6,
    &PORTA_PIN7,
    0,
    0,
    ST7735_BLACK,
};

/**
 * standard ascii 5x7 font
 * originally from glcdfont.c from Adafruit project
 */
static const uint8_t Font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
    0x18, 0x3C, 0x7E, 0x3C, 0x18,
    0x1C, 0x57, 0x7D, 0x57, 0x1C,
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
    0x00, 0x18, 0x3C, 0x18, 0x00,
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
    0x00, 0x18, 0x24, 0x18, 0x00,
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
    0x30, 0x48, 0x3A, 0x06, 0x0E,
    0x26, 0x29, 0x79, 0x29, 0x26,
    0x40, 0x7F, 0x05, 0x05, 0x07,
    0x40, 0x7F, 0x05, 0x25, 0x3F,
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
    0x7F, 0x3E, 0x1C, 0x1C, 0x08,
    0x08, 0x1C, 0x1C, 0x3E, 0x7F,
    0x14, 0x22, 0x7F, 0x22, 0x14,
    0x5F, 0x5F, 0x00, 0x5F, 0x5F,
    0x06, 0x09, 0x7F, 0x01, 0x7F,
    0x00, 0x66, 0x89, 0x95, 0x6A,
    0x60, 0x60, 0x60, 0x60, 0x60,
    0x94, 0xA2, 0xFF, 0xA2, 0x94,
    0x08, 0x04, 0x7E, 0x04, 0x08,
    0x10, 0x20, 0x7E, 0x20, 0x10,
    0x08, 0x08, 0x2A, 0x1C, 0x08,
    0x08, 0x1C, 0x2A, 0x08, 0x08,
    0x1E, 0x10, 0x10, 0x10, 0x10,
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
    0x30, 0x38, 0x3E, 0x38, 0x30,
    0x06, 0x0E, 0x3E, 0x0E, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x5F, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x07, 0x00,
    0x14, 0x7F, 0x14, 0x7F, 0x14,
    0x24, 0x2A, 0x7F, 0x2A, 0x12,
    0x23, 0x13, 0x08, 0x64, 0x62,
    0x36, 0x49, 0x56, 0x20, 0x50,
    0x00, 0x08, 0x07, 0x03, 0x00,
    0x00, 0x1C, 0x22, 0x41, 0x00,
    0x00, 0x41, 0x22, 0x1C, 0x00,
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
    0x08, 0x08, 0x3E, 0x08, 0x08,
    0x00, 0x80, 0x70, 0x30, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x00, 0x60, 0x60, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02,
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x72, 0x49, 0x49, 0x49, 0x46, // 2
    0x21, 0x41, 0x49, 0x4D, 0x33, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
    0x41, 0x21, 0x11, 0x09, 0x07, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x46, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x00, 0x14, 0x00, 0x00,
    0x00, 0x40, 0x34, 0x00, 0x00,
    0x00, 0x08, 0x14, 0x22, 0x41,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x00, 0x41, 0x22, 0x14, 0x08,
    0x02, 0x01, 0x59, 0x09, 0x06,
    0x3E, 0x41, 0x5D, 0x59, 0x4E,
    0x7C, 0x12, 0x11, 0x12, 0x7C, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x41, 0x3E, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x73, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x26, 0x49, 0x49, 0x49, 0x32, // S
    0x03, 0x01, 0x7F, 0x01, 0x03, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x59, 0x49, 0x4D, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x41,
    0x02, 0x04, 0x08, 0x10, 0x20,
    0x00, 0x41, 0x41, 0x41, 0x7F,
    0x04, 0x02, 0x01, 0x02, 0x04,
    0x40, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x03, 0x07, 0x08, 0x00,
    0x20, 0x54, 0x54, 0x78, 0x40, // a
    0x7F, 0x28, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x28, // c
    0x38, 0x44, 0x44, 0x28, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x00, 0x08, 0x7E, 0x09, 0x02, // f
    0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x40, 0x3D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x78, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0xFC, 0x18, 0x24, 0x24, 0x18, // p
    0x18, 0x24, 0x24, 0x18, 0xFC, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x24, // s
    0x04, 0x04, 0x3F, 0x44, 0x24, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x4C, 0x90, 0x90, 0x90, 0x7C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00,
    0x00, 0x00, 0x77, 0x00, 0x00,
    0x00, 0x41, 0x36, 0x08, 0x00,
    0x02, 0x01, 0x02, 0x04, 0x02,
    0x3C, 0x26, 0x23, 0x26, 0x3C,
    0x1E, 0xA1, 0xA1, 0x61, 0x12,
    0x3A, 0x40, 0x40, 0x20, 0x7A,
    0x38, 0x54, 0x54, 0x55, 0x59,
    0x21, 0x55, 0x55, 0x79, 0x41,
    0x21, 0x54, 0x54, 0x78, 0x41,
    0x21, 0x55, 0x54, 0x78, 0x40,
    0x20, 0x54, 0x55, 0x79, 0x40,
    0x0C, 0x1E, 0x52, 0x72, 0x12,
    0x39, 0x55, 0x55, 0x55, 0x59,
    0x39, 0x54, 0x54, 0x54, 0x59,
    0x39, 0x55, 0x54, 0x54, 0x58,
    0x00, 0x00, 0x45, 0x7C, 0x41,
    0x00, 0x02, 0x45, 0x7D, 0x42,
    0x00, 0x01, 0x45, 0x7C, 0x40,
    0xF0, 0x29, 0x24, 0x29, 0xF0,
    0xF0, 0x28, 0x25, 0x28, 0xF0,
    0x7C, 0x54, 0x55, 0x45, 0x00,
    0x20, 0x54, 0x54, 0x7C, 0x54,
    0x7C, 0x0A, 0x09, 0x7F, 0x49,
    0x32, 0x49, 0x49, 0x49, 0x32,
    0x32, 0x48, 0x48, 0x48, 0x32,
    0x32, 0x4A, 0x48, 0x48, 0x30,
    0x3A, 0x41, 0x41, 0x21, 0x7A,
    0x3A, 0x42, 0x40, 0x20, 0x78,
    0x00, 0x9D, 0xA0, 0xA0, 0x7D,
    0x39, 0x44, 0x44, 0x44, 0x39,
    0x3D, 0x40, 0x40, 0x40, 0x3D,
    0x3C, 0x24, 0xFF, 0x24, 0x24,
    0x48, 0x7E, 0x49, 0x43, 0x66,
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
    0xFF, 0x09, 0x29, 0xF6, 0x20,
    0xC0, 0x88, 0x7E, 0x09, 0x03,
    0x20, 0x54, 0x54, 0x79, 0x41,
    0x00, 0x00, 0x44, 0x7D, 0x41,
    0x30, 0x48, 0x48, 0x4A, 0x32,
    0x38, 0x40, 0x40, 0x22, 0x7A,
    0x00, 0x7A, 0x0A, 0x0A, 0x72,
    0x7D, 0x0D, 0x19, 0x31, 0x7D,
    0x26, 0x29, 0x29, 0x2F, 0x28,
    0x26, 0x29, 0x29, 0x29, 0x26,
    0x30, 0x48, 0x4D, 0x40, 0x20,
    0x38, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x38,
    0x2F, 0x10, 0xC8, 0xAC, 0xBA,
    0x2F, 0x10, 0x28, 0x34, 0xFA,
    0x00, 0x00, 0x7B, 0x00, 0x00,
    0x08, 0x14, 0x2A, 0x14, 0x22,
    0x22, 0x14, 0x2A, 0x14, 0x08,
    0xAA, 0x00, 0x55, 0x00, 0xAA,
    0xAA, 0x55, 0xAA, 0x55, 0xAA,
    0x00, 0x00, 0x00, 0xFF, 0x00,
    0x10, 0x10, 0x10, 0xFF, 0x00,
    0x14, 0x14, 0x14, 0xFF, 0x00,
    0x10, 0x10, 0xFF, 0x00, 0xFF,
    0x10, 0x10, 0xF0, 0x10, 0xF0,
    0x14, 0x14, 0x14, 0xFC, 0x00,
    0x14, 0x14, 0xF7, 0x00, 0xFF,
    0x00, 0x00, 0xFF, 0x00, 0xFF,
    0x14, 0x14, 0xF4, 0x04, 0xFC,
    0x14, 0x14, 0x17, 0x10, 0x1F,
    0x10, 0x10, 0x1F, 0x10, 0x1F,
    0x14, 0x14, 0x14, 0x1F, 0x00,
    0x10, 0x10, 0x10, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x10,
    0x10, 0x10, 0x10, 0x1F, 0x10,
    0x10, 0x10, 0x10, 0xF0, 0x10,
    0x00, 0x00, 0x00, 0xFF, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0xFF, 0x10,
    0x00, 0x00, 0x00, 0xFF, 0x14,
    0x00, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0x00, 0x1F, 0x10, 0x17,
    0x00, 0x00, 0xFC, 0x04, 0xF4,
    0x14, 0x14, 0x17, 0x10, 0x17,
    0x14, 0x14, 0xF4, 0x04, 0xF4,
    0x00, 0x00, 0xFF, 0x00, 0xF7,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0xF7, 0x00, 0xF7,
    0x14, 0x14, 0x14, 0x17, 0x14,
    0x10, 0x10, 0x1F, 0x10, 0x1F,
    0x14, 0x14, 0x14, 0xF4, 0x14,
    0x10, 0x10, 0xF0, 0x10, 0xF0,
    0x00, 0x00, 0x1F, 0x10, 0x1F,
    0x00, 0x00, 0x00, 0x1F, 0x14,
    0x00, 0x00, 0x00, 0xFC, 0x14,
    0x00, 0x00, 0xF0, 0x10, 0xF0,
    0x10, 0x10, 0xFF, 0x10, 0xFF,
    0x14, 0x14, 0x14, 0xFF, 0x14,
    0x10, 0x10, 0x10, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0x10,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0xFF,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x38, 0x44, 0x44, 0x38, 0x44,
    0x7C, 0x2A, 0x2A, 0x3E, 0x14,
    0x7E, 0x02, 0x02, 0x06, 0x06,
    0x02, 0x7E, 0x02, 0x7E, 0x02,
    0x63, 0x55, 0x49, 0x41, 0x63,
    0x38, 0x44, 0x44, 0x3C, 0x04,
    0x40, 0x7E, 0x20, 0x1E, 0x20,
    0x06, 0x02, 0x7E, 0x02, 0x02,
    0x99, 0xA5, 0xE7, 0xA5, 0x99,
    0x1C, 0x2A, 0x49, 0x2A, 0x1C,
    0x4C, 0x72, 0x01, 0x72, 0x4C,
    0x30, 0x4A, 0x4D, 0x4D, 0x30,
    0x30, 0x48, 0x78, 0x48, 0x30,
    0xBC, 0x62, 0x5A, 0x46, 0x3D,
    0x3E, 0x49, 0x49, 0x49, 0x00,
    0x7E, 0x01, 0x01, 0x01, 0x7E,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x44, 0x44, 0x5F, 0x44, 0x44,
    0x40, 0x51, 0x4A, 0x44, 0x40,
    0x40, 0x44, 0x4A, 0x51, 0x40,
    0x00, 0x00, 0xFF, 0x01, 0x03,
    0xE0, 0x80, 0xFF, 0x00, 0x00,
    0x08, 0x08, 0x6B, 0x6B, 0x08,
    0x36, 0x12, 0x36, 0x24, 0x36,
    0x06, 0x0F, 0x09, 0x0F, 0x06,
    0x00, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x00, 0x10, 0x10, 0x00,
    0x30, 0x40, 0xFF, 0x01, 0x01,
    0x00, 0x1F, 0x01, 0x01, 0x1E,
    0x00, 0x19, 0x1D, 0x17, 0x12,
    0x00, 0x3C, 0x3C, 0x3C, 0x3C,
    0x00, 0x00, 0x00, 0x00, 0x00,
};

/**
 * Rather than a bazillion writecommand() and writedata() calls, screen
 * initialization commands and arguments are organized in these tables
 * stored in ROM.  The table may look bulky, but that's mostly the
 * formatting -- storage-wise this is hundreds of bytes more compact
 * than the equivalent code.  Companion function follows.
 */

/* Init for 7735R, part 1 (red or green tab) */
static const uint8_t power_init_seq[] = {
    15,                         // 15 commands in list:
    ST7735_SWRESET, DELAY,      // 1: Software reset, 0 args, w/delay
    150,                        // 150 ms delay
    ST7735_SLPOUT, DELAY,       // 2: Out of sleep mode, 0 args, w/delay
    255,                        // 500 ms delay
    ST7735_FRMCTR1, 3,          // 3: Frame rate ctrl - normal mode, 3 args:
    0x01, 0x2C, 0x2D,           // Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,          // 4: Frame rate control - idle mode, 3 args:
    0x01, 0x2C, 0x2D,           // Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,          // 5: Frame rate ctrl - partial mode, 6 args:
    0x01, 0x2C, 0x2D,           // Dot inversion mode
    0x01, 0x2C, 0x2D,           // Line inversion mode
    ST7735_INVCTR, 1,           // 6: Display inversion ctrl, 1 arg, no delay:
    0x07,                       // No inversion
    ST7735_PWCTR1, 3,           // 7: Power control, 3 args, no delay:
    0xA2,
    0x02,                       // -4.6V
    0x84,                       // AUTO mode
    ST7735_PWCTR2, 1,           // 8: Power control, 1 arg, no delay:
    0xC5,                       // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3, 2,           // 9: Power control, 2 args, no delay:
    0x0A,                       // Opamp current small
    0x00,                       // Boost frequency
    ST7735_PWCTR4, 2,           // 10: Power control, 2 args, no delay:
    0x8A,                       // BCLK/2, Opamp current small & Medium low
    0x2A,
    ST7735_PWCTR5, 2,           // 11: Power control, 2 args, no delay:
    0x8A, 0xEE,
    ST7735_VMCTR1, 1,           // 12: Power control, 1 arg, no delay:
    0x0E,
    ST7735_INVOFF, 0,           // 13: Don't invert display, no args, no delay
    ST7735_MADCTL, 1,           // 14: Memory access control (directions), 1 arg:
    0xC8,                       // row addr/col addr, bottom to top refresh
    ST7735_COLMOD, 1,           // 15: set color mode, 1 arg, no delay:
    0x05
};  // 16-bit color

/* Init for 7735R, part 2 (red tab only) */
static const uint8_t addr_init_seq[] = { 
    2,                          // 2 commands in list:
    ST7735_CASET, 4,            // 1: Column addr set, 4 args, no delay:
    0x00, 0x00,                 // XSTART = 0
    0x00, 0x7F,                 // XEND = 127
    ST7735_RASET, 4,            // 2: Row addr set, 4 args, no delay:
    0x00, 0x00,                 // XSTART = 0
    0x00, 0x9F                  // XEND = 159
};                              

/* Init for 7735R, part 3 (red or green tab) */
static const uint8_t display_init_seq[] = {
    4,                          // 4 commands in list:
    ST7735_GMCTRP1, 16,         // 1: Magical unicorn dust, 16 args, no delay:
    0x02, 0x1c, 0x07, 0x12,
    0x37, 0x32, 0x29, 0x2d,
    0x29, 0x25, 0x2B, 0x39,
    0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,   // 2: Sparkles and rainbows, 16 args, no delay:
    0x03, 0x1d, 0x07, 0x06,
    0x2E, 0x2C, 0x29, 0x2D,
    0x2E, 0x2E, 0x37, 0x3F,
    0x00, 0x00, 0x02, 0x10,
    ST7735_NORON, DELAY,        // 3: Normal display on, no args, w/delay
    10,                         // 10 ms delay
    ST7735_DISPON, DELAY,       // 4: Main screen turn on, no args w/delay
    100                         // 100 ms delay
};                              

/**
 * The Data/Command pin must be valid when the eighth bit is
 * sent.  The SSI module has hardware input and output FIFOs
 * that are 8 locations deep.  Based on the observation that
 * the LCD interface tends to send a few commands and then a
 * lot of data, the FIFOs are not used when writing
 * commands, and they are used when writing data.  This
 * ensures that the Data/Command pin status matches the byte
 * that is actually being transmitted.
 * The write command operation waits until all data has been
 * sent, configures the Data/Command pin for commands, sends
 * the command, and then waits for the transmission to
 * finish.
 * The write data operation waits until there is room in the
 * transmit FIFO, configures the Data/Command pin for data,
 * and then adds the data to the transmit FIFO.
 * NOTE: These functions will crash or stall indefinitely if
 * the SSI0 module is not initialized and enabled.
 */
void static writecommand(struct ST7735 *st7735, uint8_t c) {
    while (ssi_is_busy(st7735->ssi))
        ;
    set_gpio_pin_low(st7735->dat_com);
    write_ssi(st7735->ssi, &c, 1);

    while (ssi_is_busy(st7735->ssi))
        ;
}

void static writedata(struct ST7735 *st7735, uint8_t c) {
    while(!ssi_tx_ready(st7735->ssi))
        ;
    set_gpio_pin_high(st7735->dat_com);

    write_ssi(st7735->ssi, &c, 1);
}

void Delay1ms(uint32_t n){uint32_t volatile time;
    while(n){
        time = 72724 * 2 / 91;  // 1msec, tuned at 80 MHz
        while(time){
            time--;
        }
        n--;
    }
}

void static execute_command_sequence(struct ST7735 *st7735, const uint8_t *addr) {

    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *(addr++);

    while(numCommands--) {                 
        writecommand(st7735, *(addr++));           
        numArgs = *(addr++);                
        ms = numArgs & DELAY;  // If hibit set, delay follows args
        numArgs &= ~DELAY;  // Mask out delay bit
        
        while(numArgs--) {
            writedata(st7735, *(addr++));
        }

        if (ms) {
            ms = *(addr++);  // Read post-command delay time (ms)
            if(ms == 255)  // If 255, delay for 500 ms
                ms = 500;   
            Delay1ms(ms);
        }
    }
}

void ST7735_toggle_reset(struct ST7735 *st7735) {
    set_gpio_pin_low(st7735->ssi->fss);
    set_gpio_pin_high(st7735->rst);
    Delay1ms(500);
    set_gpio_pin_low(st7735->rst);
    Delay1ms(500);
    set_gpio_pin_high(st7735->rst);
    Delay1ms(500);
}

void static ST7735_init_peripherals(struct ST7735 *st7735) {
    init_gpio_port_clock(st7735->dat_com);
    init_gpio_pin_as_output(st7735->dat_com);
    init_gpio_pin_as_output(st7735->rst);

    disable_gpio_pin_alternate_function(st7735->dat_com);
    disable_gpio_pin_alternate_function(st7735->rst);

    init_gpio_pin_as_digital(st7735->dat_com);
    init_gpio_pin_as_digital(st7735->rst);

    ST7735_toggle_reset(st7735);

    init_ssi(st7735->ssi);
    init_ssi_clock_prescale_divider(st7735->ssi, 10);
    init_ssi_mode(st7735->ssi, 0, 0, 0, 0, 8);
    enable_ssi(st7735->ssi);
}

void ST7735_init(struct ST7735 *st7735) {
    ST7735_init_peripherals(st7735);
    
    execute_command_sequence(st7735, power_init_seq);
    execute_command_sequence(st7735, addr_init_seq);
    execute_command_sequence(st7735, display_init_seq);

    ST7735_SetCursor(st7735, 0,0);
    ST7735_SetTextColor(st7735, ST7735_WHITE);
    ST7735_FillScreen(st7735, ST7735_BLACK);
}

/**
 * Set the region of the screen RAM to be modified
 * Pixel colors are sent left to right, top to bottom
 * (same as Font table is encoded; different from regular bitmap)
 * Requires 11 bytes of transmission
 */
void static setAddrWindow(struct ST7735 *st7735, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    writecommand(st7735, ST7735_CASET); // Column addr set

    writedata(st7735, 0x00);
    writedata(st7735, x0);     // XSTART
    writedata(st7735, 0x00);
    writedata(st7735, x1);     // XEND

    writecommand(st7735, ST7735_RASET); // Row addr set
    writedata(st7735, 0x00);
    writedata(st7735, y0);     // YSTART
    writedata(st7735, 0x00);
    writedata(st7735, y1);     // YEND

    writecommand(st7735, ST7735_RAMWR); // write to RAM
}

/**
 * Send two bytes of data, most significant byte first
 * Requires 2 bytes of transmission
 */
void static pushColor(struct ST7735 *st7735, uint16_t color) {
    writedata(st7735, (uint8_t) (color >> 8));
    writedata(st7735, (uint8_t) color);
}

//------------ST7735_DrawPixel------------
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawPixel(struct ST7735 *st7735, uint8_t x, uint8_t y, uint16_t color) {
    if((x < 0) || (x >= ST7735_TFTWIDTH) || (y < 0) || (y >= ST7735_TFTHEIGHT))
        return;

    setAddrWindow(st7735, x, y, x, y);

    pushColor(st7735, color);
}

//------------ST7735_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawFastVLine(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
    uint8_t hi = color >> 8, lo = color;

    // Rudimentary clipping
    if((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT))
        return;
    if((y + h - 1) >= ST7735_TFTHEIGHT)
        h = ST7735_TFTHEIGHT - y;

    setAddrWindow(st7735, x, y, x, y + h - 1);

    while (h--) {
        writedata(st7735, hi);
        writedata(st7735, lo);
    }
}

//------------ST7735_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawFastHLine(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
    uint8_t hi = color >> 8, lo = color;

    // Rudimentary clipping
    if((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT))
        return;
    if((x+w-1) >= ST7735_TFTWIDTH)
        w = ST7735_TFTWIDTH-x;

    setAddrWindow(st7735, x, y, x + w - 1, y);

    while (w--) {
        writedata(st7735, hi);
        writedata(st7735, lo);
    }
}

//------------ST7735_FillScreen------------
// Fill the screen with the given color.
// Requires 40,971 bytes of transmission
// Input: color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_FillScreen(struct ST7735 *st7735, uint16_t color) {
  ST7735_FillRect(st7735, 0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, color);
}

//------------ST7735_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_FillRect(struct ST7735 *st7735, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    uint8_t hi = color >> 8, lo = color;

    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT))
        return;
    if((x + w - 1) >= ST7735_TFTWIDTH)
        w = ST7735_TFTWIDTH  - x;
    if((y + h - 1) >= ST7735_TFTHEIGHT)
        h = ST7735_TFTHEIGHT - y;

    setAddrWindow(st7735, x, y, x + w - 1, y + h - 1);

    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            writedata(st7735, hi);
            writedata(st7735, lo);
        }
    }
}

//------------ST7735_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}

//------------ST7735_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t ST7735_SwapColor(uint16_t x) {
    return (x << 11) | (x & 0x07E0) | (x >> 11);
}

//------------ST7735_DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to ST7735_DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void ST7735_DrawCharS(struct ST7735 *st7735, uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size){
    uint8_t line; // vertical column of pixels of character in font
    int32_t i, j;
    if((x >= ST7735_TFTWIDTH)            || // Clip right
        (y >= ST7735_TFTHEIGHT)           || // Clip bottom
        ((x + 5 * size - 1) < 0) || // Clip left
        ((y + 8 * size - 1) < 0))   // Clip top
        return;

    for (i = 0; i < 6; i++ ) {
        if (i == 5)
            line = 0x0;
        else
            line = Font[(c * 5) + i];
        for (j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1) // default size
                    ST7735_DrawPixel(st7735, x + i, y + j, textColor);
                else {  // big size
                    ST7735_FillRect(st7735, x + (i * size), y + (j * size), size, size, textColor);
                }
            } else if (bgColor != textColor) {
                if (size == 1) // default size
                    ST7735_DrawPixel(st7735, x + i, y + j, bgColor);
                else {  // big size
                    ST7735_FillRect(st7735, x + i * size, y + j * size, size, size, bgColor);
                }
            }
            line >>= 1;
        }
    }
}

//------------ST7735_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void ST7735_DrawChar(struct ST7735 *st7735, uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size){
    uint8_t line; // horizontal row of pixels of character
    int32_t col, row, i, j;// loop indices
    if(((x + 5 * size - 1) >= ST7735_TFTWIDTH)  || // Clip right
        ((y + 8 * size - 1) >= ST7735_TFTHEIGHT) || // Clip bottom
        ((x + 5 * size - 1) < 0)        || // Clip left
        ((y + 8 * size - 1) < 0))        // Clip top
        return;

    setAddrWindow(st7735, x, y, x + 6 * size - 1, y + 8 * size - 1);

    line = 0x01;  // print the top row first
    // print the rows, starting at the top
    for(row = 0; row < 8; row = row + 1){
        for(i = 0; i < size; i = i + 1){
            // print the columns, starting on the left
            for(col = 0; col < 5; col = col + 1){
                if(Font[(c * 5) + col] & line){
                // bit is set in Font, print pixel(s) in text color
                    for(j = 0; j < size; j = j + 1){
                        pushColor(st7735, textColor);
                    }
                } else{
                    // bit is cleared in Font, print pixel(s) in background color
                    for(j = 0; j < size; j = j + 1){
                        pushColor(st7735, bgColor);
                    }
                }
            }
            // print blank column(s) to the right of character
            for(j = 0; j < size; j = j + 1){
                pushColor(st7735, bgColor);
            }
        }
        line <<= 1;   // move up to the next row
    }
}

//------------ST7735_DrawString------------
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
uint32_t ST7735_DrawString(struct ST7735 *st7735, uint16_t x, uint16_t y, char *pt, int16_t textColor){
    uint32_t count = 0;
    if(y > 15) 
        return 0;

    while(*pt) {
        ST7735_DrawCharS(st7735, x * 6, y * 10, *pt, textColor, ST7735_BLACK, 1);
        pt++;
        x = x + 1;
        if(x > 20)
            return count;  // number of characters printed
        count++;
    }
    return count;  // number of characters printed
}

//********ST7735_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=15)
// outputs: none
void ST7735_SetCursor(struct ST7735 *st7735, uint8_t new_x, uint8_t new_y){
    if((new_x > 20) || (new_y > 15)) {       // bad input
        return;                             // do nothing
    }
    st7735->x_pos = new_x;
    st7735->y_pos = new_y;
}

// *************** ST7735_OutChar ********************
// Output one character to the LCD
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Inputs: 8-bit ASCII character
// Outputs: none
void ST7735_OutChar(struct ST7735 *st7735, char ch){
    if((ch == 10) || (ch == 13) || (ch == 27)) {
        st7735->y_pos++; st7735->x_pos=0;
        if(st7735->y_pos > 15) {
            st7735->y_pos = 0;
        }
        ST7735_DrawString(st7735, 0, st7735->y_pos,"                     ",st7735->text_color);
        return;
    } else if (ch == 8) {
        if (st7735->x_pos == 0) {
            st7735->y_pos > 0 ? st7735->y_pos = 0 : st7735->y_pos--;
        } else {
            st7735->x_pos--;
        }
        ST7735_DrawCharS(st7735, st7735->x_pos * 6, st7735->y_pos * 10, ' ', st7735->text_color, ST7735_BLACK, 1);
        return;
    }
    ST7735_DrawCharS(st7735, st7735->x_pos * 6, st7735->y_pos * 10, ch, st7735->text_color, ST7735_BLACK, 1);
    st7735->x_pos++;
    if(st7735->x_pos > 20) {
        st7735->x_pos = 20;
        ST7735_DrawCharS(st7735, st7735->x_pos * 6, st7735->y_pos * 10, '*', ST7735_RED,ST7735_BLACK, 1);
    }
    return;
}

//********ST7735_OutString*****************
// Print a string of characters to the ST7735 LCD.
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// The string will not automatically wrap.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
void ST7735_OutString(struct ST7735 *st7735, char *ptr){
    while(*ptr){
        ST7735_OutChar(st7735, *ptr);
        ptr = ptr + 1;
    }
}

// ************** ST7735_SetTextColor ************************
// Sets the color in which the characters will be printed
// Background color is fixed at black
// Input:  16-bit packed color
// Output: none
// ********************************************************
void ST7735_SetTextColor(struct ST7735 *st7735, uint16_t color){
    st7735->text_color = color;
}
