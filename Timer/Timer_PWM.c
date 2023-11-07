#define F_CPU 16000000UL

#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>

short Duty_Cycle = 0;

/* Function Prototypes */
void Timer1_PWM_PhaseFreqCorrect_CHA(void);

int main(void)
{
  /* Set PB1 as Output */
  DDRB |= (1 << 1);

  Timer1_PWM_PhaseFreqCorrect_CHA();
  
  sei();
  while (1)
  {
    Duty_Cycle = (Duty_Cycle + 100) % 1000;
    _delay_ms(500);
  }

}

void Timer1_PWM_PhaseFreqCorrect_CHA(void)
{
  /* Clk Input: 16Mhz */

  DDRB |= (1 << 1); // PB1 (OC1A) as output mode

  /* 
    WGM: 1001 == Phase & Freq Correct
    COM1A: 10 == non-inverted mode
  */
  TCCR1A |= (1 << WGM11) | (1 << WGM10) | (3 << COM1A0);

  /* Prescaler N = 1 */
  TCCR1B |= (1 << CS10);

  // 10 bits resolution
  //OCR1A = 0X3ff; // 1023

  // duty cycle = OCR1A / (Resoluton - 1) 
  OCR1A = 100; // duty cycle of 50%

  TIMSK1 |= (1u << 1u); // OCA Match Interrupt
}

/* ISR Functions */

ISR(TIMER1_COMPA_vect)
{
  /* Can actively change duty cycle here */
  OCR1A = Duty_Cycle;
}
