
// Write string on screen
void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
}


void LCD_Cmd(char command){
  GPIO_PORTA_DATA_R = 0X00;
  GPIO_PORTB_DATA_R = command;
  GPIO_PORTA_DATA_R |= 0X80;
  delay(3);
  GPIO_PORTA_DATA_R = 0X00;
  
}


// Takes a line (0, 1) and a block(0, 15)
void LCD_set_Cursor(int line, int block){
    if(line == 0){
        LCD_command(0X80 + block);
    }
    else if (line == 1){
        LCD_command(0xC0 + block);
    }
}














void LCD_delay(int milliseconds){
    for (int i = 0; i < milliseconds; i++)
    {
        for (int j = 0; j < 3180; j++)
        {
            
        }
        
    }
}
