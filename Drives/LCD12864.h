#ifndef __LCD12864_H
#define __LCD12864_H

#include "Configure.h"

#define LCD12864Driver_CTL_NO 11						//控制线起始端口
#define LCD12864Driver_DAT_NO 16						//数据线起始端口
 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define LCD12864Driver_CSA (1<<(LCD12864Driver_CTL_NO))		//控制线端口定义
#define LCD12864Driver_CSB (1<<(LCD12864Driver_CTL_NO+1))
#define LCD12864Driver_RS	 (1<<(LCD12864Driver_CTL_NO+2))   //DI
#define LCD12864Driver_RW	 (1<<(LCD12864Driver_CTL_NO+3))
#define LCD12864Driver_EN	 (1<<(LCD12864Driver_CTL_NO+4))

//#define LCD12864Driver_BUSY (1<<LCD12864Driver_DAT_NO+7)	//忙标志位。

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define LCD12864Driver_CLR IO0CLR		//数据清零
#define LCD12864Driver_SET IO0SET		//数据置一
#define LCD12864Driver_DIR IO0DIR		//端口方向
#define LCD12864Driver_PIN IO0PIN		//端口状态

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define LCD12864Driver_SEL0 PINSEL0
#define LCD12864Driver_SEL1 PINSEL1	//端口功能

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define LCD_STARTROW 0xC0		//设置起始行指令。
#define LCD_PAGE 	 0xB8		//设置页指令。10111000
#define LCD_COL 	 0x40		//设置列指令。
#define SINGLE_SCREEN_COL  64	 //左右屏，每屏64列
#define LCD_DispOn  0x3F

//****************************************************************************
//* 名称：LCD12864Driver_WriteCMD
//* 功能：写命令
//****************************************************************************
void LCD12864Driver_WriteCMD(INT8U ucCMD);

//****************************************************************************
//* 名称：LCD12864Driver_WriteData
//* 功能：写数据
//****************************************************************************
void LCD12864Driver_WriteData(INT8U ucData);

//****************************************************************************
//* 名称：LCD12864Driver_CleanAll
//* 功能：清屏

//****************************************************************************
void LCD12864Driver_CleanALL(void);

//****************************************************************************
//* 名称：LCD12864Driver_Init
//* 功能：初始化
//****************************************************************************
void LCD12864Driver_Init(void);

//****************************************************************************
//* 名称：LCD12864Driver_DisplayByte
//* 功能: 显示一个字节,即纵向8个像素
//****************************************************************************
void LCD12864Driver_DisplayByte(INT8U ucPage,INT8U ucCol,INT8U ucData);

//****************************************************************************
//* 名称：LCD12864Driver_DispOneChinese
//* 功能: 显示一个汉字
//****************************************************************************
void LCD12864Driver_DispOneChinese(INT8U ucPage,INT8U ucCol,INT8U *pDisTable);

//****************************************************************************
//* 名称：LCD12864Driver_DisplayByte
//* 功能: 根据字模显示显示n个汉字
//* 注意：  按照这种写法要每个汉字单独取模
//****************************************************************************
void LCD12864Driver_DispChinese(INT8U ucPage,INT8U ucCol,INT8U pDisTable[][32],INT8U Num);

//****************************************************************************
//* 名称：LCD12864Driver_DisPicture
//* 功能: 根据字模显示图片
//* 注意：最大128*64点阵 ，要提供图形的高度和宽度
//****************************************************************************
void LCD12864Driver_DisPicture(INT8U ucPage,INT8U ucCol,INT8U *pDisTable,INT8U width,INT8U hight);

#endif
