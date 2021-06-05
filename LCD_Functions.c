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

void LCD_init(){

    SYSCTL_RCGCGPIO_R = 0X03;               // 0000 0011
    while(!(SYSCTL_PRGPIO_R & 0X03)){}

    GPIO_PORTA_DIR_R = 0XE0;                // 1110 0000 (Rs, Rw, E)
    GPIO_PORTA_DEN_R = 0XE0;                 
    GPIO_PORTB_DIR_R = 0XFF;                // 1111 1111
    GPIO_PORTB_DEN_R = 0XFF;

    LCD_command(0X38);

    LCD_clear();

    LCD_command(0X02);
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


void LCD_Cmd(char command){
  GPIO_PORTA_DATA_R = 0X00;
  GPIO_PORTB_DATA_R = command;
  GPIO_PORTA_DATA_R |= 0X80;
  delay(3);
  GPIO_PORTA_DATA_R = 0X00;
  
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














void Systick_Timer(int counts, char mode[]){
  NVIC_ST_CTRL_R = 0X00;
  if(mode == "us"){
    NVIC_ST_RELOAD_R = counts*(16-1);
  }else if(mode == "ms"){
    NVIC_ST_RELOAD_R = counts*(16000-1);
  }
  else if(mode == "sec"){
    NVIC_ST_RELOAD_R = counts*(16000000-1);
  }
  NVIC_ST_CURRENT_R = 0;
  NVIC_ST_CTRL_R = 5;
}
