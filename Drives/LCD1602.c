/******************************************************************************************/
#include "Configure.h"
/****************************************************************************
* File: LCD1602Driver.c
* 功能：LCD1602Driver驱动
****************************************************************************/

/****************************************************************************
* 名称：LCD1602Driver_CheckBusy()
* 功能：检查总线是否忙
注：在仿真环境中查询无效，即该函数可省略
****************************************************************************/

void LCD1602Driver_CheckBusy(void)
{
	LCD1602Driver_DIR &= 0xfffff800;           //掩码
	LCD1602Driver_DIR |= LCD1602Driver_PINCTR;//0111 0000 0000 p0.8-p0.10输出引脚
	while(1)
	{	
		//读状态 RS = L,RW = H, EN = H
		LCD1602Driver_CLR = LCD1602Driver_RS;	 
		IO0SET = LCD1602Driver_RW;
		IO0SET = LCD1602Driver_EN;
	
		if (!(IO0PIN & LCD1602Driver_BUSY) )
		{
			break;
		}
		
	
		LCD1602Driver_CLR=LCD1602Driver_EN;
	}
	

	LCD1602Driver_DIR |= LCD1602Driver_PINALL;  //LCD所对应的IO口线设置为输出功能
}


/****************************************************************************
* 名称：LCD1602Driver_WriteCMD()
* 功能：写命令
 RS = L; RW = L;D0-D7 = 指令码 E = 高脉冲
****************************************************************************/
void LCD1602Driver_WriteCMD(unsigned char dat)
{

	LCD1602Driver_CheckBusy();

	LCD1602Driver_CLR=LCD1602Driver_RS;		//全部清零
	LCD1602Driver_CLR=LCD1602Driver_RW;
	LCD1602Driver_CLR=0xff;	//先清零指令位 必不可少
	IO0SET=dat;		//再送指令
	IO0SET=LCD1602Driver_EN;		//ns级别有效指令码被识别，ARM时间是1/M ，us级别的
//	DelayMS(1);
	LCD1602Driver_CLR=LCD1602Driver_EN;	  //恢复常态 ，为了下次有高脉冲
}
/****************************************************************************
* 名称  LCD1602Driver_WriteData()
* 功能：写数据
  RS = H; RW = L;D0-D7 = 数据 E = 高脉冲
****************************************************************************/
void LCD1602Driver_WriteData(unsigned char dat)	
{

	LCD1602Driver_CheckBusy();

	IO0SET=LCD1602Driver_RS;
	LCD1602Driver_CLR=LCD1602Driver_RW;

	LCD1602Driver_CLR=0xff;	//先清零
	IO0SET=dat;	   //再送数

	IO0SET=LCD1602Driver_EN;
	//	DelayMS(1);
	LCD1602Driver_CLR=LCD1602Driver_EN;
}
/****************************************************************************
* 名称：LCD1602Driver_Init()
* 功能：lcd初始化函数
****************************************************************************/
void LCD1602Driver_Init(void)
{

	 LCD1602Driver_SEL0 &= LCD1602Driver_PINSEL0;
	 LCD1602Driver_SEL1 &= LCD1602Driver_PINSEL1;
	 LCD1602Driver_DIR  |= LCD1602Driver_PINALL;		//设置为输出
	 LCD1602Driver_CLR   = LCD1602Driver_PINALL; 
	 LCD1602Driver_WriteCMD(0x01); //显示清屏
	 LCD1602Driver_WriteCMD(0x38); //显示光标移动设置8位2行5x7点阵
	 LCD1602Driver_WriteCMD(0x0C); //显示开及光标设置显示器开、光标开、闪烁开
	 
}

/********************************************************************************
* 名称： LCD1602Driver_Set_xy()
* 功能：设定字符位置，
注：x:0~1行，y：0~15列
*********************************************************************************/
void  LCD1602Driver_Set_xy(unsigned  char x,unsigned  char y)
{ 
  switch (x)
   {
   	 case 0: y+=LCD1602Driver_FIRSTLINE;break;
	 case 1: y+=LCD1602Driver_SECONDLINE;break;
	// case 2: y+=0x90; break;   //兼容其他字符型LCD
	// case 3: y+=0xd0; break;
   }  
  LCD1602Driver_WriteCMD(y);	//显示坐标更新 设置数据指针
}
/********************************************************************************
* 名称： display_onechar（）
* 功能：在指定位置显示一个字符
*********************************************************************************/
void  LCD1602Driver_DisplayChar(unsigned  char x,unsigned  char y,unsigned  char wdate)
{
 	LCD1602Driver_Set_xy(x,y);
 	LCD1602Driver_WriteData(wdate);
}
/********************************************************************************
* 名称：display_string（）
* 功能：在指定位置显示一个字符串
*********************************************************************************/
void  LCD1602Driver_DisplayString(unsigned  char x,unsigned  char y,unsigned  char *s)
{
  LCD1602Driver_Set_xy(x,y);
  while(*s)
  {
   LCD1602Driver_WriteData(*s);
   s++;
  }
}



