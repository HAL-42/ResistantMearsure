#include "RM.h"
#include "LCD1602.h"
#include "Key.h"
#include "Timer.h"
#include "Menu.h"
#include "CalcR.h"

//----------------------------LED关全局变量-------------------------------//
sbit	Led1 = P1^0;
sbit	Led2 = P1^1;
sbit	Led3 = P1^2;

//-------------------------按键相关全局变量-------------------------------//
sbit key1=P1^5;				//注意低电平表示按键按下
sbit key2=P1^4;
sbit key3=P1^3;

uchar key1Events;			//保存Key1未处理的输入信号
uchar key2Events;			//保存Key1未处理的输入信号
uchar key3Events;			//保存Key1未处理的输入信号

bit isKeyEvents;			//记录是否有按键事件发生 

//-------------------------电阻计算相关全局变量-----------------------------//
sbit  capSel=P3^7;					//电容选择接口
bit   isLongMeasr;
bit   isAverg;
float idata curRValue;				//当前测得电阻阻值
unsigned long  idata curN;			//当前测得脉冲数
unsigned long  idata refLowRN;		//低档位下参考脉冲数
unsigned long  idata refHighRN;		//高档位下参考脉冲数

code char *mearsStr="Mearsuring...";
//-------------------------计时器相关全局变量-----------------------------//
uchar timerFun;				//选择计时器作用
bit   isTimerEvent;			//记录是否有定时器事件发生（完成一次频率测量）
extern uchar t1IntrTimes;
extern uchar t0IntrTimes;
//-------------------------筛选器相关全局变量-----------------------------//
bit   isSieveOn;			//筛选器是否打开
float errTolr;				//筛选器允许误差
float sieveRVal;			//筛选器中心值

//-------------------------调试模式相关全局变量---------------------------//
bit isDebug=1;				//是否处于调试模式

	///////////////////
	//TODO:完善以下函数 //
	///////////////////
void InitialSys();
void SetZero();
void KeyEventsCallBack();
void TimerEventsCallBack();

void main(){
	InitialSys();
	SetZero();
	isLongMeasr=0;
	StartTimer();
	while(1){
		KeyScan();
		if(isKeyEvents)  KeyEventsCallBack();
		if(isTimerEvent) TimerEventsCallBack();
	}
}

void InitialSys(){
	//定义端口输入输出,p1^0-P1^2为推挽输出，p1^3-P1^7为输入，设置PxM0，PxM1；关闭各个LED
	P1M1 = 0x47;								//8'b01000111
	P1M0 = 0xb8;								//8'b10111000
	P3M1 = 0x00;								//8'b00000000
	P3M0 = 0x20;								//8'b00100000
	Led1=Led2=Led3=0;
	//初始化其他外围设备
	LcdInitiate();
	InitialTimers();							//初始化计时器	
	KeyInitial();								//初始化键盘
	//初始化菜单与电阻计算所需变量
	InitialMenu();
	InitialCalcR();
}

void SetZero(){
	capSel=CAPSEL_LOWR;
	isLongMeasr=1;

	LCDPrintScreen("Cross Probes And","Press Any Button");
	PressAnyKey();												//等待用户按照指示短接红黑表笔
	LCDPrintScreen(mearsStr,"Please Wait");
	
	StartTimer();												//开始采样
	while(!isTimerEvent);
	GetRVal();													//低档位采样结束，算出低档位下的“标准N”
	refLowRN=curN;
	LCDCls();
	LCDPrintScreen("Press Any Button","To Switch Cap");
	PressAnyKey();
	capSel=CAPSEL_HIGHR;										//设定到高档位，并等待1秒，让继电器反应过来
	delaynms(1000);
	StartTimer();												//高档位采样
	while(!isTimerEvent);
	GetRVal();
	refHighRN=curN;
	
	LCDPrintScreen("Set Zero","Finished");						//调0完毕
	capSel=CAPSEL_LOWR;
	delaynms(1000);
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
			isDebug=!isDebug;								//短按改变调试
			break;
		case LONG_PRESS:
			isSieveOn=!isSieveOn;							//长按改变筛选模式
			break;
		default:
			break;
	}
	switch(key2Events){
		case SHORT_PRESS:
			KeyInitial();									//初始化外设，进入子程序
			SwitchTimerFun(TIMERFUN_HALT);
			MenuImpl();										//菜单处理子程序
			SwitchTimerFun(TIMERFUN_FREQ_MEASRURE);			//退出子程序，外设归位
			StartTimer();
			KeyInitial();
			LCDCls();
			LCDPrintStr(0,0,"Processing...");
			break;
		case LONG_PRESS:	
			Led3=Led2=Led1=0;								//长按关灯
			break;
		default:
			break;
	}
	switch(key3Events){
		case SHORT_PRESS:
			KeyInitial();
			SwitchTimerFun(TIMERFUN_HALT);
			MenuOpRstDev();
			break;
		case LONG_PRESS:
			Led3=Led2=Led1=1;								//长按开灯
			break;
		default:
			break;
	}
	RstKeyEvents();
}

void TimerEventsCallBack(){
	GetRVal();
	StartTimer();
	LCDCls();
	LCDPrintStr(0,0,"R=");
	LCDPrintFloat(2,0,curRValue);
	if(isDebug){
		LCDPrintNum(8,1,curN);
	}
	else{
		LCDErase(8,1,8);
	}
	if(isSieveOn){
		LCDPrintChar(15,0,0x00);
		if( (curRValue-sieveRVal)/sieveRVal < errTolr &&
			(curRValue-sieveRVal)/sieveRVal > -errTolr ){
			LCDPrintStr(0,1,"In ");
		}
		else{
			LCDPrintStr(0,1,"Out");
		}
	}
}