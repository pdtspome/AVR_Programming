#define F_CPU 16000000UL

#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>

volatile static int Adc_Value;

int main(void)
{
  DDRC = 0; PORTC = 0;

  DDRB |= (1 << 5);
  ADMUX |= (0 << REFS1) | (1 << REFS0) | (0 << MUX0);

  ADCSRA |=  (3 << ADIE) | (5 << ADATE) | (7 << ADPS0);
  ADCSRB = 0;

  ADCSRA |= (1 << ADEN); // Enable ADC
  ADCSRA |= (1 << ADSC); // Start Conversion
  sei(); // Enable global interrupt
  while(1)
  {
    if(Adc_Value > 128)
      PORTB |= (1 << 5);
    else
      PORTB &=  ~(1 << 5);
  }
}

ISR(ADC_vect)
{
  Adc_Value = ADC; // Retrieve data from ADC Data Reg
  ADCSRA |= (1 << ADSC);
}


