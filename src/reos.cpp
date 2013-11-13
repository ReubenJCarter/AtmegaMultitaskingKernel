
#include "kernel.h"
#include "shell.h"
#include "serial.h"
#include "string.h"
#include "delay.h"
#include "ADC.h"

#include <avr/io.h>
#include <math.h>

void PrintADCValues()
{
  for(char i = 0; i < 8; i++)
  {
      PrintStrStdPort("ADC channel ");
      PrintIntStdPort(i);
      PrintStrStdPort(":");
      unsigned long voltageMV = ReadADC(i);
      voltageMV = (5000 * voltageMV) / 1024;
      PrintIntStdPort(ReadADC(i));
      PrintStrStdPort("\r\n");
  }
}

void Echo()
{
  ProcessNode* proc = GetCurrentNode();
  char* argv[4];
  if((*proc).process.argc > 0)
  {
    GetSubStringArray((*proc).process.argv, argv, (*proc).process.argc);
  }
  for(int i = 0; i < (*proc).process.argc; i++)
  {
    PrintStrStdPort("\r\n"); 
    PrintStrStdPort(argv[i]);
  }
}

void ICalc()
{
  ProcessNode* proc = GetCurrentNode();
  char* argv[4];
  if((*proc).process.argc == 3)
  {
    GetSubStringArray((*proc).process.argv, argv, (*proc).process.argc);
  
    int a = StringToInt(argv[0]);
    int b = StringToInt(argv[2]);
    int c = 0;
    
    if(StringEqual(argv[1], "+"))
      c = a + b;
    else if(StringEqual(argv[1], "-"))
      c = a - b;
    else if(StringEqual(argv[1], "*"))
      c = a * b;
    else if(StringEqual(argv[1], "/"))
      c = a / b;
      
    PrintIntStdPort(c);
  }
}

int main()
{
  StartKernelBlock();
  InitKernel();
  InitUART(USART_BAUD_REG);
  InitADC();

  InitFuncDb();
  AddFuncToDb(PrintADCValues, "getadc");
  AddFuncToDb(Echo, "echo");
  AddFuncToDb(ICalc, "icalc");
  
  AddProcess(ShellMain, RAMEND - 256);
  StopKernelBlock();
  
  while(1);
  return 1;
}
