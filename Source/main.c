#include "Configure.h"


//声明Key_press task 的任务堆栈
OS_STK keyMainStk[TASK_STK_SIZE];

OS_STK timeStk[TASK_STK_SIZE];

//开始时钟中断程序
void startTimer(void* arg){
  arg=arg;
	T0TCR = 1;//关键
#if DEBUG==TRUE
  debug("Start Timer Service.");
#endif
  OSTaskDel(OS_PRIO_SELF);
}

void init(){
  TargetInit();
  OSInit();
  Environment_Initial();
  srand(OSTimeGet());
#if DEBUG==TRUE
  debug("Initialize: Target, OS, LCDs, Key, Environment.");
#endif
}

int main()
{
  init();
  OSTaskCreate(startTimer, NULL, &timeStk[TASK_STK_SIZE-1],OPEN_TIME_PRIO);
  OSTaskCreate(Key_Main, NULL, &keyMainStk[TASK_STK_SIZE-1], KEY_PRIO);
  OSStart();
}

