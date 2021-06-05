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

// Timer function
void Delay(int counts, char mode[]);


void LCD_init(void) {
  LCD_Cmd_init();
  LCD_Data_init();
  
  // Necessary intializing instruction
  LCD_8Bit();
  LCD_Move_Right();
  LCD_Cursor_Off();
  LCD_Clear();
}

 // Data Port
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

  
  LCD_8Bit();
  LCD_Move_Right();
  LCD_Cursor_Off();
  LCD_Clear();

}


void LCD_Cmd(char command) {
  GPIO_PORTA_DATA_R &= 0X1F; // R/W=0; E=0; RS=0
  GPIO_PORTB_DATA_R = command;

  GPIO_PORTA_DATA_R |= 0X80;
  Delay(3, "ms");
  GPIO_PORTA_DATA_R &= 0X1F;
  
}


// Write string on screen
void LCD_Write(char Word[]) {
  int i;
  for(i=0; i<strlen(Word); i++) 
  LCD_Data(Word[i]);
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

void LCD_clear(){
    LCD_command(0X01);
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
