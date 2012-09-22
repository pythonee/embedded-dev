#include "Configure.h"

#ifndef __KEY_PRESS__
#define __KEY_PRESS__

#define KEY_LEFT       (1<<24)  	  //　方向键对应引脚
#define KEY_UP         (1<<25)
#define KEY_RIGHT      (1<<28)
#define KEY_DOWN       (1<<27)						   
#define KEY_START      (1<<16)            // 对应引脚 开关按钮对应引脚位置

#define DIR_KEY_PIN    	  (0xFC30FFFF)	   // 方向键Pin选择值
#define DIR_KEY_PIN_OUT	  (0x1B000000)	   // 方向键的GPIO设为输出
#define START_PIN     	  (0xFFFFFFFC)	   // 开始键的Pin值
#define START_PIN_IN  	  (0xFFFEFFFF)      // 开始键的GPIO设为输入     

#define DIR_KEY_PRESSED	  (0x1B000000)
#define START_KEY_PRESSED (0x00010000)

#define DIR_KEY_SEL       PINSEL1
#define DIR_KEY_DIR       IO0DIR
#define DIR_KEY_SET       IO0SET
#define START_KEY_SEL     PINSEL2
#define START_KEY_DIR     IO1DIR
#define START_KEY_SET     IO1SET

#define EMPTY		(0x00)			// 定义空键值

static INT8U  ucKeyValue;	    // 被按下的按键

void Key_Init(void);
void Key_WaitKey(void);
INT8U Key_GetKey(void);
void Key_ProcessKey(INT8U ucKeyValue);
void Key_Main(void*);
 
#endif 

