void LCD_init()
{
    SYSCTL_RCGCGPIO_R = 0X03;               // 0000 0011
    while(!(SYSCTL_PRGPIO_R & 0X03)){}

    GPIO_PORTA_DIR_R = 0XE0;                // 1110 0000 (Rs, Rw, E)
    GPIO_PORTA_DEN_R = 0XE0;                 
    GPIO_PORTB_DIR_R = 0XFF;                // 1111 1111
    GPIO_PORTB_DEN_R = 0XFF;

    LCD_command(0X38);

    LCD_clear();

    LCD_command(0X02);
}
