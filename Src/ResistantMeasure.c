//这是一个测试版本


#include "RM.h"
#include "LCD1602.h"
#include "Key.h"
#include "Timer.h"

//----------------------------LED关全局变量-------------------------------//
sbit	Led1 = P1^0;
sbit	Led2 = P1^1;
sbit	Led3 = P1^2;

//-------------------------按键相关全局变量-------------------------------//
sbit key1=P1^5;					//注意低电平表示按键按下
sbit key2=P1^4;
sbit key3=P1^3;

uchar key1Events;			//保存Key1未处理的输入信号
uchar key2Events;			//保存Key1未处理的输入信号
uchar key3Events;			//保存Key1未处理的输入信号

bit isKeyEvents;			//记录是否有按键事件发生

//-------------------------计时器相关全局变量-----------------------------//
uchar timerFun;				//选择计时器作用

sbit capSel=P2^4;			//电容选择接口
float curRValue;			//当前测得电阻阻值
bit isTimerEvent;			//记录是否有定时器事件发生（完成一次频率测量）
long curN;					//当前测得脉冲数
float curFreqE5;			//当前测得频率
long refLowRN;				//低档位下参考脉冲数
long refHighRN;				//高档位下参考脉冲数

//-------------------------筛选器相关全局变量-----------------------------//
bit isSieveOn;				//筛选器是否打开
uchar sieveRVal;			//筛选器电阻中心值
uchar sieveRUnit;			//筛选器的中心值的单位
uchar errTolrE1;			//筛选器允许误差

//-------------------------调试模式相关全局变量---------------------------//
bit isDebug=1;				//是否处于调试模式

	///////////////////
	//TODO:完善以下函数 //
	///////////////////
void SetZero();
void KeyEventsCallBack();
void TimerEventsCallBack();

void main(){
	//定义端口输入输出,p1^0-P1^2为推挽输出，p1^3-P1^7为输入，设置PxM0，PxM1
	P1M1 = 0x07;								//8'b00000111
	P1M0 = 0xf8;								//8'b11111000
	P3M1 = 0x80;								//8'b10000000
	P3M0 = 0x20;								//8'b00100000
	Led1=Led2=Led3=0;
	//初始化其他外围设备
	InitialTimers();							//初始化计时器	
	KeyInitial();								//初始化键盘
	while(1){
		SetZero();
		Led1=Led2=Led3=0;
	}
	StartTimer();
	while(1){
		KeyScan();
		if(isKeyEvents) KeyEventsCallBack();
		if(isTimerEvent) TimerEventsCallBack();
	}
}

void SetZero(){
	extern uchar t1IntrTimes;
	extern uchar t0IntrTimes;
	LCDPrintScreen("Cross Probes And","Press Any Button");
	PressAnyKey();												//等待用户按照指示短接红黑表笔
	LCDPrintScreen("Mearsuring...","Please Wait");
	capSel=CAPSEL_LOWR;											//先归零低档位
	delaynms(3000);												//等待某些反应迟钝的用户
	StartTimer();												//开始采样
	while(!isTimerEvent);
	GetRVal();													//低档位采样结束，算出低档位下的“标准N”
	if(!TL1) Led1=1;
	refLowRN=curN;
	capSel=CAPSEL_HIGHR;										//设定到高档位，并等待0.2秒，让继电器反应过来
	delaynms(1000);
	StartTimer();												//高档位采样
	while(!isTimerEvent);
	GetRVal();
	if(!TL1) Led3=1;
	refHighRN=curN;
	LCDPrintScreen("Set Zero","Finished");						//调0完毕
	delaynms(2000);
	if(isDebug){												//调试模式下，显示调0测量结果
		LCDCls();
		LCDPrintStr(0,0,"LOW");
		LCDPrintNum(6,0,refLowRN);
		LCDPrintStr(0,1,"HIGH");
		LCDPrintNum(6,1,refHighRN);
		PressAnyKey();
	}
}

void KeyEventsCallBack(){
	switch(key1Events){
		case SHORT_PRESS:
			LCDPrintScreen("KEY1","SHORT_PRESS");
			break;
		case LONG_PRESS:
			LCDPrintScreen("KEY1","LONG_PRESS");
			break;
	}
	switch(key2Events){
		case SHORT_PRESS:
			LCDPrintScreen("KEY2","SHORT_PRESS");
			break;
		case LONG_PRESS:	
			LCDPrintScreen("KEY2","LONG_PRESS");
			break;
	}
	switch(key3Events){
		case SHORT_PRESS:
			LCDPrintScreen("KEY3","SHORT_PRESS");
			break;
		case LONG_PRESS:
			LCDPrintScreen("KEY3","LONG_PRESS");
			break;
	}
	RstKeyEvents();
	PressAnyKey();		
}

void TimerEventsCallBack(){
	GetRVal();
	StartTimer();
	LCDCls();
	LCDPrintStr(0,0,"R=");
	LCDPrintFloat(2,0,curRValue);
	if(isDebug) LCDPrintNum(8,1,curN);
	if(isSieveOn){
		LCDPrintChar(15,0,0x00);
		//////////////////////
		//TODO:Not Finished //
		//////////////////////
	}
}