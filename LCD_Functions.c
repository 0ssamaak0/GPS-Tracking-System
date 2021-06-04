void LCD_init(void)
{
  SYSCTL_RCGCGPIO_R |= 0X03;
  while (!(SYSCTL_PRGPIO_R & 0X03)) {};

  GPIO_PORTA_DIR_R |= 0XE0;
  GPIO_PORTA_DEN_R |= 0XE0;

  GPIO_PORTB_DIR_R |= 0XFF;
  GPIO_PORTB_DEN_R |= 0XFF;

  LCD_8Bit();
  LCD_Move_Right();
  LCD_Cursor_Off();
  LCD_Clear()
}
