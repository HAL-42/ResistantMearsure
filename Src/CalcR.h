#ifndef _CALCR_H
#define _CALCR_H

/**
 * 计算脉冲数，频率和电阻
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury  本函数中魔鬼数字较多，为的是提高计算精度。所有数据都是在MATLAB中计算后带入，切勿随意修改
 */
void GetRVal();
/**
 * 初始化计算R有关的全局变量
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-22
 * @Summury
 */
void InitialCalcR();

#endif