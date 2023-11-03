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

void I2C_init() {
  TWAR = 0x10;
  set_bit(TWCR, TWEN);
}

void I2C_listen() {
  while (!(TWCR & (1 << TWINT)));
}

void I2C_ack() {
  set_bit(TWCR, TWINT);
  set_bit(TWCR, TWEA);
  set_bit(TWCR, TWEN);
}

void I2C_nack() {
  set_bit(TWCR, TWINT);
  set_bit(TWCR, TWEN);
}

uint8_t I2C_receive() {
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}

void LEDs_setup(){
  set_bit(DDRD, LED_RED);
  set_bit(DDRD, LED_GREEN);
  set_bit(DDRD, LED_YELLOW);
}

int main() {
  I2C_init();
  LEDs_setup();

  while (1) {
    I2C_listen();

    uint8_t data = I2C_receive();

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

    I2C_ack();
  }

  return 0;
}
