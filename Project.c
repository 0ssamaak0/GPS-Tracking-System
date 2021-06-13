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
void UART1_Init(void) {
  // Enable uart clock
  SYSCTL_RCGCUART_R |= 0X02; //0000 0010

  // Enable GPIO clock
  SYSCTL_RCGCGPIO_R |= 0X04; //0000 0100
  // disable the UART for configuration
  UART1_CTL_R = 0;

  //calculate the values of the division registers
  UART1_IBRD_R = 520;
  UART1_FBRD_R = 53;
  UART1_CC_R = 0;

  // set the configuration of the line control register
  UART1_LCRH_R |= 0X70;

  //Enable the control back again
  UART1_CTL_R |= 0X0201;

  // GPIO configuration

  //enable the alternate function select
  GPIO_PORTC_AFSEL_R |= 0X30;
  // Choose the UART protocol
  GPIO_PORTC_PCTL_R |= GPIO_PCTL_PC4_U1RX | GPIO_PCTL_PC5_U1TX;
  GPIO_PORTC_DEN_R |= 0X30;

}
void UART1_receiver(void) {
  char rx_data = 0; //

  //Data parsing
  while ((UART1_FR_R & 0X10) != 0) {
    GPS_Data_Parser();
    if ( Calc_Active_Mode == 1) {

      itoa(Total_Time, t, 10);
      LCD_Set_Cursor(1, 6);
      LCD_Write("T=");
      LCD_Write(t);
      itoa(Total_Dis, s, 10);
      LCD_Set_Cursor(0, 5);
      LCD_Write("Dis:"); LCD_Write(s); LCD_Write("m");
      Calc_Active_Mode = 2;
    }
    
    if ( Calc_Active_Mode == 2 & Ava_arr[0] == "A" & Lati_arr[0].length()>=10 & Long_arr[0].length()>=10 & Lati_arr[1].length()>=10 & Long_arr[1].length()>=10) {
      Calc_Active_Mode = 0;

      double Lati1 = StrDeg_To_FloatDec(Lati_arr[0]);
      double Long1 = StrDeg_To_FloatDec(Long_arr[0]);
      double Lati2 = StrDeg_To_FloatDec(Lati_arr[1]);
      double Long2 = StrDeg_To_FloatDec(Long_arr[1]);

      Serial.print("Lati: "); Serial.print(Lati1 * 10000000); Serial.print(" ---> "); Serial.print(Lati2 * 10000000); Serial.print(" ---> "); Serial.println(Repeated_Lati);
      Serial.print("Long: "); Serial.print(Long1 * 10000000); Serial.print(" ---> "); Serial.print(Long2 * 10000000); Serial.print(" ---> "); Serial.println(Repeated_Long);

      if (Lati1 != 0.00 & Long1 != 0.00 & Lati2 != 0.00 & Long2 != 0.00 & Repeated_Lati == 0 & Repeated_Long == 0) {
        Total_Dis += (Distance_Calc(Lati_arr[0], Long_arr[0], Lati_arr[1], Long_arr[1])*(2.00/3.00));
      }
      LCD_Set_Cursor(0, 5);
      LCD_Write("Dis:"); LCD_Write(s); LCD_Write("m");


      Serial.println("...........");
      Serial.print("Total Distance: "); Serial.print(Total_Dis); Serial.println("m");
      Serial.println("...........");
    }

  //Get the data from the GPS
  rx_data = (char) (UART1_DR_R & 0XFF); // Store GPS output for one char
  GPS_Data[All_Data_Cursor] = rx_data; // Collect All GPS output
  All_Data_Cursor++; // Move cursor to the next char
  Delay(380, "us"); // To avoid some issues in parsing data
  Calc_Active_Mode = 1;
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
