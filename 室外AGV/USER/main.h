#ifndef MAIN_H
#define MAIN_H
#include "sys.h"
#include "stm32f4xx.h"

#define	Auto_GearCmd_ID				0x18C4D1D0		//档位控制
#define	Auto_SteeringCmd_ID		0x18C4D2D0		//转向控制
#define	Auto_DriveCmd_ID			0x18C4D3D0		//车速控制
#define	Auto_BrakingCmd_ID		0x18C4D4D0		//制动控制
#define	Auto_ParkingCmd_ID		0x18C4D5D0		//驻车控制
#define	Auto_OdometerCm_ID		0x18C4D6D0		//里程计控制

#define	Auto_GearFeedBack_ID						0x18C4D1EF		//当前档位
#define	Auto_SteeringFeedBack_ID				0x18C4D2EF		//当前转角
#define	Auto_DriveFeedBack_ID						0x18C4D3EF		//当前车速
#define	Auto_BrakingFeedBack_ID					0x18C4D4EF		//当前制动
#define	Auto_PakingFeedBack_ID					0x18C4D5EF		//当前驻车
#define	Auto_OdometerFeedback_ID				0x18C4D6EF		//里程计
#define	Auto_WheelSpdFeedback_ID				0x18C4D7EF		//当前轮速
#define	Auto_WheelPulseFeedback_ID			0x18C4D8EF		//左右轮脉冲
#define	Auto_MileageAndBodyFeedback_ID	0x18C4D9EF		//累计里程计

#define	disable				0						//使能
#define	enable				1						//失能
#define	Park					01					//驻车
#define	Reverse				02					//后退
#define	Neutral				03					//空档
#define	Drive					04					//前进
#define	Angle_Unit		0.043945		//目标转向0.043945/bit
#define	Angular_Unit	0.073242		//转动角速度0.073242/bit
#define	Vel_Unit			0.04				//0.04m/s/bit
#define	Braking_Unit	0.390625		//0.390625%/bit	0 ~ 100%
#define	Release				0						//驻车请求
#define	Apply					1						//驻车请求


typedef struct {
	float Current_X;
	float Current_Y;
	float Current_Angle;
	int Unit_X;
	int Unit_Y;
} Current_Pos;							//当前位置姿态

typedef struct {						//以小车后周中心位小车坐标原点
	double Unit_Dis_left;			//每一次计算周期内左后轮路程增量
	double Unit_Dis_right;		//每一次计算周期内右后轮路程增量
	double Unit_angle;				//每一次计算周期内角度增量
	double Now_Angle;					//每一次计算周期结束时的角度
	double Last_Angle;				//每一次计算周期开始时的角度
	double Increase_DX ;			//每一次计算周期内的x轴方向的增量
	double Increase_DY ;			//每一次计算周期内的y轴方向的增量
} Distance;

typedef struct {
	const double  Wheel_Dia;					//直径，单位mm
	const double  Wheel_Dis ;					//轮距，单位mm
	const double	Axis_Dis;						//轴距，单位mm
	const double	Car_Width;					//车宽，单位mm
	const double	Car_Length;					//车长，单位mm
	const double	Pulse_count;					//一圈的脉冲数
	const double 	Unit_Time;					//计算周期,单位s
	double Front_alpha;								//小车的车头姿态角 单位：弧度
	double Tra_alpha;									//小车的轨迹姿态角 单位：弧度
	double MAX_Vel ;									//AGV运行的最大速度 单位：mm
}AGV_Model;

typedef struct	
{
	int Left	;							//右轮 50ms与上次脉冲次数的差值
	int Right	;							//左轮 50ms与上次脉冲次数的差值
}Wheel_Pulse;							//轮子脉冲

typedef struct	
{
	int X	;					
	int Y	;					
}Aim_Ponit;								//目标点坐标

typedef struct
{
	u32 ID;				
	u8 Data[8]; 
}CAN_Msg;									//CAN的数据帧信息

typedef struct
{
	char	Gear_back;				//当前档位返回01--驻车02--后退03--空档04--前进
	char	Gear_Fault;				//变速箱故障信00--无故障02--一级故障02--二级故障03--三级故障04-四级故障05--五级故障
	int		Angle_back;				//当前转向角度返回0.043945/bit	-90-90
	char	Angle_Fault;			//转向系统故障00--无故障01--一级故障02--三级故障04--四级故障05--五级故障
	int		Vel_back;					//当前速度返回0.04/bit
	char	Vel_Fault;				//驱动系统故障00--五故障01--一级故障02--二级故障03--三级故障04--四级故障05--五级故障
	int 	Brak_back;				//当前制动踏板返回0.390625/bit 0-100
	char	Brak_Fault;				//制动系统故障00--无故障01--一级故障02--二级故障03--三级故障04--四级故障05--五级故障
	char 	Park_back;				//当前驻车状态返回00--未知01--已制动02--已释放03--完全释放04--正在制动05--正在释放06--预留
	char	Park_Fault;				//驻车系统故障00--五故障01--一级故障02--二级故障03--三级故障04--四级故障05--五级故障
	int		Wheel_speed_R;		//右轮反馈速度0.04/bit
	int		Wheel_speed_L;		//左轮反馈速度0.04/bit
	int		Odom_add_back;		//车辆累计里程计返回0.1km/bit
	char	Knock_back;				//车辆碰撞信息
}AGV_Back;								//AGV返回信息


#endif
