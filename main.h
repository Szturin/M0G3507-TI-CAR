#include "Encoder.h"
#include "ti_msp_dl_config.h"
#include "OLED.h"
#include "bsp_mpu6050.h"
#include "inv_mpu.h"
#include "Serial.h"
#include "Timer.h"
#include "motor.h"
#include "PID.h"
#include "GWKJ.h"
#include <stdio.h>

void SYS_RxPro();
void Screen_RxPro();
void SOC_RxPro();

//PS:PWM可调范围，0~100以内任意浮点数
extern float MT_l;//左轮速度初值
extern float MT_R;//右轮速度初值

extern uint8_t RxData;//串口接收寄存器存储变量
extern uint8_t MID_Speed;
/*MPU6050*/
extern uint8_t ret;
extern float pitch,roll,yaw;