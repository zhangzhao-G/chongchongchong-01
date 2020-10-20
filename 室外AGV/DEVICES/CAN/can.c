#include "can.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h" 
#include "test.h"
#include "main.h"
#include "usart2.h"
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//其他,初始化失败; 

u16 std_id_01 = 0x0000;
u32 ext_id_01 = Auto_SteeringFeedBack_ID;//Auto_WheelSpdFeedback_ID | Auto_OdometerFeedback_ID | Auto_DriveFeedBack_ID|Auto_SteeringCmd_ID|Auto_SteeringFeedBack_ID|Auto_DriveFeedBack_ID|Auto_BrakingFeedBack_ID|Auto_PakingFeedBack_ID|Auto_OdometerFeedback_ID|Auto_WheelSpdFeedback_ID|Auto_WheelPulseFeedback_ID|Auto_MileageAndBodyFeedback_ID;
//u32 mask_01 	= 0x00000000;
u32 mask_01 	= 0x11111111;

u16 std_id_02 = 0x0000;
u32 ext_id_02 = 0x000f;

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			//使能PORTA时钟	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); 		//GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); 		//GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;					//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;					//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;					//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;					//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;					//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;					//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 					//模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;							//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; 						//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;							//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp; 				//分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);  				// 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  												//过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 					//32位 
		/*
		//若使用标准ID
  	CAN_FilterInitStructure.CAN_FilterIdHigh=std_id_01<<5;									//32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000|CAN_ID_STD;							//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01>>16)&0xffff;			//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01&0xffff;
		*/
		/*
		//若使用拓展ID
		CAN_FilterInitStructure.CAN_FilterIdHigh=((ext_id_01<<3)>>16)&0xffff;							//32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=((u16)ext_id_01<<3)|CAN_ID_EXT;						//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01 >> 16) & 0xffff;	//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01 & 0xffff;
		*/
		CAN_FilterInitStructure.CAN_FilterIdHigh=((ext_id_01<<3)>>16)&0xffff;							//32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=((u16)ext_id_01<<3)|CAN_ID_EXT;						//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01 >> 16) & 0xffff;						//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01 & 0xffff;		
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;			//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 										//激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);																//滤波器初始化
		
#if 1
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);												//FIFO0消息挂号中断允许.		    
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
} 


/*
	* brief          : 发送CAN信息
	* author         : zhangzhao
	* Function Name  : CAN_TX_Message
	* Description    : 
	* Input          : StdId ExtId Data
	* Output         : 0--发送成功 1--发送失败
	* Return         : None
	* Attention		   : None
*/
u8 CAN_TX_Message(u32 StdId,u32 ExtId,u8 Data1,u8 Data2,u8 Data3,u8 Data4,u8 Data5,u8 Data6,u8 Data7,u8 Data8)
{
	u8 mbox = 0;
	u16 i=0;
	CanTxMsg TX_Message;
  TX_Message.StdId=StdId;	 						// 标准标识符为0
  TX_Message.ExtId=ExtId;	 						// 设置扩展标示符（29位）
  TX_Message.IDE=CAN_ID_EXT;					// 使用扩展标识符
  TX_Message.RTR=CAN_RTR_Data;		  	// 消息类型为数据帧，一帧8位
  TX_Message.DLC=8;
	TX_Message.Data[0] = Data1 ;
	TX_Message.Data[1] = Data2 ;
	TX_Message.Data[2] = Data3 ;
	TX_Message.Data[3] = Data4 ;
	TX_Message.Data[4] = Data5 ;
	TX_Message.Data[5] = Data6 ;
	TX_Message.Data[6] = Data7 ;
	TX_Message.Data[7] = Data8 ;
	mbox = CAN_Transmit(CAN1, &TX_Message);
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	//printf("发送\r\n");
	//delay_ms(1);
	//	printf("%d\r\n",TX_Message.ExtId);
//	printf("%d\r\n",TX_Message.Data[0]);
//	printf("%d\r\n",TX_Message.Data[1]);
//	printf("%d\r\n",TX_Message.Data[2]);
//	printf("%d\r\n",TX_Message.Data[3]);
//	printf("%d\r\n",TX_Message.Data[4]);
//	printf("%d\r\n",TX_Message.Data[5]);
//	printf("%d\r\n",TX_Message.Data[6]);
//	printf("%d\r\n",TX_Message.Data[7]);
//	if(i>=0XFFF)return 1;
  return 0;
}

