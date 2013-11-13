#include "ADC.h"

#include <avr/io.h>

void InitADC()
{
 ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= (1 << REFS0);                //Voltage reference from Avcc (5v)
 ADCSRA |= (1 << ADEN);                //Turn on ADC
 ADCSRA |= (1 << ADSC); //do inital read
}

unsigned int ReadADC(unsigned char fChannel)
{
   ADMUX &= 0xF0;                    //Clear the older channel that was read
   ADMUX |= fChannel;                //Defines the new ADC channel to be read
   ADCSRA |= (1 << ADSC);                //Starts a new conversion
   while(ADCSRA & (1 << ADSC));            //Wait until the conversion is done
   return (unsigned int)ADCW;   
}
