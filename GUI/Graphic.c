#include "Configure.h"

OS_STK  TwinkleTaskStk[TASK_STK_SIZE];			//定义任务堆栈区
OS_STK  DelTwinkleStk[TASK_STK_SIZE];			//删除任务堆栈区

INT8U LCD12864_Screen[8][128] = {0};

INT8U str_Minute[] = "00";
INT8U str_Second[] = "00";
INT8U str_Level[]  = "00";
INT8U str_Score[]  = "00";

INT8U Snake_Up[] = {
  0x0F,0x0F,0x0F,0x0F
};
INT8U Snake_Down[] = {
  0xF0,0xF0,0xF0,0xF0
};

/**************************************************************************************************
* 函数名 : GUI_Init()                                                                             *
* 功能描述 : GUI模块初始化                                                                     	  *
* 日期  :    2009-07-16                                                              		  *
* 参数  :    void                                                            			  *
* 返回值 :   void                                                                              	  *
* 作者  :     王飞                                                                                *
**************************************************************************************************/
void GUI_Init(void)
{
  LCD1602Driver_Init();   //LCD1602初始化
  LCD12864Driver_Init();  //LCD12864初始化
  LCD1602Driver_DisplayString(0, 0, "Time");  //  小屏显示时间字符串
  LCD1602Driver_DisplayString(1, 0, "Score"); //  小屏显示分数字符串
  LCD1602Driver_DisplayString(1, 10, "Lev");  //  小屏显示等级字符串

 /*
  * 初始化时间，分数，等级都为00
  */
  GUI_DisplayTime((INT32U)0);
  GUI_DisplayScore((INT8U)0);
  GUI_DisplayLevel((INT8U)0);

  GUI_DisplayPicture(0,0,ucWelcomeBmp,&ucWelcomeBmpSize);  // 显示欢迎界面
  GUI_TwinklePic();                                        // 文字闪烁，提示用户按键进入游戏
}

/**************************************************************************************************
* 函数名 : GUI_DrawSnake                                                                          *
* 功能描述 : 游戏从暂停切换到进行时，GameControl调用此接口，重新绘制新蛇                      	  *
* 日期  :    2009-07-17                                                       			  *
* 参数  : Snake_t *pSnake_tSnake                                                          	  *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                 	  *
**************************************************************************************************/
void GUI_DrawSnake(Snake_t *pSnake_tSnake)
{
  INT8U loop = 0;
  // 判断蛇头下标是否大于蛇尾
  if(pSnake_tSnake->ucHead>pSnake_tSnake->ucTail){
    for(loop = pSnake_tSnake -> ucTail;loop <= pSnake_tSnake -> ucHead;loop++)
    {
      GUI_DrawNode(pSnake_tSnake->aSnakeBody[loop].ucX,pSnake_tSnake->aSnakeBody[loop].ucY,DRAWNODE);
    }
  }
  // 蛇头下标小于蛇尾
  else{
    // 从蛇尾坐标开始打到蛇身数组的结束
    for(loop = pSnake_tSnake -> ucTail;loop <SNAKE_MAX_LENGTH ;loop++)
    {
      GUI_DrawNode(pSnake_tSnake->aSnakeBody[loop].ucX,pSnake_tSnake->aSnakeBody[loop].ucY,DRAWNODE);
    }
    loop %=SNAKE_MAX_LENGTH;
    // 从蛇头打印到蛇尾
    while(loop<=pSnake_tSnake->ucHead){
      GUI_DrawNode(pSnake_tSnake->aSnakeBody[loop].ucX,pSnake_tSnake->aSnakeBody[loop].ucY,DRAWNODE);
      loop++;
    }
  }
}

