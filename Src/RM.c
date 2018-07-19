#include "RM.h"
/**
 * 函数功能：延时nms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary Omitted
 */
void delaynms(uchar n)
{
	uchar  i;
	for(i=0;i<n;i++) delay1ms();
}
/**
 * 函数功能：延时1ms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary (3j+2)*i=(3 × 33+2)× 10=1010(微秒)，可以认为是1 毫秒
 */
void delay1ms()
{
	uchar i,j;
	for(i=0;i<10;i++) 
		for(j=0;j<33;j++);
}