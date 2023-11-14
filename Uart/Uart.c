#define F_CPU 16000000UL
#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>
#include "Uart.h"

// Stores each byte of Buffer from Uart Reception
char Rx_Buffer[MAX_BUFFER]; 
//index of Rx_Buffer
char idx = 0; 
// Flag to indicate full string has been successfully received
char Rx_Complete = 0; 
// Flag to indicate the "valid string" to start to receive
char Rx_Start = 0;


void Uart_init(void)
{
  /* Setting baud rate */
  UBRR0 = 103; //BAUD RATE = 9600bps

  /* Setting Data frame */
  UCSR0A &= ~(1 << U2X0); // U2X0 = 0
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8bit, 1 stop bit
  UCSR0B |= (1 << RXCIE0); // Enable Rx Interrupt

  UCSR0B |= (1<<TXEN0) | (1 << RXEN0);// Enable Transmitter & Receiver.
}

void Uart_Txchar(char c)
{
  /* wait for transmit buffer to be ready */
  while( !(UCSR0A & (1 << UDRE0)) ); 
  /* write to transmit data buffer TXB */
  UDR0 = c; 
}

/* Uart_Txstr() is to transmit a string to terminal */
void Uart_Txstr(char* str)
{
  while(*str)
  {
    Uart_Txchar(*str);
    str++;
  }
}

/* 
Uart_Rx() takes in Receive String on main program and return true if there is a buffer 
*/
char Uart_Rx(char *Rx_Str)
{
  if(Rx_Complete)
  {
    uint8_t i = 0;  
    Rx_Buffer[idx] = '\0';
    while(Rx_Buffer[i])
    {
      Rx_Str[i] = Rx_Buffer[i];
      i++;
    }
    Rx_Str[i] = '\0';
    Rx_Complete = 0;
    return 1;
  }
  return 0;
}

ISR(USART_RX_vect)
{
  
  Rx_Buffer[idx] = UDR0; /* read from receive data buffer RXB */

  if(Rx_Buffer[idx] == START_CHARACTER)
  {
    idx = 0;
    Rx_Start = 1;
  }
  else if(Rx_Buffer[idx] == END_CHARACTER || idx >= MAX_BUFFER - 1 && Rx_Start)
  {
    Rx_Complete = 1; //Set flag 
    Rx_Start = 0;
  }
  else if(Rx_Start)
    idx++;
}

