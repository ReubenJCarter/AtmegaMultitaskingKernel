#include "delay.h"
#include "kernel.h"

void InitDelay()
{
  
}

void Delay(unsigned long fTimeMS)
{
  unsigned long t = GetKernelTickCount();
  unsigned long timeSacled = (fTimeMS * 1000) / KERNEL_TICK_TIME;
  while((GetKernelTickCount() - t) < timeSacled && GetKernelTickCount() >= t);
  
  if(GetKernelTickCount() < t)
  {
    t = GetKernelTickCount();
    while((GetKernelTickCount() - t) < timeSacled && GetKernelTickCount() >= t);
  }
}
