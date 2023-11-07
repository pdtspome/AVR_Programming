#define F_CPU 16000000UL

#ifndef __AVR_ATmega328p__
#define __AVR_ATmega328p__
#endif
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "../Uart/Uart.h" // include uart lib to debug ADC

#define TOTAL_CHANNELS 2 // Number of TOTAL_CHANNELS used

volatile static uint16_t Adc_Value[TOTAL_CHANNELS];
volatile static uint8_t cur_channel = 0; //Default CH for conversion is A0

uint16_t Adc_Read(uint8_t channel);
void Adc_init(void);

int main(void)
{
	uint16_t Adc_ch0; uint16_t Adc_ch1;
	char str[50];

	DDRC = 0; PORTC = 0;
	DDRB |= (1 << 5); // PD5 as output
	DDRD |= (1 << 2); // PD1 as output
	
  Adc_init();
	Uart_init(); //Init Uart (Tx only) - 9600bps
	sei(); // Enable global interrupt

		while(1)
	{
		Adc_ch0 = Adc_Read(0);
		Adc_ch1 = Adc_Read(1);

		if(Adc_ch0 > 500)
		  PORTB |= (1 << 5);
		else
		  PORTB &= ~(1 << 5);

		if(Adc_ch1 > 500)
		  PORTD |= (1 << 2);
		else
		  PORTD &= ~(1 << 2);

		sprintf(str, "ADC_CH0: %d\n\r------------------\n", Adc_ch0);
		Uart_Txstr(str);
		sprintf(str, "ADC_CH1: %d\n\r", Adc_ch1);
		Uart_Txstr(str);
		_delay_ms(200);
	}
}

void Adc_init(void)
{
  /* initnit ADC to operate in single conversion mode */

  ADMUX |= 0x40 | cur_channel; // Voltage Ref: AVCC.

	ADCSRA |=  (3 << ADIE)  | (7 << ADPS0); // Prescaler = 128; Enable Interrupt, 

	ADCSRA |= (1 << ADEN); // Enable ADC
	ADCSRA |= (1 << ADSC); // Start Conversion
}

uint16_t Adc_Read(uint8_t channel)
{
	return Adc_Value[channel]; //return result of ADC on selected CH
}

ISR(ADC_vect)
{
	Adc_Value[cur_channel] = ADC; // Retrieve data from ADC Data Reg
	
	cur_channel = (cur_channel + 1) % TOTAL_CHANNELS; // change channel
	ADMUX = 0x40 | cur_channel; // will convert ADC CHs in sequentially
	ADCSRA |= (1 << ADSC); // Start Conversion
}


