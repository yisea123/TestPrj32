#ifndef  _74HC595_
#define  _74HC595_

#include "_include.h"   

void WriteToOutLed(u8* data, u8 num);
void WriteToInLed(u8* data, u8 num);
void Init74HC595(void);

#endif
