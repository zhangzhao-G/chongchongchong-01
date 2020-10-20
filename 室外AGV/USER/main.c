#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "lcd.h"
#include "can.h"
#include "test.h"
#include "main.h"
#include "time.h"

//unsigned char GPS_TEST[] = {'$','G','P','R','M','C',',','1','4','4','3','2','6','.','0','0',',','A',',','5','1','0','7','.','0','0','1','7','7','3','7',',','N',',','1','1','4','0','2','.','3','2','9','1','6','1','1',',','W',',','0','.','0','8','0',',','3','2','3','.','3',',','2','1','0','3','0','7',',','0',',','0','.','0',',','E',',',0x0d,0x0a};

//Current_Pos		AGV_Pos;							//AGV��ǰ��λ��״̬
//AGV_Back 			AGV_current_state;		//AGV���ص���Ϣ
CAN_Msg Gear_Msg = {Auto_GearCmd_ID,{0,0,0,0,0,0,0,0}};		//���巢�����ݽṹ
CAN_Msg Dir_Msg = {Auto_SteeringCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Vel_Msg = {Auto_DriveCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Brak_Msg = {Auto_BrakingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Park_Msg = {Auto_ParkingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Odom_Msg = {Auto_OdometerCm_ID,{0,0,0,0,0,0,0,0}};


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart1_init(115200);	//����1	��������ӣ�����ʹ��
	uart2_init(115200);	//����2	�빤�ػ����ӣ�����ָ��
	TIM3_Int_Init(500-1,8400-1);		//��ʱ��3�����ڸ�AGV������Ϣ��20HZ
	TIM6_Int_Init(10000-1,8400-1);		//��ʱ��6�������жϽ������ݳ�ʱ��1s
	LCD_Init();					//LCD��ʼ��  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,0);//CAN��ʼ������ģʽ,������500Kbps    
	POINT_COLOR=RED;//��������Ϊ���
	
	Set_Gear(1,0);							//��ʼ����λΪפ����
	Set_Direction(1,2048);			//��ʼ���Ƕ�Ϊ0
	Set_Steering_Speed(1,0);		//��ʼ��ת���ٶ�Ϊ
	Set_Vel(1,0);								//��ʼ���ٶ�Ϊ0
	Set_Brak(1,0);						//��ʼ���ƶ�̤�忪��Ϊ100
	Set_Park(1,0);							//��ʼ��פ��Ϊ1-פ������
	Set_Odom(1,1,0);						//��ʼ����̼����õ�ǰ����Ϊ0��
	
	while(1)
	{
		
	}
}


