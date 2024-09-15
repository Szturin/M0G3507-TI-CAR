#ifndef PID_H
#define PID_H

#include "bsp_system.h"
#include "math.h"

/*PID结构体类型定义*/
typedef struct 
{
	float Kp;
	float Ki;
	float Kd;
	float error;
	float last_error;
	float error_sum;
	float error_difference;
	float velocity_sum;
}PID;

extern PID Test;

extern uint8_t Trace_Byte;
extern uint8_t Angle_PID_Flag;
extern uint8_t Test_pid_flag;
extern float Speed_midset;//预设直线速度

//float Turn_cv_PID(int measure, int caclu);
float Turn_hd_PID();
//float Turn_hd_PID_Seven();
//float Velocity_PID_L(float velocity,float velocity_calcu);
//float Velocity_PID_R(float velocity,float velocity_calcu);
void PID_Init();
void Get_TraceData();
float Turn_imu_PID(int yaw, int caclu_yaw);
char Huidu_Counter();

#endif

