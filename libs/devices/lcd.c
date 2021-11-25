#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "lcd.h"
#include "st7735.h"

static void format_str(char format, uint32_t val, char *output_buf);
static void lcd_write_char(LCD *lcd, char c);
static void lcd_write_string(LCD *lcd, char *s);
static void int_to_dec_str(uint32_t val, char *buf);
static void int_to_hex_str(uint32_t val, char *buf);

void lcd_init(LCD *lcd) {
    lcd->model = &ST7735;
    ST7735_init(lcd->model);
}

void lcd_write(LCD *lcd, char *s, ...) {
    char buf[20];
    va_list list;
    uint32_t val;
    va_start(list, s);

    while (*s != '\0') {
        if (*s == '%') {
            format_str(*++s, va_arg(list, uint32_t), buf);
            lcd_write_string(lcd, buf);
        } else {
            lcd_write_char(lcd, *s);
        }
        s++;
    }
}

static void lcd_write_char(LCD *lcd, char c) {
    ST7735_OutChar(lcd->model, c); 
}

static void lcd_write_string(LCD *lcd, char *s) {
    ST7735_OutString(lcd->model, s);
}

static void format_str(char format, uint32_t val, char *output_buf) {
    switch (format) {
        case '%':
            output_buf[0] = '%';
            output_buf[1] = '\0';
            break;
        case 'd':
        case 'D':
            int_to_dec_str(val, output_buf);
            break;
        case 'x':
        case 'X':
            int_to_hex_str(val, output_buf);
            break;
        case 'b':
        case 'B':
            output_buf[0] = 8;
            output_buf[1] = '\0';
            break;
        default:
            output_buf[0] = '\0';
    }
}

static void int_to_dec_str(uint32_t val, char *buf) {
    uint32_t i = 0;
    char copy[20];
    if (val == 0) {
        copy[i++] = '0';
    } else {
        while (val > 0) {
            copy[i++] = (val % 10) + '0';  // convert to ASCII number
            val /= 10;
        }
    }
    uint32_t j = 0;
    while (i > 0) {
        buf[j++] = copy[--i];
    }
    buf[j] = '\0';
}

static void int_to_hex_str(uint32_t val, char *buf) {
    char hex_convert[] = "abcdef";
    uint32_t i = 0;
    char copy[20];
    if (val == 0) {
        copy[i++] = '0';
    } else {
        while (val > 0) {
            uint8_t tmp = (val & 0xF);
            if (tmp > 9) {
                tmp = hex_convert[tmp - 10];
            } else {
                tmp += '0';
            }
            copy[i++] = tmp;
            val >>= 4;
        }
    }
    uint32_t j = 2;
    buf[0] = '0';
    buf[1] = 'x';
    while (i > 0) {
        buf[j++] = copy[--i];
    }
    buf[j] = '\0';
}

