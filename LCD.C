#include "project.h"
#define LCD_d ((1<<18)|(1<<19)|(1<<21)|(1<<22)|(1<<23)|(1<<24)|(1<<25)|(1<<27))
#define RS (1<<8)
#define E  (1<<9)

void LCD_WRITE(unsigned char value)
{
    IOCLR0 = LCD_d;

    if(value & 0x01) IOSET0 = (1<<18);
    if(value & 0x02) IOSET0 = (1<<19);
    if(value & 0x04) IOSET0 = (1<<21);
    if(value & 0x08) IOSET0 = (1<<22);
    if(value & 0x10) IOSET0 = (1<<23);
    if(value & 0x20) IOSET0 = (1<<24);
    if(value & 0x40) IOSET0 = (1<<25);
    if(value & 0x80) IOSET0 = (1<<27);
}

void LCD_COMMAND(unsigned char cmd)
{
    LCD_WRITE(cmd);

    IOCLR0 = RS;
    IOSET0 = E;
    delay_milliseconds(2);
    IOCLR0 = E;

    delay_milliseconds(2);
}

void LCD_DATA(unsigned char d)
{
    LCD_WRITE(d);

    IOSET0 = RS;
    IOSET0 = E;
    delay_milliseconds(2);
    IOCLR0 = E;

    delay_milliseconds(2);
}

void LCD_INIT(void)
{
    IODIR0 |= LCD_d | RS | E;
PINSEL0 &= ~((3<<16) | (3<<18));

PINSEL1 &= ~((3<<4)  | (3<<6)  | (3<<10) | (3<<12) |
             (3<<14) | (3<<16) | (3<<18) | (3<<22));

    IOCLR0 = LCD_d | RS | E;

    delay_milliseconds(20);

    LCD_COMMAND(0x38);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x01);
    LCD_COMMAND(0x06);
}

void LCD_STR(unsigned char *s)
{
    while(*s)
    {
        LCD_DATA(*s++);
    }
}

void LCD_INTEGER(int n)
{
    int i = 0;
    unsigned char arr[10];

    if(n == 0)
    {
        LCD_DATA('0');
        return;
    }

    if(n < 0)
    {
        LCD_DATA('-');
        n = -n;
    }

    while(n > 0)
    {
        arr[i++] = n % 10;
        n = n / 10;
    }

    while(i > 0)
    {
        LCD_DATA(arr[--i] + 48);
    }
}