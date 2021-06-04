
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