#include <RM.h>

#define KEY_STATE_UNPRESSED	0x00		//按键状态
#define KEY_STATE_DEBOUNCE	0x01
#define KEY_STATE_PRESSED   0x02
#define KEY_STATE_WAITLOOSE 0x03

extern sbit key1;						//外部全局变量，见main.c
extern sbit key2;
extern sbit key3;

extern uchar key1Events;				//外部全局变量，见main.c
extern uchar key2Events;
extern uchar key3Events;

uchar key1State;						//记录各个按键状态
uchar key2State;
uchar key3State;

uchar key1Timer;						//各个按键的计时器，用在消颤和长按判断上
uchar key2Timer;
uchar key3Timer;

bit IsKeyEvnets;
/**
 * 逐个扫描按键，并把按键扫描结果（没有按下，长按，短按）放到keyxPressFlag全局变量中等待处理
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void KeyScan(){
	switch(key1State){
		case KEY_STATE_UNPRESSED:
			if(!key1) 
			break;
	}
}
