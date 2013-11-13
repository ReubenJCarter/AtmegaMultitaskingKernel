#ifndef SHELL_H
#define SHELL_H

#include "port.h"
#include "kernel.h"

#define SCAN_BUFFER_SIZE 64

#define SHELL_ACTIVE 1
#define SHELL_PASSIVE 2

#define FUNC_LIST_MAX 12
#define FUNC_CMDSTR_MAX_LENGTH 9

typedef struct _shell
{
  char scanBuffer[SCAN_BUFFER_SIZE];
  unsigned char scanBufferLength;
  ProcessNode* programNode;
  unsigned char mode;
} Shell;

void InitFuncDb();
void AddFuncToDb(ProcessFunctionPtr fFunc, char* fName);
void ShellMain();
void PrintStrStdPort(const char* fStr);
void PrintCharStdPort(char fChar);
void PrintIntStdPort(int fVal);
void PrintLongStdPort(long fVal);

#endif
