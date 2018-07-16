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

/**
 * 函数功能：延时nms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary Omitted
 */
void Delaynms(uchar n);
/**
 * 函数功能：延时1ms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary (3j+2)*i=(3 × 33+2)× 10=1010(微秒)，可以认为是1 毫秒
 */
void Delay1ms();

#endif