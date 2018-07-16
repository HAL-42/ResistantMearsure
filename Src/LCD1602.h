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
static bit BusyTest();
/**
 * 向LCD1602写入指令
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  Omitted
 * @param    CMD        8bit指令
 */
static void LCDWriteCommand(uchar CMD);
/**
 * 移动LCD1602显示位置
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury  
 * @param    x          x坐标，在0~15之间
 * @param    y          y坐标，只有0,1两种选择
 */
static void LCDMoveCursor(uchar x,uchar y);
/**
 * 向LCD1602写入数据，让LCD1602在光标位置打出输入的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    data       要显示的字符的ASCII码
 */
void LCDWriteData(uchar data);
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
 * 从坐标x，y开始清除长度为len的字符
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-15
 * @Summury
 * @param    x          清除开始横坐标x
 * @param    y          清除开始纵坐标y
 * @param    len        要清除的长度
 */
void LCDErase(ucahr x,uchar y,uchar len);
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