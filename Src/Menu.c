#include "RM.h"
#include "Menu.h"
#include "Timer.h"
#include "Key.h"
#include "LCD1602.h"
#include "CalcR.h"

#define MENUID_DEBUG_MODE_ON 	0					//菜单选项ID的宏定义
#define MENUID_SIEVE_MODE_ON 	1
#define MENUID_SET_SIEVE     	3
#define MENUID_START_PLOT    	4
#define MENUID_RESET_DEVICE  	5
#define MENUID_EXIT          	6
#define MENUID_DEBUG_YES		7
#define MENUID_DEBUG_NO    		8
#define MENUID_SIEVE_YES    	9		
#define MENUID_SIEVE_NO    		10	
#define MENUID_AVERGE_YES       11
#define MENUID_AVERGE_NO		12
#define MENUID_AVERG_MODE_ON	13	

#define MENU_OPTION_NUM_MAIN 	7					//菜单条目数的宏定义
#define MENU_OPTION_NUM_YN	 	2

typedef struct MenuRecord *ptrToMenuRecord;			//定义指向菜单记录的指针
typedef ptrToMenuRecord   Menu;						//指向菜单记录的指针直接叫Menu
typedef struct MenuOp     *ptrToMenuOp;				//指向菜单选项的指针

struct MenuOp{										//菜单选项的结构
	uchar opID;										//选项的ID
	char *opString;									//选项显示的字符串
	Menu subMenu;									//选项的子菜单
	void (*pOpFunc) (void);  						//选择该选项执行的函数
};

struct MenuRecord{									//菜单记录的结构
	uchar opNum;									//该菜单选项的条目数
	Menu superMenu;									//该菜单的上一级菜单
	ptrToMenuOp OpArray;							//该菜单的条目数组
};

static code struct MenuOp debugYNOpArray[MENU_OPTION_NUM_YN]={								//Yes No 菜单的选项数组
	{MENUID_DEBUG_YES,"Yes",NULL,MenuOpYN},
	{MENUID_DEBUG_NO,"No",NULL,MenuOpYN}
};
static struct MenuRecord debugYNMenuRecord={MENU_OPTION_NUM_YN,NULL,debugYNOpArray};		//Yes No  菜单的菜单记录
static code Menu debugYNMenu=&debugYNMenuRecord;											//用叫做ynMenu的指针指向记录

static code struct MenuOp sieveYNOpArray[MENU_OPTION_NUM_YN]={								//Yes No 菜单的选项数组
	{MENUID_SIEVE_YES,"Yes",NULL,MenuOpYN},
	{MENUID_SIEVE_NO,"No",NULL,MenuOpYN}
};
static struct MenuRecord sieveYNMenuRecord={MENU_OPTION_NUM_YN,NULL,sieveYNOpArray};		//Yes No  菜单的菜单记录
static code Menu sieveYNMenu=&sieveYNMenuRecord;											//用叫做ynMenu的指针指向记录

static code struct MenuOp avergYNOpArray[MENU_OPTION_NUM_YN]={								//Yes No 菜单的选项数组
	{MENUID_AVERGE_YES,"Yes",NULL,MenuOpYN},
	{MENUID_AVERGE_NO,"No",NULL,MenuOpYN}
};
static struct MenuRecord avergYNMenuRecord={MENU_OPTION_NUM_YN,NULL,avergYNOpArray};		//Yes No  菜单的菜单记录
static code Menu avergYNMenu=&avergYNMenuRecord;											//用叫做ynMenu的指针指向记录

static code struct MenuOp MainMenuOpArray[MENU_OPTION_NUM_MAIN]={							//main菜单的选项数组				
	{MENUID_DEBUG_MODE_ON,"Debug Mode On",&debugYNMenuRecord,NULL},
	{MENUID_AVERG_MODE_ON,"Averg Mode On",&avergYNMenuRecord,NULL},
	{MENUID_SIEVE_MODE_ON,"Sieve Mode On",&sieveYNMenuRecord,NULL},
	{MENUID_SET_SIEVE,"Set Sieve",NULL,MenuOpSetSieve},
	{MENUID_START_PLOT,"Start Plot",NULL,MenuOpStartPlot},
	{MENUID_RESET_DEVICE,"Reset Device",NULL,MenuOpRstDev},
	{MENUID_EXIT,"Exit",NULL,MenuOpExit}
};
static code struct MenuRecord mainMenuRecord={MENU_OPTION_NUM_MAIN,NULL,MainMenuOpArray};	//main菜单的菜单记录
static code Menu mainMenu=&mainMenuRecord;												    //用叫做mainMenu的指针指向记录

