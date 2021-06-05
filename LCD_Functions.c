#include <stdint.h>

/* define registers memory location */
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))

#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_AMSEL_R      (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_AFSEL_R      (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_PCTL_R       (*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_CR_R         (*((volatile uint32_t *)0x40004524))

#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_AMSEL_R      (*((volatile uint32_t *)0x40005528))
#define GPIO_PORTB_AFSEL_R      (*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_PCTL_R       (*((volatile uint32_t *)0x4000552C))
#define GPIO_PORTB_CR_R         (*((volatile uint32_t *)0x40005524))

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))

// LCD control functions
void LCD_init();
void LCD_Cmd_init();
void LCD_Data_init();
void LCD_Cmd(char command);
void LCD_Data(char data);
void LCD_Write(char Word[]);
void LCD_Set_Cursor(int line, int block);
void LCD_Clear_Block(int line, int block);
void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block );

// LCD basic instruction functions
void LCD_Clear();
void LCD_Home();
void LCD_Move_Right();
void LCD_Move_Left();
void LCD_Cursor_On();
void LCD_Cursor_Off();
void LCD_8Bit();
void LCD_Cursor_Blink();
void LCD_Shift_Right();
void LCD_Shift_Left();


// Timer function
void Delay(int counts, char mode[]);


vvoid LCD_init(void) {
  LCD_Cmd_init();
  LCD_Data_init();
  
  // Necessary intializing instruction
  LCD_8Bit();
  LCD_Move_Right();
  LCD_Cursor_Off();
  LCD_Clear();
}

void LCD_Cmd_init(void){
  // Clock for port A
  SYSCTL_RCGCGPIO_R |= 0X01;
  while (!(SYSCTL_PRGPIO_R & 0X01)) {};

  // Rs   -> A5
  // Rw   -> A6
  // E    -> A7
  
  // LCD Control Port intialization
  GPIO_PORTA_DIR_R |= 0XE0;
  GPIO_PORTA_DEN_R |= 0XE0;
  GPIO_PORTA_AMSEL_R = 0X00;
  GPIO_PORTA_AFSEL_R = 0X00;
  GPIO_PORTA_PCTL_R = 0X00;
  GPIO_PORTA_CR_R = 0X00;
}
void LCD_Data_init(void){
  // Clock for port B
  SYSCTL_RCGCGPIO_R |= 0X02;
  while (!(SYSCTL_PRGPIO_R & 0X02)) {};

  // Data -> (B0 - B7)
  
  // LCD Data Port intialization
  GPIO_PORTB_DIR_R |= 0XFF;
  GPIO_PORTB_DEN_R |= 0XFF;
  GPIO_PORTB_AMSEL_R = 0X00;
  GPIO_PORTB_AFSEL_R = 0X00;
  GPIO_PORTB_PCTL_R = 0X00;
  GPIO_PORTB_CR_R = 0X00;
}

// Timer control condition variables
int Cmd_Timer_Condition;
int Data_Timer_Condition;

void LCD_Cmd(char command) {
  GPIO_PORTA_DATA_R = 0X00; // RS=0; R/W=0; E=0

  // Sends the commands into the data ports (B)
  GPIO_PORTB_DATA_R = command;

  // Sets and resets the enable, because the command transmission works only between the raising and the falling edge
  GPIO_PORTA_DATA_R |= 0X80;
  Cmd_Timer_Condition = 0;
  Systick_Timer(5, "ms");
  while(Cmd_Timer_Condition == 0){
    if(NVIC_ST_CTRL_R & 0X10000){
      GPIO_PORTA_DATA_R = 0X00;
      Cmd_Timer_Condition = 1;
    }
  }

}

/* Takes the hexacode of the data */
void LCD_data(char data){
    // Sets the Rs
    GPIO_PORTA_DATA_R = 0X20;
    // Sends the data into the data ports (B)
    GPIO_PORTB_DATA_R = data;

    GPIO_PORTA_DATA_R |= 0X80;
    LCD_delay(3);
    GPIO_PORTA_DATA_R = 0X00;

}


// Write string on screen
void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
}


// Clear the screen

void LCD_clear(){
    LCD_command(0X01);
}

void LCD_Clear_Block(int line, int block) {
  LCD_Set_Cursor(line, block);
  LCD_Write(" ");
}

void LCD_Clear_Blocks(int start_line, int end_line, int start_block, int end_block ) {
  int i, j;
  int diff_line = abs(start_line - end_line);
  int diff_block = abs(start_block - end_block);
  for(i=0; i<=diff_line; i++){
    for(j=0; j<=diff_block; j++){
      LCD_Clear_Block(start_line+i, start_block+j);
    }
  }
}


// Takes a line (0, 1) and a block(0, 15)
void LCD_set_Cursor(int line, int block){
    if(line == 0){
        LCD_command(0X80 + block);
    }
    else if (line == 1){
        LCD_command(0xC0 + block);
    }
}

// Start typing form the first line & first block
void LCD_Home(void) {
  LCD_Cmd(0X80);
}

// Move the cursor to the right after every writing process
void LCD_Move_Right(void) {
  LCD_Cmd(0x06);
}

// Move the cursor to the left after every writing process
void LCD_Move_Left(void) {
  LCD_Cmd(0x8);
}

// Show the cursor
void LCD_Cursor_On(void) {
  LCD_Cmd(0x0E);
}

// Hide the cursor
void LCD_Cursor_Off(void) {
  LCD_Cmd(0x0C);
}

// Make the cursor blinking
void LCD_Cursor_Blink(void) {
  LCD_Cmd(0x0F);
}

// Use D0 -> D7 as data pins
void LCD_8Bit(void) {
  LCD_Cmd(0x38);
}

// Shift all the displayed data to the right
void LCD_Shift_Right(void) {
  LCD_Cmd(0x1C);
}

// Shift all the displayed data to the left
void LCD_Shift_Left(void) {
  LCD_Cmd(0x1C);
}

/* End LCD internal instrctions*/


// Initializing systick timer as a delay function
void Delay(int counts, char mode[]){
  int i;
  for(i=0; i<(5*counts); i++){
    NVIC_ST_CTRL_R = 0X00;
    // delay for n Microseconds
    if(mode == "us"){ 
      NVIC_ST_RELOAD_R = (16-1);
    // delay for n Milliseconds
    }else if(mode == "ms"){
      NVIC_ST_RELOAD_R = (16000-1);
    // delay for n Seconds
    }else if(mode == "sec"){
      NVIC_ST_RELOAD_R = (16000000-1);
    }
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 5;
    while((NVIC_ST_CTRL_R & 0X10000) != 0X10000){}
  }
}
