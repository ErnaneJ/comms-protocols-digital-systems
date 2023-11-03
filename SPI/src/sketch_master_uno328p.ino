#define F_CPU 16000000 UL

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

#define MOSI PB3
#define SCK PB5
#define SS PB2
#define MISO PB4

void SPI_master_init(){
  set_bit(DDRB, MOSI);
  set_bit(DDRB, SCK);
  set_bit(DDRB, SS);
}

void SPI_control_register(){
  set_bit(SPCR, SPIE); // bit 1 - SPIE - SPI Interrupt Enable
  set_bit(SPCR, SPE); // bit 6 - SPI Enable
  set_bit(SPCR, MSTR);
  set_bit(SPCR, SPR0);
  
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t SPI_data_transmit(uint8_t data){
  SPDR = data;
  while(!test_bit(SPSR, SPIF));
  return SPDR;
}

int main(void){
  Serial.begin(57600);
  
  SPI_master_init();
  SPI_control_register();
 
  while(1){
    clear_bit(PORTB, SS);
    set_bit(PORTB, SS);
    SPI_data_transmit('R');
    _delay_ms(500);

    SPI_data_transmit('G');
    set_bit(PORTB, SS);
    _delay_ms(500);

    SPI_data_transmit('Y');
    set_bit(PORTB, SS);
    _delay_ms(500);

    SPI_data_transmit('C');
    set_bit(PORTB, SS);
    _delay_ms(500);
  }
  
  return 0;
}