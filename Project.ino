#include "LCD_Functions.c"
#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// To store gps data
char GPS_Data[600];

// gps output counter to store the data
int All_Data_Cursor = 0;

// counter for parsing the data
int Char_Position;

// min & max allowed readings difference between (lati1 & lati2) & (long1 & long2)
double Error1 = 0.000;
double Error2 = 0.0015;

// uart controller using button
int Start_Uart = 1;


char t[] = "0";
char s[] = "0";

// To avoid calc small readings
int Repeated_Lati = 0;
int Repeated_Long = 0;

/* Collector Vars */
String Lati_arr[3];
int Lati_arr_cursor = 0;
String Long_arr[3];
int Long_arr_cursor = 0;
String Ava_arr[1];

// allow calc adding new distance
int Calc_Active_Mode = 0;

// Total counted distance
double Total_Dis = 0.00;

// Total counted time
double Total_Time = 0;


// funcitons decleration
void dis_100m(void);
void UART1_Init(void);
void UART1_receiver(void);
void GPS_Data_Parser(void);
float String_To_Float(String string);
double StrDeg_To_FloatDec(String Deg_cord);
double Distance_Calc(String Lati1_Str, String Long1_Str, String Lati2_Str, String Long2_Str);
double Deg_To_Rad(double deg);



int t_time;
void setup() {
  // initialize LCD & display default data
  LCD_init();
  Serial.begin(9600);
  UART1_Init();

  // initialize port A pin 4 for turning on 100 distance led
  
  SYSCTL_RCGCGPIO_R |= 0x20; 
  while (!(SYSCTL_PRGPIO_R & 0X20)) {};
  GPIO_PORTF_DIR_R |= 0X02;
  GPIO_PORTF_DEN_R |= 0X02;
   
}

void loop() {
// if the button on run gps and start calc distance
  if(GPIO_PORTF_DATA_R == 0X01 & Start_Uart == 1){
    UART1_receiver();
  }
  
  dis_100m(); // when distance >= 100 meters turn on red ligth & stop reading new distance

}

// This function will run when the total distance exceed 100m
void dis_100m(void){
  if(atoi(Total_Dis) >= 100){
    GPIO_PORTA_DATA_R |= 0X10; // power led on
    Start_Uart = 0; // turn off uart
  }
}
void UART1_Init(void) {
  // Enable uart clock
  SYSCTL_RCGCUART_R |= 0X02;

  // Enable GPIO clock
  SYSCTL_RCGCGPIO_R |= 0X04;
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
  char rx_data = 0; // clear data var

  //Data parsing
  while ((UART1_FR_R & 0X10) != 0) { // as fifo is empty do calculation, processing & storing the gps output data
    GPS_Data_Parser();
    if ( Calc_Active_Mode == 1) { // if there're data, start printing, to be ready for new calc process

      itoa(Total_Time, t, 10); // convert the time from int to arr so the function can deal with it and print it on the lcd screen
      LCD_Set_Cursor(1, 6); // set cursor position
      LCD_Write("T="); // Print on the lcd (T), which means total time
      LCD_Write(t); // Print the time
      itoa(Total_Dis, s, 10); // convert the distance from int to arr so we can print it on the lcd screen
      LCD_Set_Cursor(0, 5); // set cursor position
      LCD_Write("Dis:"); LCD_Write(s); LCD_Write("m"); // Print on the lcd (Dis), which means total distance
      Calc_Active_Mode = 2; // calc lvl 2 is ready
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
        Total_Dis += (Distance_Calc(Lati_arr[0], Long_arr[0], Lati_arr[1], Long_arr[1])*(2.00/3.00)); // add the new distance, and multiply it by (2/3) which is the correction factor
      }
      LCD_Set_Cursor(0, 5);
      LCD_Write("Dis:"); LCD_Write(s); LCD_Write("m"); // print the total distance


      Serial.println("...........");
      Serial.print("Total Distance: "); Serial.print(Total_Dis); Serial.println("m");
      Serial.println("...........");
    }

    memset(GPS_Data, 0, 600); // clear the collector var
    All_Data_Cursor = 0; // clear the collector var counter
  }

  //Get the data from the GPS
  rx_data = (char) (UART1_DR_R & 0XFF); // Store GPS output for one char
  GPS_Data[All_Data_Cursor] = rx_data; // Collect All GPS output
  All_Data_Cursor++; // Move cursor to the next char
  Delay(380, "us"); // To avoid some issues in parsing data
  Calc_Active_Mode = 1;
}
  void GPS_Data_Parser(void) {
  Char_Position = 0;
  while (Char_Position != 150) { // check the first 150 chars in the data, cause gprmc is in the first 100 chars so we don't have to check all the data casue it will take more processing & time 
    GPRMC_Data_Parser(); // start parsing the data from our stored var
  }
}

