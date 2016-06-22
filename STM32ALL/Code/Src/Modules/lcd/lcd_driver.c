#include "lcd_driver.h"

/****************************************************************************
* Function Name  : TFT_GPIO_Config
* Description    : ��ʼ��TFT������IO��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_GPIO_Config(void)
{	
	/* ��ʱ��ʹ�� */
    RCC->APB2ENR |= 0x00000020 | 0x00000040 | 0x00000100;

    /* ����IOΪ����������� */
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
* Description    : ��ʼ��FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_FSMC_Config(void)
{
    /* ʹ��FSMC��ʱ�� */
    RCC->AHBENR |= 1 << 8;     	 	//ʹ��FSMCʱ��
    
	/* bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����*/
	/* ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]�� */				    
	FSMC_Bank1->BTCR[6] = 0x00000000;
	FSMC_Bank1->BTCR[7] = 0x00000000;
	FSMC_Bank1E->BWTR[6] = 0x00000000;
	/* ����BCR�Ĵ���	ʹ���첽ģʽ */
	FSMC_Bank1->BTCR[6] |= 1 << 12;		//�洢��дʹ��
	FSMC_Bank1->BTCR[6] |= 1 << 14;		//��дʹ�ò�ͬ��ʱ��
	FSMC_Bank1->BTCR[6] |= 1 << 4; 		//�洢�����ݿ��Ϊ16bit 	    
	/* ����BTR�Ĵ��� */	
	/* ��ʱ����ƼĴ��� */							    
	FSMC_Bank1->BTCR[7] |= 0 << 28;		//ģʽA 	 							  	 
	FSMC_Bank1->BTCR[7] |= 1 << 0; 		//��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns	 	 
	/* ��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�졣 */
	FSMC_Bank1->BTCR[7] |= 0x0F << 8;  	//���ݱ���ʱ��Ϊ16��HCLK	 	 
	/* дʱ����ƼĴ��� */ 
	FSMC_Bank1E->BWTR[6] |= 0 << 28; 	//ģʽA 	 							    
	FSMC_Bank1E->BWTR[6] |= 0 << 0;		//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 
 	/* 4��HCLK��HCLK=72M����ΪҺ������IC��д�ź���������Ҳ��50ns��72M/4=24M=55ns */  	 
	FSMC_Bank1E->BWTR[6] |= 3 << 8; 	//���ݱ���ʱ��Ϊ4��HCLK	
	/* ʹ��BANK1,����4 */
	FSMC_Bank1->BTCR[6] |= 1 << 0;		//ʹ��BANK1������4	
}

/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCDд������
* Input          : cmd��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	TFT->TFT_CMD = cmd;
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCDд������
* Input          : dat��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(uint16_t dat)
{
	TFT->TFT_DATA = dat;
}

/****************************************************************************
* Function Name  : TFT_Init
* Description    : ��ʼ��LCD��
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
* Description    : ����Ҫ�����Ĵ��ڷ�Χ
* Input          : xStart��������ʼX����
*                * yStart��������ʼY����
*                * xEnd�����ڽ���X����
*                * yEnd�����ڽ���Y����
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
* Description    : ��LCD��������Ӧ����ɫ
* Input          : color��������ɫ
* Output         : None
* Return         : None
****************************************************************************/
	  
void TFT_ClearScreen(uint16_t color)
{
 	uint16_t i, j ;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //��������
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData(color);
		}
	}
}
