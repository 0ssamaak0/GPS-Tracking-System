# GPS Tracking System

```C 
LCD_Write("CSE 221 | Introduction to Embedded Systems");
LCD_Set_Cursor(0, 0);
LCD_Write("Final Prject");
```

## Overview

### In this project, we used **Tiva™ C Series TM4C123G LaunchPad** in order to develop a GPS tracking system that is able to calculate the distance between two points<sup id="a1">[1](#f1)</sup> depending on the starting point and the end point.


## Components

- ### Tiva™ C Series TM4C123GXL (ARM Cortex M4F Based) LaunchPad
- ### 16×2 LCD
- ### Ublox NEO-6m GPS Module
- ### USB to TTL Module
- ### LEDs, potentiometer, etc.

## Steps
### 1. Preparing the LCD
#### in the first step, our goal was only to display a sequence of **ASCII** characters on the LCD, with the full control of it's position.
#### In [LCD_Functions.c](https://github.com/0ssamaak0/GPS-Tracking-System/blob/main/LCD_Functions.c) we started by initializing the used pins from *PORT A* and *PORT B*
#### we have known that the **LCD functions** will be used a lot in the project, so we needed to make the code reuseable and readable. so we followed the functional programming paradigm, so our code became nothing more than a set of functions that call each other, we can summarize as following


### 1. The control functions
```C 
void LCD_init();
```
#### calls the initializing functions, clears the LCD and sets the cursor
```C
void LCD_Cmd_init();
```
#### initializes the Command ports
```C
void LCD_Data_init();
```
#### initializes the Data ports
```C
void LCD_Write(char Word[]);
```
#### Taking an array of characters that contains the word to be displayed in the LCD
```C
void LCD_Set_Cursor(int line, int block);
```
#### taking the row and the column, and sets the cursor in the selected position
```C
void LCD_Clear_Block(int line, int block);
```
#### clears a certain block
```C
void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block );
```
#### clears a group of blocks






<b id="f1">1</b> The calculated distance is not completely correct due to inaccuracy in the GPS module itself, we've tried our code with real coordinates, our LCD has shown 259.5m while google maps has shown 260m. [↩](#a1)

