#ifndef _NEWSTRING_H_
#define _NEWSTRING_H_

#include <stdarg.h>
#include <stdint.h>

void strConcat(char *Destination, int MaxLength, int Count, ...);

int strCompare(char* String1, char *String2);

char *num2Str(uint32_t Number, char * Str);

#endif
