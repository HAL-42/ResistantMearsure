#ifndef _RM_H
#define _RM_H

#include <reg52.h>
#include <intrins.h>
#include "STC12C.h"

#define Uchar unsigned char
#define Uint unsigned int
#define uchar unsigned char
#define uint unsigned int

/**
 * 函数功能：延时nms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary Omitted
 */
void delaynms(uint n);
/**
 * 函数功能：延时1ms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary (3j+2)*i=(3 × 33+2)× 10=1010(微秒)，可以认为是1 毫秒
 */
void delay1ms();
/**
 * 延迟n个机器周期,其实是3n还要多，我不确定，意思意思就行
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    n          和延迟时间成正比的某个数，调试确定
 */
void delaynMC(uint n);

#endif