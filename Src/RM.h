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
void delaynms(uchar n);
/**
 * 函数功能：延时1ms
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summary (3j+2)*i=(3 × 33+2)× 10=1010(微秒)，可以认为是1 毫秒
 */
void delay1ms();

#endif