#define F_CPU 16000000 UL

#include <avr/io.h>
#include <util/delay.h>

//============== Macro definitions for working with bits ======================== // 
#define set_bit(y, bit)(y |= (1 << bit))    // sets the x bit of the variable Y to 1
#define clear_bit(y, bit)(y &= ~(1 << bit)) // sets bit x of variable Y to 0
#define toggle_bit(y, bit)(y ^= (1 << bit)) // changes the logical state of bit x of variable Y
#define test_bit(y, bit)(y & (1 << bit))    // returns 0 or 1 depending on the bit reading
//================================================================================ //

#define SS PB1
#define MISO PB4
#define LED_RED PD7
#define LED_GREEN PD6
#define LED_YELLOW PD5

void SPI_slave_init(){
  set_bit(DDRB, MISO);
  clear_bit(DDRB, SS);
}

void SPI_control_register(){ 
  SPCR = (1 << SPE);
}

uint8_t SPI_data_receive(){
  while(!test_bit(SPSR, SPIF));
  uint8_t data = SPDR;
  Serial.println(data);
  return data;
}

int main(void){
  Serial.begin(57600);
  
  SPI_slave_init();
  SPI_control_register();
 
  while(1){
    while (!test_bit(PINB, SS));
  	uint8_t data = SPI_data_receive();
    Serial.println(data);
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
    while(test_bit(PINB, SS));
  }
  
  return 0;
}