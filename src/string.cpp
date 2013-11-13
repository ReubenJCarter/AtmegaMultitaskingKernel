#include "string.h"

unsigned int IntToString(int fValue, char* fCstr)
{
	unsigned int len = 0;
        unsigned char isPreNum = 1;
        int powerTenLength = 7;
	int powerTen[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
	char decNumber[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int val;
	
	if(fValue < 0)
	{
		fCstr[len] = '-';	
		len++;
		val = -1 * fValue;
	}
	else
		val = fValue;
	
	for(int i = powerTenLength - 1; i >= 0; i--)
	{
        char temp;
		temp = decNumber[(val / powerTen[i]) % 10];
                
		if(temp != '0' && isPreNum == 1)
			isPreNum = 0;
		
		if(isPreNum == 0)
		{
			fCstr[len] = temp;
			len++;
		}
	}
	
        if(fValue == 0)
        {
          len = 1;
          fCstr[0] = '0';
          fCstr[1] = '\0';
        }

	fCstr[len] = '\0';
	return len;
}

unsigned int LongToString(long fValue, char* fCstr)
{
	unsigned int len = 0;
        unsigned char isPreNum = 1;
        int powerTenLength = 10;
	long powerTen[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	char decNumber[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	long val;
	
	if(fValue < 0)
	{
		fCstr[len] = '-';	
		len++;
		val = -1 * fValue;
	}
	else
		val = fValue;
	
	for(int i = powerTenLength - 1; i >= 0; i--)
	{
        char temp;
		temp = decNumber[(val / powerTen[i]) % 10];
                
		if(temp != '0' && isPreNum == 1)
			isPreNum = 0;
		
		if(isPreNum == 0)
		{
			fCstr[len] = temp;
			len++;
		}
	}
	
        if(fValue == 0)
        {
          len = 1;
          fCstr[0] = '0';
          fCstr[1] = '\0';
        }

	fCstr[len] = '\0';
	return len;
}

int StringToInt(char* fCstr)
{
    int powerTenLength = 7;
    int powerTen[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
    char decNumber[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
 
    int num = 0;
    int i = 0;
    int numLength = 0;
    
    while(fCstr[numLength] != '\0')
      numLength++;
      
    while(fCstr[i] != '\0' && i < powerTenLength)
    {
      for(int j = 0; j < 10; j++)
      {
        if(fCstr[i] == decNumber[j])
        {
          num += powerTen[numLength - 1 - i] * j;
          break;
        }
      }
      i++;
    }
    
    return num;
}

long StringToLong(char* fCstr)
{
    int powerTenLength = 9;
    long powerTen[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    char decNumber[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
 
    long num = 0;
    int i = 0;
    int numLength = 0;
    
    while(fCstr[numLength] != '\0')
      numLength++;
      
    while(fCstr[i] != '\0' && i < powerTenLength)
    {
      for(int j = 0; j < 10; j++)
      {
        if(fCstr[i] == decNumber[j])
        {
          num += powerTen[numLength - 1 - i] * j;
          break;
        }
      }
      i++;
    }
    
    return num;
}

unsigned int SubStringBreak(char* fStr)
{
  int i = 0;
  unsigned int subStringNumber = 0;
  while(fStr[i] != '\0')
  {
    if(fStr[i] == ' ' || fStr[i] == '\t')
    {
      fStr[i] = '\0';
      subStringNumber++;
    }
    i++;
  }
  return subStringNumber + 1;
}

void GetSubStringArray(char* fStr, char* fSubStr[], unsigned int fSubStringNumber)
{
  unsigned int i = 1;
  unsigned int subIndex = 1;
  fSubStr[0] = &fStr[0];
  while(subIndex < fSubStringNumber)
  {
    if(fStr[i] == '\0')
    {
      i++;
      fSubStr[subIndex] = &fStr[i];
      subIndex++;
    }
    else
    {
      i++;
    }
  }
}

char* GetSubString(char* fStr, unsigned int fIndex)
{
  unsigned int i = 0;
  unsigned int subStringNumber = 0;
  while(subStringNumber < fIndex)
  {
    if(fStr[i] == '\0')
    {
      subStringNumber++;
    }
    i++;
  }
  return &(fStr[i]);
}

int StringEqual(const char* fStr0, const char* fStr1)
{
  int isEq = 1;
  int i = 0;
  
  while(fStr0[i] != '\0' && fStr1[i] != '\0')
  {
    if(fStr0[i] != fStr1[i])
      break;
    i++;
  }
  
  if(fStr0[i] != '\0' || fStr1[i] != '\0')
    isEq = 0;
    
  return isEq;
}

void CopyStr(const char* fFrom, char* fTo)
{
  int i = 0;
  while(fFrom[i] != '\0')
  {
    fTo[i] = fFrom[i];
    i++;
  }
  fTo[i] = '\0';
}

void CopyChars(const char* fFrom, char* fTo, unsigned int fLength)
{
  for(int i = 0; i < fLength; i++)
  {
    fTo[i] = fFrom[i];
  }
}
