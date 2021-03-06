#include "usart2.h"	
#include "main.h"
#include "lcd.h"
#include "test.h"

#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart2_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART2, ENABLE);  //使能串口1 

	//USART_ClearFlag(USART1, USART_FLAG_TC);

	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x01;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}
//串口接收中断函数，接收到的数据以'$'开头以0x0d和0x0a结尾
//接收到的命令格式为：开始标志位（1字节） +  数据4 + 结束标志为（2字节）
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		//USART_SendData(USART2,Res);
		//while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else 												//接收完成了 
				{
					USART2_RX_STA|=0x8000;
					usart2_test();
				}
				}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
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

 





