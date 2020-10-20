#ifndef MAIN_H
#define MAIN_H
#include "sys.h"
#include "stm32f4xx.h"

#define	Auto_GearCmd_ID				0x18C4D1D0		//��λ����
#define	Auto_SteeringCmd_ID		0x18C4D2D0		//ת�����
#define	Auto_DriveCmd_ID			0x18C4D3D0		//���ٿ���
#define	Auto_BrakingCmd_ID		0x18C4D4D0		//�ƶ�����
#define	Auto_ParkingCmd_ID		0x18C4D5D0		//פ������
#define	Auto_OdometerCm_ID		0x18C4D6D0		//��̼ƿ���

#define	Auto_GearFeedBack_ID						0x18C4D1EF		//��ǰ��λ
#define	Auto_SteeringFeedBack_ID				0x18C4D2EF		//��ǰת��
#define	Auto_DriveFeedBack_ID						0x18C4D3EF		//��ǰ����
#define	Auto_BrakingFeedBack_ID					0x18C4D4EF		//��ǰ�ƶ�
#define	Auto_PakingFeedBack_ID					0x18C4D5EF		//��ǰפ��
#define	Auto_OdometerFeedback_ID				0x18C4D6EF		//��̼�
#define	Auto_WheelSpdFeedback_ID				0x18C4D7EF		//��ǰ����
#define	Auto_WheelPulseFeedback_ID			0x18C4D8EF		//����������
#define	Auto_MileageAndBodyFeedback_ID	0x18C4D9EF		//�ۼ���̼�

#define	disable				0						//ʹ��
#define	enable				1						//ʧ��
#define	Park					01					//פ��
#define	Reverse				02					//����
#define	Neutral				03					//�յ�
#define	Drive					04					//ǰ��
#define	Angle_Unit		0.043945		//Ŀ��ת��0.043945/bit
#define	Angular_Unit	0.073242		//ת�����ٶ�0.073242/bit
#define	Vel_Unit			0.04				//0.04m/s/bit
#define	Braking_Unit	0.390625		//0.390625%/bit	0 ~ 100%
#define	Release				0						//פ������
#define	Apply					1						//פ������


typedef struct {
	float Current_X;
	float Current_Y;
	float Current_Angle;
	int Unit_X;
	int Unit_Y;
} Current_Pos;							//��ǰλ����̬

typedef struct {						//��С����������λС������ԭ��
	double Unit_Dis_left;			//ÿһ�μ��������������·������
	double Unit_Dis_right;		//ÿһ�μ����������Һ���·������
	double Unit_angle;				//ÿһ�μ��������ڽǶ�����
	double Now_Angle;					//ÿһ�μ������ڽ���ʱ�ĽǶ�
	double Last_Angle;				//ÿһ�μ������ڿ�ʼʱ�ĽǶ�
	double Increase_DX ;			//ÿһ�μ��������ڵ�x�᷽�������
	double Increase_DY ;			//ÿһ�μ��������ڵ�y�᷽�������
} Distance;

typedef struct {
	const double  Wheel_Dia;					//ֱ������λmm
	const double  Wheel_Dis ;					//�־࣬��λmm
	const double	Axis_Dis;						//��࣬��λmm
	const double	Car_Width;					//������λmm
	const double	Car_Length;					//��������λmm
	const double	Pulse_count;					//һȦ��������
	const double 	Unit_Time;					//��������,��λs
	double Front_alpha;								//С���ĳ�ͷ��̬�� ��λ������
	double Tra_alpha;									//С���Ĺ켣��̬�� ��λ������
	double MAX_Vel ;									//AGV���е�����ٶ� ��λ��mm
}AGV_Model;

typedef struct	
{
	int Left	;							//���� 50ms���ϴ���������Ĳ�ֵ
	int Right	;							//���� 50ms���ϴ���������Ĳ�ֵ
}Wheel_Pulse;							//��������

typedef struct	
{
	int X	;					
	int Y	;					
}Aim_Ponit;								//Ŀ�������

typedef struct
{
	u32 ID;				
	u8 Data[8]; 
}CAN_Msg;									//CAN������֡��Ϣ

typedef struct
{
	char	Gear_back;				//��ǰ��λ����01--פ��02--����03--�յ�04--ǰ��
	char	Gear_Fault;				//�����������00--�޹���02--һ������02--��������03--��������04-�ļ�����05--�弶����
	int		Angle_back;				//��ǰת��Ƕȷ���0.043945/bit	-90-90
	char	Angle_Fault;			//ת��ϵͳ����00--�޹���01--һ������02--��������04--�ļ�����05--�弶����
	int		Vel_back;					//��ǰ�ٶȷ���0.04/bit
	char	Vel_Fault;				//����ϵͳ����00--�����01--һ������02--��������03--��������04--�ļ�����05--�弶����
	int 	Brak_back;				//��ǰ�ƶ�̤�巵��0.390625/bit 0-100
	char	Brak_Fault;				//�ƶ�ϵͳ����00--�޹���01--һ������02--��������03--��������04--�ļ�����05--�弶����
	char 	Park_back;				//��ǰפ��״̬����00--δ֪01--���ƶ�02--���ͷ�03--��ȫ�ͷ�04--�����ƶ�05--�����ͷ�06--Ԥ��
	char	Park_Fault;				//פ��ϵͳ����00--�����01--һ������02--��������03--��������04--�ļ�����05--�弶����
	int		Wheel_speed_R;		//���ַ����ٶ�0.04/bit
	int		Wheel_speed_L;		//���ַ����ٶ�0.04/bit
	int		Odom_add_back;		//�����ۼ���̼Ʒ���0.1km/bit
	char	Knock_back;				//������ײ��Ϣ
}AGV_Back;								//AGV������Ϣ


#endif
