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

//Current_Pos		AGV_Pos;							//AGVµ±Ç°µÄÎ»ÖÃ×´Ì¬
//AGV_Back 			AGV_current_state;		//AGV·µ»ØµÄĞÅÏ¢
CAN_Msg Gear_Msg = {Auto_GearCmd_ID,{0,0,0,0,0,0,0,0}};		//¶¨Òå·¢ËÍÊı¾İ½á¹¹
CAN_Msg Dir_Msg = {Auto_SteeringCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Vel_Msg = {Auto_DriveCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Brak_Msg = {Auto_BrakingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Park_Msg = {Auto_ParkingCmd_ID,{0,0,0,0,0,0,0,0}};
CAN_Msg Odom_Msg = {Auto_OdometerCm_ID,{0,0,0,0,0,0,0,0}};


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖĞ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);    //³õÊ¼»¯ÑÓÊ±º¯Êı
	uart1_init(115200);	//´®¿Ú1	ÓëµçÄÔÁ¬½Ó£¬µ÷ÊÔÊ¹ÓÃ
	uart2_init(115200);	//´®¿Ú2	Óë¹¤¿Ø»úÁ¬½Ó£¬½ÓÊÕÖ¸Áî
	TIM3_Int_Init(500-1,8400-1);		//¶¨Ê±Æ÷3£¬ÓÃÓÚ¸øAGV·¢ËÍÏûÏ¢£¬20HZ
	TIM6_Int_Init(10000-1,8400-1);		//¶¨Ê±Æ÷6£¬ÓÃÓÚÅĞ¶Ï½ÓÊÕÊı¾İ³¬Ê±£¬1s
	LCD_Init();					//LCD³õÊ¼»¯  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,0);//CAN³õÊ¼»¯»·»ØÄ£Ê½,²¨ÌØÂÊ500Kbps    
	POINT_COLOR=RED;//ÉèÖÃ×ÖÌåÎªºìÉ
	
	Set_Gear(1,0);							//³õÊ¼»¯µµÎ»Îª×¤³µµµ
	Set_Direction(1,2048);			//³õÊ¼»¯½Ç¶ÈÎª0
	Set_Steering_Speed(1,0);		//³õÊ¼»¯×ª½ÇËÙ¶ÈÎª
	Set_Vel(1,0);								//³õÊ¼»¯ËÙ¶ÈÎª0
	Set_Brak(1,0);						//³õÊ¼»¯ÖÆ¶¯Ì¤°å¿ª¶ÈÎª100
	Set_Park(1,0);							//³õÊ¼»¯×¤³µÎª1-×¤³µÇëÇó
	Set_Odom(1,1,0);						//³õÊ¼»¯Àï³Ì¼ÆÉèÖÃµ±Ç°×ø±êÎª0µã
	
	while(1)
	{
		
	}
}


