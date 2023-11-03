#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

void I2C_init() {
  TWSR = 0;
  TWBR = ((F_CPU / 100000) - 16) / 2;
  set_bit(TWCR, TWEN);
}

void I2C_start() {
  set_bit(TWCR, TWSTA);
  set_bit(TWCR, TWEN);
  set_bit(TWCR, TWINT);
  while (!(TWCR & (1 << TWINT)));
}

void I2C_stop() {
  set_bit(TWCR, TWSTO);
  set_bit(TWCR, TWINT);
  set_bit(TWCR, TWEN);
}

void I2C_write(uint8_t data) {
  TWDR = data;
  set_bit(TWCR, TWEN);
  set_bit(TWCR, TWINT);
  while (!(TWCR & (1 << TWINT)));
}

int main() {
  I2C_init();
  uint8_t slaveAddress = 0x10;

  while (1){
  	I2C_start();
    I2C_write((slaveAddress << 1) | 0);
    I2C_write('R');
    I2C_stop();
    _delay_ms(500);
    
    I2C_start();
    I2C_write((slaveAddress << 1) | 0);
    I2C_write('G');
    I2C_stop();
    _delay_ms(500);
    
    I2C_start();
    I2C_write((slaveAddress << 1) | 0);
    I2C_write('Y');
    I2C_stop();
    _delay_ms(500);
    
    I2C_start();
    I2C_write((slaveAddress << 1) | 0);
    I2C_write('C');
    I2C_stop();
    _delay_ms(500);
  }

  return 0;
}
