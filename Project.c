#include "LCD_Functions.c"

// Dummy Data
char Total_Dis[] = "127";
char Total_Time[] = "0";
char Current_velocity[] = "5";
char Angle[] = "258";

int t_time = 0;

main(){
// initialize LCD & display default data
  LCD_init();
  LCD_Default_Data();

  // initialize port A pin 4 for turning on 100 distance led
  GPIO_PORTA_DIR_R |= 0X10;
  GPIO_PORTA_DEN_R |= 0X10;
}

void loop(){
  while(1){
  LCD_Default_Data();
  Delay(1,"sec");
  t_time = atoi(Total_Time)+1;
  itoa(t_time, Total_Time, 10);
  
  
  Total_Dis_Calc(Total_Dis);
    
  }
}

// This function will run when the total distance exceed 100m
void dis_100m(void){
  if(atoi(Total_Dis) >= 100){
    GPIO_PORTA_DATA_R |= 0X10; // power led on
  }
}

// claculate total distance
int Total_Dis_Calc(char Dis[]){
  int Total_distance;
  dis_100m();

  return Total_distance;
  }


// dummy data
void LCD_Default_Data(void) {
  LCD_Home();
  LCD_Write("D="); LCD_Write(Total_Dis); LCD_Write("m"); LCD_Set_Cursor(0,7); LCD_Write(",T="); LCD_Write(Total_Time); LCD_Write("s");
  LCD_Set_Cursor(1,0); LCD_Write("V="); LCD_Write(Current_velocity); LCD_Write("m/s"); LCD_Set_Cursor(1,7); LCD_Write(",A="); LCD_Write(Angle); LCD_Write("deg");
}
