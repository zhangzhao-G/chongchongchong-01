#include "time.h"
#include "test.h"
#include "lcd.h"
#include "delay.h"
#include "can.h"
#include "usart.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!

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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 	 				//ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 									//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 					 				//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);						//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
}


void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
	TIM_SetCounter(TIM3, 0);
}



void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); 	 				//ʹ��TIM6ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 									//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 					 				//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);						//��ʼ��TIM6
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��6

}

int n = 0;

void TIM6_DAC_IRQHandler(void)									//����ڹ涨ʱ����û���յ����ػ����͵�ָ���ͣ��
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //����ж�
	{
		Set_Gear(enable,1);						//פ����
		Set_Direction(enable,2048);		//�Ƕ�Ϊ0
		Set_Vel(enable,0);						//�ٶ�Ϊ0
		Set_Brak(enable,0);						//�ƶ�̤�忪��100
		Set_Park(enable,0);						//פ������0
		LCD_ShowxNum(410,110,n,5,24,0);
		n++;
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //����жϱ�־λ
}
