#ifndef _MENU_H
#define _MENU_H

/**
 * 初始化菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void InitialMenu();
/**
 * 打开菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuImpl();
/**
 * 显示菜单
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void ShowMenu();
/**
 * 处理菜单模式下的按键信息
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void MenuKeyTreat();
/**
 * 什么都不做
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void MenuOpExit();
/**
 * 利用IAP_CONTR进行软件复位
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
void MenuOpRstDev();
/**
 * YN菜单执行，用于改变debug和sieve模式
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void MenuOpYN();
/**
 * 转动舵机画曲线
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void MenuOpStartPlot();
/**
 * 设置筛选条件
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-21
 * @Summury
 */
static void MenuOpSetSieve();

#endif