/* https://github.com/Romshh */

#include <msp430.h>
#include "lcd.h"

const char banner[] = "                MSP430G2553 driving a US2066 OLED over I2C.                ";

void main(void)
{
    unsigned int len = 0;
    unsigned int i = 0;
    unsigned int frame = 0;

    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    __delay_cycles(300000);

    i2c_init();
    lcd_init();
    lcd_clear();

    while (banner[len]) 
    {
        len++;
    }

    for(;;)
    {
        lcd_gotoxy(0,0);
        lcd_send(LCD_DATA, banner + i, 16);

        lcd_line(1);
        lcd_puts("                ");
        lcd_line(1);
        lcd_puts("frame: ");
        lcd_num(frame);

        __delay_cycles(220000);

        i++;
        if (i + 16 > len) {
            i = 0;
        }

        frame++;
    }
}
