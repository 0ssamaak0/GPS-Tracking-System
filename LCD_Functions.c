void LCD_init(){

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

// Write string on screen


void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
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
