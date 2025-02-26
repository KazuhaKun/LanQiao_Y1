#include <REG52.H>
#include "HC138.h"
void OutPutP0(unsigned char channel,unsigned char dat)
{
    InitHC138(channel);
    P0 = dat;
}