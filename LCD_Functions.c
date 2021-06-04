
void LCD_init(void)
{
  // Rs   -> A5
  // Rw   -> A6
  // E    -> A7
  // Data -> (B0 - B7)

  // Clock for port A & B
  SYSCTL_RCGCGPIO_R |= 0X03;
  while (!(SYSCTL_PRGPIO_R & 0X03)) {};

  GPIO_PORTA_DIR_R |= 0XE0;
  GPIO_PORTA_DEN_R |= 0XE0;
  GPIO_PORTA_AMSEL_R = 0X00;
  GPIO_PORTA_AFSEL_R = 0X00;
  GPIO_PORTA_PCTL_R = 0X00;
  GPIO_PORTA_CR_R = 0X00;

  GPIO_PORTB_DIR_R |= 0XFF;
  GPIO_PORTB_DEN_R |= 0XFF;
  GPIO_PORTB_AMSEL_R = 0X00;
  GPIO_PORTB_AFSEL_R = 0X00;
  GPIO_PORTB_PCTL_R = 0X00;
  GPIO_PORTB_CR_R = 0X00;

  LCD_8Bit();
  LCD_Move_Right();
  LCD_Cursor_Off();
  LCD_Clear();

}
