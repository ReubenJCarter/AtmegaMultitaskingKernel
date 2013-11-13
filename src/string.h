#ifndef STRING_H
#define STRING_H

unsigned int IntToString(int fValue, char* fCstr);
unsigned int LongToString(long fValue, char* fCstr);
int StringToInt(char* fCstr);
long StringToLong(char* fCstr);
unsigned int SubStringBreak(char* fStr);
void GetSubStringArray(char* fStr, char* fSubStr[], unsigned int fSubStringNumber);
char* GetSubString(char* fStr, unsigned int fIndex);
int StringEqual(const char* fStr0, const char* fStr1);
void CopyStr(const char* fFrom, char* fTo);
void CopyChars(const char* fFrom, char* fTo, unsigned int fLength);

#endif
