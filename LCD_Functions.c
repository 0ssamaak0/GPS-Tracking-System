
// Write string on screen
void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
}
















void LCD_delay(int milliseconds){
    for (int i = 0; i < milliseconds; i++)
    {
        for (int j = 0; j < 3180; j++)
        {
            
        }
        
    }
}
