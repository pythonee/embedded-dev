#include"Configure.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//****************************************************************************
//* 名称：LCD12864Driver_WriteCMD
//* 功能：写命令
//****************************************************************************
void LCD12864Driver_WriteCMD(INT8U ucCMD)
{
	LCD12864Driver_CLR=LCD12864Driver_RS;
	LCD12864Driver_CLR=LCD12864Driver_RW;

	LCD12864Driver_CLR=0xFF <<LCD12864Driver_DAT_NO;
	LCD12864Driver_SET=ucCMD<<LCD12864Driver_DAT_NO;
	LCD12864Driver_SET=LCD12864Driver_EN;
	LCD12864Driver_CLR=LCD12864Driver_EN;
}

//****************************************************************************
//* 名称：LCD12864Driver_WriteData
//* 功能：写数据
//****************************************************************************
void LCD12864Driver_WriteData(INT8U ucData)
{
	LCD12864Driver_SET=LCD12864Driver_RS;
	LCD12864Driver_CLR=LCD12864Driver_RW;

	LCD12864Driver_CLR=0xFF <<LCD12864Driver_DAT_NO;
	LCD12864Driver_SET=ucData<<LCD12864Driver_DAT_NO;
	
	LCD12864Driver_SET=LCD12864Driver_EN;
	LCD12864Driver_CLR=LCD12864Driver_EN;

}

//****************************************************************************
//* 名称：LCD12864Driver_CleanAll
//* 功能：清屏

//****************************************************************************
void LCD12864Driver_CleanAll(void)
{
	INT8U ucPage,ucCol;
	LCD12864Driver_SET=LCD12864Driver_CSA;	//左右两屏都选中
	LCD12864Driver_SET=LCD12864Driver_CSB;
	for(ucPage=0;ucPage<8;ucPage++)
	{
	
		LCD12864Driver_WriteCMD(LCD_PAGE+ucPage);	  //页
		LCD12864Driver_WriteCMD(LCD_COL);		 //列
		for(ucCol=0;ucCol<64;ucCol++)
			LCD12864Driver_WriteData(0x00);
	}
}

//****************************************************************************
//* 名称：LCD12864Driver_Init
//* 功能：初始化
//****************************************************************************
void LCD12864Driver_Init(void)
{

	LCD12864Driver_SEL0 &=0x003FFFFF;
	LCD12864Driver_DIR &= 0x000FF801;     //掩码
	LCD12864Driver_DIR |= 0x00FFF800;
	LCD12864Driver_CLR |= 0x00FFF800;
		
	LCD12864Driver_WriteCMD(LCD_DispOn);  //开显示
	LCD12864Driver_WriteCMD(LCD_STARTROW);  //设置起始行地址，列地址和页地址是变化的
	
	LCD12864Driver_CleanAll();
}

 /* *************************************************************************************************
* Function Name : LCD12864Driver_DisplayByte                                                             *
* Description	: 显示一个字节,即纵向8个像素    							             	       *
* Date		    :                                                             	           		   *
* Parameter     : ucPage         显示的页 (屏幕一共8页)                                            *
*                 ucCol          要显示的列 (屏幕一共128列)                                        *
*                 ucData         要显示的数据                                                      *
* Return Code   : 无                                                                               *
* Author	    :                                                                 	               *
************************************************************************************************* */
void LCD12864Driver_DisplayByte(INT8U ucPage,INT8U ucCol,INT8U ucData)
{	
	
	if(ucCol/SINGLE_SCREEN_COL==0)
	{
		LCD12864Driver_SET=LCD12864Driver_CSA;
		LCD12864Driver_CLR=LCD12864Driver_CSB;
	}
	else{
		LCD12864Driver_CLR=LCD12864Driver_CSA;
		LCD12864Driver_SET=LCD12864Driver_CSB;
	}

	//设置开始的页，列

	LCD12864Driver_WriteCMD(LCD_PAGE+ucPage);
	LCD12864Driver_WriteCMD(LCD_COL+ucCol%SINGLE_SCREEN_COL);
	LCD12864Driver_WriteData(ucData);
	
}

//****************************************************************************
//* 名称：LCD12864Driver_DispOneChinese
//* 功能：显示一个汉字
//一个汉字需要两页*16列
//*注意：页,列有改变
//****************************************************************************

void LCD12864Driver_DispOneChinese(INT8U ucPage,INT8U ucCol,INT8U *pDisTable)
{
	INT8U i=0;
	for(i= 0;i<16;i++)	//汉字上半部分
	{
	LCD12864Driver_DisplayByte(ucPage,ucCol+i,*(pDisTable+i));

	}
	for(i= 0;i<16;i++)	 //汉字下半部分
	{
	LCD12864Driver_DisplayByte(ucPage+1,ucCol+i,*(pDisTable+16+i));

	}
}

 //****************************************************************************
//* 名称：LCD12864Driver_DispChinese
//* 功能：显示n个汉字
//一个汉字需要两页*16列
//*注意：  按照这种写法要每个汉字单独取模
//**************************************************
void LCD12864Driver_DispChinese(INT8U ucPage,INT8U ucCol,INT8U pDisTable[][32],INT8U Num)
{
	INT8U i=0;
	for(i= 0;i<Num;i++)
	LCD12864Driver_DispOneChinese (ucPage,ucCol+16*i,*(pDisTable+i));
}
  //****************************************************************************
//* 名称：LCD12864Driver_DisPicture
//* 功能：显示一幅图
//
//*注意：最大128*64点阵 ，要提供图形的高度和宽度
//**************************************************
void LCD12864Driver_DisPicture(INT8U ucPage,INT8U ucCol,INT8U *pDisTable,INT8U width,INT8U hight)
{
	INT8U i,j;
	for(i = ucPage;i<ucPage+hight/8;i++)
		for(j = ucCol;j<ucCol+ width;j++)	
			LCD12864Driver_DisplayByte(i,j,*(pDisTable+i*128+j));

}


