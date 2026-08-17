/* https://github.com/Romshh */

#ifndef LCD_H
#define LCD_H

#define LCD_ADDR    0x3C

#define LCD_CMD     0x00
#define LCD_DATA    0x40

void i2c_init(void);
void lcd_init(void);
void lcd_clear(void);
void lcd_line(unsigned char line);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_puts(const char *s);
void lcd_send(unsigned char ctrl, const char *s, unsigned int len);
void lcd_num(unsigned int n);

#endif