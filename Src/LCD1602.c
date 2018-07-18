#include "RM.h"
#include "LCD1602.h"

#define DataPort P2

sbit RS=P3^2;
sbit RW=P3^3;
sbit EN=P3^4;
sbit isBusy=P2^7;			//返回LCD1602是否正忙

#define rs RS
#define	rw RW
#define en EN

#define ICON_HEIGHT													//自定义字符高度
uchar funnelIcon[]={0x1F,0x11,0x1B,0x0A,0x0A,0x0A,0x04,0x04};		//自定义字符：漏斗

/**
 * 测试LCD1602是否正处于忙碌状态
 * @Author   Xiaobo        Yang
 * @DateTime 2018-07-15
 * @Summury	  Omitted
 * @return   返回1以表示正在忙碌.
 */
bit BusyTest(){
	bit result;
	rs=0;
	rw=1;
	en=1;				 //LCD1602读命令时en应该是高电平
	_nop_();             //延迟4个机器周期，给LCD1602反应时间
	_nop_();			 //！！可能需要更长的反应时间，但该函数调用频繁，效率要尽可能的高才好
	_nop_();
	_nop_();
	result=isBusy;
	en=0;				 //en恢复到0
	return result;
}
/**
 * 向LCD1602写入指令
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  Omitted
 * @param    CMD        8bit指令
 */
void LCDWriteCommand(uchar CMD){
	while(BusyTest());
	rs=0;
	rw=0;
	en=0;
	DataPort=CMD;
	_nop_();			
	_nop_();
	_nop_();
	_nop_();
	en=1;				//让en产生从0到1的跳变，写入指令
	_nop_();			
	_nop_();
	_nop_();
	_nop_();			
	en=0;				//en下降沿，LCD1602执行指令
}
/**
 * 移动LCD1602显示位置
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  
 * @param    x          x坐标，在0~15之间
 * @param    y          y坐标，只有0,1两种选择
 */
void LCDMoveCursor(uchar x,uchar y){
	uchar co;
	co=x;
	if(y){
		co|=0xc0;
	}
	else{
		co|=0x80;
	}
	LCDWriteCommand(co);
}
/**
 * 向LCD1602写入数据，让LCD1602在光标位置打出输入的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    data       要显示的字符的ASCII码
 */
void LCDWriteData(uchar data){
	while(BusyTest());
	rs=1;
	rw=0;
	en=0;
	DataPort=data;
	_nop_();			
	_nop_();
	_nop_();
	_nop_();
	en=1;				//让en产生从0到1的跳变，写入数据
	_nop_();			
	_nop_();
	_nop_();
	_nop_();			
	en=0;				//en下降沿，LCD1602开始输出
}
/**
 * Initialte LCD1602
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void LcdInitiate(void)
{
	delaynms(15); 				//延时15ms，首次写指令时应给LCD 一段较长的反应时间
	LCDWirteCommand(0x38); 		//显示模式设置： 16× 2 显示， 5× 7 点阵， 8 位数据接口
	delaynms(5); 				//延时5ms ，给硬件一点反应时间
	LCDWirteCommand(0x38);
	delaynms(5); 				//延时5ms ，给硬件一点反应时间
	LCDWirteCommand(0x38); 		//连续三次，确保初始化成功
	delaynms(5); 				//延时5ms ，给硬件一点反应时间
	LCDWirteCommand(0x0c); 		//显示模式设置：显示开，无光标，光标不闪烁
	delaynms(5); 				//延时5ms ，给硬件一点反应时间
	LCDWirteCommand(0x06); 		//显示模式设置：光标右移，字符不移
	delaynms(5);				//延时5ms ，给硬件一点反应时间
	LCDWirteCommand(0x40);		//写入自定义字符
	uchar i;					
	for(i=0;i<8;i++){
		LCDWriteData(funnelIcon[i]);
		}
	LCDWirteCommand(0x01); 		//清屏幕指令，将以前的显示内容清除
	delaynms(5); 				//延时5ms ，给硬件一点反应时间
	LCDMoveCursor(0,0);
}
/**
 * 清屏函数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void LCDCls(){
	LCDWriteData(0x01);
}
/**
 * 在指定位置显示一个字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    c          要显示字符的ASCII码
 */
void LCDPrintChar(uchar x,uchar y,uchar c){
	LCDMoveCursor(x,y);
	LCDWriteData(c);
}
/**
 * 在指定位置显示一个字符串
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符串起始x坐标，在0~15之间
 * @param    y          字符串起始y坐标，在0~1之间
 * @param    str        要显示字符串
 */
void LCDPrintStr(uchar x,uchar y,uchar str[]){
	LCDMoveCursor(x,y);
	while(*str){
		LCDWriteData(*str);
		str++;
	}
}
/**
 * 打印一行，从初始坐标x，y开始，先清除要打印行原有所有字符，然后显示输入的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符显示起始横坐标
 * @param    y          字符显示起始纵坐标
 * @param    str        要显示的字符串
 */
void LCDPrintLine(uchar x,uchar y,uchar str[]){
	LCDMoveCursor(0,y);
	uchar i;
	for(i=0;i<0x10;i++) LCDWriteData(0x20);
	LCDMoveCursor(x,y);
	while(*str){
		LCDWriteData(*str);
		str++;
	}
}
/**
 * 从坐标x，y开始清除长度为len的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          清除开始横坐标x
 * @param    y          清除开始纵坐标y
 * @param    len        要清除的长度
 */
void LCDErase(ucahr x,uchar y,uchar len){
	LCDMoveCursor(x,y);
	uchar i;
	for(i=0;i<len;i++) LCDWriteData(0x20);
}
/**
 * 打开显示屏并显示闪烁的光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_show_LCD1602(){
	LCDWirteCommand(0x0f);
}
/**
 * 隐藏光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_hide_LCD1602(){
	LCDWirteCommand(0x0c);
}