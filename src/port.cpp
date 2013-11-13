#include "port.h"

void PortWrite(Port* fPort, const char* fData)
{
  if((*fPort).interfaceWriteFunction != 0)
    (*(*fPort).interfaceWriteFunction)(fData);
}

void PortRead(Port* fPort, char* fData)
{
  if((*fPort).interfaceReadFunction != 0)
    (*(*fPort).interfaceReadFunction)(fData);  
}

unsigned int PortGetStream(Port* fPort, char** fStream)
{
  if((*fPort).interfaceGetStreamFunction != 0)
    return (*(*fPort).interfaceGetStreamFunction)(fStream);
  else
    return 0;
}

void PortFlush(Port* fPort)
{
  if((*fPort).interfaceFlushFunction != 0)
    (*(*fPort).interfaceFlushFunction)(); 
}

void PortInterface(Port* fPort, InterfaceReadFunction fRead, InterfaceWriteFunction fWrite, InterfaceGetStreamFunction fInterfaceGetStreamFunction, InterfaceFlushFunction fInterfaceFlushFunction)
{
   (*fPort).interfaceWriteFunction = fWrite;
   (*fPort).interfaceReadFunction = fRead;
   (*fPort).interfaceGetStreamFunction = fInterfaceGetStreamFunction;
   (*fPort).interfaceFlushFunction = fInterfaceFlushFunction;
}

void RemoveInterface(Port* fPort)
{
   (*fPort).interfaceWriteFunction = 0;
   (*fPort).interfaceReadFunction = 0;
   (*fPort).interfaceGetStreamFunction = 0;
   (*fPort).interfaceFlushFunction = 0;
}

void PortToPortInterface(Port* fPort, Port* fPortInterface)
{
   (*fPort).interfaceWriteFunction = (*fPortInterface).interfaceWriteFunction;
   (*fPort).interfaceReadFunction = (*fPortInterface).interfaceReadFunction;
   (*fPort).interfaceGetStreamFunction = (*fPortInterface).interfaceGetStreamFunction;
   (*fPort).interfaceFlushFunction = (*fPortInterface).interfaceFlushFunction;
}

unsigned char PortsConnected(Port* fPort0, Port* fPort1)
{
   if(((*fPort0).interfaceWriteFunction == (*fPort1).interfaceWriteFunction) && ((*fPort0).interfaceReadFunction == (*fPort1).interfaceReadFunction))
     return 1;
   else
     return 0;
}
