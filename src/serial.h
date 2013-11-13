#ifndef SERIAL_H
#define SERIAL_H

#include "port.h"
#define ARDUINOMEGA_9600BAUD 0x67
#define ATMEGA20MHZ_115200 0xA
#define ATMEGA20MHZ_9600 81
#define USART_BAUD_REG ARDUINOMEGA_9600BAUD
#define SERIAL_BUFFER_SIZE 64

Port* GetSerialPort();
void InitUART(short fUBaudRateReg);
void SerialWriteByte(const char* fData);
void SerialWrite(char* fData, unsigned int fLength);
void SerialReadByte(char* fData);
unsigned int GetSerialBuffer(char** fBuffer);
void ClearSerialBuffer();

#endif
