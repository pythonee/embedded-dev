#include "Configure.h"

//游戏运行时间
INT32U ulTime;

//游戏中蛇吃到的豆子数目
INT8U ucBeansNumber;

//游戏中的延迟
INT32U ulDelay;

//按键按下的方向
INT8U ucKeyDir;

//蛇的数据结构
Snake_t *pSnake;
Snake_t snake={
  (INT8U)0,(INT8U)0,(INT8U)0,
  {
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
  }
};

//豆子的数据结构
Node_t *pBean_t;
Node_t Bean_t={0,0};

//使用的地图
INT8U ucMapNumber;

//使用的位图
INT8U BitMap_Game[64]={0};

INT8U ucNewHead;
OS_STK MoveSTK[TASK_STK_SIZE];//蛇移动任务堆栈
OS_STK DelSTK[TASK_STK_SIZE];//删除蛇移动任务堆栈
OS_STK TimeSTK[TASK_STK_SIZE];//时间显示堆栈

//外部接口函数统一声明
/*************************************************************************************************
 * 函数名:GameControl_InitialGame                                                                 *
 * 功能描述:接管程序控制权，初始化游戏                                                            *
 * 日期:2009.07.16                                                                            		 *
 * 参数:map 游戏地图编号                                                            							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialGame(INT8U map);

/*************************************************************************************************
 * 函数名:GameControl_KeyPress                                                                    *
 * 功能描述:按键事件捕获函数                                                                      *
 * 日期:2009.07.16                                                                            		 *
 * 参数:map 游戏地图编号                                                            							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_KeyPress(INT8U ucKeyValue);

//内部函数统一声明
/*************************************************************************************************
 * 函数名:GameControl_ChangeDirection                                                             *
 * 功能描述:判断蛇改变方向是否有效                                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:ucKeyValue 改变到的方向值                                                  							 *
 * 返回值: BOOLEAN 是否能够改变方向                                                           	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
BOOLEAN GameControl_ChangeDirection(INT8U ucKeyValue);
/*************************************************************************************************
 * 函数名:GameControl_CreateBean                                                                  *
 * 功能描述:产生一个豆子                                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_CreateBean(void);
/*************************************************************************************************
 * 函数名:GameControl_Eaten                                                                       *
 * 功能描述:蛇吃到豆子后的后续操作                                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:ulDelay游戏进行时延时时间,ucBeansNumber 已经吃到的豆子总数                  							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_Eaten(INT32U* ulDelay, INT8U* ucBeansNumber);
/*************************************************************************************************
 * 函数名:GameControl_GameEnd                                                                     *
 * 功能描述:游戏结束后做后续清理工作                                                              *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_GameEnd(void);
/*************************************************************************************************
 * 函数名:GameControl_InitialInfo                                                                 *
 * 功能描述:初始化游戏信息，吃到的豆子数目，游戏进行的时间                                        *
 * 日期:2009.07.17                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialInfo(void);
/*************************************************************************************************
 * 函数名:GameControl_InitialSnake                                                                *
 * 功能描述:初始化蛇的相关数据结构，蛇的位置，豆子                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialSnake(void);
/*************************************************************************************************
 * 函数名:GameControl_Move                                                                        *
 * 功能描述:蛇移动任务                                                                            *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   无                                                                       							 *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_Move(void *arg);
/*************************************************************************************************
 * 函数名:GameControl_ResumeSnake                                                                 *
 * 功能描述:恢复蛇移动任务和时间显示任务                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_ResumeSnake(void);
/*************************************************************************************************
 * 函数名:GameControl_SuspendSnake                                                                *
 * 功能描述:挂起蛇移动任务和时间显示任务                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_SuspendSnake(void);
/*************************************************************************************************
 * 函数名:GameControl_SetBit                                                                      *
 * 功能描述:设置向量位图的数据点                                                                  *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   pNode_t 具体的一个点坐标,PaBitMap 所操作的位图, ucBit 要设置的值                       *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_SetBit(Node_t* pNode_t, INT8U* paBitMap,INT8U ucBit);
/*************************************************************************************************
 * 函数名:GameControl_GetBit                                                                      *
 * 功能描述:获取向量位图的数据点                                                                  *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   pNode_t 具体的一个点坐标,PaBitMap 所操作的位图                                         *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
INT8U GameControl_GetBit(Node_t* pNode_t, INT8U paBitMap[64]);
/*************************************************************************************************
 * 函数名:GameControl_GameEndTask                                                                 *
 * 功能描述:游戏结束后做后续清理工作的任务                                                        *
 * 日期:2009.07.17                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_GameEndTask(void*);
/*************************************************************************************************
 * 函数名:GameControl_DisplayTime                                                                 *
 * 功能描述:显示游戏时间的任务                                                                    *
 * 日期:2009.07.18                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_DisplayTime(void*arg);

//具体定义
/*************************************************************************************************
 * 函数名:GameControl_InitialGame                                                                 *
 * 功能描述:接管程序控制权，初始化游戏                                                            *
 * 日期:2009.07.16                                                                            		 *
 * 参数:map 游戏地图编号                                                            							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialGame(INT8U map){
  ucMapNumber = map;
  GameControl_InitialInfo();
  GameControl_InitialSnake();
}

/*************************************************************************************************
 * 函数名:GameControl_KeyPress                                                                    *
 * 功能描述:按键事件捕获函数                                                                      *
 * 日期:2009.07.16                                                                            		 *
 * 参数:map 游戏地图编号                                                            							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_KeyPress(INT8U ucKeyValue){
#if DEBUG==TRUE
  debug("GameControl get the key press.");
#endif
  if(ucGameState==GAME_STAT_GAMING){
    switch(ucKeyValue) {
      case START:
        GameControl_SuspendSnake();
        GUI_DisplayPause();
        ucGameState=GAME_STAT_PAUSE;
#if DEBUG==TRUE
        debug("Game to pause.");
#endif
        break;
      default:
        ucKeyDir = ucKeyValue;
    }
  }else if(ucGameState==GAME_STAT_PAUSE){
    if(ucKeyValue==START){
      GUI_DisplayMap(ucMapNumber);
      GUI_DrawSnake(pSnake);
      ucGameState=GAME_STAT_GAMING;
      GameControl_ResumeSnake();
#if DEBUG==TRUE
      debug("Game to GAMIN.");
#endif
    }
  }
}

/*************************************************************************************************
 * 函数名:GameControl_ChangeDirection                                                             *
 * 功能描述:判断蛇改变方向是否有效                                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:ucKeyValue 改变到的方向值                                                  							 *
 * 返回值: BOOLEAN 是否能够改变方向                                                           	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
BOOLEAN GameControl_ChangeDirection(INT8U ucKeyValue){
  BOOLEAN f = FALSE;
  switch(ucKeyValue){
    case DIR_UP:
      if(pSnake->ucDir!=DIR_DOWN){
        f = TRUE;
      }
      break;
    case DIR_DOWN:
      if(pSnake->ucDir!=DIR_UP){
        f = TRUE;
      }
      break;
    case DIR_LEFT:
      if(pSnake->ucDir!=DIR_RIGHT){
        f = TRUE;
      }
      break;
    case DIR_RIGHT:
      if(pSnake->ucDir!=DIR_LEFT){
        f = TRUE;
      }
      break;
  }
#if DEBUG==TRUE
  if(f==TRUE)
    debug("Change direction:TRUE.");
  else
    debug("Change direction:FALSE.");
#endif
  return f;
}

/*************************************************************************************************
 * 函数名:GameControl_CreateBean                                                                  *
 * 功能描述:产生一个豆子                                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_CreateBean(void){
#if DEBUG==TRUE
  debug("Go to Create Bean.");
#endif
  do{
    Bean_t.ucX = 1+rand()%31;
    Bean_t.ucY = 1+rand()%15;	
  }
  while(GameControl_GetBit(pBean_t, BitMap_Game)==1);   
#if DEBUG==TRUE
  debug("The bean location:x,y");
  debug_INT8U(pBean_t->ucX);
  debug_INT8U(pBean_t->ucY);
#endif
  GUI_DrawNode(pBean_t->ucX, pBean_t->ucY,DRAWNODE);
  OSTimeDlyHMSM(0,0,0,20);
  GUI_DrawNode(pBean_t->ucX, pBean_t->ucY,CLEARNODE);
  OSTimeDlyHMSM(0,0,0,20);
  GUI_DrawNode(pBean_t->ucX, pBean_t->ucY,DRAWNODE);
}

/*************************************************************************************************
 * 函数名:GameControl_Eaten                                                                       *
 * 功能描述:蛇吃到豆子后的后续操作                                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:ulDelay游戏进行时延时时间,ucBeansNumber 已经吃到的豆子总数                  							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_Eaten(INT32U *ulDelay, INT8U* ucBeansNumber){
  (*ucBeansNumber)=(*ucBeansNumber)+1;
#if DEBUG==TRUE
  debug("Now the Beans number:");
  debug_INT8U((*ucBeansNumber));
#endif

  GUI_DisplayScore((*ucBeansNumber)*BEAN_SCORE);
#if DEBUG==TRUE
  debug("Now the Score:");
  debug_INT8U((*ucBeansNumber)*BEAN_SCORE);
#endif
  if( (*ucBeansNumber)>=MAX_BEANS){

    GameControl_GameEnd();
  }else if((*ucBeansNumber)%LEVEL_BEANS==0){
    GUI_DisplayLevel(INITIAL_LEVEL+(*ucBeansNumber)/LEVEL_BEANS);
    (*ulDelay)-=DECREASE_DELAY_TIME;
  }
  GameControl_CreateBean();
}

/*************************************************************************************************
 * 函数名:GameControl_GameEnd                                                                     *
 * 功能描述:游戏结束后做后续清理工作                                                              *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_GameEnd(void){
  OSTaskCreate(GameControl_GameEndTask, NULL, &DelSTK[TASK_STK_SIZE-1],DEL_SNAKE_MOVE_PRIO);
}

/*************************************************************************************************
 * 函数名:GameControl_GameEndTask                                                                 *
 * 功能描述:游戏结束后做后续清理工作的任务                                                        *
 * 日期:2009.07.17                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_GameEndTask(void*arg){
  OSTaskDel(SNAKE_MOVE_PRIO);
  OSTaskDel(GAME_TIME_PRIO);
#if DEBUG==TRUE
  debug("Task Move delete.");
#endif

  Environment_PassScore(ucBeansNumber*BEAN_SCORE);
  OSTaskDel(OS_PRIO_SELF);
}

/*************************************************************************************************
 * 函数名:GameControl_InitialInfo                                                                 *
 * 功能描述:初始化游戏信息，吃到的豆子数目，游戏进行的时间                                        *
 * 日期:2009.07.17                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialInfo(void){
  INT8U i;
  ulTime = (INT32U)0x0;
  ucBeansNumber = (INT8U)0x0;
  ulDelay = INITIAL_DELAY_TIME;
  pSnake = &snake;
  for(i=0;i<64;i++)
    BitMap_Game[i] = BMP[ucMapNumber][i];
  GUI_DisplayTime(ulTime);
  GUI_DisplayScore(ucBeansNumber*BEAN_SCORE);
  GUI_DisplayLevel(ucBeansNumber/LEVEL_BEANS+INITIAL_LEVEL);

#if DEBUG==TRUE
  debug("Game Initial information.Inital delay time:");
  debug_INT32U(ulDelay);
#endif
}

/*************************************************************************************************
 * 函数名:GameControl_InitialSnake                                                                *
 * 功能描述:初始化蛇的相关数据结构，蛇的位置，豆子                                                *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_InitialSnake(void){
  INT8U n;

  pSnake->ucHead=2;
  pSnake->ucTail=0;
  pSnake->ucDir=DIR_RIGHT;
  ucKeyDir = pSnake->ucDir;
  for(n=0;n<3;n++){
    pSnake->aSnakeBody[n].ucX=n+2;
    pSnake->aSnakeBody[n].ucY=2;
  }
  pBean_t = &Bean_t;

  GUI_DisplayMap(ucMapNumber);
  GUI_DrawSnake(pSnake);
  GameControl_CreateBean();

#if DEBUG==TRUE
  debug("Initial snake. And Create Task.");
#endif
  OSTaskCreate(GameControl_Move, NULL, &MoveSTK[TASK_STK_SIZE-1],SNAKE_MOVE_PRIO);
  OSTaskCreate(GameControl_DisplayTime, NULL, &TimeSTK[TASK_STK_SIZE-1],GAME_TIME_PRIO);

#if DEBUG==TRUE
  debug("Initial snake.Created Task.");
#endif
}

/*************************************************************************************************
 * 函数名:GameControl_Move                                                                        *
 * 功能描述:蛇移动任务                                                                            *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   无                                                                       							 *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_Move(void *arg){
#if DEBUG==TRUE
  debug("Move STARTED.");
#endif
  if(GameControl_ChangeDirection(ucKeyDir)==TRUE){
    pSnake->ucDir = ucKeyDir;
#if DEBUG==TRUE
    debug("Game Snake change the direction.");
#endif
  }
  //GameControl_GameEnd();
  //set new head at the position of head
  ucNewHead = (pSnake->ucHead+1)%SNAKE_MAX_LENGTH;
  pSnake->aSnakeBody[ucNewHead].ucX = pSnake->aSnakeBody[pSnake->ucHead].ucX;
  pSnake->aSnakeBody[ucNewHead].ucY = pSnake->aSnakeBody[pSnake->ucHead].ucY;

  //move forward new head
  switch(pSnake->ucDir){
    case DIR_UP: 	pSnake->aSnakeBody[ucNewHead].ucY--;
                  break;
    case DIR_DOWN: pSnake->aSnakeBody[ucNewHead].ucY++;
                   break;
    case DIR_LEFT:	pSnake->aSnakeBody[ucNewHead].ucX--;
                    break;
    case DIR_RIGHT:	pSnake->aSnakeBody[ucNewHead].ucX++;
                    break;
  }

  // update head to new head
  pSnake->ucHead = ucNewHead;
#if DEBUG==TRUE
  debug("Snake move.New Head:");
  debug_INT8U(ucNewHead);
#endif

  // whether there is a bean
  if(pSnake->aSnakeBody[pSnake->ucHead].ucX!=pBean_t->ucX||          
      pSnake->aSnakeBody[pSnake->ucHead].ucY!=pBean_t->ucY){
    GUI_DrawNode(pSnake->aSnakeBody[pSnake->ucTail].ucX,
        pSnake->aSnakeBody[pSnake->ucTail].ucY,
        CLEARNODE);


    //update bitmap and screen 
    GameControl_SetBit(&(pSnake->aSnakeBody[pSnake->ucTail]), BitMap_Game, 0);
    pSnake->ucTail = (pSnake->ucTail+1)%SNAKE_MAX_LENGTH;
  }
  else{
    GameControl_Eaten(&ulDelay, &ucBeansNumber);
  }

  //whether touch the wall or body
  if(GameControl_GetBit(&(pSnake->aSnakeBody[pSnake->ucHead]), BitMap_Game)!=1){
    GameControl_SetBit(&(pSnake->aSnakeBody[pSnake->ucHead]),
        BitMap_Game,
        1);   
  }
  else 
    GameControl_GameEnd();

  //Draw Snake's head 
  GUI_DrawNode(pSnake->aSnakeBody[pSnake->ucHead].ucX,
      pSnake->aSnakeBody[pSnake->ucHead].ucY,
      DRAWNODE);

  OSTimeDlyHMSM(0, 0, 0, ulDelay);
}

/*************************************************************************************************
 * 函数名:GameControl_DisplayTime                                                                 *
 * 功能描述:显示游戏时间的任务                                                                    *
 * 日期:2009.07.18                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_DisplayTime(void*arg){
  OSTimeDlyHMSM(0,0,0,TIME_DELAY);
  ulTime+=TIME_DELAY;
  GUI_DisplayTime(ulTime);
}

/*************************************************************************************************
 * 函数名:GameControl_ResumeSnake                                                                 *
 * 功能描述:恢复蛇移动任务和时间显示任务                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_ResumeSnake(void){
  GUI_DrawSnake(pSnake);
  GUI_DrawNode(pBean_t->ucX, pBean_t->ucY, DRAWNODE);
  OSTaskResume(SNAKE_MOVE_PRIO);
  OSTaskResume(GAME_TIME_PRIO);
#if DEBUG==TRUE
  debug("Resume Snake move task.");
#endif
}

/*************************************************************************************************
 * 函数名:GameControl_SuspendSnake                                                                *
 * 功能描述:挂起蛇移动任务和时间显示任务                                                          *
 * 日期:2009.07.16                                                                            		 *
 * 参数:无                                                                         							 *
 * 返回值: 无                                                                                	   *
 * 作者:   王鹏程                                                                               	 *
 **************************************************************************************************/
