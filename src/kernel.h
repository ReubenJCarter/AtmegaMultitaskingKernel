#ifndef KERNEL_H
#define KERNEL_H

#include "port.h"

#define KERNEL_TICK_TIME 1000
#define CORE_FREQ 16
#define TIMER_PRESCALAR 1024

#define MAX_PROCESS_NUMBER 8
#define PROCESS_STOPPED 1
#define PROCESS_IDLE 2
#define PROCESS_ACTIVE 3
#define PROCESS_PAUSED 4
#define PROCESS_DEAD 5

#define ARGV_BUFFER_SIZE 34

typedef void (*ProcessFunctionPtr)();
typedef void (*ProcessFunctionPtrArguments)(int argc, char* argv[]);

typedef struct _process
{
	ProcessFunctionPtr functionPtr;
	unsigned long stackAddress;
	unsigned long contextAddress;
	unsigned char state;	
        Port stdPort;
        char argv[ARGV_BUFFER_SIZE]; 
        unsigned char argc;
}Process; 

struct _processNode
{
	struct _processNode* nextNode;
	Process process; 
};
typedef struct _processNode ProcessNode;

void StartKernelBlock();
void StopKernelBlock();
unsigned long GetKernelTickCount();
void InitKernel();
ProcessNode* AddProcess(ProcessFunctionPtr fFunctionPtr, unsigned long fStackAddress);
ProcessNode* AddProcessPaused(ProcessFunctionPtr fFunctionPtr, unsigned long fStackAddress);
void RemoveProcess(ProcessNode* fProcessNode);
void PauseProcess(ProcessNode* fProcessNode);
void RestartProcess(ProcessNode* fProcessNode);
void ResumeProcess(ProcessNode* fProcessNode);
ProcessNode* GetNode(int fIndex);
unsigned int GetProcessNumber();
ProcessNode* GetCurrentNode();
Port* GetStdPort();

#endif
