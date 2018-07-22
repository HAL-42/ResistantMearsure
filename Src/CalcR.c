#include "RM.h"
#include "CalcR.h"

sbit   capSel=P3^7;								//电容选择接口
extern float idata curRValue;					//当前测得电阻阻值
extern unsigned long idata curN;				//当前测得脉冲数
extern unsigned long idata refLowRN;			//低档位下参考脉冲数
extern unsigned long idata refHighRN;			//高档位下参考脉冲数
extern bit   isAverg;


extern uchar t1IntrTimes;

/**
 * 计算脉冲数，频率和电阻
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-18
 * @Summury  本函数中魔鬼数字较多，为的是提高计算精度。所有数据都是在MATLAB中计算后带入，切勿随意修改
 */
void GetRVal(){
	curN=((long) t1IntrTimes)*(65536L)+(long) TH1*(256L)+TL1; // 计算脉冲次数
	if(curN==0){
		curRValue=1e10;
		return;
	}
	if(capSel==CAPSEL_LOWR){
		curRValue=4.375e7F*(1.0F/curN)-1942.0F;
	}
	else{
		curRValue=1.004e10*(1.0F/curN)-9.392e4F;
	}
	if( (capSel==CAPSEL_LOWR)&&
		(curN<350)){
		capSel=CAPSEL_HIGHR;
		delaynms(500);
	}
	else if( (capSel==CAPSEL_HIGHR)&&
		curN>65529L){
		capSel=CAPSEL_LOWR;
		delaynms(500);
	}
}
/**
 * 初始化计算R有关的全局变量
 * @Author   Xiaobo     Yang
 * @DateTime 2018-07-22
 * @Summury
 */
void InitialCalcR(){
	capSel=CAPSEL_LOWR;				//重置所有全局变量
	curRValue=0;
	curN=0;
	refLowRN=0;
	refHighRN=0;
	isAverg=0;
}