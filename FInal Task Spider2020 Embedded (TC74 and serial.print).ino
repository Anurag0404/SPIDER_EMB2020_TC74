#include<avr/io.h>
#define BAUDRATE (16000000/16/9600-1)

void initialize()
{
  TWSR = 0x00; //set prescaler
  TWBR = 0x47; // to set clock frequency
  TWCR = 0x04;// enabled TWI
}

void start()
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);  //start comdition
  while((TWCR&(1<<TWINT))==0);  
}

void write(unsigned char add)
{
  TWDR = add;               // send data to data register
  TWCR = (1<<TWINT)|(1<<TWEN); //TWI enabled
  while((TWCR&(1<<TWINT))==0);
}

unsigned char read(){
TWCR = (1<< TWINT) | (1<<TWEN) ;
while ( (TWCR & (1 <<TWINT) ) == 0) ;
return TWDR ;
}

void stop()
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);   //stop condition
}

void print( char * string )
{
  if (UCSR0B != (1<<TXEN0))
  {
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;
    UCSR0A = 0;
    UCSR0B = (1<<TXEN0);    //enabled transmission
    UCSR0C = (3<<UCSZ00);   //to set the character size (8 bit) and no parity
  }

  while (*string)
  {
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = *string++; //sending data to data register
  }
}


int main() 
{
  unsigned char temp = 0;
  DDRB = 0xff;
  while(1){
  initialize();
  start();
  write(0b10011010);
  write(0b00000010);
  start();
  write(0b10011011);
  temp = read();
  if(temp>=30)
  {
    PORTB |= (1<<PB5);
    print(" DANGER!!! ");
  }
  else
  {
    PORTB &= ~(1<<PB5);
    print(" SAFE "); 
  }
  stop();
  }
  return 0;
}