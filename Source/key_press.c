#include "Configure.h"

/**************************************************************************************************
* 函数名:    Key_Init                                                                             *
* 功能描述:  初始化按键                                                                           *
* 日期:      2009-7-17                                                                       	  *
* 参数:      -                                                      				              *
* 返回值:    -                                                                             	      *
* 作者:      仝珂                                                                                 *
**************************************************************************************************/
void Key_Init(void)			        // 初始化按键对应引脚的状态
{
    DIR_KEY_SEL    &= DIR_KEY_PIN;       //将P0.24、P0.25、P0.27、P0.28 设为GPIO口
	DIR_KEY_DIR    |= DIR_KEY_PIN_OUT;   //设置这些引脚为输出引脚 
	DIR_KEY_SET     = DIR_KEY_PIN_OUT;   //将这些引脚状态设为输出
	START_KEY_SEL  &= START_PIN;		 //将P1.16 设为GPIO口
	START_KEY_DIR  &= START_PIN_IN;		 //设置P1.16为输入引脚
	START_KEY_SET   = START_PIN_IN;

#if DEBUG==TRUE
  debug("Initial the keys.");
#endif
}



/**************************************************************************************************
* 函数名:    Key_GetKey                                                                           *
* 功能描述:  对按键键值进行判断，获取按键键值                                                     *
* 日期:      2009-7-17                                                                       	  *
* 参数:      -                                                      				              *
* 返回值:    ucKeyValue                                                                           *
* 作者:      仝珂                                                                                 *
**************************************************************************************************/ 
INT8U Key_GetKey(void)		       //获取所按方向键的键值
{ 
	if (!(IO0PIN & KEY_LEFT))	  // 按下左键
	{
		ucKeyValue = LEFT;
		return ucKeyValue;
	}
	if (!(IO0PIN & KEY_UP))	      // 按下上键
	{
		ucKeyValue = UP;
		return ucKeyValue;
	}
	if (!(IO0PIN & KEY_RIGHT))	  // 按下右键
	{
		ucKeyValue = RIGHT;
		return ucKeyValue;
	}
	if (!(IO0PIN & KEY_DOWN)) 	  // 按下下键
	{
		ucKeyValue = DOWN;
		return ucKeyValue;
	}
	if (!(IO1PIN & KEY_START)) 	  // 按下开始键
	{
		ucKeyValue = START;
		return ucKeyValue;
	}
	else					      //没有按键按下
	{
		ucKeyValue = EMPTY;
		return ucKeyValue;
	}
}



/**************************************************************************************************
* 函数名:    Key_ProcessKey                                                                       *
* 功能描述:  对特定键值进行处理，即将键值传递给接口函数                                           *
* 日期:      2009-7-17                                                                       	  *
* 参数:      ucKeyValue                                                      				      *
* 返回值:    -                                                                             	      *
* 作者:      仝珂                                                                                 *
**************************************************************************************************/ 
void Key_ProcessKey(INT8U ucKeyValue)
{
	if(ucKeyValue != EMPTY)					  //如果按键不为空
	{
		GUI_KeyPress(ucKeyValue);			  //将键值传递给GUI的接口函数
		GameControl_KeyPress(ucKeyValue);	  //将键值传递给GameControl的接口函数
		Environment_KeyPress(ucKeyValue);	  //将键值传递给Environment的接口函数
	}
	else
	{}
}



/**************************************************************************************************
* 函数名:    Key_Main                                                                             *
* 功能描述:  Key处理主程序，防止按键抖动，防止按键长按                                            *
* 日期:      2009-7-17                                                                       	  *
* 参数:      -                                                      				              *
* 返回值:    -                                                                             	      *
* 作者:      仝珂                                                                                 *
**************************************************************************************************/ 
void Key_Main(void*arg)
{ 
	  if(((IO0PIN & DIR_KEY_PRESSED)!= DIR_KEY_PRESSED)||(IO1PIN & START_KEY_PRESSED)!=START_KEY_PRESSED)
	  {
		OSTimeDlyHMSM(0,0,0,3);
	 	if(((IO0PIN & DIR_KEY_PRESSED)!= DIR_KEY_PRESSED)||(IO1PIN & START_KEY_PRESSED)!=START_KEY_PRESSED)
		Key_ProcessKey(Key_GetKey());
	  }
	
	  if((IO0PIN & DIR_KEY_PRESSED)!= DIR_KEY_PRESSED)
	  {
	 	 if(ucGameState != GAME_STAT_GAMING)
	  	 {
			while ((IO0PIN & DIR_KEY_PRESSED)!= DIR_KEY_PRESSED);
	     }
	  }

      if((IO1PIN & START_KEY_PRESSED)!=START_KEY_PRESSED)
   	  {
		 while ((IO1PIN & START_KEY_PRESSED)!=START_KEY_PRESSED);
      }
   	  OSTimeDlyHMSM(0,0,0,30);
}

