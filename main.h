#include "ti_msp_dl_config.h"
#include "OLED.h"
#include "Serial.h"
#include "Timer.h"
#include "motor.h"
#include "PID.h"
#include <stdio.h>
#include "nqei.h"
#include "JY61P.h"
#include "Delay.h"
#include "Key.h"

#define LX_2_IMU_ANGEL 185//要求2，直线C->D姿态
#define LX_3_IMU_ANGEL_1 38//要求3，4，A->C发车姿态
#define LX_3_IMU_ANGEL_2 57.5//要求3，B->D姿态

#define LX_4_IMU_ANGEL_1 63.5//要求4,A->C行驶姿态，PS：存在惯性不同于发车姿态
#define Speed_WD 40 //弯道速度
#define Speed_ZX 60 //直线速度

void SYS_RxPro();
void Screen_RxPro();
void SOC_RxPro();
void LX_Proc_1();
void LX_Proc_2();
void LX_Proc_3();
void LX_Proc_4();
void Beep_Proc();

//PS:PWM可调范围，0~100以内任意浮点数
extern float MT_l;//左轮速度初值
extern float MT_R;//右轮速度初值
extern float K_ZX;
extern uint8_t RxData;//串口接收寄存器存储变量
extern uint8_t MID_Speed;//基准速度
