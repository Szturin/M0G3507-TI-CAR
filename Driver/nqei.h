#ifndef __NQEI_H
#define __NQEI_H

#define quadrature_decoder_enable 1//正交解码使能
#include "bsp_system.h"

void QEI0_IRQHandler(void);
void QEI1_IRQHandler(void);

float get_left_motor_speed(void);
float get_right_motor_speed(void);
void GROUP1_IRQHandler(void);
void BP_Encoder_Init();
void motor_total_cnt_reset();
extern encoder NEncoder;

#endif









