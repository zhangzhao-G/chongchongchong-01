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

//Current_Pos		AGV_Pos;							//AGV当前的位置状态
//AGV_Back 			AGV_current_state;		//AGV返回的信息
CAN_Msg Gear_Msg = {Auto_GearCmd_ID,{0,0,0,0,0,0,0,0}};		//定义发送数据结构
CAN_Msg Dir_Msg = {Auto_SteeringCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Vel_Msg = {Auto_DriveCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Brak_Msg = {Auto_BrakingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Park_Msg = {Auto_ParkingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Odom_Msg = {Auto_OdometerCm_ID,{0,0,0,0,0,0,0,0}};


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart1_init(115200);	//串口1	与电脑连接，调试使用
	uart2_init(115200);	//串口2	与工控机连接，接收指令
	TIM3_Int_Init(500-1,8400-1);		//定时器3，用于给AGV发送消息，20HZ
	TIM6_Int_Init(10000-1,8400-1);		//定时器6，用于判断接收数据超时，1s
	LCD_Init();					//LCD初始化  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,0);//CAN初始化环回模式,波特率500Kbps    
	POINT_COLOR=RED;//设置字体为红�
	
	Set_Gear(1,0);							//初始化档位为驻车档
	Set_Direction(1,2048);			//初始化角度为0
	Set_Steering_Speed(1,0);		//初始化转角速度为
	Set_Vel(1,0);								//初始化速度为0
	Set_Brak(1,0);						//初始化制动踏板开度为100
	Set_Park(1,0);							//初始化驻车为1-驻车请求
	Set_Odom(1,1,0);						//初始化里程计设置当前坐标为0点
	
	while(1)
	{
		
	}
}


