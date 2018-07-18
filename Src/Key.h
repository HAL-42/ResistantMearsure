#ifndef _KEY_H
#define _KEY_H

#define NONE_PRESS	0x00			//按键没有按下
#define SHORT_PRESS 0x01			//按键短按
#define LONG_PRESS	0x02			//按键长按

/**
 * 逐个扫描按键，并把按键扫描结果（无动作，长按，短按）放到keyxEvents全局变量中等待处理
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void KeyScan();
/**
 * 完成按键信号处理后，应该调用该函数及时清理掉上一次按键扫描取得的事件
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 注意，在本程序中，只有按键信号被处理后，才会进行下一次KeyScan引入新的事件。
 * 而已处理按键事件要及时reset掉，才能让每次KeyScan互不相扰。
 */
void RstKeyEvents();
/**
 * 初始化按键状态，用于软重置
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury 由于重置按钮被占用，因此采用软重置函数，软重置时要重设所有和keyx有关变量
 */
void KeyInitial();

#endif