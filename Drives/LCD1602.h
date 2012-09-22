#ifdef  LCD1602_H
#else
#define LCD1602_H
#include "Configure.h"

#define LCD1602Driver_RS (0X1<<29)
#define LCD1602Driver_RW (0X1<<30)
#define LCD1602Driver_EN (0X1<<10)
#define LCD1602Driver_BUSY (0X1<<7)
#define LCD1602Driver_FIRSTLINE 0X80
#define LCD1602Driver_SECONDLINE 0XC0
#define LCD1602Driver_PINSEL0 0xffcf0000 //p0.0--p0.7 p0.10
#define LCD1602Driver_PINSEL1 0xc3ffffff//p0.29 p0.32
#define LCD1602Driver_PINALL 0x600004ff //LCD1602Driver所有引脚
#define LCD1602Driver_PINCTR 0x60000400
#define LCD1602Driver_SEL0 PINSEL0
#define LCD1602Driver_SEL1 PINSEL1	//端口功能
#define LCD1602Driver_CLR IO0CLR		//数据清零
#define LCD1602Driver_SET IO0SET		//数据置一
#define LCD1602Driver_DIR IO0DIR		//端口方向


#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/****************************************************************************
* 名称：LCD1602Driver_check_busy()
* 功能：检查总线是否忙
注：在仿真环境中查询无效，即该函数可省略
****************************************************************************/
void LCD1602Driver_CheckBusy(void);

/****************************************************************************
* 名称：LCD1602Driver_write_command()
* 功能：写命令
 RS = L; RW = L;D0-D7 = 指令码 E = 高脉冲
****************************************************************************/
void LCD1602Driver_WriteCMD(INT8U dat);

/****************************************************************************
* 名称  LCD1602Driver_write_data()
* 功能：写数据
  RS = H; RW = L;D0-D7 = 数据 E = 高脉冲
****************************************************************************/
void LCD1602Driver_WriteData(INT8U dat);


/****************************************************************************
* 名称：LCD1602Driver_init()
* 功能：LCD初始化函数
****************************************************************************/
void LCD1602Driver_Init(void);


/********************************************************************************
 * 名称： set_xy()
 * 功能：设定字符位置，
 注：x:0~1行，y：0~15列
 *********************************************************************************/
void  LCD1602Driver_Set_xy(INT8U x,INT8U y);

/********************************************************************************
 * 名称： LCD1602Driver_display_onechar
 * 功能：在指定位置显示一个字符
 *********************************************************************************/
void  LCD1602Driver_DisplayChar(INT8U x,INT8U y,INT8U wdate);


/********************************************************************************
 * 名称：LCD1602Driver_display_string
 * 功能：在指定位置显示一个字符串
 *********************************************************************************/
void  LCD1602Driver_DisplayString(INT8U x,INT8U y,INT8U *s);

#endif
