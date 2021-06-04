void LCD_init(void){
  Serial.begin(9600);
  SYSCTL_RCGCGPIO_R |= 0X03;
  
  GPIO_PORTA_DIR_R |= 0XE0;
  GPIO_PORTA_DEN_R |= 0XE0;

  GPIO_PORTB_DIR_R |= 0XFF;
  GPIO_PORTB_DEN_R |= 0XFF;

  LCD_Cmd(0X38);
  LCD_Cmd(0x06);
  LCD_Cmd(0X0F);
  LCD_Cmd(0X01);

}

void LCD_Cmd(char command){
  GPIO_PORTA_DATA_R = 0X00;
  GPIO_PORTB_DATA_R = command;
  GPIO_PORTA_DATA_R |= 0X80;
  delay(3);
  GPIO_PORTA_DATA_R = 0X00;
  
}

// LCD control functions














/* Start LCD internal instrctions*/
// Clear the screen

void LCD_Clear_Block(int line, int block) {
  LCD_Set_Cursor(line, block);
  LCD_Write(" ");
}

void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block ) {
  int i, j;
  int diff_line = abs(start_line - end_line);
  int diff_block = abs(start_block - end_block);
  for(i=0; i<=diff_line; i++){
    for(j=0; j<=diff_block; j++){
      LCD_Clear_Block(start_line+i, start_block+j);
    }
  }
}