void GameControl_SuspendSnake(void){
  OSTaskSuspend(SNAKE_MOVE_PRIO);
  OSTaskSuspend(GAME_TIME_PRIO);
#if DEBUG==TRUE
  debug("Suspend Snake move task");
#endif
}

/*************************************************************************************************
 * 函数名:GameControl_SetBit                                                                      *
 * 功能描述:设置向量位图的数据点                                                                  *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   pNode_t 具体的一个点坐标,PaBitMap 所操作的位图, ucBit 要设置的值                       *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
void GameControl_SetBit(Node_t* pNode_t, INT8U paBitMap[64],INT8U ucBit){
  INT8U ucX,ucY,ucMark;
  ucX = pNode_t->ucX;
  ucY = pNode_t->ucY;

  if(ucBit==1){
    if(ucY<8){
      ucMark = 1<<ucY;
      paBitMap[ucX] |= ucMark;
    }
    else{
      ucMark = 1<<(ucY-8);
      paBitMap[32+ucX]  |= ucMark;
    }		
  }
  else{
    if(ucY<8){
      ucMark = ~(1<<ucY);
      paBitMap[ucX]  &= ucMark;			
    }
    else{
      ucMark = ~(1<<(ucY-8));
      paBitMap[32+ucX]  &= ucMark;
    }
  }
}

/*************************************************************************************************
 * 函数名:GameControl_GetBit                                                                      *
 * 功能描述:获取向量位图的数据点                                                                  *
 * 日期:2009.07.16                                                                            		 *
 * 参数:   pNode_t 具体的一个点坐标,PaBitMap 所操作的位图                                         *
 * 返回值: 无                                                                                 	   *
 * 作者:   王喆                                                                                 	 *
 **************************************************************************************************/
INT8U GameControl_GetBit(Node_t* pNode_t, INT8U paBitMap[64]){
  INT8U ucX,ucY,ucTemp;
  ucX = pNode_t->ucX;
  ucY = pNode_t->ucY;

  if(ucY<8){
    ucTemp = paBitMap[ucX];
    ucTemp = ucTemp>>ucY;
  }
  else{
    ucTemp = paBitMap[32+ucX];
    ucTemp = ucTemp >> (ucY-8);
  }
  ucTemp &= 0x01;
  return ucTemp;
}

