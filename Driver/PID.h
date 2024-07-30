#include"ti_msp_dl_config.h"
extern uint8_t Trace_Byte;
extern uint8_t Angle_PID_Flag;
extern uint8_t Test_pid_flag;
extern float Speed_midset;//预设直线速度
float Turn_cv_PID(int measure, int caclu);
float Turn_hd_PID();
float Turn_hd_PID_Seven();
float Velocity_PID_L(float velocity,float velocity_calcu);
float Velocity_PID_R(float velocity,float velocity_calcu);
void PID_Init();
void Get_TraceData();
float Turn_imu_PID(int yaw, int caclu_yaw);
char Huidu_Counter();