// Parsing the Lati & Long form GPRMC Line
void GPRMC_Data_Parser() {
  // Check if we are in the line of $GPRMC or not yet
  if (GPS_Data[Char_Position] == '$' &
      GPS_Data[Char_Position + 1] == 'G' &
      GPS_Data[Char_Position + 2] == 'P' &
      GPS_Data[Char_Position + 3] == 'R' &
      GPS_Data[Char_Position + 4] == 'M' &
      GPS_Data[Char_Position + 5] == 'C') {
    int comma = 0; // To determine which element we're collecting
    while (GPS_Data[Char_Position] != '\r') { // Chick if the GPRMC line ended or not, if not continue
      if (GPS_Data[Char_Position] == ',') { // Locate the beginning of every element
        Char_Position++;
        
        // get the data after comma 4 which is Avaliblity, so we can know it there and signal & avalible data or not
        if (comma == 1 & GPS_Data[Char_Position] != ',') {
          Serial.println(Total_Time);
          Total_Time++;
          Ava_arr[0] = "";
          while (GPS_Data[Char_Position] != ',') {
            Ava_arr[0] = GPS_Data[Char_Position];
            Char_Position++;
          }
          Serial.print("Avaliblity: "); Serial.println(Ava_arr[0]);

        } 
        
        // get the data after comma 4 which is Lati, if it's avalible
        else if (comma == 2 & GPS_Data[Char_Position] != ',' & Ava_arr[0] == "A") {


          Lati_arr[2] = "";
          while (GPS_Data[Char_Position] != ',') {
            Lati_arr[2] += GPS_Data[Char_Position]; // collect the data
            Char_Position++;
          }
          if (Lati_arr[2].length() >= 8) { // check if the data is correct
            if (Lati_arr_cursor == 0) { // store the first record
              Lati_arr[Lati_arr_cursor] = Lati_arr[2];
              Lati_arr_cursor++;
            } if (Lati_arr_cursor == 1) { // Store the second record

              Lati_arr[1] = Lati_arr[2];
              Lati_arr_cursor++;
              Repeated_Lati = 0;
            } else {
              Repeated_Lati = 1;
            }
            
            // Store the 3rd record and all the next records
            if (Lati_arr_cursor == 2 & abs(StrDeg_To_FloatDec(Lati_arr[2]) - StrDeg_To_FloatDec(Lati_arr[1])) >= Error1 & abs(StrDeg_To_FloatDec(Lati_arr[2]) - StrDeg_To_FloatDec(Lati_arr[1])) <= Error2) {
              Lati_arr[0] = Lati_arr[1];
              Lati_arr[1] = "";
              Lati_arr[1] = Lati_arr[2];
              Repeated_Lati = 0;
            } else {
              Repeated_Lati = 1;
            }
          }

        }
        // get the data after comma 4 which is Long, if it's avalible
        else if (comma == 4 & GPS_Data[Char_Position] != ',' & Ava_arr[0] == "A") {
          Long_arr[2] = "";
          while (GPS_Data[Char_Position] != ',') {
            Long_arr[2] += GPS_Data[Char_Position]; // collect the data
            Char_Position++;
          }
          if (Long_arr[2].length() >= 8) { // check if the data is correct
            if (Long_arr_cursor == 0) { // store the first record
              Long_arr[0] = Long_arr[2];
              Long_arr_cursor++;
            } if (Long_arr_cursor == 1) { // Store the second record
              Long_arr[1] = Long_arr[2];
              Long_arr_cursor++;
              Repeated_Long = 0;
            } else {
              Repeated_Long = 1;
            }
            
            // Store the 3rd record and all the next records
            if (Long_arr_cursor == 2 & abs(StrDeg_To_FloatDec(Long_arr[2]) - StrDeg_To_FloatDec(Long_arr[1])) >= Error1 & abs(StrDeg_To_FloatDec(Long_arr[2]) - StrDeg_To_FloatDec(Long_arr[1])) <= Error2) {
              Long_arr[0] = Long_arr[1];
              Long_arr[1] = "";
              Long_arr[1] = Long_arr[2];
              Repeated_Long = 0;
            }
            else {
              Repeated_Long = 1;
            }
          }

        }
        comma++;
      } else {
        Char_Position++;
      }

    }
  }
  Char_Position++;
}
  

