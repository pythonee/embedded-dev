#include "Configure.h"

#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#define WIN_SCORE  ((MAX_BEANS)*(BEAN_SCORE))  //游戏通关需要的分数
#define TOP_NUMBER ((INT8U)3) //英雄榜的数目

//被按键模块调用，传入按键值
INT8U Environment_KeyPress (INT8U ucKeyValue );	  

//被游戏控制模块调用，得到游戏运行结束时的分数
INT8U Environment_PassScore (INT8U ucScore );	 

//初始化一些内部变量和GUI模块（主要包括LCD1和LCD2）
void Environment_Initial(void);

#endif

