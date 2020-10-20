#include "time.h"
#include "test.h"
#include "lcd.h"
#include "delay.h"
#include "can.h"
#include "usart.h"
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!

extern CAN_Msg Gear_Msg;
extern CAN_Msg Dir_Msg;
extern CAN_Msg Vel_Msg;
extern CAN_Msg Brak_Msg;
extern CAN_Msg Park_Msg;
extern CAN_Msg Odom_Msg;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 	 				//使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 									//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 					 				//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);						//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
}


void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		Send_Msg(&Gear_Msg);
		delay_ms(1);
		Send_Msg(&Dir_Msg);
		delay_ms(1);
		Send_Msg(&Vel_Msg);
		delay_ms(1);
		Send_Msg(&Brak_Msg);
		delay_ms(1);
		Send_Msg(&Park_Msg);
		delay_ms(1);
		
	//Send_Msg(&Odom_Msg);
	//printf("TIM3\r\n");

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	TIM_SetCounter(TIM3, 0);
}



void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); 	 				//使能TIM6时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 									//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 					 				//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);						//初始化TIM6
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM6,ENABLE); //使能定时器6

}

int n = 0;

void TIM6_DAC_IRQHandler(void)									//如果在规定时间内没有收到工控机发送的指令，则停车
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断
	{
		Set_Gear(enable,1);						//驻车档
		Set_Direction(enable,2048);		//角度为0
		Set_Vel(enable,0);						//速度为0
		Set_Brak(enable,0);						//制动踏板开度100
		Set_Park(enable,0);						//驻车请求0
		LCD_ShowxNum(410,110,n,5,24,0);
		n++;
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位
}
