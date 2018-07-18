#include "RM.h"
#include "LCD1602.h"
#include "Key.h"
#include "Timer.h"

////////////////////////////////
//TODO:设置各个I/O口的输出模式 //
///////////////////////////////

//-------------------------按键相关全局变量-------------------------------//
sbit key1=P1^5;					//注意低电平表示按键按下
sbit key2=P1^4;
sbit key3=P1^3;

uchar key1Events;			//保存Key1未处理的输入信号
uchar key2Events;			//保存Key1未处理的输入信号
uchar key3Events;			//保存Key1未处理的输入信号

bit isKeyEvents;			//记录是否有按键事件发生

//-------------------------计时器相关全局变量-----------------------------//
sbit capSel=P2^4;			//电容选择接口
float curRValue;				//当前测得电阻阻值
bit isTimerEvnet;			//记录是否有定时器事件发生（完成一次频率测量）
long curN;					//当前测得脉冲数
float curFreq;				//当前测得频率
long refLowRN;				//低档位下参考脉冲数
long refHignRN;				//高档位下参考脉冲数

