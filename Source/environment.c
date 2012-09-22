#include "Configure.h"

//用这个变量来记录当前显示的是第几幅地图	
INT8U ucMiniMapCur;	 	

//记录当前光标位置（大于0小于4）
INT8U ucCursorCur;		

//记录英雄榜中玩家
Hero_Score_t apTopThree[TOP_NUMBER]={
  {{0, 0, 0},0},
  {{0, 0, 0},0},
  {{0, 0, 0},0}
};

//保存用户的名字的3个字符
INT8U aucNameCur[3]={0,0,0};	

//字母编号
INT8U ucLetterID[3]={0,0,0};

//临时分数
INT8U ucScore;

//声明内部函数

//初始化一些内部变量和GUI模块（主要包括LCD1和LCD2）
void Environment_Initial(void);
//根据按键值和当前地图的编号来改变地图
void Environment_ChangeMap(INT8U ucKeyValue);
//根据游戏运行结束后的分数来确定是否玩家通关或者游戏失败
BOOLEAN Environment_IsWin(INT8U ucScore);
//根据游戏运行结束后的分数来确定玩家是否能进入前3名
BOOLEAN Environment_IsTopThree(INT8U ucScore);
//根据按键值和当前输入名字的光标的位置来确定输入的名字
void Environment_InputName(INT8U ucKeyValue);
//更新英雄榜
void Environment_UpdateHeroboard(void);


/*************************************************************************************************
* 函数名 :    Environment_KeyPress                                                               *                    
* 功能描述 :  被按键模块调用，传入按键值                                                         *                    	  
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U  ucKeyValue                                                        			 *				
* 返回值 :      0                                                                         	     *
*作者  :        欧玉龙                                                                           *
**************************************************************************************************/

INT8U Environment_KeyPress(INT8U ucKeyValue ){
#if DEBUG==TRUE
  debug("Environment get the press key.");
#endif

  switch(ucGameState)
  {
    case GAME_STAT_WELCOME:
      {
        GUI_DisplayMiniMap(0);
        ucGameState = GAME_STAT_CHANGEMAP;
#if DEBUG==TRUE
        debug("Set the Map state.");
#endif
        break;
      }
    case GAME_STAT_CHANGEMAP:
      {
        //设置地图的界面
        if(ucKeyValue == LEFT||ucKeyValue == RIGHT)
        {
          Environment_ChangeMap(ucKeyValue);
        }
#if DEBUG==TRUE
        debug("Set the Map.");
#endif
        if(ucKeyValue == START)
        {
          GameControl_InitialGame(ucMiniMapCur);
          ucGameState = GAME_STAT_GAMING;
        }
#if DEBUG==TRUE
        debug("Finish the Map.");
#endif
        break;
      }
    case GAME_STAT_DEAD:
      {
        if(Environment_IsTopThree(ucScore) ==TRUE)
        {
          ucGameState = GAME_STAT_INPUTNAME;
#if DEBUG==TRUE
          debug("Press the name.");
#endif
        }
        else
        {
          ucGameState = GAME_STAT_DISPLAYHERO;
#if DEBUG==TRUE
          debug("Display the Hero List.");
#endif
        }
        break;
      }
    case GAME_STAT_DONE:
      {
        GUI_DisplayPicture(0, 0, ucCongratulationsBmp, &ucCongratulationsBmpSize);
        ucGameState = GAME_STAT_INPUTNAME;
#if DEBUG==TRUE
        debug("Get the name.");
#endif
        break;
      }
    case GAME_STAT_INPUTNAME:
      {
        if(ucKeyValue !=START)
        {
          Environment_InputName(ucKeyValue);
        }
        else
        {
          Environment_UpdateHeroboard();
        }
        break;
      }
    case GAME_STAT_DISPLAYHERO:
      {
        Environment_Initial();
        break;
      }
    default : 
      break;
  }
  return 0;

}


/*************************************************************************************************
* 函数名 :    Environment_PassScore                                                              *                      
* 功能描述 :  被游戏控制模块调用，得到游戏运行结束时的分数                                       *                                       	 
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U  ucScoreTmp                                                        			 *				
* 返回值 :      0                                                                         	     *
*作者  :        欧玉龙                                                                           *
**************************************************************************************************/

INT8U Environment_PassScore(INT8U ucScoreTmp ){
#if DEBUG==TRUE
  debug("Environment get Score.");
#endif 
  ucScore = ucScoreTmp;
  if(Environment_IsWin(ucScore) ==TRUE)
  {
    ucGameState = GAME_STAT_DONE;
  }
  else
  {
    ucGameState = GAME_STAT_DEAD;
  }
  return 0;

}


/*************************************************************************************************
* 函数名 :    Environment_Initial                                                                *                  
* 功能描述 :  初始化一些内部变量和GUI模块（主要包括LCD1和LCD2）                                  *                                         	
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  void                                                        						 *
* 返回值 :      void                                                                         	 *
*作者  :        郭菲                                                                             *
**************************************************************************************************/
void Environment_Initial(void){
  INT8U i;
  ucMiniMapCur = 0;
  ucCursorCur = 1;
  for(i=0;i<3;i++) 
  {
    aucNameCur[i] = 0;
    ucLetterID[i] = 0;
  }
  ucScore = 0;
  ucGameState =GAME_STAT_WELCOME;
  GUI_Init();
  Key_Init();
#if DEBUG==TRUE
  debug("Initial the vars.");
#endif

}


