#include "can.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h" 
#include "test.h"
#include "main.h"
#include "usart2.h"
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//����,��ʼ��ʧ��; 

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
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			//ʹ��PORTAʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); 		//GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); 		//GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;					//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;					//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;					//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;					//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;					//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;					//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 					//ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;							//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; 						//Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;							//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp; 				//��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);  				// ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  												//������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 					//32λ 
		/*
		//��ʹ�ñ�׼ID
  	CAN_FilterInitStructure.CAN_FilterIdHigh=std_id_01<<5;									//32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000|CAN_ID_STD;							//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01>>16)&0xffff;			//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01&0xffff;
		*/
		/*
		//��ʹ����չID
		CAN_FilterInitStructure.CAN_FilterIdHigh=((ext_id_01<<3)>>16)&0xffff;							//32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=((u16)ext_id_01<<3)|CAN_ID_EXT;						//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01 >> 16) & 0xffff;	//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01 & 0xffff;
		*/
		CAN_FilterInitStructure.CAN_FilterIdHigh=((ext_id_01<<3)>>16)&0xffff;							//32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=((u16)ext_id_01<<3)|CAN_ID_EXT;						//0 | CAN_ID_STD
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask_01 >> 16) & 0xffff;						//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask_01 & 0xffff;		
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;			//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 										//���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);																//�˲�����ʼ��
		
#if 1
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);												//FIFO0��Ϣ�Һ��ж�����.		    
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
} 


/*
	* brief          : ����CAN��Ϣ
	* author         : zhangzhao
	* Function Name  : CAN_TX_Message
	* Description    : 
	* Input          : StdId ExtId Data
	* Output         : 0--���ͳɹ� 1--����ʧ��
	* Return         : None
	* Attention		   : None
*/
u8 CAN_TX_Message(u32 StdId,u32 ExtId,u8 Data1,u8 Data2,u8 Data3,u8 Data4,u8 Data5,u8 Data6,u8 Data7,u8 Data8)
{
	u8 mbox = 0;
	u16 i=0;
	CanTxMsg TX_Message;
  TX_Message.StdId=StdId;	 						// ��׼��ʶ��Ϊ0
  TX_Message.ExtId=ExtId;	 						// ������չ��ʾ����29λ��
  TX_Message.IDE=CAN_ID_EXT;					// ʹ����չ��ʶ��
  TX_Message.RTR=CAN_RTR_Data;		  	// ��Ϣ����Ϊ����֡��һ֡8λ
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
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	//printf("����\r\n");
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
	* brief          : ����CAN��Ϣ
	* author         : zhangzhao
	* Function Name  : CAN_Send_Message
	* Description    : 
	* Input          : StdId ExtId msg len
	* Output         : 0--���ͳɹ� 1--����ʧ��
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

//u8 USART_SED_BUF[10];					//���ڷ��ظ����ػ����ݣ�����ͷ1�ֽ�+��ǰ��λ1�ֽ�+��ǰ������ת��2���ֽ�+��ǰ�ٶ�1���ֽ�+��ǰ�ƶ���λ��1���ֽ�+��̼�X
//u8 USART_SED_BUF[10];					//���ڷ��ظ����ػ����ݣ�����ͷ1�ֽ�+��ǰ��λ1�ֽ�+��ǰ������ת��2���ֽ�+��ǰ�ٶ�1���ֽ�+��ǰ�ƶ���λ��1���ֽ�+��̼�X 3�ֽ�+��̼�Y 3�ֽ�
u8 USART_Back_Gear[4] = {0x41,' ',0x0d,0x0a};				//���ڷ��ص�λ��Ϣ ����ͷ0x41+��λ1�ֽ�+����β2�ֽ�
u8 USART_Back_Angle[6] = {0x42,' ',' ',' ',0x0d,0x0a};		//���ڷ��ط���ת����Ϣ������ͷ0x42+ת��2�ֽ�+����β2�ֽ�
u8 USART_Back_Vel[4] = {0x43,' ',0x0d,0x0a};					//���ڷ����ٶ���Ϣ������ͷ0x43+�ٶ�1�ֽ�+����β2�ֽ�
u8 USART_Back_Brak[4] = {0x44,' ',0x0d,0x0a};				//���ڷ����ƶ���λ����Ϣ������ͷox44���ƶ���λ��1�ֽ�+����β2�ֽ�
u8 USART_Back_Park[4] = {0x45,' ',0x0d,0x0a};				//���ڷ���פ��״̬��Ϣ������ͷ0x45��פ��פ��״̬1�ֽڣ�+����β2�ֽ�
#if CAN1_RX0_INT_ENABLE
/*
	* brief          : CAN1_RX0�жϷ�����
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
		if(Flag_of_receive_time==0)					//��ʼ�����սṹ��
	{
		Flag_of_receive_time++;
		RX_Message.StdId   = 0x00       ;   //���ĵı�׼��ʶ��
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
	
	CAN_Receive(CAN1, CAN_FIFO0, &RX_Message);		//����CAN_FIFO0������
	
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









