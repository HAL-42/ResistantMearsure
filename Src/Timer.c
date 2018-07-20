#include "RM.h"
#include "Timer.h"

#define LOADCOEF_TH 254
#define LOADCOEF_TL	51

extern uchar timerFun;

sbit capSel=P2^4;					//电容选择接口
extern bit isTimerEvent;			//记录是否有定时器事件发生（完成一次频率测量）
extern long curN;					//当前测得脉冲数
extern long curFreqE5;				//当前测得频率
extern float curRValue;				//记录当前测得电阻阻值
extern long refLowRN;				//低档位下参考脉冲数
extern long refHighRN;				//高档位下参考脉冲数

static uchar t0IntrTimes;			//T0用于计时，T1用于计数，两个变量储存他们的溢出次数
static uchar t1IntrTimes;
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
	capSel=CAPSEL_LOWR;				//重置所有全局变量
	curRValue=0;
	isTimerEvent=0;
	curN=0;
	curFreqE5=0;
	refLowRN=0;
	refHighRN=0;
	t0IntrTimes=0;
	t1IntrTimes=0;
	TH0=TL0=0;
	TH1=TL1=0;
	TR1=TR0=0;
	timerFun=TIMERFUN_FREQ_MEASRURE;		//计时器初始功能为频率测量
}
/**
 * 切换计时器功能
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    fun        功能描述
 */
void SwitchTimerFun(uchar fun){
	timerFun=fun;
	TR1=TR0=0;								//切换功能前先让定时器停止工作，下次使用时强制用StartTimer初始化
	switch(fun){
		case TIMERFUN_FREQ_MEASRURE:		//频率测量，T1计数，T2计时
			TMOD=0x51;
			break;
		case TIMERFUN_PWM:					//PWM波输出控制舵机，两个都计时
			TMOD=0x11;
			break;
		case TIMERFUN_KEY_SCAN:				//键盘扫描，两个都计时
			TMOD=0x11;
			break;
		case TIMERFUN_HALT:					//键盘扫描
			break;
	}
}
/**
 * 初始化T0,T1;开启计时器，开始测量频率
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void StartTimer(){
	isTimerEvent=0;							//保险起见，再重置一次时钟事件，防止客户没有使用GetRVal函数
	t0IntrTimes=0;						
	t1IntrTimes=0;
	switch(timerFun){
		case TIMERFUN_FREQ_MEASRURE:
			TH0=TL0=0;						//计时器从0开始计时
			TH1=TL1=0;						//计数器从0开始计数
			TR0=1;							//开启计时器
			TR1=1;							//开启计数器
			break;
		case TIMERFUN_PWM:
			TH1=LOADCOEF_TH;				//0.5ms中断的计时器
			TL1=LOADCOEF_TL;
			TR0=0;
			TR1=1;							//打开计时器1
			break;
		case TIMERFUN_KEY_SCAN:
			TH1=LOADCOEF_TH;				//0.5ms中断的计时器
			TL1=LOADCOEF_TL;
			TR0=0;
			TR1=1;							//打开计时器1
			break;
		case TIMERFUN_HALT:							
			break;
	}
}
/**
 * T1中断，溢出计数加一
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void T1INT() interrupt 3 using 2{
	switch(timerFun){
		case TIMERFUN_FREQ_MEASRURE:		//溢出计数加一，自动重装
			t1IntrTimes++;	
			break;					
		case TIMERFUN_PWM:
			/////////
			//TODO:NOT FINISHED//
			/////////
			break;
		case TIMERFUN_KEY_SCAN:
			TL1=LOADCOEF_TL;
			TH1=LOADCOEF_TH;
			isTimerEvent=1;
			if(++t1IntrTimes>=2){
				t1IntrTimes=0;
				KeyScan();
			} 
			break;
		case TIMERFUN_HALT:
			TR1=TR0=0;
			break;
	}
}
/**
 * T0中断
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 */
void T0INT() interrupt 1 using 1{
	if((capSel==CAPSEL_LOWR)		//若为低电阻档且两次溢出
		&&(++t0IntrTimes==2)){
		TR1=0;						//关停计数器，计时器
		TR0=0;
		isTimerEvent=1;				//告知主程序发生计时器事件
	}
	else if((capSel==CAPSEL_HIGHR)	//若为高电阻档且15次溢出
		&&(++t0IntrTimes==15)){
		TR1=0;						//关停计数器，计时器
		TR0=0;
		isTimerEvent=1;				//告知主程序发生计时器事件
	}
}

/////////
//TODO:防止溢出 //
/////////

/**
 * 计算脉冲数，频率和电阻
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury  本函数中魔鬼数字较多，为的是提高计算精度。所有数据都是在MATLAB中计算后带入，切勿随意修改
 */
void GetRVal(){
	isTimerEvent=0;
	curN=t1IntrTimes*65536+TH1*256+TL1; // 计算脉冲次数
	if(capSel==CAPSEL_LOWR){
		curFreqE5=703125*curN;
		curRValue=1.69065825104e9F*( (refLowRN-curN)/(refLowRN*curN) );
	}
	else{
		curFreqE5=93750*curN;
		curRValue=1.53887471028e11F*( (refHighRN-curN)/(refHighRN*curN) );
	}
}
