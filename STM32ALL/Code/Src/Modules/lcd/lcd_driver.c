#include "lcd_driver.h"

/****************************************************************************
* Function Name  : TFT_GPIO_Config
* Description    : 初始化TFT彩屏的IO口
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_GPIO_Config(void)
{	
	/* 打开时钟使能 */
    RCC->APB2ENR |= 0x00000020 | 0x00000040 | 0x00000100;

    /* 设置IO为复用推挽输出 */
    GPIOG->CRL &= 0xFFFFFFF0;  //PG0
    GPIOG->CRL |= 0x0000000B;
    GPIOG->CRH &= 0xFFF0FFFF;  //PG12
    GPIOG->CRH |= 0x000B0000;

    GPIOD->CRL &= 0xFF00FF00;
    GPIOD->CRL |= 0x00BB00BB;
    GPIOD->CRH &= 0x00000000;
    GPIOD->CRH |= 0xBBBBBBBB;

    GPIOE->CRL &= 0x0FFFFFFF;
    GPIOE->CRL |= 0xB0000000;
    GPIOE->CRH &= 0x00000000;
    GPIOE->CRH |= 0xBBBBBBBB;

}

/****************************************************************************
* Function Name  : TFT_FSMC_Config
* Description    : 初始化FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_FSMC_Config(void)
{
    /* 使能FSMC的时钟 */
    RCC->AHBENR |= 1 << 8;     	 	//使能FSMC时钟
    
	/* bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。*/
	/* 这里我们使用NE4 ，也就对应BTCR[6],[7]。 */				    
	FSMC_Bank1->BTCR[6] = 0x00000000;
	FSMC_Bank1->BTCR[7] = 0x00000000;
	FSMC_Bank1E->BWTR[6] = 0x00000000;
	/* 操作BCR寄存器	使用异步模式 */
	FSMC_Bank1->BTCR[6] |= 1 << 12;		//存储器写使能
	FSMC_Bank1->BTCR[6] |= 1 << 14;		//读写使用不同的时序
	FSMC_Bank1->BTCR[6] |= 1 << 4; 		//存储器数据宽度为16bit 	    
	/* 操作BTR寄存器 */	
	/* 读时序控制寄存器 */							    
	FSMC_Bank1->BTCR[7] |= 0 << 28;		//模式A 	 							  	 
	FSMC_Bank1->BTCR[7] |= 1 << 0; 		//地址建立时间（ADDSET）为2个HCLK 1/36M=27ns	 	 
	/* 因为液晶驱动IC的读数据的时候，速度不能太快。 */
	FSMC_Bank1->BTCR[7] |= 0x0F << 8;  	//数据保存时间为16个HCLK	 	 
	/* 写时序控制寄存器 */ 
	FSMC_Bank1E->BWTR[6] |= 0 << 28; 	//模式A 	 							    
	FSMC_Bank1E->BWTR[6] |= 0 << 0;		//地址建立时间（ADDSET）为1个HCLK 
 	/* 4个HCLK（HCLK=72M）因为液晶驱动IC的写信号脉宽，最少也得50ns。72M/4=24M=55ns */  	 
	FSMC_Bank1E->BWTR[6] |= 3 << 8; 	//数据保存时间为4个HCLK	
	/* 使能BANK1,区域4 */
	FSMC_Bank1->BTCR[6] |= 1 << 0;		//使能BANK1，区域4	
}

/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCD写入命令
* Input          : cmd：写入的16位命令
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	TFT->TFT_CMD = cmd;
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCD写入数据
* Input          : dat：写入的16位数据
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(uint16_t dat)
{
	TFT->TFT_DATA = dat;
}

