#define F_CPU 16000000UL
#ifndef __AVR_ATmega328p__
  #define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "Uart.h"


uint8_t Data_Rx[MAX_BUFFER];
int main(void)
{
  Uart_init(); // Init USART0
  Uart_Txstr("Hello World!!!\n\r");

  sei(); //Enable global interupt
  while (1)
  {
    if(Uart_Rx(Data_Rx)) //return 1 if there is a buffer in Rx_Buffer
    {
      Uart_Txstr(Data_Rx); // Transmit back the buffer.
    }
  }
  return 0;
}
