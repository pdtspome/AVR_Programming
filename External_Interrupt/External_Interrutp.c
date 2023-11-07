#ifndef __AVR_ATmega328p__
#define __AVR_ATmega328p__
#endif

#include <avr/io.h>
#include <util/atomic.h>

#define SET_BIT(reg, bit) reg |= (1u << bit)
#define CLEAR_BIT(reg, bit) reg &= ~(1u << bit)
#define TOGGLE_BIT(reg, bit) reg ^= (1u << bit)

/* Function Prototypes */

/*                     */


int main(void)
{
  SET_BIT(DDRB, 5u); // PB5 selected as output

  /* config input */
  CLEAR_BIT(DDRD, 3u); // PD3 selected as input
  SET_BIT(PORTD, 3U); // Input as pull-up

  /* Init External Interrupt */
  EICRA |= (1U << 3u); // 10: falling edge
  EIMSK |= (1u << 1u); // enable INT1

  /* Enable Global Interrupt */
  sei();

  while (1)
  {
  }

}

ISR(INT1_vect)
{
  TOGGLE_BIT(PORTB, 5u); // Toggle between HIGH and LOW output
}


