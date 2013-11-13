#include "kernel.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//Kernel Variables////////////////////////////////

unsigned long kernelTickCount;

unsigned long currentStackAddress;
unsigned long currentStackValueLong;
unsigned long funcArgument;
ProcessNode* currentProcessNode;

ProcessNode processNode[MAX_PROCESS_NUMBER + 1];
ProcessNode baseNode;
unsigned char processListLength;
ProcessNode* endNode;
ProcessNode* poolStartNode;

//Asm////////////////////////////////////////////

#define SAVE_CONTEXT()\
asm volatile (\
"push r0        \n\t"\
"in r0, __SREG__        \n\t"\
"cli    \n\t"\
"push r0        \n\t"\
"push r1        \n\t"\
"clr r1 \n\t"\
"push r2        \n\t"\
"push r3        \n\t"\
"push r4        \n\t"\
"push r5        \n\t"\
"push r6        \n\t"\
"push r7        \n\t"\
"push r8        \n\t"\
"push r9        \n\t"\
"push r10       \n\t"\
"push r11       \n\t"\
"push r12       \n\t"\
"push r13       \n\t"\
"push r14       \n\t"\
"push r15       \n\t"\
"push r16       \n\t"\
"push r17       \n\t"\
"push r18       \n\t"\
"push r19       \n\t"\
"push r20       \n\t"\
"push r21       \n\t"\
"push r22       \n\t"\
"push r23       \n\t"\
"push r24       \n\t"\
"push r25       \n\t"\
"push r26       \n\t"\
"push r27       \n\t"\
"push r28       \n\t"\
"push r29       \n\t"\
"push r30       \n\t"\
"push r31       \n\t"\
"in r26, __SP_L__       \n\t"\
"in r27, __SP_H__       \n\t"\
"sts currentStackAddress+1, r27        \n\t"\
"sts currentStackAddress, r26  \n\t"\
"sei    \n\t" : :);

#define RESTORE_CONTEXT()\
asm volatile (\
"out __SP_L__, %A0      \n\t"\
"out __SP_H__, %B0      \n\t"\
"pop r31        \n\t"\
"pop r30        \n\t"\
"pop r29        \n\t"\
"pop r28        \n\t"\
"pop r27        \n\t"\
"pop r26        \n\t"\
"pop r25        \n\t"\
"pop r24        \n\t"\
"pop r23        \n\t"\
"pop r22        \n\t"\
"pop r21        \n\t"\
"pop r20        \n\t"\
"pop r19        \n\t"\
"pop r18        \n\t"\
"pop r17        \n\t"\
"pop r16        \n\t"\
"pop r15        \n\t"\
"pop r14        \n\t"\
"pop r13        \n\t"\
"pop r12        \n\t"\
"pop r11        \n\t"\
"pop r10        \n\t"\
"pop r9 		\n\t"\
"pop r8 		\n\t"\
"pop r7 		\n\t"\
"pop r6 		\n\t"\
"pop r5 		\n\t"\
"pop r4 		\n\t"\
"pop r3 		\n\t"\
"pop r2 		\n\t"\
"pop r1 		\n\t"\
"pop r0 		\n\t"\
"out __SREG__, r0		\n\t"\
"pop r0 \n\t": : "r" (currentStackAddress));

#define PUSH_STACK_VALUE_LONG()\
asm volatile(\
"cli    \n\t"\
"mov r0, %A0    \n\t"\
"push r0        \n\t"\
"mov r0, %B0    \n\t"\
"push r0        \n\t"\
"sei": : "r" (currentStackValueLong))

#define SET_STACK_ADDRESS()\
asm volatile(\
"cli    \n\t"\
"OUT __SP_L__, %A0      \n\t"\
"OUT __SP_H__, %B0      \n\t"\
"sei": : "r" (currentStackAddress))

#define PASS_FUNCTION_ARGUMENT()\
asm volatile(\
"cli    \n\t"\
"mov R25, %B0   \n\t"\
"mov R24, %A0   \n\t"\
"sei": : "r" (funcArgument))

//Linked List//////////////////////////////////////////////////////////

void InitProcessList()
{
	processListLength = 0;
	endNode = &baseNode;
	
	for(unsigned char i = 0; i < MAX_PROCESS_NUMBER -1 ; i++)
	{
		processNode[i].nextNode = &processNode[i + 1];
                processNode[i].process.state = PROCESS_DEAD;
	}
	processNode[MAX_PROCESS_NUMBER -1].nextNode = 0;
	poolStartNode = &processNode[0];
}

ProcessNode* ProcessListPush()
{
	(*endNode).nextNode = poolStartNode;
    poolStartNode = (*poolStartNode).nextNode;
	endNode = (*endNode).nextNode;
	(*endNode).nextNode = 0;
	processListLength++;
    return endNode;
}

void ProcessListRemove(ProcessNode* fNode)
{
	ProcessNode* processToRemove = fNode;
	ProcessNode* beforeProcessToRemove = &baseNode;
	for(unsigned char i = 0; i < MAX_PROCESS_NUMBER; i++)
	{
		if((*beforeProcessToRemove).nextNode == fNode)
            break;
        beforeProcessToRemove = (*beforeProcessToRemove).nextNode;
	}
	(*beforeProcessToRemove).nextNode = (*processToRemove).nextNode;
	(*processToRemove).nextNode = poolStartNode;
	poolStartNode = processToRemove;
        if(endNode == processToRemove)
          endNode = beforeProcessToRemove;
	processListLength--;
}