// convert String to float  
  float String_To_Float(String string) {
  char arr[string.length()];
  int string_cursor;
  for (string_cursor = 0; string_cursor < string.length(); string_cursor++) {
    arr[string_cursor] = string[string_cursor];
  }
  char* pend;
  float Float = strtof(arr, &pend);
  return Float;
}

// convert Lati & Long from Deg & type string to Decimal & type float
double StrDeg_To_FloatDec(String Deg_cord) {
  char Deg_arr[15];
  char Min_arr[15];
  char Sec_arr[15];
  int i;
  String Deg_cord_2[1];
  if (Deg_cord[0] == '0') {
    for (i = 1; i < Deg_cord.length(); i++) {
      Deg_cord_2[0] += Deg_cord[i];
    }
    Deg_cord = Deg_cord_2[0];
  }
  int Cursor;

  int Min_Cursor = 0;
  int Sec_Cursor = 0;
  for (Cursor = 0; Cursor < 11; Cursor++) {
    if (Cursor < 2) {
      Deg_arr[Cursor] = Deg_cord[Cursor];
    } else if (Cursor < 4 & Cursor >= 2) {
      Min_arr[Min_Cursor] = Deg_cord[Cursor];
      Min_Cursor++;
    } else if (Cursor > 4) {
      Sec_arr[Sec_Cursor] = Deg_cord[Cursor];
      Sec_Cursor++;
      if (Cursor == 6) {
        Sec_arr[Sec_Cursor] = '.';
        Sec_Cursor++;
      }
    }
  }

  char* pend;
  double Deg = strtof(Deg_arr, &pend);
  double Min = strtof(Min_arr, &pend);
  double Sec = strtof(Sec_arr, &pend);
  double Dec = Deg + (Min / 60.00) + (Sec / 3600.00);

  return Dec;
}


// calculating distance using Lati & Long
double Distance_Calc(String Lati1_Str, String Long1_Str, String Lati2_Str, String Long2_Str) {

  // convert Long & Lati from degree to Decimal
  double Lati1 = StrDeg_To_FloatDec(Lati1_Str);
  double Long1 = StrDeg_To_FloatDec(Long1_Str);
  double Lati2 = StrDeg_To_FloatDec(Lati2_Str);
  double Long2 = StrDeg_To_FloatDec(Long2_Str);
  
  // Starting calculations
  double R = 6371000; // Radius of the earth in meter
  double Lati = Deg_To_Rad(Lati2 - Lati1);
  double Long = Deg_To_Rad(Long2 - Long1);
  double a =
    sin(Lati / 2) * sin(Lati / 2) +
    cos(Deg_To_Rad(Lati1)) * cos(Deg_To_Rad(Lati2)) *
    sin(Long / 2) * sin(Long / 2)
    ;
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = R * c; // Distance in meter
  return d;
}

double Deg_To_Rad(double deg) {
  return deg * (PI / 180);
}
