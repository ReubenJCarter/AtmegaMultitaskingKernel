#include "serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//serial variables////////////////////////////////////////////////////

char serialBuffer[SERIAL_BUFFER_SIZE];
unsigned int serialBufferLength;
Port serialPort;

//serial/////////////////////////////////////////////////////////////

Port* GetSerialPort()
{
  return &serialPort;
}

void InitUART(short fUBaudRateReg)
{
  PortInterface(&serialPort, SerialReadByte, SerialWriteByte, GetSerialBuffer, ClearSerialBuffer);
	UBRR0 = fUBaudRateReg;
        serialBufferLength = 0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
}

void SerialWriteByte(const char* fData)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = *fData;
}

void SerialReadByte(char* fData)
{
    char data = -1;
    if(serialBufferLength > 0)
    {
      data = serialBuffer[serialBufferLength - 1];
      serialBufferLength--;
    }
    *fData = data;
}

void SerialWrite(char* fData, unsigned int fLength)
{
  for(unsigned int i = 0; i < fLength; i++)
  {
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = fData[i];
  }
}

unsigned int GetSerialBuffer(char** fBuffer)
{
  *fBuffer = serialBuffer;
  return serialBufferLength;
}

void ClearSerialBuffer()
{
  serialBufferLength = 0;
}

ISR(USART0_RX_vect)
{
    serialBuffer[serialBufferLength] = UDR0;
    
    serialBufferLength++;
    if(serialBufferLength > SERIAL_BUFFER_SIZE - 1)
      serialBufferLength = 0;
    serialBuffer[serialBufferLength] = '\0';
}
