#ifndef _LCD_H_
#define _LCD_H_

typedef struct LCD {
    void *model;
} LCD;

void lcd_init(LCD *lcd);
void lcd_write(LCD *lcd, char *s, ...);
void lcd_backspace(LCD *lcd, uint32_t count);

#endif /* _LCD_H_ */