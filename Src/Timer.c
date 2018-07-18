#include "RM.h"
#include "Timer.h"

extern sbit capSel=P2^4;			//电容选择接口
extern bit isTimerEvnet;			//记录是否有定时器事件发生（完成一次频率测量）
extern long curN;					//当前测得脉冲数
extern float curFreq;				//当前测得频率
extern float curRValue;				//记录当前测得电阻阻值
extern long refLowRN;				//低档位下参考脉冲数
extern long refHignRN;				//高档位下参考脉冲数

uchar t0IntrTimes;					//T0用于计时，T1用于计数，两个变量储存他们的溢出次数
uchar t1IntrTimes;
/**
 * 设定初值,中断允许,计时优先，设定TMOD(51)
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 用于软重设，因此所有全局变量都需要重设
 */
void InitialTimers(){
	EA=1;							//允许中断，并让计时器的中断优先级最高
	ET0=1;
	ET1=1;
	PT0=1;
	PT1=0;
	TMOD=0x51;						//T0用于计时，T1用于计数
	capSel=CAPSEL_LOW;				//重置所有全局变量
	curRValue=0;
	isTimerEvnet=0;
	curN=0;
	curFreq=0;
	refLowRN=0;
	refHignRN=0;
	t0IntrTimes=0;
	t1IntrTimes=0;
	TH0=TL0=0;
	TH1=TL1=0;
}
/**
 * 初始化T0,T1;开启计时器，开始测量频率
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void StartTimer(){
	t0IntrTimes=0;
	t1IntrTimes=0;
	TH0=TL0=0;
	TH1=TL1=0;
	TR0=1;
	TR1=1;
}

void T1INT() interrupt 3 using 2{
	t1IntrTimes++;
}

void T0INT() interrupt 1 using 1{
	
}
