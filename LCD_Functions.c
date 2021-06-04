


void LCD_Cmd(char command){
  GPIO_PORTA_DATA_R = 0X00;
  GPIO_PORTB_DATA_R = command;
  GPIO_PORTA_DATA_R |= 0X80;
  delay(3);
  GPIO_PORTA_DATA_R = 0X00;
  
}
















void LCD_delay(int milliseconds){
    for (int i = 0; i < milliseconds; i++)
    {
        for (int j = 0; j < 3180; j++)
        {
            
        }
        
    }
}