unsigned int GetProcessNumber()
{
  return processListLength;
}

ProcessNode* GetNode(int fIndex)
{
  ProcessNode* node = 0;
  if(fIndex >= 0 && fIndex < processListLength)
  {
    node = baseNode.nextNode;
    for(int i = 0; i < fIndex; i++)
    {
       node = (*node).nextNode; 
    }
  }
  return node;
}

//Timmer/////////////////////////////////////////////////////////////

short TimeToTimerCounts(int fTime, int fCoreFreq, int fPrescaler)
{
	int res = (fPrescaler / fCoreFreq);
	return (fTime / res) - 1;
}

void InitTimer1Interupt(short fCounts)
{
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = fCounts;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
}

//External///////////////////////////////////////////////////////////

void StartKernelBlock()
{
    cli();
}

void StopKernelBlock()
{
    sei();
}

unsigned long GetKernelTickCount()
{
  return kernelTickCount;
}

void InitKernel()
{
	InitProcessList();
	InitTimer1Interupt(TimeToTimerCounts(KERNEL_TICK_TIME, CORE_FREQ, TIMER_PRESCALAR));
	baseNode.process.state = PROCESS_PAUSED;
	currentProcessNode = &baseNode;
        kernelTickCount = 0;
}

ProcessNode* AddProcess(ProcessFunctionPtr fFunctionPtr, unsigned long fStackAddress)
{
	ProcessNode* procNode = ProcessListPush();
	(*procNode).process.functionPtr = fFunctionPtr;
	(*procNode).process.stackAddress = fStackAddress;
	(*procNode).process.contextAddress = 0;
        (*procNode).process.argc = 0;	
        RemoveInterface(&(*procNode).process.stdPort);
        (*procNode).process.state = PROCESS_STOPPED; //happens last to stop kernel ISR exicuting process before setup
        return procNode;
}

ProcessNode* AddProcessPaused(ProcessFunctionPtr fFunctionPtr, unsigned long fStackAddress)
{
	ProcessNode* procNode = ProcessListPush();
        (*procNode).process.state = PROCESS_PAUSED;
	(*procNode).process.functionPtr = fFunctionPtr;
	(*procNode).process.stackAddress = fStackAddress;
	(*procNode).process.contextAddress = 0;
        (*procNode).process.argc = 0;	
        RemoveInterface(&(*procNode).process.stdPort);
        return procNode;
}

void RemoveProcess(ProcessNode* fProcessNode)
{
        (*fProcessNode).process.state = PROCESS_DEAD;
}

void PauseProcess(ProcessNode* fProcessNode)
{
	(*fProcessNode).process.state = PROCESS_PAUSED;
}

void ResumeProcess(ProcessNode* fProcessNode)
{
        (*fProcessNode).process.state = PROCESS_IDLE;
}

void RestartProcess(ProcessNode* fProcessNode)
{
	(*fProcessNode).process.state = PROCESS_STOPPED;
}

ProcessNode* GetCurrentNode()
{
  return currentProcessNode;
}

Port* GetStdPort()
{
  return &(*currentProcessNode).process.stdPort;
}

//ISR////////////////////////////////////////////////////////////////

void baseFunction(ProcessNode* fNode)
{
  ProcessNode* node = fNode;
  (*((*node).process.functionPtr))();
  RemoveProcess(node);
  while(1);
}

void RunProcess(ProcessNode* fProcessNode)
{
	if((*fProcessNode).process.state == PROCESS_STOPPED)
	{
		(*fProcessNode).process.state = PROCESS_ACTIVE;
		currentStackAddress = (*fProcessNode).process.stackAddress;
		SET_STACK_ADDRESS();
		currentStackValueLong = (unsigned long)baseFunction;
                funcArgument = (unsigned long)fProcessNode;
		PUSH_STACK_VALUE_LONG();
                PASS_FUNCTION_ARGUMENT();
	}
	else if((*fProcessNode).process.state == PROCESS_IDLE)
	{
        (*fProcessNode).process.state = PROCESS_ACTIVE;
		currentStackAddress = (*fProcessNode).process.contextAddress;
		RESTORE_CONTEXT();
	}
	asm("reti");
}

ProcessNode* Scheduler()
{
	ProcessNode* temp;
        ProcessNode* tempPrev;
        temp = (*currentProcessNode).nextNode;
        
	if(temp == 0)
		temp = baseNode.nextNode;
        
        while((*temp).process.state == PROCESS_PAUSED || (*temp).process.state == PROCESS_DEAD)
        {
          tempPrev = temp;
          temp = (*temp).nextNode;
          
          if((*tempPrev).process.state == PROCESS_DEAD)
            ProcessListRemove(tempPrev);

          if (temp == 0)
            temp = baseNode.nextNode;
        }
          
	return temp;
}

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
	SAVE_CONTEXT();
	if((*currentProcessNode).process.state == PROCESS_ACTIVE)
	{
		(*currentProcessNode).process.state = PROCESS_IDLE;	
	}
        (*currentProcessNode).process.contextAddress = currentStackAddress;
	currentProcessNode = Scheduler();
        kernelTickCount++;
	RunProcess(currentProcessNode);	
	asm("reti");
} 


