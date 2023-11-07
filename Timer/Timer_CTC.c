#define F_CPU 16000000UL

#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>

volatile char Delay_Completed;

/* Function Prototypes */
void Timer1_CTC_Init(void);
void Delay_ms(int ms);
void Delay_us(int us);

int main(void)
{
  /* Set PB3 as Output */
  DDRB |= (1 << 5);
  Timer1_CTC_Init();

  /* Enalble Global Interrupt */
  sei();
  while (1)
  {
    PORTB ^= (1U << 5);
    Delay_ms(1000);
  }
}


void Timer1_CTC_Init(void)
{
  /* WGM13:0 = 0100; COM1B1:0 = 00*/ 
  TCCR1A = 0;
  TCCR1B |= (1 << WGM12); 
  
  /* Output Compare A Match Interrupt Enable */
  TIMSK1 |= (1 << OCIE1A);
}

void Delay_us(int us)
{
  Delay_Completed = 0;
  OCR1A = 2 * (us); 
  /* Enable counter; N = 8 */
  TCCR1B |= (1 << CS11);
  while(!Delay_Completed);
  /* Disable counter */
  TCCR1B &= ~(1 << CS11);
}

void Delay_ms(int ms)
{
  for(int i = 0; i < ms; i++)
    Delay_us(1000);
}

/* ISR Functions */
ISR(TIMER1_COMPA_vect)
{
  Delay_Completed = 1;
}
