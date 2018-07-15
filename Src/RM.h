#ifndef _RM_H
#define _RM_H

#include <reg52.h>
#include <intrins.h>
#include "STC12.h"

#define Uchar unsigned char
#define Uint unsigned int
#define uchar unsigned char
#define uint unsigned int

#define 3nFCAP 3e-9
#define 10pFCAP 1e-11
#define LN2 0.69314718

void Delaynms(uchar n);
void Delay1ms();

#endif