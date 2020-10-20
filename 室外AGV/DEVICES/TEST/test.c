#include "test.h"
#include "can.h"
#include "math.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "lcd.h"

int CMD;								//��ͣ
Aim_Ponit Aim_test;			//����Ŀ�������
AGV_Model	MY_AGV={0,0,0,0,0,0,0,0,0,0};	//�趨AGVģ�Ͳ���
Distance	MY_Distance={0.0,0.0,0.0,0.0,0.0,0.0,0.0};	//AGV��λʱ�����˶�����
uint32_t Flag_of_receive_time = 0;		//����CAN����ʱ��ʼ��
uint8_t Init=0;							//����can����ʱ�ĳ�ʼ��
Wheel_Pulse pulse = {0,0};	//���ص�������


extern CAN_Msg Gear_Msg;
extern CAN_Msg Dir_Msg;
extern CAN_Msg Vel_Msg;
extern CAN_Msg Brak_Msg;
extern CAN_Msg Park_Msg;
extern CAN_Msg Odom_Msg;

extern Current_Pos	AGV_Pos;							//AGV��ǰ��λ��״̬
extern AGV_Back 		AGV_current_state;		//AGV���ص���Ϣ

/*
	* brief          : �趨Ŀ�굵λgear
	* author         : zhangzhao
	* Function Name  : Set_Gear
	* Description    : gear--01=פ�� 02=���� 03 =�յ� 04=ǰ��
	* Input          : able gear
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Gear(int able,int gear)				
{
	Gear_Msg.Data[0] = able;
	Gear_Msg.Data[1] = gear;
	}


/*
	* brief          : �趨Ŀ�귽��
	* author         : zhangzhao
	* Function Name  : Set_Direction
	* Description    : direction--Ŀ�귽�򣬵�λ0.043945
	* Input          : able direction steering_speed
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Direction(int able,int direction)
{
	Dir_Msg.Data[0] = able;
	Dir_Msg.Data[1] = direction&0xff;
	Dir_Msg.Data[2] = direction >> 8;
	}

	/*
	* brief          : �趨ת���ٶ�
	* author         : zhangzhao
	* Function Name  : Set_Direction
	* Description    : steering_speed--���ٶȣ���λ0.073242
	* Input          : able direction steering_speed
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Steering_Speed(int able,int steering_speed)
{
	Dir_Msg.Data[0] = able;
	Dir_Msg.Data[3] = steering_speed&0xff;
	Dir_Msg.Data[4] = steering_speed >> 8;
	}

/*
	* brief          : �趨Ŀ���ٶ�
	* author         : zhangzhao
	* Function Name  : Set_Vel
	* Description    : vel--�ٶ�,��λ0.04m/s(velΪ5���ٶ�Ϊ0.2m/s)
	* Input          : able vel
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Vel(int able,int vel)	
{
	Vel_Msg.Data[0] = able;
	Vel_Msg.Data[1] = vel&0xff;
	}


/*
	* brief          : �趨Ŀ���ƶ�
	* author         : zhangzhao
	* Function Name  : Set_Brak
	* Description    : brak--�ƶ�̤�壬��λ0.390625
	* Input          : able brak
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Brak(int able,int brak)
{
	Brak_Msg.Data[0] = able;
	Brak_Msg.Data[1] = brak&0xff;
	}

/*
	* brief          : �趨פ��
	* author         : zhangzhao
	* Function Name  : Set_Park
	* Description    : park--פ��
	* Input          : able brak
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Park(int able ,int park)
{
	Park_Msg.Data[0] = able;
	Park_Msg.Data[1] = park;
	}


/*
	* brief          : �趨��̼�
	* author         : zhangzhao
	* Function Name  : Set_Odom
	* Description    : park--פ��
	* Input          : able
	* Output         : None
	* Return         : None
	* Attention		   : None
*/
void Set_Odom(int able,int set,int clear)
{
	Odom_Msg.Data[0] = able;
	Odom_Msg.Data[1] = set;
	Odom_Msg.Data[2] = clear;
	}

/*
	* brief          : ����CAN��Ϣ
	* author         : zhangzhao
	* Function Name  : Send_Msg
	* Description    : * Msg
	* Input          : able
	* Output         : None
	* Return         : None
	* Attention		   : None
*/

void Send_Msg( CAN_Msg* Msg)
{
	Msg->Data[6] = Msg->Data[6] + 16;
	Msg->Data[7] = Msg->Data[0]^Msg->Data[1]^Msg->Data[2]^Msg->Data[3]^Msg->Data[4]^Msg->Data[5]^Msg->Data[6];
	CAN_TX_Message(0,Msg->ID,Msg->Data[0],Msg->Data[1],Msg->Data[2],Msg->Data[3],Msg->Data[4],Msg->Data[5],Msg->Data[6],Msg->Data[7]);
}










