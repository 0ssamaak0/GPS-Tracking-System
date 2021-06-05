#include "LCD_Functions.c"

// Dummy Data
char Total_Dis[] = "127";
char Total_Time[] = "389";
char Current_velocity[] = "5";
char Angle[] = "258";



void setup() {
  
  
}

void loop() {
  
  
  
  // dummy data
void LCD_Default_Data(void) {
  LCD_Home();
  LCD_Write("D="); LCD_Write(Total_Dis); LCD_Write("m"); LCD_Set_Cursor(0,7); LCD_Write(",T="); LCD_Write(Total_Time); LCD_Write("s");
  LCD_Set_Cursor(1,0); LCD_Write("V="); LCD_Write(Current_velocity); LCD_Write("m/s"); LCD_Set_Cursor(1,7); LCD_Write(",A="); LCD_Write(Angle); LCD_Write("deg");
}
