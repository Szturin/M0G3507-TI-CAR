#ifndef __NQEI_H
#define __NQEI_H

#define quadrature_decoder_enable 1//��������ʹ��

void QEI0_IRQHandler(void);
void QEI1_IRQHandler(void);

float get_left_motor_speed(void);
float get_right_motor_speed(void);
void GROUP1_IRQHandler(void);
extern encoder NEncoder;

#endif




