/* https://github.com/Romshh */

#include <msp430.h>
#include "lcd.h"

void i2c_init(void)
{
    P1SEL |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    UCB0CTL1 |= UCSWRST;

    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0CTL1 = UCSWRST | UCSSEL_2;

    UCB0BR0 = 10;
    UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;
}

void lcd_send(unsigned char ctrl, const char *s, unsigned int len)
{
    UCB0I2CSA = LCD_ADDR;
    UCB0CTL1 |= UCTR | UCTXSTT;

    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = ctrl;

    while (len--) {
        while (!(IFG2 & UCB0TXIFG));
        UCB0TXBUF = *s++;
    }

    while (!(IFG2 & UCB0TXIFG));
    
    UCB0CTL1 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
}

static void lcd_command(unsigned char c)
{
    lcd_send(LCD_CMD, (const char *)&c, 1);
}

void lcd_init(void)
{
    lcd_command(0x2A); lcd_command(0x71); lcd_send(LCD_DATA, "\x00", 1);
    lcd_command(0x28); lcd_command(0x08);
    lcd_command(0x2A); lcd_command(0x79);
    lcd_command(0xD5); lcd_command(0x70);
    lcd_command(0x78); lcd_command(0x08); lcd_command(0x06);
    lcd_command(0x72); lcd_send(LCD_DATA, "\x00", 1);
    lcd_command(0x2A); lcd_command(0x79);
    lcd_command(0xDA); lcd_command(0x00);
    lcd_command(0xDC); lcd_command(0x00);
    lcd_command(0x81); lcd_command(0x7F);
    lcd_command(0xD9); lcd_command(0xF1);
    lcd_command(0xDB); lcd_command(0x40);
    lcd_command(0x78); lcd_command(0x28);

    lcd_command(0x01);
    __delay_cycles(20000);
    lcd_command(0x0C);
    __delay_cycles(100000);
}

void lcd_clear(void)
{
    lcd_command(0x01);
    __delay_cycles(20000);
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char addr;

    if (y == 0) {
        addr = 0x00;
    }
    else {
        addr = 0x40;
    }

    lcd_command(0x80 | (addr + x));
}

void lcd_line(unsigned char line)
{
    lcd_gotoxy(0, line);
}

void lcd_puts(const char *s)
{
    unsigned int n = 0;

    while (s[n]) {
        n++;
    }

    lcd_send(LCD_DATA, s, n);
}

void lcd_num(unsigned int n)
{
    unsigned int count = 0;
    char holder[16];
    unsigned int temp = n;
    unsigned int tempcount = 0;

    while (temp > 0) {
        count++;
        temp = temp / 10;
    }

    temp = n;
    tempcount = count;

    if (count > 0 && n != 0) {

        for (; count > 0; count--) {
            holder[count - 1] = '0' + (temp % 10);
            temp = temp / 10;
        }

        holder[tempcount] = '\0';
        lcd_puts(holder);
    }
    else if (n == 0) {
        lcd_puts("0");
    }
}