#include "RM.h"
/**
 * 函数功能：延时nms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary Omitted
 */
void delaynms(uint n)
{
	uint  i;
	for(i=0;i<n;i++) delay1ms();
}
/**
 * 函数功能：延时1ms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary 网上抄的，错了不怪我
 */
void delay1ms()
{
	uint x,y;
	for(x=1; x>0; x--)
		for(y=856; y>0; y--);

}
/**
 * 延迟n个机器周期,其实是3n还要多，我不确定，意思意思就行
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    n          和延迟时间成正比的某个数，调试确定
 */
void delaynMC(uint n){
	while(n){
		_nop_();
		n--;
	}
}