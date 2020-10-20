#include "test.h"
#include "can.h"
#include "math.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "lcd.h"

int CMD;								//急停
Aim_Ponit Aim_test;			//定义目标点坐标
AGV_Model	MY_AGV={0,0,0,0,0,0,0,0,0,0};	//设定AGV模型参数
Distance	MY_Distance={0.0,0.0,0.0,0.0,0.0,0.0,0.0};	//AGV单位时间内运动参数
uint32_t Flag_of_receive_time = 0;		//用于CAN接收时初始化
uint8_t Init=0;							//用于can接收时的初始化
Wheel_Pulse pulse = {0,0};	//返回的脉冲数


extern CAN_Msg Gear_Msg;
extern CAN_Msg Dir_Msg;
extern CAN_Msg Vel_Msg;
extern CAN_Msg Brak_Msg;
extern CAN_Msg Park_Msg;
extern CAN_Msg Odom_Msg;

extern Current_Pos	AGV_Pos;							//AGV当前的位置状态
extern AGV_Back 		AGV_current_state;		//AGV返回的信息

/*
	* brief          : 设定目标档位gear
	* author         : zhangzhao
	* Function Name  : Set_Gear
	* Description    : gear--01=驻车 02=后退 03 =空档 04=前进
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
	* brief          : 设定目标方向
	* author         : zhangzhao
	* Function Name  : Set_Direction
	* Description    : direction--目标方向，单位0.043945
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
	* brief          : 设定转动速度
	* author         : zhangzhao
	* Function Name  : Set_Direction
	* Description    : steering_speed--角速度，单位0.073242
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
	* brief          : 设定目标速度
	* author         : zhangzhao
	* Function Name  : Set_Vel
	* Description    : vel--速度,单位0.04m/s(vel为5即速度为0.2m/s)
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
	* brief          : 设定目标制动
	* author         : zhangzhao
	* Function Name  : Set_Brak
	* Description    : brak--制动踏板，单位0.390625
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
	* brief          : 设定驻车
	* author         : zhangzhao
	* Function Name  : Set_Park
	* Description    : park--驻车
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
	* brief          : 设定里程计
	* author         : zhangzhao
	* Function Name  : Set_Odom
	* Description    : park--驻车
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
	* brief          : 发送CAN信息
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










