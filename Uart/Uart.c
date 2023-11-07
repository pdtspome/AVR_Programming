#define F_CPU 16000000UL
#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>
#include "Uart.h"


void Uart_init(void)
{
  /* Setting baud rate */
  UBRR0 = 103; //BAUD RATE = 9600bps

  /* Setting Data frame */
  UCSR0A &= ~(1 << U2X0); // U2X0 = 0
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //8bit

  UCSR0B |= (1<<TXEN0) | (1 << RXEN0);// Enable Transmit

}

void Uart_Txchar(char c)
{
  while( !(UCSR0A & (1 << UDRE0)) );
  UDR0 = c;
}

void Uart_Txstr(char* str)
{
  while(*str)
  {
    Uart_Txchar(*str);
    str++;
  }
}


