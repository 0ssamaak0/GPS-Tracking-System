#include "LCD_Functions.c"

// Dummy Data
char Total_Dis[] = "127";
char Total_Time[] = "389";
char Current_velocity[] = "5";
char Angle[] = "258";

main(){
// initialize LCD & display default data
  LCD_init();
  LCD_Default_Data();

  // initialize port A pin 4 for turning on 100 distance led
  GPIO_PORTA_DIR_R |= 0X10;
  GPIO_PORTA_DEN_R |= 0X10;
}

// This function will run when the total distance exceed 100m
void dis_100m(void){
  if(int(Total_Dis) >= 100){
    GPIO_PORTA_DATA_R |= 0X10; // power led on
  }
}

int Total_Dis_Calc(int Total_Dis){
  dis_100m();
  }
