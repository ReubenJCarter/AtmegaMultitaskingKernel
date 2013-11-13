#include <avr/io.h>
#include <avr/interrupt.h>

#include "shell.h"
#include "serial.h"
#include "string.h"
#include "port.h"

//shell variables/////////////////////////////////////////////////////

const char bootMessage[] = 
"*******************\r\n"
"**  REOS SHELL   **\r\n"
"*******************\r\n\r\n"; 

typedef struct _funcdbNode
{
  char command[FUNC_CMDSTR_MAX_LENGTH]; //names have to be less than 8 chars
  ProcessFunctionPtr pointer;
}FuncDBNode;

FuncDBNode funcdb[FUNC_LIST_MAX];
unsigned char funcDbLength;

//Print///////////////////////////////////////////////////////////////

void PrintStrStdPort(const char* fStr)
{
  int i = 0;
  while(fStr[i] != '\0')
  {
    PortWrite(GetStdPort(), &fStr[i]);
    i++;
  }
}

void PrintCharStdPort(char fChar)
{
  PortWrite(GetStdPort(), &fChar);
}

void PrintIntStdPort(int fVal)
{
  char strVal[10];
  IntToString(fVal, strVal);
  PrintStrStdPort(strVal);
}

void PrintLongStdPort(long fVal)
{
  char strVal[16];
  LongToString(fVal, strVal);
  PrintStrStdPort(strVal);
}

//utils///////////////////////////////////////////////////////////////

void PrintProcessInfo()
{
   PrintStrStdPort("\r\n");
   PrintStrStdPort("---Process Info---\r\n");
   ProcessNode* proc = GetNode(0);
   int procIndex = 0;
   while(proc != 0)
   {
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort(" Index: ");
     PrintIntStdPort(procIndex);
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort("-Main function pointer: ");
     PrintIntStdPort((int)(*proc).process.functionPtr);
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort("-Stack base address: ");
     PrintIntStdPort((*proc).process.stackAddress);
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort("-Bytes into stack: ");
     PrintIntStdPort(RAMEND - (*proc).process.stackAddress);
     PrintStrStdPort("\r\n"); 
     
     PrintStrStdPort("-Stack size: ");
     PrintIntStdPort((*proc).process.stackAddress - (*proc).process.contextAddress);
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort("-State: ");
     PrintIntStdPort((*proc).process.state);
     PrintStrStdPort("\r\n");
     
     PrintStrStdPort("-argv: ");
     int j = 0;
     for(int i = 0; i < (*proc).process.argc; i++)
     {
       while((*proc).process.argv[j] != 0)
       {
         PrintCharStdPort((*proc).process.argv[j]);
         j++;
       }
       j++;
       PrintCharStdPort(' ');
     }
     PrintStrStdPort("\r\n");
     
     proc = (*proc).nextNode;
     procIndex++;
   }
   PrintStrStdPort("\r\n");
}

//function list//////////////////////////////////////////////////////

void AddFuncToDb(ProcessFunctionPtr fFunc, char* fName)
{
  funcdb[funcDbLength].pointer = fFunc;
  CopyStr(fName, funcdb[funcDbLength].command);
  funcDbLength++;
}

void InitFuncDb()
{
  funcDbLength = 0;
  AddFuncToDb(PrintProcessInfo, "pinfo");
  AddFuncToDb(ShellMain, "shell");
}

ProcessFunctionPtr LookUpPointer(const char* fStr)
{
  for(int i = 0; i < funcDbLength; i++)
  {
     if(StringEqual(funcdb[i].command, fStr))
       return funcdb[i].pointer;
  }
  return 0;
}

char* LookUpString(ProcessFunctionPtr fPointer)
{
  for(int i = 0; i < funcDbLength; i++)
  {
     if(fPointer == funcdb[i].pointer)
       return funcdb[i].command;
  }
  return 0;
}

//shell///////////////////////////////////////////////////////////////

void FormatInput(Shell* fShell)
{
   //Reads the standard port and copy the char into the scan buffer
   char result = 0;
   char temp;
   PortRead(GetStdPort(), &temp);
   if(temp >= 0 && temp <= 127)
   {
     result = 1;
     (*fShell).scanBuffer[(*fShell).scanBufferLength] = temp;
     (*fShell).scanBufferLength++;
     (*fShell).scanBuffer[(*fShell).scanBufferLength] = '\0';
   }
   else
   {
     result = 0;
   }
  
  //echos each new charecter recived to the std port, and formats the delete char
  if(result == 1)
  {
    if((*fShell).scanBuffer[(*fShell).scanBufferLength -1 ] == '\b' || (*fShell).scanBuffer[(*fShell).scanBufferLength -1 ] == 127)
    {
      if((*fShell).scanBufferLength >= 2)
      {
        if((*fShell).scanBuffer[(*fShell).scanBufferLength -1 ] == '\b')
          PrintCharStdPort('\b');
        else
          PrintCharStdPort(127);
        (*fShell).scanBuffer[(*fShell).scanBufferLength - 2] = '\0'; 
        (*fShell).scanBufferLength -= 2;
      }
    }
    else
    {
      PrintCharStdPort((*fShell).scanBuffer[(*fShell).scanBufferLength -1 ]);
    }
  }
}

