#define F_CPU 16000000 UL

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

#define LED_RED PD7
#define LED_GREEN PD6
#define LED_YELLOW PD5

void USART_setup(){
  UBRR0 = 103; // 9600bps

  set_bit(UCSR0B, TXCIE0); // Bit 7 - RXCIEn - RX Complete Interrupt Enable
  set_bit(UCSR0B, TXEN0);  // Bit 3 - TXENn — Transmitter Enable
  set_bit(UCSR0B, TXB80); // Character size 8-bit
  
  set_bit(UCSR0C, UCSZ01);
  set_bit(UCSR0C, UCSZ00);
}

void LEDs_setup(){
  set_bit(DDRD, LED_RED);
  set_bit(DDRD, LED_GREEN);
  set_bit(DDRD, LED_YELLOW);
}

int main(void){
  USART_setup();
  LEDs_setup();

  for(;;);
  
  return 0;
}

ISR(USART0_RX_vect) {
  char data = '\0';
  data = UDR0;
  
  if(data == 'G'){
    toggle_bit(PORTD, LED_GREEN);
  }else if(data == 'R'){
    toggle_bit(PORTD, LED_RED);
  }else if(data == 'Y'){
    toggle_bit(PORTD, LED_YELLOW);
  }else if(data == 'C'){
    clear_bit(PORTD, LED_GREEN);
    clear_bit(PORTD, LED_RED);
    clear_bit(PORTD, LED_YELLOW);
  }
}
