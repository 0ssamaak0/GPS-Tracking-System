# GPS Tracking System

```C 
LCD_Write("CSE 221 | Introduction to Embedded Systems");
LCD_Set_Cursor(0, 0);
LCD_Write("Final Project");
```

# Overview

In this project, we used **Tiva™ C Series TM4C123G LaunchPad** in order to develop a GPS tracking system that is able to calculate the distance between two points<sup id="a1">[1](#f1)</sup> depending on the starting point and the end point.


# Components

### Tiva™ C Series TM4C123GXL (ARM Cortex M4F Based) LaunchPad
### 16×2 LCD
### Ublox NEO-6m GPS Module
### USB to TTL Module
### LEDs, potentiometer, etc.

# Steps

## 1. The LCD

in the first step, our goal was only to display a sequence of **ASCII** characters on the LCD, with the full control of it's position.

In [LCD_Functions.c](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c) we started by initializing the used pins from *PORT A* and *PORT B*

we have known that the **LCD functions** will be used a lot in the project, so we needed to make the code reuseable and readable. so we followed the functional programming paradigm, so our code became nothing more than a set of functions that call each other<sup id="a2">[2](#f2)</sup>, we can summarize as following


### 1. The control functions
```C 
void LCD_init();
```
calls the initializing functions, clears the LCD and sets the cursor
```C
void LCD_Cmd_init();
```
initializes the Command ports
```C
void LCD_Data_init();
```
initializes the Data ports
```C
void LCD_Write(char Word[]);
```
Taking an array of characters that contains the word to be displayed in the LCD
```C
void LCD_Set_Cursor(int line, int block);
```
taking the row and the column, and sets the cursor in the selected position
```C
void LCD_Clear_Block(int line, int block);
```
clears a certain block
```C
void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block );
```
clears a group of blocks

### 2. The basic instruction functions

```C
void LCD_Clear();
```
Clears the LCD
```C
void LCD_Home();
```
sets the cursor to the beginning
```C
void LCD_Move_Right();
```
Moves the cursor right
```C
void LCD_Move_Left();
```
Moves the cursor left
```C
void LCD_Cursor_On();
```
leaves the cursor bar on
```C
void LCD_Cursor_Off();
```
turns off the cursor bar
```C
void LCD_8Bit();
```
makes the LCD mode 8 bits
```C
void LCD_Cursor_Blink();
```
turns on LCD blinking after typing
```C
void LCD_Shift_Right();
```
shifts current displayed characters right
```C
void LCD_Shift_Left();
```
shifts current displayed characters left

### 3. The delay function

```C
void Delay(int counts, char mode[])
```
Instead of using meaningless delay such as looping in an empty loop, we have used the systick timer, giving us the choice from choosing between 3 modes in counting **us** for microseconds, **ms** for milliseconds, and **sec** for seconds, and of course the number of counts in the selected mode.


## 2. Taking GPS data
we have chosen to use the UART protocol in order to take the GPS data, so our work has divided into two major parts:

### 1. Preparing the UART & Receiving the GPS Data
in this process we created to functions
```C
void UART1_Init(void)
```
which initializes the UART and the pins to be used in **PORT C**
```C
void UART1_receiver(void)
```
This is the core function of the program, in this function we start receiving the data from the GPS module, and starting passing it as a parameters to different functions.

we also write in the LCD in this function<sup id="a3">[3](#f3)</sup>, to synchronize between the receiving process and the displaying process.

## 3. Parsing the Data
The main Parsing process was implemented in **GPRMC_Data_Parser** function in which we searched until we find the **GPRMC** Data and start storing it after checking its availability 

we have also created two functions **String_To_Float** and
**StrDeg_To_FloatDec** which converts the received data into a float, and from degree into decimal respectively.

## 4. Distance Calculation<sup id="4">[4](#f4)</sup>
```C
double Distance_Calc(string Lati1_Str, string Long1_Str, string Lati2_Str, string Long2_Str)
```

in this function, we take the starting point and the end point,  calculating the distance between them and returning it's value to the **UART1_receiver** function.

after calculation and returning the value into the **UART1_receiver**, we tried the system many times, and the margin of error was comparably high, so we suggested that multiplying the result with a **correction factor** will solve this problem. The correction factor was calculated after many tries, so the error was quite predictable. 





# Footnotes
<sup><b id="f1">1</b> The calculated distance is not completely correct due to inaccuracy in the GPS module itself, we've tried our code with real coordinates, our LCD has shown 259.5m while google maps has shown 260m. [↩](#a1)</sup>

<sup><b id="f2">2</b> The implementation of many functions is nothing more than just calling another functions. [↩](#a2)</sup>

<sup><b id="f3">3</b> In terms of design, we'd better to implement this in a separate function, and call this function from the **UART1_receiver** function. [↩](#a3)</sup>

<sup><b id = "f4">4</b> We have no idea about how to calculate the distance between two points given their coordinates, we heavily depended on this [Stack Overflow answer](https://stackoverflow.com/questions/27928/calculate-distance-between-two-latitude-longitude-points-haversine-formula)[↩](#a4)</sub>

