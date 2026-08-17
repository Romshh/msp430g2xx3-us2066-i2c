# msp430g2xx3-us2066-i2c

This is a C library for driving 16x2 character displays with a US2066 controller from an MSP430G2xx3 microcontroller over I2C.

## Hardware

| | |
| --- | --- |
| MCU | MSP430G2553 on an MSP-EXP430G2 LaunchPad |
| Display | Newhaven NHD-0216MW-AB3, I2C mode, address 0x3C |
| Supply | 3.3 V |
| Clock | SMCLK at 1 MHz (DCO, factory calibration) |

## Wiring

    Display        LaunchPad
    -------        ---------
    SCL     ->     P1.6
    SDA     ->     P1.7
    VDD     ->     3V3
    VSS     ->     GND


On the MSP-EXP430G2, **remove the green LED jumper on J5**. P1.6 is shared with
the green LED on this board, and leaving the jumper in loads the SCL line. The
display stays silent while the software looks perfectly fine. This small issue cost me a
morning.

## Supported devices

Works unchanged on MSP430G2 parts that have **USCI_B0** with I2C on P1.6/P1.7.
If the part number ends in **3** it has USCI and this works.

Parts ending in 1 or 2 have **USI** instead (G2452, G2232, G2212 and so on). USI
is a different peripheral with different registers, so `i2c_init()` and
`lcd_send()` would have to be rewritten.

On the larger parts (G2444, G2544, G2744, G2755, G2855, G2955) the registers are
the same, but USCI_B0 may sit on different pins, check the datasheet and adjust
the two `P1SEL` lines in `i2c_init()`.

## API

```c
void i2c_init(void);

void lcd_init(void);
void lcd_clear(void);
void lcd_line(unsigned char line);                  /* 0 or 1 */
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_puts(const char *s);
void lcd_send(unsigned char ctrl, const char *s, unsigned int len);
void lcd_num(unsigned int n);                       /* 0 - 65535 */
```

`lcd_send()` takes a length instead of a terminator, so you can print part of a
longer string without copying it.

## Usage

```c
#include <msp430.h>
#include "lcd.h"

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    i2c_init();
    lcd_init();
    lcd_clear();

    lcd_line(0);
    lcd_puts("Hello");
    lcd_line(1);
    lcd_num(1234);

    for (;;);
}
```

Call `i2c_init()` yourself. `lcd_init()` does not do it, because the I2C
peripheral is shared.

The clock lines are required. `i2c_init()` uses `UCB0BR0 = 10` for 100 kHz, which
assumes SMCLK is 1 MHz.

## Building

Code Composer Studio, TI v21.6.2 compiler, target MSP430G2553. Import the folder
with `File > Import > CCS Projects`.

Nothing here is CCS-specific; `msp430-gcc` works too with your own linker command
file.

## License

MIT. See [LICENSE](LICENSE).
