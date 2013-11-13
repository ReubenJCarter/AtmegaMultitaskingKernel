#ifndef IOPORT_H
#define IOPORT_H

#include "port.h"

Port* GetIoPort();
void InitPortB();
void IoWriteByteB(const char* fData);
void IoReadByteB(char* fData);

#endif
