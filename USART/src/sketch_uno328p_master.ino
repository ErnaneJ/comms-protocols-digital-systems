#define F_CPU 16000000 UL

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

void USART_setup(){
  UBRR0 = 103; // 9600bps

  set_bit(UCSR0B, TXEN0);  // Bit 3 - TXENn — Transmitter Enable
  set_bit(UCSR0B, TXB80); // Character size 8-bit
  
  set_bit(UCSR0C, UCSZ01);
  set_bit(UCSR0C, UCSZ00);
}

void USART_Transmit(char data) {
  while (!(UCSR0A & (1 << UDRE0))); // wait until the transmit buffer is empty
  UDR0 = data;
}

int main(void){
  USART_setup();

  for(;;){
    USART_Transmit('R');
    _delay_ms(500);
    USART_Transmit('G');
    _delay_ms(500);
    USART_Transmit('Y');
    _delay_ms(500);
    USART_Transmit('C');
    _delay_ms(500);
  }
  
  return 0;
}