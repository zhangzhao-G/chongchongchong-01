#ifndef TEST_H
#define TEST_H
#include "stm32f4xx.h"
#include "main.h"


void Set_Gear(int ,int);							//�趨Ŀ�굵λ
void Set_Direction(int ,int);					//�趨Ŀ�귽��
void Set_Steering_Speed(int ,int );		//�趨���ٶ�
void Set_Vel(int ,int);								//�趨Ŀ���ٶ�
void Set_Brak(int ,int);							//�趨Ŀ���ƶ�
void Set_Park(int ,int);							//�趨פ��
void Set_Odom(int,int,int);						//�趨��̼�

void Send_Msg(CAN_Msg*);							//������Ϣ


int Driving_Cycle(float Vel,int Rad);	//��Բ��
//void usart_test(void);							//���ڽ��ղ���
#endif