/**************************************************************************************************
* 函数名 : GUI_Mix                                                                                *
* 功能描述 : 用两种可选的方式实现局部混合                                                     	  *
* 日期  :  2009-07-15                                                                  		  *
* 参数  :  ucX  混合图片的起始横坐标                                                              *
* 	   ucY  混合图片的起始纵坐标                                                              *
* 	   *pucPicture 混合图片的字模数组      							  *
* 	   *pucPictureSize 混合图片的大小                                                         *
* 	   ReplaceORAppend 两种可选的方式                                                         *
* 返回值 : void                                                                                	  *
* 作者  :   王飞                                                                               	  *
**************************************************************************************************/
void GUI_Mix(INT8U ucX,INT8U ucY,const INT8U *pucPicture,const Size_t *pucPictureSize,BOOLEAN ReplaceORAppend)
{
  INT8U xloop = 0;
  INT8U yloop = 0;

  for(yloop = ucY; yloop < ucY + (pucPictureSize -> ucHight); yloop += 8){
    for(xloop = ucX;xloop < ucX + pucPictureSize -> ucLength; xloop++){
      if(ReplaceORAppend == APPEND)
      {
	// 算出一维混合字模与虚拟LCD12864内存的对应关系后，做或操作，实现填充式混合
        LCD12864_Screen[yloop/8][xloop]  |= pucPicture[(yloop- ucY)/8*(pucPictureSize -> ucLength) + xloop - ucX];
      }
      else
      {
	// 算出一维混合字模与虚拟LCD12864内存的对应关系后，做一次赋值，实现覆盖式混合
        LCD12864_Screen[yloop/8][xloop]  = pucPicture[(yloop- ucY)/8*(pucPictureSize -> ucLength) + xloop - ucX];
      }
      // 刷新屏幕
      LCD12864Driver_DisplayByte(yloop/8,xloop,LCD12864_Screen[yloop/8][xloop]);
    }
  }
}

