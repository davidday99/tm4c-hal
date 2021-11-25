#ifndef _LCD_H_
#define _LCD_H_

typedef struct LCD {
    void *model;
} LCD;

void lcd_init(struct LCD *lcd);
void lcd_write(struct LCD *lcd, char *s, ...);

#endif /* _LCD_H_ */