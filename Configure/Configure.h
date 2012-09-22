/****************************************Copyright (c)**************************************************

********************************************************************************************************/
#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#ifndef TRUE
#define TRUE  (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif


#define TASK_STK_SIZE ((INT8U)64)

/********************************/
/*      uC/OS-II的特殊代码      */
/********************************/
#include "Includes.h"

/********************************/
/*      ARM的特殊代码           */
/********************************/
#include "LPC2124.h"

/********************************/
/*      	LCD1602驱动         */
/********************************/
#include "LCD1602.h"

/********************************/
/*      	LCD12864驱动        */
/********************************/
#include "LCD12864.h"

/********************************/
/*     应用程序配置             */
/********************************/

#include    <stdio.h>
#include 	<stdlib.h>
#include    <stdarg.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h> 

/********************************/
/*     本例子的配置             */
/********************************/
/* 系统设置 */
#define Fosc            10000000                    //晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍


#include    "target.h"

// LPC21000 misc uart0 definitions
#define UART0_PCB_PINSEL_CFG     ((INT32U)0x00000005)
#define UART0_INT_BIT            ((INT32U)0x0040)
#define LCR_DISABLE_LATCH_ACCESS ((INT32U)0x00000000)
#define LCR_ENABLE_LATCH_ACCESS  ((INT32U)0x00000080)
#define LCR_DISABLE_BREAK_TRANS  ((INT32U)0x00000000)
#define LCR_ODD_PARITY           ((INT32U)0x00000000)
#define LCR_ENABLE_PARITY        ((INT32U)0x00000008)
#define LCR_1_STOP_BIT           ((INT32U)0x00000000)
#define LCR_CHAR_LENGTH_8        ((INT32U)0x00000003) 
#define LSR_THR_EMPTY            ((INT32U)0x00000020)
#define KEY_SEL PINSEL1  
#define KEY_DIR IO0DIR   
#define KEY_SET IO0SET   
#define KEY_CLR IO0CLR   
#define KEY_PIN IO0PIN   
//#define KEY_BEG 24

// 打开/关闭debug输出
#define DEBUG FALSE

//方向键
#define LEFT       ((INT8U)0x01)		    // 定义上下左右四个方向
#define UP         ((INT8U)0x02)
#define RIGHT      ((INT8U)0x03)
#define DOWN       ((INT8U)0x04)
#define START      ((INT8U)0x05)	    	// 定义开关键键值

// 全局宏定义

#define GAME_STAT_WELCOME	     ((INT8U)0x2)   	//游戏状态“欢迎状态”	
#define GAME_STAT_CHANGEMAP	   ((INT8U)0x3)   	//游戏状态“选地图状态”	
#define GAME_STAT_DEAD	       ((INT8U)0x4)   	//游戏状态“死亡状态”	
#define GAME_STAT_DONE	       ((INT8U)0x5)   	//游戏状态“通关状态”	
#define GAME_STAT_INPUTNAME	   ((INT8U)0x6)   	//游戏状态“输入名字状态”	
#define GAME_STAT_DISPLAYHERO	 ((INT8U)0x7)   	//游戏状态“显示英雄榜状态”	
#define GAME_STAT_GAMING	     ((INT8U)0x8)   	//游戏状态“游戏中”
#define GAME_STAT_PAUSE	       ((INT8U)0x9)     //游戏状态暂停

#define MAX_BEANS		           ((INT8U)30)      //游戏中所有的豆子数目
#define BEAN_SCORE	      	   ((INT8U)0x1)     //一个豆子对应的分数
#define MAX_MAPS               ((INT8U)4)       //图的数目
#define SNAKE_MAX_LENGTH	     ((INT8U)60)      //蛇的最大长度

#define LEVEL_BEANS	           ((INT8U)3)       //升级需要的豆子数目
#define DECREASE_DELAY_TIME	   ((INT32U)15)     //每次升级减少的延迟时间
#define INITIAL_DELAY_TIME	   ((INT32U)200)    //默认级别的延迟时间
#define INITIAL_LEVEL	         ((INT8U)0)       //初始级别
#define TIME_DELAY             ((INT32U)20)     //游戏时钟更新延时

#define OPEN_TIME_PRIO         ((INT8U)0x1)     //打开时钟中断程序任务优先级
#define KEY_PRIO               ((INT8U)0x2)     //击键优先级
#define SNAKE_MOVE_PRIO	       ((INT8U)0x3)     //蛇移动任务优先级
#define DEL_SNAKE_MOVE_PRIO    ((INT8U)0x4)     //删除蛇移动任务的临时task优先级
#define GAME_TIME_PRIO         ((INT8U)0x5)     //游戏时间控制任务
#define TWINK_PIC_PRIO         ((INT8U)0x6)     //闪烁任务
#define DEL_TWINK_PRIO         ((INT8U)0x7)     //删除闪烁任务

typedef struct tagNode_t{
  INT8U ucX,
        ucY;
}Node_t;

typedef struct tagSnake_t{
  INT8U ucDir;//蛇的运动方向
  INT8U ucHead;//指向头
  INT8U ucTail;//指向尾
  Node_t aSnakeBody[SNAKE_MAX_LENGTH];//记录蛇的所有节点的状态
}Snake_t;

//表示一幅图的尺寸，包含了图的长和高
typedef struct tagSize_t
{
     INT8U ucLength;
     INT8U ucHight;
}Size_t;

//重定义一个英雄的结构体
typedef struct tagHero_Score_t
{
	INT8U cName[3];
	INT8U ucScore;
}Hero_Score_t; 

/*******
 * 加入常量表
 */
#include "Data.h"
#include "Letter.h"
#include "Map.h"
#include "MiniMap.h"
#include "number.h"
#include "StaticPic.h"
#include "TwinklePic.h"
#include "bitmap.h"

/********************************/
/*        游戏信息         	*/
/********************************/
#include "environment.h"

/********************************/
/*          按键处理         	*/
/********************************/
#include "key_press.h"

/********************************/
/*          游戏处理         	*/
/********************************/
#include "game_control.h"

/********************************/
/*          打印方法            */
/********************************/
#include "print.h"

/********************************/
/*      	图形函数         */
/********************************/
#include "Graphic.h"

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

