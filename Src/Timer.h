#ifndef _TIMER_H
#define _TIMER_H

#define CAPSEL_LOWR 0
#define CAPSEL_HIGHR 1

#define TIMERFUN_FREQ_MEASRURE  0					//计时器作用：测量频率
#define TIMERFUN_PWM     		1					//计时器作用：输出PWM波控制舵机
#define TIMERFUN_KEY_SCAN       2					//计时器作用：扫描键盘
#define TIMERFUN_HALT	        3					//计时器作用：挂起暂停，不工作

/**
 * 设定初值,中断允许,计时优先，设定TMOD(51)
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 用于软重设，因此所有全局变量都需要重设
 */
void InitialTimers();
/**
 * 切换计时器功能
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    fun        功能描述
 */
void SwitchTimerFun(uchar fun);
/**
 * 初始化T0,T1;开启计时器，开始测量频率
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void StartTimer();
/**
 * T1中断，溢出计数加一
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void T1INT();
/**
 * T0中断
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void T0INT();
/**
 * 计算脉冲数，频率和电阻
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury  本函数中魔鬼数字较多，为的是提高计算精度。所有数据都是在MATLAB中计算后带入，切勿随意修改
 */
void GetRVal();

#endif