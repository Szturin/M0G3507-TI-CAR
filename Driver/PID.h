#include"ti_msp_dl_config.h"
extern uint8_t Trace_Byte;
extern uint8_t Angle_PID_Flag;
extern uint8_t Test_pid_flag;
float Turn_cv_PID(int measure, int caclu);
float Turn_hd_PID();
float Velocity_PID(float velocity,float velocity_calcu);
void PID_Init();
void Get_TraceData();