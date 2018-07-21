#include "RM.h"
#include "Key.h"
#include "Timer.h"

#define KEY_STATE_UNPRESSED	0x00		//按键状态：没有按下
#define KEY_STATE_DEBOUNCE	0x01		//按键状态：按下，但按下时间不足，可能还在颤动，因此现在次状态等候一段时间
#define KEY_STATE_PRESSED   0x02		//按键状态：确定按键按下并稳定，在此状态下区分短按和长按
#define KEY_STATE_WAITLOOSE 0x03		//按键状态：等待长按后松后会到没有按下状态

//////////////////////////////////////////////////////////////////////////////////////////////////
//TODO:时长还有待调试确定，如果有需要DEBOUNCE_TIME置为0，LONG_PRESS_TIME置为无穷，取消消颤和长按功能 //
/////////////////////////////////////////////////////////////////////////////////////////////////
#define DEBOUNCE_TIME_FREQ 			50				//50次KeyScan完成消颤(基于认为两次KeyScan相距50个机器周期，调整！)
#define LONG_PRESS_TIME_FREQ 		60000			//60000次KeyScan判定为长按
#define DEBOUNCE_TIME_KEYSCAN  		1				//2ms消颤
#define LONG_PRESS_TIME_KEYSCAN   	2000  			//2s判定为长按


sbit key1=P1^5;						    			//外部全局变量，见main.c
sbit key2=P1^4;
sbit key3=P1^3;

extern uchar timerFun;								//外部全局变量，见main.c

extern uchar key1Events;							//外部全局变量，见main.c
extern uchar key2Events;
extern uchar key3Events;

extern bit isKeyEvents;								//外部全局变量，见main.c

static uchar key1State;								//记录各个按键状态
static uchar key2State;
static uchar key3State;

static uint key1Timer;								//各个按键的计时器，用在消颤和长按判断上
static uint key2Timer;	
static uint key3Timer;

