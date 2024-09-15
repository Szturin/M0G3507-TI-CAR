#ifndef __JY61P_H
#define __JY61P_H

#include "bsp_system.h"

void Serial_JY61P_Zero_Yaw(void);
void UART_JY61P_INST_IRQHandler(void);
extern float Pitch,Roll,Yaw;
#endif
