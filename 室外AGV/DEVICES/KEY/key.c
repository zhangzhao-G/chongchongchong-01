#include "key.h"
#include "delay.h"

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;						 //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;																//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;													//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;																//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);																			//初始化GPIOE2,3,4
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;																		//WK_UP对应引脚PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;															//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);																			//初始化GPIOA0
}
//char GPS_TEST[] = {'$','G','P','R','M','C',',','1','4','4','3','2','6','.','0','0',',','A',',','5','1','0','7','.','0','0','1','7','7','3','7',',','N',',','1','1','4','0','2','.','3','2','9','1','6','1','1',',','W',',','0','.','0','8','0',',','3','2','3','.','3',',','2','1','0','3','0','7',',','0',',','0','.','0',',','E',',','\t','\n'};

//按键扫描函数
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;														//按键按松开标志
	if(mode)key_up=1;  														//支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);																//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;																			// 无按键按下
}