static uint debounceTime;
static uint longPressTime;
/**
 * 逐个扫描按键，并把按键扫描结果（无动作，长按，短按）放到keyxEvents全局变量中等待处理
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void KeyScan(){
	if(timerFun==TIMERFUN_FREQ_MEASRURE){
		debounceTime=DEBOUNCE_TIME_FREQ;
		longPressTime=LONG_PRESS_TIME_FREQ;
	}
	else{
		debounceTime=DEBOUNCE_TIME_KEYSCAN;
		longPressTime=LONG_PRESS_TIME_KEYSCAN;
	}
	switch(key1State){										//选择状态					
		case KEY_STATE_UNPRESSED:							//没有按下状态	
			if(!key1) {										//按键按下（低电平），转移到消颤状态
				key1State=KEY_STATE_DEBOUNCE;
				key1Timer=0;								//初始化计时器
			}
			break;
		case KEY_STATE_DEBOUNCE:							//消颤状态
			if(key1) {										//按键松开，说明还在颤动，回到没有按下状态
				key1State=KEY_STATE_UNPRESSED;					
			}
			else if(++key1Timer>debounceTime){				//按键按下，就计数，若按下时长足够，说明按键稳定，转入按下状态
				key1State=KEY_STATE_PRESSED;				
				key1Timer=0;								//重设计时器
			}
			break;
		case KEY_STATE_PRESSED:
			if(key1){										//如果按键松开，按下时间又不够，就给出短按事件，并转移到未按下状态
				key1State=KEY_STATE_UNPRESSED;
				key1Events=SHORT_PRESS;
				isKeyEvents=1;								//告诉主程序有按键事件发生
			}
			else if(++key1Timer>longPressTime){				//如果按键没有松开，计时，足够长后发出长按事件，转移到等待松手状态
				key1Events=LONG_PRESS;
				isKeyEvents=1;								//告诉主程序有按键事件发生
				key1State=KEY_STATE_WAITLOOSE;
			}
			break;
		case KEY_STATE_WAITLOOSE:
			if(key1) key1State=KEY_STATE_UNPRESSED;			//一旦松手，就会到未按下状态
			break;
	}

	switch(key2State){										//参见key1State部分注释
		case KEY_STATE_UNPRESSED:
			if(!key2) {
				key2State=KEY_STATE_DEBOUNCE;
				key2Timer=0;
			}
			break;
		case KEY_STATE_DEBOUNCE:
			if(key2) {
				key2State=KEY_STATE_UNPRESSED;
			}
			else if(++key2Timer>debounceTime){
				key2State=KEY_STATE_PRESSED;
				key2Timer=0;
			}
			break;
		case KEY_STATE_PRESSED:
			if(key2){
				key2State=KEY_STATE_UNPRESSED;
				key2Events=SHORT_PRESS;
				isKeyEvents=1;
			}
			else if(++key2Timer>longPressTime){
				key2Events=LONG_PRESS;
				isKeyEvents=1;
				key2State=KEY_STATE_WAITLOOSE;
			}
			break;
		case KEY_STATE_WAITLOOSE:
			if(key2) key2State=KEY_STATE_UNPRESSED;
			break;
	}

	switch(key3State){										//参见key1State部分注释
		case KEY_STATE_UNPRESSED:
			if(!key3) {
				key3State=KEY_STATE_DEBOUNCE;
				key3Timer=0;
			}
			break;
		case KEY_STATE_DEBOUNCE:
			if(key3) {
				key3State=KEY_STATE_UNPRESSED;
			}
			else if(++key3Timer>debounceTime){
				key3State=KEY_STATE_PRESSED;
				key3Timer=0;
			}
			break;
		case KEY_STATE_PRESSED:
			if(key3){
				key3State=KEY_STATE_UNPRESSED;
				key3Events=SHORT_PRESS;
				isKeyEvents=1;
			}
			else if(++key3Timer>longPressTime){
				key3Events=LONG_PRESS;
				isKeyEvents=1;
				key3State=KEY_STATE_WAITLOOSE;
			}
			break;
		case KEY_STATE_WAITLOOSE:
			if(key3) key3State=KEY_STATE_UNPRESSED;
			break;
	}
}
/**
 * 完成按键信号处理后，应该调用该函数及时清理掉上一次按键扫描取得的事件
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 注意，在本程序中，只有按键信号被处理后，才会进行下一次KeyScan引入新的事件。
 * 而已处理按键事件要及时reset掉，才能让每次KeyScan互不相扰。
 */
void RstKeyEvents(){
	key1Events=NONE_PRESS;
	key2Events=NONE_PRESS;
	key3Events=NONE_PRESS;
	isKeyEvents=NONE_PRESS;
}
/**
 * 初始化按键状态，用于软重置
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 由于重置按钮被占用，因此采用软重置函数，软重置时要重设所有和keyx有关变量
 */
void KeyInitial(){
	key1Events=NONE_PRESS;
	key2Events=NONE_PRESS;
	key3Events=NONE_PRESS;
	isKeyEvents=NONE_PRESS;
	key1State=KEY_STATE_UNPRESSED;
	key2State=KEY_STATE_UNPRESSED;
	key3State=KEY_STATE_UNPRESSED;
	key1Timer=0;
	key2Timer=0;
	key3Timer=0;
}
/**
 * 我也不知道为什么要写这个函数，感觉很好玩的样子
 */
void WaitKey(void (*key1Fun) (void),void (*key2Fun) (void),void (*key3Fun) (void)){
	while(!isKeyEvents) KeyScan();
	if(key1Events) (*key1Fun) ();
	if(key2Events) (*key2Fun) ();
	if(key3Events) (*key3Fun) ();
	RstKeyEvents();
}
/**
 * 按下任意键，否则阻塞
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 */
void PressAnyKey(){
	while(!isKeyEvents) KeyScan();
	RstKeyEvents();
}
