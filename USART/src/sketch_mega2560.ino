#define F_CPU 16000000 UL

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

#define LED_GREEN PE3
#define LED_YELLOW PH3
#define LED_RED PH4

void USART_setup(){
  UBRR0 = 103; // 9600bps
  UCSR0A = 0; // not configured

  set_bit(UCSR0B, RXCIE0); // Bit 7 - RXCIEn - RX Complete Interrupt Enable
  set_bit(UCSR0B, RXEN0);  // Bit 4 - RXENn - Receiver Enable
  set_bit(UCSR0B, TXEN0);  // Bit 3 - TXENn — Transmitter Enable

  // Character size 8-bit
  set_bit(UCSR0C, UCSZ00);
  set_bit(UCSR0C, UCSZ01);
}

void LEDs_setup(){
  set_bit(DDRE, LED_GREEN);
  set_bit(DDRH, LED_YELLOW);
  set_bit(DDRH, LED_RED);
}

int main(void){
  USART_setup();
  LEDs_setup();

  sei();
  
  for(;;);
  
  return 0;
}

void USART_Transmit(char data) {
  while (!(UCSR0A & (1 << UDRE0))); // wait until the transmit buffer is empty
  UDR0 = data;
}

void serialPrint(const char *str){
  int i = 0;
  while (str[i] != '\0') {
    USART_Transmit(str[i]);
    i++;
  }
}

ISR(USART0_RX_vect){
  char data = '\0';
  data = UDR0;

  if(data == 'G'){
    serialPrint(".:: [G] -> Green LED\n");
    toggle_bit(PORTE, LED_GREEN);
  }else if(data == 'R'){
    serialPrint(".:: [R] -> Red LED\n");
    toggle_bit(PORTH, LED_RED);
  }else if(data == 'Y'){
    serialPrint(".:: [Y] -> Yellow LED\n");
    toggle_bit(PORTH, LED_YELLOW);
  }else if(data == 'C'){
    serialPrint(".:: [C] -> Clear All LEDs!\n");
    clear_bit(PORTE, LED_GREEN);
    clear_bit(PORTH, LED_RED);
    clear_bit(PORTH, LED_YELLOW);
  }else if(data != '\n'){
    serialPrint(".:: [");
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    serialPrint("] -> Command not found.\n");
  }
}