#ifndef  __CAN_H
#define  __CAN_H
#include "sys.h"
#define CAN1_RX0_INT_ENABLE	1								//0,不使能;1,使能.


u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN1_Receive_Msg(u8 *buf);								//接收数据

u8 CAN_TX_Message(u32 StdId,u32 ExtId,u8 Data1,u8 Data2,u8 Data3,u8 Data4,u8 Data5,u8 Data6,u8 Data7,u8 Data8);

u8 CAN_Send_Message(u8* msg,u8 len,u8 StdId,u8 ExtId);

#endif
