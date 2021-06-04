// Write string on screen
void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
}

// Start typing form the first line & first block
void LCD_Home(void) {
  LCD_Cmd(0X80);
}

// Move the cursor to the right after every writing process
void LCD_Move_Right(void) {
  LCD_Cmd(0x06);
}

// Move the cursor to the left after every writing process
void LCD_Move_Left(void) {
  LCD_Cmd(0x8);
}

