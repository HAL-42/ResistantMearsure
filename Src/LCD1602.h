#ifndef _LCD1602_H
#define _LCD1602_H

#define LCD_XMAX 0x0f
#define LCD_YMAX 0x02

/**
 * 测试LCD1602是否正处于忙碌状态
 * @Author   Xiaobo        Yang
 * @DateTime 2018-07-15
 * @Summury	  Omitted
 * @return   返回1以表示正在忙碌.
 */
bit BusyTest();
/**
 * 向LCD1602写入指令
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  Omitted
 * @param    CMD        8bit指令
 */
void LCDWriteCommand(uchar CMD);
/**
 * 移动LCD1602显示位置
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  
 * @param    x          x坐标，在0~15之间
 * @param    y          y坐标，只有0,1两种选择
 */
void LCDMoveCursor(uchar x,uchar y);
/**
 * 向LCD1602写入数据，让LCD1602在光标位置打出输入的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    data       要显示的字符的ASCII码
 */
void LCDWriteData(uchar x);
/**
 * Initialte LCD1602
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-16
 * @Summury
 */
void LcdInitiate(void);
/**
 * 清屏函数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void LCDCls();
/**
 * 在指定位置显示一个字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    c          要显示字符的ASCII码
 */
void LCDPrintChar(uchar x,uchar y,uchar c);
/**
 * 在指定位置显示一个整数
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    num        要显示整数
 */
void LCDPrintNum(uchar x,uchar y,int num);
/**
 * 在指定位置显示一个浮点数，三位整数，三位小数，数量级以10E3递增，大于999M定义为无穷
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury
 * @param    x          字符的x坐标，在0~15之间
 * @param    y          字符的y坐标，在0~1之间
 * @param    num        要显示整数
 */
void LCDPrintFloat(uchar x,uchar y,float num);
/**
 * 在指定位置显示一个字符串
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符串起始x坐标，在0~15之间
 * @param    y          字符串起始y坐标，在0~1之间
 * @param    str        要显示字符串
 */
void LCDPrintStr(uchar x,uchar y,uchar str[]);
/**
 * 打印一行，从初始坐标x，y开始，先清除要打印行原有所有字符，然后显示输入的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          字符显示起始横坐标
 * @param    y          字符显示起始纵坐标
 * @param    str        要显示的字符串
 */
void LCDPrintLine(uchar x,uchar y,uchar str[]);
/**
 * 打印两行字在屏幕上
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-19
 * @Summury
 * @param    str1       第一行字
 * @param    str2       第二行字
 */
void LCDPrintScreen(uchar str1[],uchar str2[]);
/**
 * 从坐标x，y开始清除长度为len的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          清除开始横坐标x
 * @param    y          清除开始纵坐标y
 * @param    len        要清除的长度
 */
void LCDErase(uchar x,uchar y,uchar len);
/**
 * 打开显示屏并显示闪烁的光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_show_LCD1602();
/**
 * 隐藏光标。
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 */
void cursor_hide_LCD1602();

#endif