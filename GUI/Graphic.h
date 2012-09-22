#include "Configure.h"

#ifndef __GRAPHIC__
#define __GRAPHIC__



/*
 * 宏定义
*/
#define TWINK_PIC_X     ((INT8U)4) //闪烁条目的x位置
#define TWINK_PIC_Y     ((INT8U)48) //闪烁条目的x位置
#define TWINK_DELAY_TIME      ((INT32U)150)//闪烁延时


/*
* 名称：GUI_Init()
* 功能：两个液晶屏的初始化
*/
void GUI_Init(void);

/*
 * 名称：GUI_TwinklePic()
 * 功能：图形闪烁
*/
void GUI_TwinklePic(void);

/*
 * 名称: GUI_KeyProcess()
 * 功能：响应按键，创建闪烁任务，状态切换时杀死任务
 */
void GUI_KeyPress(INT8U ucKeyValue);

/*
 * 名称: GUI_DrawSnake()
 * 功能: 暂停恢复时，恢复蛇身
 */ 
//void GUI_DrawSnake(Snake_t *pSnake_tSnake);

/*
* 名称: GUI_DisplayTime()
* 功能: 显示游戏使用的时间 
* 参数：INT32U ucSeconds 
*/
void GUI_DisplayTime(INT32U ucSeconds);

/*
* 名称: GUI_DisplayScore()
* 功能: 显示玩家得分 
* 参数：INT32U ucScore
*/
void GUI_DisplayScore(INT32U ucScore);

/*
* 名称: GUI_DisplayLevel()
* 功能: 显示玩家等级 
* 参数：INT8U ucLevel 	
*/
void GUI_DisplayLevel(INT8U ucLevel);

/*
* 名称: GUI_DrawNode()
* 功能: 画蛇的每个节点 
* 参数：节点的横坐标：INT8U x,INT8U y	
*/
void GUI_DrawNode(INT8U ucX,INT8U ucY,BOOLEAN DrawORClear);

/*
 * 名称：GUI_DrawSnake
 * 功能：绘制蛇
 */
void GUI_DrawSnake(Snake_t *pSnake_tSnake);

/*
* 名称: GUI_DisplayMiniMap()
* 功能: 选择图时，显示地图的缩略图 
* 参数：地图标号	
*/
void GUI_DisplayMiniMap(INT8U ucMiniMapNumber);

/* 
* 名称：GUI_DisplayMap()
* 功能：显示所选的关卡地图
* 参数：地图的标号
*/
void GUI_DisplayMap(INT8U ucMapNumber);

/*
* 名称: GUI_DisplayPicture()
* 功能: 在指定坐标打印指定的图片
* 参数：横坐标: INT8U uxX,纵坐标：INT8U ucY ,图片：pucPicture	
*/
void GUI_DisplayPicture(INT8U ucX,INT8U ucY,const INT8U *pucPicture,const Size_t *Size_tPictureSize);

/*
*  名称：GUI_DisplayHeroName()； 
*  功能：显示输入的英雄名
*  参数：
*/
void GUI_DisplayHeroName(INT8U Heros[],INT8U CurPosition) ;

/*
* 名称: GUI_DisplayHero()
* 功能: 显示英雄榜 
* 参数：英雄榜单数组 	
*/
void GUI_DisplayHero(Hero_Score_t aHero_tHeroes[]);

/*
* 名称: GUI_DisplayHero()
* 功能: 显示暂停状态
* 参数： 	
*/
void GUI_DisplayPause(void);

#endif

