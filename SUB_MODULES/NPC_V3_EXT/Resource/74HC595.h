#ifndef  _74HC595_
#define  _74HC595_

#include "cdv_include.h"   

void WriteToOutLed(u8* data, u8 num);
void WriteToInLed(u8* data, u8 num);
void PinConfig74HC595(void);

#endif