/****************************************************************************
* Function Name  : TFT_Init
* Description    : 初始化LCD屏
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_Init(void)
{
	uint16_t i;

	TFT_GPIO_Config();
	TFT_FSMC_Config();

	for(i=500; i>0; i--);
	TFT_WriteCmd(0x11);
	for(i=500; i>0; i--);
	TFT_WriteCmd(0xB9);
    TFT_WriteData(0xFF);
	TFT_WriteData(0x83);
	TFT_WriteData(0x57);
    for(i=500; i>0; i--);

	TFT_WriteCmd(0xB1);
    TFT_WriteData(0x00);
	TFT_WriteData(0x14);
	TFT_WriteData(0x1C);
	TFT_WriteData(0x1C);
	TFT_WriteData(0xC3);
	TFT_WriteData(0x44);
    TFT_WriteData(0x70);
   for(i=500; i>0; i--);
    TFT_WriteCmd(0xB4);
    TFT_WriteData(0x22);
	TFT_WriteData(0x40);
	TFT_WriteData(0x00);
	TFT_WriteData(0x2A);
	TFT_WriteData(0x2A);
	TFT_WriteData(0x20);
	TFT_WriteData(0x91);
  for(i=500; i>0; i--);
    TFT_WriteCmd(0x36);
    TFT_WriteData(0x4c);

    TFT_WriteCmd(0xC0);
    TFT_WriteData(0x50);
	TFT_WriteData(0x50);
	TFT_WriteData(0x01);
	TFT_WriteData(0x3C);
	TFT_WriteData(0xC8);
	TFT_WriteData(0x08);
    TFT_WriteData(0x00);
	TFT_WriteData(0x08);
	TFT_WriteData(0x04);
     for(i=500; i>0; i--);
    for(i=500; i>0; i--);
    TFT_WriteCmd(0xE0);
    TFT_WriteData(0x0B); 
    TFT_WriteData(0x11); 
    TFT_WriteData(0x1E); 
    TFT_WriteData(0x30); 
    TFT_WriteData(0x3A); 
    TFT_WriteData(0x43); 
    TFT_WriteData(0x4E); 
    TFT_WriteData(0x56); 
    TFT_WriteData(0x45); 
    TFT_WriteData(0x3F); 
    TFT_WriteData(0x39); 
    TFT_WriteData(0x32); 
    TFT_WriteData(0x2F); 
    TFT_WriteData(0x2A); 
    TFT_WriteData(0x29); 
    TFT_WriteData(0x21); 
    TFT_WriteData(0x0B); 
    TFT_WriteData(0x11); 
    TFT_WriteData(0x1E); 
    TFT_WriteData(0x30); 
    TFT_WriteData(0x3A); 
    TFT_WriteData(0x43); 
    TFT_WriteData(0x4E); 
    TFT_WriteData(0x56); 
    TFT_WriteData(0x45); 
    TFT_WriteData(0x3F); 
    TFT_WriteData(0x39); 
    TFT_WriteData(0x32); 
    TFT_WriteData(0x2F); 
    TFT_WriteData(0x2A); 
    TFT_WriteData(0x29); 
    TFT_WriteData(0x21); 
    TFT_WriteData(0x00); 
    TFT_WriteData(0x01); 

    TFT_WriteCmd(0x21);
 	TFT_WriteCmd(0x3a);
 	TFT_WriteData(0x05);

    TFT_WriteCmd(0x29);

}

/****************************************************************************
* Function Name  : TFT_SetWindow
* Description    : 设置要操作的窗口范围
* Input          : xStart：窗口起始X坐标
*                * yStart：窗口起始Y坐标
*                * xEnd：窗口结束X坐标
*                * yEnd：窗口结束Y坐标
* Output         : None
* Return         : None
****************************************************************************/

void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
 	TFT_WriteCmd(0x2A);

    TFT_WriteData(xStart>>8);
    TFT_WriteData(xStart&0XFF);
    TFT_WriteData(xEnd>>8);
    TFT_WriteData(xEnd&0XFF);

    TFT_WriteCmd(0x2b);
    TFT_WriteData(yStart>>8);
    TFT_WriteData(yStart&0XFF);
    TFT_WriteData(yEnd>>8);
    TFT_WriteData(yEnd&0XFF);
    TFT_WriteCmd(0x2c); 
}

/****************************************************************************
* Function Name  : TFT_ClearScreen
* Description    : 将LCD清屏成相应的颜色
* Input          : color：清屏颜色
* Output         : None
* Return         : None
****************************************************************************/
	  
void TFT_ClearScreen(uint16_t color)
{
 	uint16_t i, j ;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //作用区域
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData(color);
		}
	}
}
