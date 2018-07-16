#include <RM.h>
#include <LCD1602.h>
#include <Key.h>

sbit key1=P1^5;					//注意低电平表示按键按下
sbit key2=P1^4;
sbit key3=P1^3;

uchar key1Events;			//保存Key1未处理的输入信号
uchar key2Events;			//保存Key1未处理的输入信号
uchar key3Events;			//保存Key1未处理的输入信号