static Menu  curMenu;								//指向当前菜单的记录
static char curOp;									//当前条目

static bit isExit;									//消息变量：是否退出菜单

extern bit   isSieveOn;								//消息变量:筛选是否打开
extern float sieveRVal;								//筛选电阻的中心值
extern float errTolr;								//误差限
extern bit   isDebug;								//消息变量:调试模式打开
extern bit   isAverg;

sbit	Led1 = P1^0;								//Led变量
sbit	Led2 = P1^1;
sbit	Led3 = P1^2;

sbit servoConOut = P3^7;

static uchar pwmTCount;				
static uchar pwmNCount;				
static xdata float pwmRVal[17];	

sbit  capSel=P3^7;
extern float idata curRValue;
extern code char *mearsStr;
code char *disconnectStr="Disconnect P37";
code char *connectStr="Connect P37";

extern uchar t1IntrTimes;		
extern bit isKeyEvents;
extern bit isTimerEvent;

extern uchar key1Events;
extern uchar key2Events;
extern uchar key3Events;

static uint  rNumPart;								//筛选电阻的三位数字部分
static uchar rUnit;									//筛选电阻的单位，可以是 0x20 'K' 'M'
static uchar errTolrE2;								//误差限*100
/**
 * 初始化菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void InitialMenu(){
	debugYNMenu->superMenu=mainMenu;				//菜单前后互指，因此一部分指向工作要到程序完成
	sieveYNMenu->superMenu=mainMenu;
	avergYNMenu->superMenu=mainMenu;
	curMenu=mainMenu;								//初始化所有有关全局变量
	curOp=0;
	isExit=0;
	isSieveOn=0;
	sieveRVal=0.0;
	errTolr=0.0;
	rNumPart=0;
	rUnit=0x20;
	errTolrE2=0;
	isDebug=1;
	pwmTCount=0;
	pwmNCount=0;
}
/**
 * 打开菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuImpl(){
	curMenu=mainMenu;								//从头开始显示菜单
	curOp=0;
	isExit=0;
	ShowMenu();
	SwitchTimerFun(TIMERFUN_KEY_SCAN);				//开始键盘扫描
	StartTimer();
	while(!isExit){
		if(isKeyEvents){
			SwitchTimerFun(TIMERFUN_HALT);			//TODO:或许可以优化掉
			MenuKeyTreat();
			SwitchTimerFun(TIMERFUN_KEY_SCAN);		//重启键盘扫描
			StartTimer();
		}
	}
}
/**
 * 显示菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void ShowMenu(){
	LCDCls();
	if(curOp!=0){
		LCDMoveCursor(1,0);										//此处有莫名其妙的bug，需要专门重设光标
		LCDPrintStr(1,0,curMenu->OpArray[curOp-1].opString);
		LCDPrintStr(1,1,curMenu->OpArray[curOp].opString);
		LCDPrintChar(0,1,'>');
	}
	else{
		LCDMoveCursor(1,0);
		LCDPrintStr(1,0,curMenu->OpArray[curOp].opString);
		LCDPrintStr(1,1,curMenu->OpArray[curOp+1].opString);
		LCDPrintChar(0,0,'>');
	}
}
/**
 * 处理菜单模式下的按键信息
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuKeyTreat(){
	if(key1Events){											//key1优先级>key2优先级>key3优先级
		switch(key1Events){
			case SHORT_PRESS:
				curOp=(++curOp)%(curMenu->opNum);			//向下选择
				ShowMenu();
				break;
			case LONG_PRESS:
				curOp=curMenu->opNum-1;
				ShowMenu();
				break;
		}
	}
	else if(key2Events){
		switch(key2Events){
			case SHORT_PRESS:								//向上选择
				curOp=((--curOp)<0?curOp+(curMenu->opNum):curOp);	
				ShowMenu();
				break;
			case LONG_PRESS:
				curOp=0;
				ShowMenu();
				break;
		}
	}
	else{
		switch(key3Events){
			case SHORT_PRESS:
			if(curMenu->OpArray[curOp].subMenu!=NULL){
				curMenu=curMenu->OpArray[curOp].subMenu;	//进入子菜单
				curOp=0;
				ShowMenu();
			}
			else{
				KeyInitial();								//初始化外设，时钟已经在MenuImpl中停止
				isExit=1;									//设定退出模式
				(*(curMenu->OpArray[curOp].pOpFunc)) ();	//执行选项的函数
			}	
				break;
			case LONG_PRESS:								//长按可以回到上级菜单
				if(curMenu->superMenu!=NULL){
					curMenu=curMenu->superMenu;
					curOp=0;
					ShowMenu();
				}
				break;
			default:
				break;
		}
	}
	RstKeyEvents();
}
/**
 * 什么都不做
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpExit(){
	return;
}
/**
 * 利用IAP_CONTR进行软件复位
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpRstDev(){
	uint i=3000;
	Led3=Led2=Led1=0;										//灭灯，等待激动人心的一刻
	SwitchTimerFun(TIMERFUN_KEY_SCAN);
	StartTimer();
	while(i){
		if(isTimerEvent){
			isTimerEvent=0;
			i--;											//每0.5ms让i减一，0.5S亮一个灯
			if(i==2000){
				Led3=1;
			}
			else if(i==1000){
				Led2=1;
			}
		}
		if(isKeyEvents){									//如有键盘事件，立即中断函数执行
			Led3=Led2=0;
			return;
		}
	}
	Led1=1;
	delaynms(500);
	ISP_CONTR=0x60;											//软件复位
}
/**
 * YN菜单执行，用于改变debug和sieve模式
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpYN(){
	switch(curMenu->OpArray[curOp].opID){
		case MENUID_DEBUG_YES:
			isDebug=1;
			break;
		case MENUID_DEBUG_NO:
			isDebug=0;
			break;
		case MENUID_SIEVE_YES:
			isSieveOn=1;
			break;
		case MENUID_SIEVE_NO:
			isSieveOn=0;
			break;
		case MENUID_AVERGE_YES:
			isAverg=1;
			break;
		case MENUID_AVERGE_NO:
			isAverg=0;
			break;
	}
}
/**
 * 转动舵机画曲线
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpStartPlot(){
	uchar curShow;
	uchar isBreak;
	LCDCls();
	capSel=CAPSEL_LOWR;
	delaynms(500);
	LCDPrintLine(0,0,disconnectStr);
	PressAnyKey();

	LCDPrintLine(0,0,mearsStr);
	pwmTCount=0;
	pwmNCount=0;
	SwitchTimerFun(TIMERFUN_PWM);
	StartTimer();
	while(1){
		if(isTimerEvent){
			if(pwmTCount==17){					
				break;
			}

			if(t1IntrTimes==201){				
				++pwmNCount;					
				t1IntrTimes=1;
			}
			if(t1IntrTimes<=7+pwmTCount)
				servoConOut=1;					
			else
				servoConOut=0;

			if(pwmNCount==100){
				servoConOut=0;
				pwmNCount=0;
				SwitchTimerFun(TIMERFUN_FREQ_MEASRURE);	
				StartTimer();
				while(!isTimerEvent);
				GetRVal();						
				pwmRVal[pwmTCount++]=curRValue;	
				SwitchTimerFun(TIMERFUN_PWM);	
				StartTimer();					
			}
		}
	}
	curShow=0;
	isBreak=0;
	LCDCls();
	LCDPrintFloat(0,0,pwmRVal[curShow]);
	SwitchTimerFun(TIMERFUN_KEY_SCAN);
	StartTimer();
	while(!isBreak){
		if(isKeyEvents){
			if(key1Events){
				switch(key1Events){
					case SHORT_PRESS:
						curShow=(++curShow)%17;
						break;
					case LONG_PRESS:
						curShow=16;
						break;
				}
			}
			else if(key2Events){
				switch(key2Events){
					case SHORT_PRESS:
						curShow= (--curShow)<0?16:curShow;
						break;
					case LONG_PRESS:
						curShow=0;
						break;
				}
			}
			else{
				switch(key3Events){
					case SHORT_PRESS:
						isBreak=1;
						break;
					default:
						break;
				}
			}
			LCDPrintFloat(0,0,pwmRVal[curShow]);
			RstKeyEvents();
		}
	}
	LCDPrintLine(0,0,connectStr);
	PressAnyKey();
	capSel=CAPSEL_LOWR;
}
/**
 * 设置筛选条件
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpSetSieve (){
	uchar curDigit=0;
	uchar isBreak=0;
	LCDCls();
	LCDPrintNumFixdgt(0,0,rNumPart,3);
	LCDPrintChar(3,0,rUnit);
	LCDPrintChar(curDigit,1,'^');
	SwitchTimerFun(TIMERFUN_KEY_SCAN);
	StartTimer();
	while(!isBreak){
		if(isKeyEvents){
			if(key1Events){
				switch(key1Events){
					case SHORT_PRESS:
						switch(curDigit){
							case 0:
								rNumPart=(rNumPart+100)%1000;
								break;
							case 1:
								rNumPart=(rNumPart+10)%1000;
								break;
							case 2:
								rNumPart=(rNumPart+1)%1000;
								break;
							case 3:
								switch(rUnit){
									case 0x20:
										rUnit='K';
										break;
									case 'K':
										rUnit='M';
										break;
									case 'M':
										rUnit=0x20;
										break;
								}
							break;
						}
						break;
					case LONG_PRESS:
						switch(curDigit){
							case 0:
								rNumPart%=100;
								break;
							case 1:
								rNumPart-=((rNumPart%100)/10)*10;
								break;
							case 2:
								rNumPart-=rNumPart%10;
								break;
							case 3:
								rUnit=0x20;
								break;
						}
						break;
				}
				LCDPrintNumFixdgt(0,0,rNumPart,3);
				LCDPrintChar(3,0,rUnit);
			}
			else if(key2Events){
				LCDPrintChar(curDigit,1,0x20);
				switch(key2Events){
					case SHORT_PRESS:
						curDigit=++curDigit%4;
						break;
					case LONG_PRESS:
						if(curDigit==3){
							curDigit=0;
						}
						else{
							curDigit=3;
						}
						break;
				}
				LCDPrintChar(curDigit,1,'^');
			}
			else{
				switch(key3Events){
					case SHORT_PRESS:
						isBreak=1;
						break;
					default:
						break;
				}
			}
			RstKeyEvents();	
		}
	}
	curDigit=0;
	isBreak=0;
	LCDCls();
	LCDPrintNumFixdgt(0,0,errTolrE2,2);
	LCDPrintChar(2,0,'%');
	LCDPrintChar(curDigit,1,'^');
	while(!isBreak){
		if(isKeyEvents){
			if(key1Events){
				switch(key1Events){
					case SHORT_PRESS:
						switch(curDigit){
							case 0:
								errTolrE2=(errTolrE2+10)%100;
								break;
							case 1:
								errTolrE2=(++errTolrE2)%100;
								break;
						}
						break;
					case LONG_PRESS:
						switch(curDigit){
							case 0:
								errTolrE2%=10;
								break;
							case 1:
								errTolrE2-=errTolrE2%10;
								break;
						}
						break;
				}
				LCDPrintNumFixdgt(0,0,errTolrE2,2);
			}
			else if(key2Events){
				LCDPrintChar(curDigit,1,0x20);
				switch(key2Events){
					case SHORT_PRESS:
						curDigit=(++curDigit)%2;
						break;
					case LONG_PRESS:
						curDigit=0;
						break;
				}
				LCDPrintChar(curDigit,1,'^');
			}
			else{
				switch(key3Events){
					case SHORT_PRESS:
						isBreak=1;
						break;
					default:
						break;
				}
			}
			RstKeyEvents();
		}
	}
	switch(rUnit){
		case 0x20:
			sieveRVal=rNumPart;
			break;
		case 'K':
			sieveRVal=rNumPart*1e3;
			break;
		case 'M':
			sieveRVal=rNumPart*1e6;
			break;
	}
	errTolr=(float) errTolrE2/100.0;
	LCDCls();
	LCDPrintNumFixdgt(0,0,rNumPart,3);
	LCDPrintChar(3,0,rUnit);
	LCDPrintNumFixdgt(0,1,errTolrE2,2);
	LCDPrintFloat(8,0,sieveRVal);
	LCDPrintFloat(8,1,errTolr);
	delaynms(3000);
}