/*
	* brief          : 发送CAN信息
	* author         : zhangzhao
	* Function Name  : CAN_Send_Message
	* Description    : 
	* Input          : StdId ExtId msg len
	* Output         : 0--发送成功 1--发送失败
	* Return         : None
	* Attention		   : None
*/
u8 CAN_Send_Message(u8* msg,u8 len,u8 StdId,u8 ExtId)
{
	return CAN_TX_Message(StdId,ExtId,msg[0],msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]);
	}


extern Current_Pos AGV_Pos;
extern AGV_Model	MY_AGV;
extern Distance	MY_Distance;
extern uint32_t Flag_of_receive_time;
extern uint8_t Init;	
extern Wheel_Pulse pulse;

extern float angle;

//u8 USART_SED_BUF[10];					//串口返回给工控机数据：数据头1字节+当前档位1字节+当前方向盘转角2个字节+当前速度1个字节+当前制动板位置1个字节+里程计X
//u8 USART_SED_BUF[10];					//串口返回给工控机数据：数据头1字节+当前档位1字节+当前方向盘转角2个字节+当前速度1个字节+当前制动板位置1个字节+里程计X 3字节+里程计Y 3字节
u8 USART_Back_Gear[4] = {0x41,' ',0x0d,0x0a};				//串口返回档位信息 数据头0x41+档位1字节+数据尾2字节
u8 USART_Back_Angle[6] = {0x42,' ',' ',' ',0x0d,0x0a};		//串口返回方向转角信息，数据头0x42+转角2字节+数据尾2字节
u8 USART_Back_Vel[4] = {0x43,' ',0x0d,0x0a};					//串口返回速度信息，数据头0x43+速度1字节+数据尾2字节
u8 USART_Back_Brak[4] = {0x44,' ',0x0d,0x0a};				//串口返回制动板位置信息，数据头ox44，制动板位置1字节+数据尾2字节
u8 USART_Back_Park[4] = {0x45,' ',0x0d,0x0a};				//串口返回驻车状态信息，数据头0x45，驻车驻车状态1字节，+数据尾2字节
#if CAN1_RX0_INT_ENABLE
/*
	* brief          : CAN1_RX0中断服务函数
	* author         : zhangzhao
	* Function Name  : CAN1_RX0_IRQHandler
	* Description    : 
	* Input          : None
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RX_Message;
		if(Flag_of_receive_time==0)					//初始化接收结构体
	{
		Flag_of_receive_time++;
		RX_Message.StdId   = 0x00       ;   //报文的标准标识符
		RX_Message.IDE     = CAN_ID_EXT ;
		RX_Message.DLC     = 0          ;
		RX_Message.Data[0] = 0x00       ;
		RX_Message.Data[1] = 0x00       ;
		RX_Message.Data[2] = 0x00       ;
		RX_Message.Data[3] = 0x00       ;
		RX_Message.Data[4] = 0x00       ;
		RX_Message.Data[5] = 0x00       ;
		RX_Message.Data[6] = 0x00       ;
		RX_Message.Data[7] = 0x00       ;
	}
	
	CAN_Receive(CAN1, CAN_FIFO0, &RX_Message);		//接收CAN_FIFO0的数据
	
	switch(RX_Message.ExtId)
	{
		case Auto_SteeringFeedBack_ID:
			USART_Back_Angle[1] = RX_Message.Data[1];
			USART_Back_Angle[2] = RX_Message.Data[2]&0x0f;
			USART_Back_Angle[3] = USART_Back_Angle[0] + USART_Back_Angle[1] + USART_Back_Angle[2];
			//UART2_Send_Msg(USART_Back_Angle,5);
		USART_SendData(USART2,USART_Back_Angle[0]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2,USART_Back_Angle[1]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2,USART_Back_Angle[2]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2,USART_Back_Angle[3]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2,USART_Back_Angle[4]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2,USART_Back_Angle[5]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		break;
//		if(USART_Back_LR[1] != RX_Message.Data[1])
//		{
//			
//		}
//	printf("RX_Message.ExtId:%d\r\n",RX_Message.ExtId);
//	printf("Data[0]:%d\r\n",RX_Message.Data[0]);
//	printf("Data[1]:%d\r\n",RX_Message.Data[1]);
//	printf("Data[2]:%d\r\n",RX_Message.Data[2]);
//	printf("Data[3]:%d\r\n",RX_Message.Data[3]);
//	printf("Data[4]:%d\r\n",RX_Message.Data[4]);
//	printf("Data[5]:%d\r\n",RX_Message.Data[5]);
//	printf("Data[6]:%d\r\n",RX_Message.Data[6]);
//	printf("Data[7]:%d\r\n",RX_Message.Data[7]);
//	printf("left:%d\triglt:%d\r\n",USART_Back_LR[1],USART_Back_LR[2]);
	}
		return;	
	}	

#endif









