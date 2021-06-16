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
[LCD_init](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L59) calls the initializing functions, clears the LCD and sets the cursor

[LCD_Cmd_init](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L70) initializes the Command ports

[LCD_Data_init](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L87) initializes the Data ports

[LCD_Write](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L135)
Taking an array of characters that contains the word to be displayed in the LCD

[LCD_Set_Cursor](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L144) taking the row and the column, and sets the cursor in the selected position\

[LCD_Clear_Block](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L164)
clears a certain block

[LCD_Clear_Blocks](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L170)
clears a group of blocks

### 2. The basic instruction functions


[LCD_Clear](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L158)
Clears the LCD

[LCD_Home](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L185)
sets the cursor to the beginning

[LCD_Move_Right](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L190)
Moves the cursor right

[LCD_Move_Left](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L195)
Moves the cursor left

[LCD_Cursor_On](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L200)
leaves the cursor bar on

[LCD_Cursor_Off](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L205)
turns off the cursor bar

[LCD_8Bit](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L215)
makes the LCD mode 8 bits

[LCD_Cursor_Blink](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#210)
turns on LCD blinking after typing

[LCD_Shift_Right](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#220)
shifts current displayed characters right

[LCD_Shift_Left](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#225)
shifts current displayed characters left

### 3. [The delay function](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L243)

Instead of using meaningless delay such as looping in an empty loop, we have used the systick timer, giving us the choice from choosing between 3 modes in counting **us** for microseconds, **ms** for milliseconds, and **sec** for seconds, and of course the number of counts in the selected mode.


## 2. Taking GPS data
we have chosen to use the UART protocol in order to take the GPS data, so our work has divided into two major parts:

### 1. Preparing the UART & Receiving the GPS Data
in this process we created to functions

[void UART1_Init](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L94)
which initializes the UART and the pins to be used in **PORT C**

[UART1_receiver](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L123)
This is the core function of the program, in this function we start receiving the data from the GPS module, and starting passing it as a parameters to different functions.

we also [write in the LCD](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L132) in this function<sup id="a3">[3](#f3)</sup>, to synchronize between the receiving process and the displaying process.

## 3. Parsing the Data
The main Parsing process was implemented in [GPRMC_Data_Parser](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c#L182) function in which we searched until we find the **GPRMC** Data and start storing it after checking its availability 

we have also created two functions [String_To_Float](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L286) and [StrDeg_To_FloatDec](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L298) which converts the received data into a float, and from degree into decimal respectively.

## 4. Distance Calculation<sup id="4">[4](#f4)</sup>

[Distance_Calc](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/Project.ino#L341)

in this function, we take the starting point and the end point,  calculating the distance between them and returning it's value to the **UART1_receiver** function.

after calculation and returning the value into the **UART1_receiver**, we tried the system many times, and the margin of error was comparably high, so we suggested that multiplying the result with a **correction factor** will solve this problem. The correction factor was calculated after many tries, so the error was quite predictable. 


# Footnotes
<sup><b id="f1">1</b> The calculated distance is not completely correct due to inaccuracy in the GPS module itself, we've tried our code with real coordinates, our LCD has shown 259.5m while google maps has shown 260m. [↩](#a1)</sup>

<sup><b id="f2">2</b> The implementation of many functions is nothing more than just calling another functions. [↩](#a2)</sup>

<sup><b id="f3">3</b> In terms of design, we'd better to implement this in a separate function, and call this function from the **UART1_receiver** function. [↩](#a3)</sup>

<sup><b id = "f4">4</b> We have no idea about how to calculate the distance between two points given their coordinates, we heavily depended on this [Stack Overflow answer](https://stackoverflow.com/questions/27928/calculate-distance-between-two-latitude-longitude-points-haversine-formula)[↩](#a4)</sub>