/*************************************************************************************************
* 函数名 :    Environment_ChangeMap                                                              *                  
* 功能描述 :  根据按键值和当前地图的编号来改变地图                                               *                                         	 
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U ucKeyValue                                                       		     *
* 返回值 :      void                                                                         	 *
*作者  :        郭菲                                                                             *
**************************************************************************************************/
void Environment_ChangeMap(INT8U ucKeyValue){

  if(ucKeyValue == LEFT){
    if(ucMiniMapCur <= (INT8U)0)
      ucMiniMapCur = MAX_MAPS-(INT8U)1;
    else
      ucMiniMapCur--;
  }
  if(ucKeyValue == RIGHT){
    if(ucMiniMapCur >= (MAX_MAPS-(INT8U)1))
      ucMiniMapCur = 0;
    else
      ucMiniMapCur++;
  }

  GUI_DisplayMiniMap(ucMiniMapCur);

}


/*************************************************************************************************
* 函数名 :    Environment_IsWin                                                                  *                  
* 功能描述 :  根据游戏运行结束后的分数来确定是否玩家通关或者游戏失败                             *                                         	 
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U ucScore                                                       		         *
* 返回值 :      TRUE/FALSE                                                                       *
*作者  :        郭菲                                                                             *
**************************************************************************************************/
BOOLEAN Environment_IsWin(INT8U ucScore){

  if(ucScore==WIN_SCORE){
    GUI_DisplayPicture(0, 0, ucYouWinBmp, &ucYouWinBmpSize);
    GUI_TwinklePic();
    return TRUE;
  }
  else{
    GUI_DisplayPicture(0, 0, ucGameOverBmp, &ucGameOverBmpSize);
    GUI_TwinklePic();
    return FALSE;
  }

}


/*************************************************************************************************
* 函数名 :    Environment_IsTopThree                                                             *                  
* 功能描述 :  根据游戏运行结束后的分数来确定玩家是否能进入前3名                                  *                                         	 
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U ucScore                                                       		         *
* 返回值 :      TRUE/FALSE                                                                       *
*作者  :        郭菲                                                                             *
**************************************************************************************************/
BOOLEAN Environment_IsTopThree(INT8U ucScore){
  if(ucScore>=apTopThree[TOP_NUMBER-(INT8U)1].ucScore){
    GUI_DisplayPicture(0, 0, ucCongratulationsBmp, &ucCongratulationsBmpSize);
    return TRUE;
  }
  else{
    GUI_DisplayHero(apTopThree);
    return FALSE;
  }
}


/*************************************************************************************************
* 函数名 :    Environment_InPutName                                                              *                   
* 功能描述 :  根据按键值和当前输入名字的光标的位置来确定输入的名字                               *                                         	 
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  INT8U ucKeyValue                                                       		     *
* 返回值 :      void                                                                             *
*作者  :        欧玉龙                                                                           *
**************************************************************************************************/
void Environment_InputName(INT8U ucKeyValue){
  if(ucKeyValue == LEFT){
    if(ucCursorCur == 1)
      ucCursorCur = 1;
    else
      ucCursorCur--;
  }
  if(ucKeyValue == RIGHT){
    if(ucCursorCur == 3)
      ucCursorCur = 3;
    else
      ucCursorCur++;
  }
  if(ucKeyValue == UP){
    if(ucLetterID[ucCursorCur-1]<= 0)
      ucLetterID[ucCursorCur-1] = 26;
    else
      ucLetterID[ucCursorCur-1]--;
  }
  if(ucKeyValue == DOWN)
  {
    if(ucLetterID[ucCursorCur-1] >= 26)
      ucLetterID[ucCursorCur-1] = 0;
    else
      ucLetterID[ucCursorCur-1]++;
  }
  aucNameCur[ucCursorCur -1] = ucLetterID[ucCursorCur-1];
  GUI_DisplayHeroName(aucNameCur,ucCursorCur);
}

//
/*************************************************************************************************
* 函数名 :    Environment_UpdateHeroboard                                                        *                  
* 功能描述 :  更新英雄榜                                                                         *
* 日期  :     2009-7-16                                                                      	 *
* 参数     :  void                                                      		                 *
* 返回值 :      void                                                                             *
*作者  :        欧玉龙                                                                           *
**************************************************************************************************/
void Environment_UpdateHeroboard(void)
{
  INT8U i ,j;
  for( i=0;i<TOP_NUMBER;i++)
  {
    if(ucScore >= apTopThree[i].ucScore)
      break;
  }
   for(j=TOP_NUMBER-1;j>i;j--)
      {
        apTopThree[j].cName[0] = apTopThree[j-1].cName[0];
        apTopThree[j].cName[1] = apTopThree[j-1].cName[1];
        apTopThree[j].cName[2] = apTopThree[j-1].cName[2];
        apTopThree[j].ucScore = apTopThree[j-1].ucScore;
      }
  apTopThree[i].cName[0] = aucNameCur[0];
  apTopThree[i].cName[1] = aucNameCur[1];
  apTopThree[i].cName[2] = aucNameCur[2];
  apTopThree[i].ucScore = ucScore;
  GUI_DisplayHero(apTopThree);
  ucGameState = GAME_STAT_DISPLAYHERO;
}

