Skip to content
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 
@0ssamaak0 
0ssamaak0
/
GPS-Tracking-System
2
26
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
Settings
GPS-Tracking-System/README.md
@0ssamaak0
0ssamaak0 Update README.md
Latest commit 18f19b3 2 minutes ago
 History
 6 contributors
@0ssamaak0@FadiiBassem@MostafaHamdyAbdelhady@OmarGamal85@Mokhtar-2000@huss0123
139 lines (110 sloc)  3.82 KB
  
GPS Tracking System
LCD_Write("CSE 221 | Introduction to Embedded Systems");
LCD_Set_Cursor(0, 0);
LCD_Write("Final Prject");
Overview
In this project, we used Tiva™ C Series TM4C123G LaunchPad in order to develop a GPS tracking system that is able to calculate the distance between two points1 depending on the starting point and the end point.
Components
Tiva™ C Series TM4C123GXL (ARM Cortex M4F Based) LaunchPad
16×2 LCD
Ublox NEO-6m GPS Module
USB to TTL Module
LEDs, potentiometer, etc.
Steps
1. The LCD
in the first step, our goal was only to display a sequence of ASCII characters on the LCD, with the full control of it's position.

In LCD_Functions.c we started by initializing the used pins from PORT A and PORT B

we have known that the LCD functions will be used a lot in the project, so we needed to make the code reuseable and readable. so we followed the functional programming paradigm, so our code became nothing more than a set of functions that call each other2, we can summarize as following

1. The control functions
void LCD_init();
calls the initializing functions, clears the LCD and sets the cursor

void LCD_Cmd_init();
initializes the Command ports

void LCD_Data_init();
initializes the Data ports

void LCD_Write(char Word[]);
Taking an array of characters that contains the word to be displayed in the LCD

void LCD_Set_Cursor(int line, int block);
taking the row and the column, and sets the cursor in the selected position

void LCD_Clear_Block(int line, int block);
clears a certain block

void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block );
clears a group of blocks

2. The basic instruction functions
void LCD_Clear();
Clears the LCD

void LCD_Home();
sets the cursor to the beginning

void LCD_Move_Right();
Moves the cursor right

void LCD_Move_Left();
Moves the cursor left

void LCD_Cursor_On();
leaves the cursor bar on

void LCD_Cursor_Off();
turns off the cursor bar

void LCD_8Bit();
makes the LCD mode 8 bits

void LCD_Cursor_Blink();
turns on LCD blinking after typing

void LCD_Shift_Right();
shifts current displayed characters right

void LCD_Shift_Left();
shifts current displayed characters left

3. The delay function
void Delay(int counts, char mode[])
Instead of using meaningless delay such as looping in an empty loop, we have used the systick timer, giving us the choice from choosing between 3 modes in counting us for microseconds, ms for milliseconds, and sec for seconds, and of course the number of counts in the selected mode.

2. Taking GPS data
we have chosen to use the UART protocol in order to take the GPS data, so our work has divided into two major parts:

1. Preparing the UART
in this process we created to functions

void UART1_Init(void)
which initializes the UART and the pins to be used in PORT C

void UART1_receiver(void)
This is the core function of the program, in this function we start receiving the data from the GPS module, and starting passing it as a parameters to different functions.

we also write in the LCD in this function, to synchronize between the receiving process and the displaying process.

Footnotes
1 The calculated distance is not completely correct due to inaccuracy in the GPS module itself, we've tried our code with real coordinates, our LCD has shown 259.5m while google maps has shown 260m. ↩

2 The implementation of many functions is nothing more than just calling another functions↩

© 2021 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
