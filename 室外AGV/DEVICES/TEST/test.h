#ifndef TEST_H
#define TEST_H
#include "stm32f4xx.h"
#include "main.h"


void Set_Gear(int ,int);							//设定目标档位
void Set_Direction(int ,int);					//设定目标方向
void Set_Steering_Speed(int ,int );		//设定角速度
void Set_Vel(int ,int);								//设定目标速度
void Set_Brak(int ,int);							//设定目标制动
void Set_Park(int ,int);							//设定驻车
void Set_Odom(int,int,int);						//设定里程计

void Send_Msg(CAN_Msg*);							//发送消息


int Driving_Cycle(float Vel,int Rad);	//走圆弧
//void usart_test(void);							//串口接收测试
#endif



