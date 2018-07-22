#include "RM.h"
#include "LCD1602.h"

#define DataPort P2

sbit RS=P3^2;
sbit RW=P3^3;
sbit EN=P3^4;
sbit isBusy=P2^7;					//返回LCD1602是否正忙

#define rs RS
#define	rw RW
#define en EN

#define ICON_HEIGHT															//自定义字符高度
static code uchar funnelIcon[]={0x1F,0x11,0x1B,0x0A,0x0A,0x0A,0x04,0x04};		//自定义字符：漏斗

static idata uchar tmp[10];														//公用临时数组
static int   intPart;															//公用整数部分临时储存
static int   decPart;															//公用小数部分临时储存

static unsigned long pow;
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
	en=1;							//LCD1602读命令时en应该是高电平
	delaynMC(6);
	result=isBusy;
	en=0;				 			//en恢复到0
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
	delaynMC(6);
	en=1;							//让en产生从0到1的跳变，写入指令
	delaynMC(6);		
	en=0;							//en下降沿，LCD1602执行指令
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
void LCDWriteData(uchar x){
	while(BusyTest());
	rs=1;
	rw=0;
	en=0;
	DataPort=x;
	delaynMC(6);
	en=1;								//让en产生从0到1的跳变，写入数据
	delaynMC(6);		
	en=0;								//en下降沿，LCD1602开始输出
}
/**
 * Initialte LCD1602
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void LcdInitiate(void)
{
	uchar i;
	delaynms(15); 						//延时15ms，首次写指令时应给LCD 一段较长的反应时间
	LCDWriteCommand(0x38); 				//显示模式设置： 16× 2 显示， 5× 7 点阵， 8 位数据接口
	delaynms(5); 						//延时5ms ，给硬件一点反应时间
	LCDWriteCommand(0x38);
	delaynms(5); 						//延时5ms ，给硬件一点反应时间
	LCDWriteCommand(0x38); 				//连续三次，确保初始化成功
	delaynms(5); 						//延时5ms ，给硬件一点反应时间
	LCDWriteCommand(0x0c); 				//显示模式设置：显示开，无光标，光标不闪烁
	delaynms(5); 						//延时5ms ，给硬件一点反应时间
	LCDWriteCommand(0x06); 				//显示模式设置：光标右移，字符不移
	delaynms(5);						//延时5ms ，给硬件一点反应时间
	LCDWriteCommand(0x40);				//写入自定义字符
					
	for(i=0;i<8;i++){
		LCDWriteData(funnelIcon[i]);
		}
	LCDWriteCommand(0x01); 				//清屏幕指令，将以前的显示内容清除
	delaynms(5); 						//延时5ms ，给硬件一点反应时间
	LCDMoveCursor(0,0);
}
/**
 * 清屏函数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void LCDCls(){
	LCDWriteCommand(0x01);
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
 * 在指定位置显示一个整数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    num        要显示整数
 */
void LCDPrintNum(uchar x,uchar y,unsigned long num){
	uchar i=0;
	LCDMoveCursor(x,y);	
	do{
		tmp[i++]=num%10;						//从个位开始压栈
		num/=10;
	}while(num);
	while(i){
		LCDWriteData(tmp[--i]+'0');				//从栈中把数字顺序pop出来
	}
}
/**
 * 在指定位置显示一个指定位数的整数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 * @param    x          [description]
 * @param    y          [description]
 * @param    num        [description]
 */
void LCDPrintNumFixdgt(uchar x,uchar y,uint num,uchar dgt){
	uchar i;
	pow=1;
	for(i=1;i<dgt;i++) pow*=10;
	LCDMoveCursor(x,y);
	while(dgt--){
		LCDWriteData( num/pow +'0');
		num%=pow;
		pow/=10;
	}
}
/**
 * 在指定位置显示一个浮点数，三位整数，三位小数，数量级以10E3递增，大于999M定义为无穷
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    num        要显示整数
 */
void LCDPrintFloat(uchar x,uchar y,float num){
	if(num<=0){
		LCDPrintNum(x,y,0);
	}
	else if(num>=1e9){								//大于1G，显示无穷
		LCDPrintStr(x,y,"Infinite");
	}
	else if(num>=1e6){							//1M到1G之间，以M为单位显示三位整数，三位小数
		num/=1e6;
		intPart=(int) num;						//截尾获取整数部分
		decPart=(int) ((num-intPart)*1000);		//num减去整数部分后*1000后所得数组的整数部分，就是小数的前三位
		LCDPrintNum(x,y,intPart);				//打印整数部分
		LCDWriteData('.');						//打印小数点
		LCDWriteData(decPart/100+'0');			//取得百位数的ASICC码
		LCDWriteData( (decPart%100)/10+'0' );	//取得十位数的ASICC码
		LCDWriteData( (decPart%10)+'0' );		//取得个位数的ASICC码
		LCDWriteData('M');						//打印单位
	}
	else if(num>1e3){							//1K到1M之间，以K为单位显示三位整数，三位小数
		num/=1e3;
		intPart=(int) num;
		decPart=(int) ((num-intPart)*1000);
		LCDPrintNum(x,y,intPart);
		LCDWriteData('.');
		LCDWriteData(decPart/100+'0');
		LCDWriteData( (decPart%100)/10+'0' );
		LCDWriteData( (decPart%10)+'0' );
		LCDWriteData('K');
	}
	else{										//小于1K，显示三位整数，四位小数
		intPart=(int) num;
		decPart=(int) ((num-intPart)*10000);
		LCDPrintNum(x,y,intPart);
		LCDWriteData('.');
		LCDWriteData(decPart/1000+'0');
		LCDWriteData( (decPart%1000)/100 +'0' );
		LCDWriteData( (decPart%100)/10+'0' );
		LCDWriteData( (decPart%10)+'0' );
	}
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
	uchar i;
	LCDMoveCursor(0,y);
	for(i=0;i<0x10;i++) LCDWriteData(0x20);			//将要打印字符串所在行用空格覆盖一遍，相当于擦除
	LCDMoveCursor(x,y);
	while(*str){
		LCDWriteData(*str);
		str++;
	}
}
/**
 * 打印两行字在屏幕上
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    str1       第一行字
 * @param    str2       第二行字
 */
void LCDPrintScreen(uchar str1[],uchar str2[]){
	LCDWriteCommand(0x01);							//清屏
	LCDMoveCursor(0,0);
	while(*str1){									//打印第一行字符串
		LCDWriteData(*str1++);
	}
	LCDMoveCursor(0,1);								//打印第二行字符串
	while(*str2){
		LCDWriteData(*str2++);
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
void LCDErase(uchar x,uchar y,uchar len){
	uchar i;
	LCDMoveCursor(x,y);
	for(i=0;i<len;i++) LCDWriteData(0x20);			//从开始坐标开始用空格擦除掉制定长度的字符
}
/**
 * 打开显示屏并显示闪烁的光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_show_LCD1602(){
	LCDWriteCommand(0x0f);
}
/**
 * 隐藏光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_hide_LCD1602(){
	LCDWriteCommand(0x0c);
}