char ParseScanBuffer(Shell* fShell)
{
   char retValue = 0;
   if((*fShell).scanBuffer[(*fShell).scanBufferLength - 1] == '\n' || (*fShell).scanBuffer[(*fShell).scanBufferLength - 1] ==  '\r')
   {
     (*fShell).scanBuffer[(*fShell).scanBufferLength - 1] = '\0';
     
     if(StringEqual("clear", (*fShell).scanBuffer))
     {
       PrintCharStdPort('\f');
     }
     else
     {
       int strNumber = 0; 
       strNumber = SubStringBreak((*fShell).scanBuffer);
       if(strNumber >= 2)
       { 
         char* cmdStr = GetSubString((*fShell).scanBuffer, 0);
         char* stackStr = GetSubString((*fShell).scanBuffer, 1);
         unsigned int stackAddress = StringToInt(stackStr);
         if(StringEqual("watch", cmdStr))
         {
           ProcessNode* proc = GetNode(stackAddress);
           if(proc != 0)
           {
             (*fShell).programNode = proc;
             PortToPortInterface(&(*proc).process.stdPort, GetStdPort());
             (*fShell).mode = SHELL_PASSIVE;
             PrintStrStdPort("\r\n");
           }
         }
         else if(StringEqual("kill", cmdStr))
         {
           ProcessNode* proc = GetNode(stackAddress);
           if(proc != 0)
           {
             RemoveProcess(proc);
             PrintStrStdPort("\r\nproc killed\r\n");
           }
         }
         else if(StringEqual("restart", cmdStr))
         {
           ProcessNode* proc = GetNode(stackAddress);
           if(proc != 0)
           {
             RestartProcess(proc);
             PrintStrStdPort("\r\nproc restarted\r\n");
           }
         }
         else if(StringEqual("pause", cmdStr))
         {
           ProcessNode* proc = GetNode(stackAddress);
           if(proc != 0)
           {
             PauseProcess(proc);
             PrintStrStdPort("\r\nproc pause\r\n");
           }
         }
         else if(StringEqual("resume", cmdStr))
         {
           ProcessNode* proc = GetNode(stackAddress);
           if(proc != 0)
           {
             ResumeProcess(proc);
             PrintStrStdPort("\r\nproc resume\r\n");
           }
         }
         else
         {
           for(int i = 0; i < funcDbLength; i++)
           {
             if(StringEqual(funcdb[i].command, cmdStr))
             {
               ProcessNode* proc = AddProcessPaused(funcdb[i].pointer, RAMEND - stackAddress);
               (*fShell).programNode = proc;
               if(strNumber > 2)
               {
                 (*proc).process.argc = strNumber - 2;
                 char* str = GetSubString(stackStr, 1);
                 int charCount = 0;
                 for(int i = 0; i < (*proc).process.argc; i++)
                 {
                   while(str[charCount] != '\0')
                   {
                     charCount++;
                   }
                   charCount++;
                 }
                 if(charCount < ARGV_BUFFER_SIZE - 1) 
                   CopyChars(str, (*proc).process.argv, charCount);
               }
               PortToPortInterface(&(*proc).process.stdPort, GetStdPort());
               (*fShell).mode = SHELL_PASSIVE;
              
               PrintStrStdPort("\r\n");
               PrintStrStdPort("run ");
               PrintStrStdPort(cmdStr);
               PrintCharStdPort('[');
               PrintIntStdPort((int)funcdb[i].pointer);
               PrintStrStdPort("]:\r\n");
                           
               RestartProcess(proc);
               break;
             }
           }
         }
       }
     }
     
     (*fShell).scanBufferLength = 0;
     if((*fShell).mode != SHELL_PASSIVE)
       PrintStrStdPort("\r\n->");
   }
   return retValue;
}

void ShellMain()
{
  PortToPortInterface(GetStdPort(), GetSerialPort());
  Shell shell;
  shell.mode = SHELL_ACTIVE;
  
  PrintStrStdPort(bootMessage);
  PrintStrStdPort("\r\n->");
  PortFlush(GetStdPort());
  
  while(1)
  {
    if(shell.mode == SHELL_ACTIVE)
    {
      FormatInput(&shell);
      ParseScanBuffer(&shell);
    }
    else if(shell.mode == SHELL_PASSIVE)
    {
      char* portStream[1];
      unsigned int streamLength = PortGetStream(GetStdPort(), portStream);
      if(streamLength > 0)
      {
        if(portStream[0][streamLength - 1] == 3)//ctrlC
        {
          shell.mode = SHELL_ACTIVE;
          RemoveInterface(&(*shell.programNode).process.stdPort);
          PrintStrStdPort("\r\n->");
          shell.scanBufferLength = 0;
          PortFlush(GetStdPort());
        }
        if(portStream[0][streamLength - 1] == 26)//ctrlZ
        {
          shell.mode = SHELL_ACTIVE;
          RemoveInterface(&(*shell.programNode).process.stdPort);
          RemoveProcess(shell.programNode);
          PrintStrStdPort("\r\nproc killed\r\n");
          shell.scanBufferLength = 0;
          PortFlush(GetStdPort());
        }
      }
      if((*shell.programNode).process.state == PROCESS_PAUSED || (*shell.programNode).process.state == PROCESS_DEAD)
      {
        shell.mode = SHELL_ACTIVE;
        PrintStrStdPort("\r\n->");
        shell.scanBufferLength = 0;
        PortFlush(GetStdPort());
      }
    }
  }
}
