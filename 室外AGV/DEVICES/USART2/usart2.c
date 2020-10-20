#include "usart2.h"	
#include "main.h"
#include "lcd.h"
#include "test.h"

#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart2_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 

	//USART_ClearFlag(USART1, USART_FLAG_TC);

	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x01;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

}
//���ڽ����жϺ��������յ���������'$'��ͷ��0x0d��0x0a��β
//���յ��������ʽΪ����ʼ��־λ��1�ֽڣ� +  ����4 + ������־Ϊ��2�ֽڣ�
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		//USART_SendData(USART2,Res);
		//while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else 												//��������� 
				{
					USART2_RX_STA|=0x8000;
					usart2_test();
				}
				}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
} 

void UART2_Send_Msg(unsigned char *s,int len)
{
	int i = 0;
	for(i = 0;i<len;i++)
	{
		USART_SendData(USART2,s[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		i++;
	}
}

extern CAN_Msg Gear_Msg;
extern CAN_Msg Dir_Msg;
extern CAN_Msg Vel_Msg;
extern CAN_Msg Brak_Msg;
extern CAN_Msg Park_Msg;
extern CAN_Msg Odom_Msg;

void usart2_test()
{
//	printf("%d",USART2_RX_BUF[0]);
//	printf("%d",USART2_RX_BUF[1]);
//	printf("%d",USART2_RX_BUF[2]);
//	printf("%d",USART2_RX_BUF[3]);
//	printf("%d",USART2_RX_BUF[4]);
//	printf("%d",USART2_RX_BUF[5]);
//	printf("%d",USART2_RX_BUF[6]);
//	printf("%d",USART2_RX_BUF[7]);
//	printf("%d",USART2_RX_BUF[8]);
//	printf("%d",USART2_RX_BUF[9]);
//	printf("%d",USART2_RX_BUF[10]);
//	printf("%d",USART2_RX_BUF[11]);
//	printf("%d",USART2_RX_BUF[12]);
//	printf("%d\r\n",USART2_RX_BUF[13]);
//	printf("%d\r\n",USART2_RX_STA&0X3FFF);
	if((USART2_RX_STA&0X3FFF) == 11)
	{
		printf("B");
		Set_Gear(enable,USART2_RX_BUF[2]);
		Set_Direction(enable,(USART2_RX_BUF[8]<<8)+USART2_RX_BUF[7]);
		Set_Steering_Speed(enable,(USART2_RX_BUF[10]<<8)+USART2_RX_BUF[9]);
		Set_Vel(enable,USART2_RX_BUF[3]);
		Set_Brak(enable,USART2_RX_BUF[4]);
		Set_Park(enable,USART2_RX_BUF[5]);
		Set_Odom(enable,USART2_RX_BUF[6],USART2_RX_BUF[6]);
		TIM_SetCounter(TIM6, 0);
	}
				USART2_RX_STA=0;
}
#endif	

 