/**************************************************************************************************
* 函数名 : GUI_DisplayTime                                                                        *
* 功能描述 : 显示游戏进行的时间                                                                	  *
* 日期  :  2009-07-18                                                          			  *
* 参数  :  ucSeconds 游戏进行的毫秒数         							  *
* 返回值 : void                                                                               	  *
* 作者  :  王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayTime(INT32U ucSeconds)
{
  ucSeconds/=1000;  //毫秒转化为秒
  //判断游戏进行是否超过10秒
  if(ucSeconds%60 < 10)
  {
    str_Second[0] = '0';
    str_Second[1] =(ucSeconds%60)+'0';
  }
  else
  {
    str_Second[0] = ((ucSeconds%60)/10)+'0';
    str_Second[1] = ((ucSeconds%60)%10)+'0';
  }
  //判断游戏进行是否超过60秒
  if(ucSeconds/60 < 10)
  {
    str_Minute[0] = '0';
    str_Minute[1] = (ucSeconds/60)+'0';
  }
  else
  {
    str_Minute[0] = ((ucSeconds/60)/10)+'0';
    str_Minute[1] = ((ucSeconds/60)%10)+'0';
  }
  LCD1602Driver_DisplayString(0,MINUTE_X,str_Minute);
  LCD1602Driver_DisplayString(0,CURSOR_MINUTE_X,":");
  LCD1602Driver_DisplayString(0,SECOND_X,str_Second);
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayScore                                                                      *
* 功能描述 : 小屏上显示游戏所得分数                                                           	  *
* 日期  :    2009-07-16                                                        			  *
* 参数  :    ucScore  游戏所得分数            							  *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayScore(INT32U ucScore)
{
  // 判断分数是否超过10,如果没有，十位数显示0
  // 如果超过10分，十位数才变化
  if(ucScore < 10)
  {
  	str_Score[0] = '0';
	str_Score[1] = (ucScore%10)+'0';
  }
  else
  {
  	str_Score[0] = ucScore/10+'0';
	str_Score[1] = ucScore%10+'0';
  }
#if DEBUG==TRUE
  debug("Score:");
  debug(str_Score);
#endif
  LCD1602Driver_DisplayString(1,SCORE_X,str_Score);
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayLevel                                                                      *
* 功能描述 : 小屏上显示游戏所得等级                                                           	  *
* 日期  :    2009-07-16                                                        			  *
* 参数  :    ucLevel  游戏所得等级            							  *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayLevel(INT8U ucLevel)
{
  // 判断等级是否超过10,如果没有，十位数显示0
  // 如果超过10级，十位数才变化
  if(ucLevel < 10)
  {
  	str_Level[0] = '0';
	  str_Level[1] = (ucLevel%10)+'0';
  }
  else
  {
  	str_Level[0] = ucLevel/10+'0';
	  str_Level[1] = ucLevel%10+'0';
  }
#if DEBUG==TRUE
  debug("Level:");
  debug(str_Level);
#endif
  LCD1602Driver_DisplayString(1,LEVEL_X,str_Level);
}

/**************************************************************************************************
* 函数名 :  GUI_DrawNode                                                                          *
* 功能描述 : 画豆子，画蛇身上的一个节点                                                        	  *
* 日期  :    2009-07-17                                                        			  *
* 参数  :    ucX  节点起始横坐标                                                                  *
*            ucY  节点起始纵坐标                                                                  *
*            DrawORClear 是画点还是清点                                                           *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DrawNode(INT8U ucX,INT8U ucY,BOOLEAN DrawORClear)
{
  INT8U realX = 0;
  INT8U realY = 0;

  INT8U xloop = 0;
  INT8U yloop = 0;

  // 因为蛇的节点是4*4,外部调用者看到的坐标
  // 为16*32,所以在此之前做坐标恢复
  realX = ucX * 4;
  realY = ucY * 4;

  for(yloop = realY; yloop < realY + NODE_HIGHT; yloop += 8)
  {
    for(xloop = realX;xloop < realX +NODE_LENGTH; xloop++)
    {
      if(DrawORClear == DRAWNODE)
      {
        if(yloop%8 < 4)  //判断是否是低四位
        {

          LCD12864_Screen[yloop/8][xloop] |= Snake_Up[xloop - realX]; //如果是画点，则做或操作
        }
        else // 如果是高四位
        {
          LCD12864_Screen[yloop/8][xloop] |= Snake_Down[xloop - realX];
        }
        LCD12864Driver_DisplayByte(yloop/8,xloop,LCD12864_Screen[yloop/8][xloop]);
      }
      else  // 如果是清除一个节点
      {
        if(yloop%8 < 4) //判断是否是低四位
        {
          LCD12864_Screen[yloop/8][xloop] &= Snake_Down[xloop - realX]; // 清除点，则做与操作
	}
        else  // 如果是高四位
        {
          LCD12864_Screen[yloop/8][xloop] &= Snake_Up[xloop - realX];
        }
        LCD12864Driver_DisplayByte(yloop/8,xloop,LCD12864_Screen[yloop/8][xloop]);
      }
    }

  }
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayMiniMap                                                                    *
* 功能描述 : 选择地图时，显示小地图                                                        	  *
* 日期  :    2009-07-17                                                        			  *
* 参数  :    ucMiniMapNumber 小地图的标号                                                         *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayMiniMap(INT8U ucMiniMapNumber)
{
  GUI_Mix(0,0,ucChooseMapBmp,&ucChooseMapBmpSize,REPLACE);//做清屏操作，并加载静态图片
  GUI_Mix(MINI_MAP_X,MINI_MAP_Y,pucaMiniMaps[ucMiniMapNumber],&MiniMapSize,REPLACE); //覆盖之前的显示的小地图
#if DEBUG==TRUE
  debug("Change the map:");
  debug_INT8U(ucMiniMapNumber);
#endif
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayMap                                                                        *
* 功能描述 : 显示玩家所选地图                                                           	  *
* 日期  :    2009-07-17                                                        			  *
* 参数  :    ucMapNumber 在地图的标号                                                             *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayMap(INT8U ucMapNumber)
{
    GUI_Mix(0,0,pucaMaps[ucMapNumber],&MapSize,REPLACE); // 做清屏操作，并显示地图
}

/**************************************************************************************************
* 函数名 : GUI_DisplayPicture                                                                     *
* 功能描述 : 显示静态图片                                                               	  *
* 日期  :  2009-07-15                                                                  		  *
* 参数  :  ucX  图片的起始横坐标                                                                  *
* 	   ucY  图片的起始纵坐标                                                                  *
* 	   *pucPicture 混合图片的字模数组      							  *
* 	   *pucPictureSize 混合图片的大小                                                         *
* 返回值 : void                                                                                	  *
* 作者  :   王飞                                                                               	  *
**************************************************************************************************/
void GUI_DisplayPicture(INT8U ucX,INT8U ucY,const INT8U *pucPicture,const Size_t *Size_tPictureSize)
{
  INT8U ucPage,ucCol;

  INT8U ucHight = Size_tPictureSize -> ucHight;
  INT8U ucLength = Size_tPictureSize -> ucLength;

  for(ucPage = ucY;ucPage<ucY+ucHight;ucPage += 8)
    for(ucCol = ucX;ucCol<ucX+ ucLength;ucCol++)
      //以8bit轮询刷新屏幕
      LCD12864Driver_DisplayByte(ucPage/8,ucCol,pucPicture[(ucPage-ucY)/8*ucLength + ucCol - ucX]);
}
/**************************************************************************************************
* 函数名 :  GUI_DisplayHeroName                                                                   *
* 功能描述 : 在输入英雄名画面上，逐位显示玩家的输入                                            	  *
* 日期  :    2009-07-18                                                        			  *
* 参数  :    Heros[] 获取用户输入字符串对应的字母数组的下标数组                                   *
* 	     CurPosition 当前光标的位置                                                           *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayHeroName(INT8U Heros[],INT8U CurPosition)
{
  INT8U loop = 0;
  INT8U Gap = 10;
  INT8U inputName_x = INPUT_NAME_X;

  // 光标的坐标计算
  INT8U cursor_x  = inputName_x;
  INT8U cursor_y  = INPUT_NAME_Y + 8;

  GUI_Mix(0,0,ucCongratulationsBmp,&ucCongratulationsBmpSize,REPLACE);
  GUI_Mix(cursor_x + CurPosition * Gap,cursor_y,ucCursorBmp,&ucCursorBmpSize,APPEND);

  for(loop =0; loop < 3; loop++)
  {
      inputName_x += Gap; // 以一定的间隙移动光标
      GUI_Mix(inputName_x,INPUT_NAME_Y,ucLetters[Heros[loop]],&LetterSize,REPLACE);
  }
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayHero                                                                       *
* 功能描述 : 显示英雄榜                                                                  	  *
* 日期  :    2009-07-17                                                        			  *
* 参数  :    aHero_tHeroes[] 三个英雄信息的结构体                                                 *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayHero(Hero_Score_t aHero_tHeroes[])
{
  INT8U xloop = 0;
  INT8U yloop = 0;
  INT8U xGap = 10;
  INT8U yGap = 8;
  INT8U HeroScore[] = {0,0,0};

  // 英雄榜名字的起始坐标
  INT8U HeroBoardName_x = HERO_BOARD_NAME_X;
  INT8U HeroBoardName_y = HERO_BOARD_NAME_Y;

  // 分数的起始坐标
  INT8U score_x = HERO_BOARD_SCORE_X;
  INT8U score_y = HERO_BOARD_NAME_Y ;

  GUI_Mix(0,0,ucHeroBoardBmp,&ucHeroBoardSize,REPLACE);

  for(xloop = 0; xloop < 3;xloop++)
  {
    HeroBoardName_y += yGap;
    score_y += yGap;

	HeroScore[0] = aHero_tHeroes[xloop].ucScore/100;  // 计算英雄分数的百位
	HeroScore[1] = aHero_tHeroes[xloop].ucScore/10;   // 计算英雄分数的十位
	HeroScore[2] = aHero_tHeroes[xloop].ucScore%10;   // 计算英雄分数的个位

    for(yloop = 0; yloop < 3; yloop++)
    {
      HeroBoardName_x += xGap;
      score_x += xGap;

      // 局部打印上英雄的名字
      GUI_Mix(HeroBoardName_x,HeroBoardName_y,ucLetters[aHero_tHeroes[xloop].cName[yloop]],&LetterSize,APPEND);

      // 局部打印上英雄的分数
      GUI_Mix(score_x,score_y,ucNumbers[HeroScore[yloop]],&LetterSize,APPEND);
    }
      // 打印完一行之后，恢复名字和分数的起始坐标，准备打下一行
      HeroBoardName_x = HERO_BOARD_NAME_X;
      score_x = HERO_BOARD_SCORE_X;
  }
}

/**************************************************************************************************
* 函数名 :  GUI_DisplayPause                                                                      *
* 功能描述 : 显示暂停提示图片                                                           	  *
* 日期  :    2009-07-17                                                        			  *
* 参数  :    void			                                                          *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DisplayPause()
{
  GUI_Mix(PAUSE_BMP_X,PAUSE_BMP_Y,ucPauseBmp,&ucPauseBmpSize,REPLACE);
}

/**************************************************************************************************
* 函数名 :  GUI_TwinklePicTask                                                                    *
* 功能描述 : 屏幕闪烁任务                                                               	  *
* 日期  :    2009-07-18                                                        			  *
* 参数  :    void			                                                          *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_TwinklePicTask(void *arg)
{
#if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr;
#endif
#if DEBUG==TRUE
  debug("Twinkle task begin.");
#endif
      // 通过有文字与无文字之间的快速切换达到闪烁的效果
      GUI_Mix(TWINK_PIC_X,TWINK_PIC_Y,ucContinueBmp,&ucContinueBmpSize,REPLACE);         // 先混合上有文字的图片
      OSTimeDlyHMSM(0,0,0,TWINK_DELAY_TIME);
      GUI_Mix(TWINK_PIC_X,TWINK_PIC_Y,ucTwinkleBlankBmp,&ucTwinkleBlankBmpSize,REPLACE); // 再混合上没文字的图片
      OSTimeDlyHMSM(0,0,0,TWINK_DELAY_TIME);
#if DEBUG==TRUE
  debug("Twinkle task end.");
#endif
}

/**************************************************************************************************
* 函数名 :  GUI_TwinklePic                                                                        *
* 功能描述 : 创建闪烁任务                                                               	  *
* 日期  :    2009-07-18                                                        			  *
* 参数  :    void			                                                          *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_TwinklePic()
{
  OSTaskCreate(GUI_TwinklePicTask,NULL,&TwinkleTaskStk[TASK_STK_SIZE-1],TWINK_PIC_PRIO);
}

/**************************************************************************************************
* 函数名 :  GUI_DelTwinkTask                                                                      *
* 功能描述 : 删除闪烁任务                                                               	  *
* 日期  :    2009-07-18                                                        			  *
* 参数  :    void			                                                          *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_DelTwinkTask(void *arg){
  GUI_Mix(TWINK_PIC_X,TWINK_PIC_Y,ucTwinkleBlankBmp,&ucTwinkleBlankBmpSize,REPLACE);
  OSTaskDel(TWINK_PIC_PRIO);
  OSTaskDel(OS_PRIO_SELF);
}

/**************************************************************************************************
* 函数名 :  GUI_KeyPress                                                                          *
* 功能描述 : 创建删除闪烁任务                                                           	  *
* 日期  :    2009-07-18                                                        			  *
* 参数  :    void			                                                          *
* 返回值 :   void                                                                              	  *
* 作者  : 王飞                                                                                	  *
**************************************************************************************************/
void GUI_KeyPress(INT8U ucKeyValue)
{
  if( OSTCBPrioTbl[TWINK_PIC_PRIO]!=(OS_TCB*)0){
    OSTaskCreate(GUI_DelTwinkTask,NULL,&DelTwinkleStk[TASK_STK_SIZE-1],DEL_TWINK_PRIO);
  }
}


