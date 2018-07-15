#include "RM.h"

void delaynms(uchar n)
{
	uchar  i;
	for(i=0;i<n;i++) delay1ms();
}

void Delay1ms()
{
	uchar i,j;
	for(i=0;i<10;i++) 
		for(j=0;j<33;j++);
}