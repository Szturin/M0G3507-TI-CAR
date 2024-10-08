#ifndef MOTOR_H
#define MOTOR_H

#include "bsp_system.h"

//宏定义简化写法
#define AIN1(x)   x?DL_GPIO_setPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN):DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN)
#define AIN2(x)   x?DL_GPIO_setPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN):DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN)
#define BIN1(x)   x?DL_GPIO_setPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN):DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN)
#define BIN2(x)   x?DL_GPIO_setPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN):DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN)

void Motor_On(void);
void Motor_Off(void);
void Set_Speed(float duty1,float duty2);

#endif
