#ifndef PORT_H
#define PORT_H

typedef void (*InterfaceWriteFunction)(const char* fData);
typedef void (*InterfaceReadFunction)(char* fData);
typedef unsigned int (*InterfaceGetStreamFunction)(char** fData);
typedef void (*InterfaceFlushFunction)();

typedef struct _port
{
  InterfaceWriteFunction interfaceWriteFunction;
  InterfaceReadFunction interfaceReadFunction;
  InterfaceGetStreamFunction interfaceGetStreamFunction;
  InterfaceFlushFunction interfaceFlushFunction;
} Port;

void PortWrite(Port* fPort, const char* fData);
void PortRead(Port* fPort, char* fData);
unsigned int PortGetStream(Port* fPort, char** fStream);
void PortFlush(Port* fPort);
void PortInterface(Port* fPort, InterfaceReadFunction fRead, InterfaceWriteFunction fWrite, InterfaceGetStreamFunction fInterfaceGetStreamFunction, InterfaceFlushFunction fInterfaceFlushFunction);
void RemoveInterface(Port* fPort);
void PortToPortInterface(Port* fPort, Port* fPortInterface);
unsigned char PortsConnected(Port* fPort0, Port* fPort1);

#endif
