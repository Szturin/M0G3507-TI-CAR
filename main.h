#include "bsp_system.h"
#include <stdio.h>
#include <stdlib.h>

#define LX_2_IMU_ANGEL 185//要求2，直线C->D姿态
#define LX_3_IMU_ANGEL_1 38//要求3，4，A->C发车姿态
#define LX_3_IMU_ANGEL_2 57.5//要求3，B->D姿态

#define LX_4_IMU_ANGEL_1 63.5//要求4,A->C行驶姿态，PS：存在惯性不同于发车姿态
#define Speed_WD 40 //弯道速度
#define Speed_ZX 60 //直线速度

//PS:PWM可调范围，0~100以内任意浮点数
extern float MT_l;//左轮速度初值
extern float MT_R;//右轮速度初值
extern float K_ZX;
extern uint8_t RxData;//串口接收寄存器存储变量
extern uint8_t MID_Speed;//基准速度
extern uint32_t Timer_Angel_Sleep;
extern uint8_t Timer_Angel_Sleep_flag;
extern uint8_t beep_flag;//声光提示标志位
extern uint8_t beep_key_flag;
extern uint8_t Motor_flag;
extern float yaw_val;//记录Yaw角
extern float yaw_detect;//预测yaw角
extern uint8_t System_Mode;

extern char hmi_speed[];//测速
extern char hmi_yaw[];//测角度
extern char hmi_trace[];//测循迹
extern char hmi_totol[];//测里程
extern char hmi_imu[];//测记忆角度
extern char data_HMI[];//串口屏数据存储空间
extern float M_Speed_L;//左轮测速
extern float M_Speed_R;//右轮测速
extern uint8_t Key_Val,Key_Down,Key_Up,Key_Old;
extern uint8_t Key_Timer;