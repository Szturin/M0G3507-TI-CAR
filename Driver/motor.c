#include "motor.h"
#include <iso646.h>

void Motor_On(void)
{
    DL_GPIO_setPins(GPIO_MOTOR_PIN_FSTBY_PORT, GPIO_MOTOR_PIN_FSTBY_PIN);
}

void Motor_Off(void)
{
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FSTBY_PORT, GPIO_MOTOR_PIN_FSTBY_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);
}

void Set_Speed(float duty1,float duty2)
{
    uint32_t compareValue = 0;
    if(duty1 < 0)
    {
        compareValue = 3199 - 3199 * (-duty1/100.0);
        DL_TimerA_setCaptureCompareValue(PWM_MOTOR_INST, compareValue, DL_TIMER_CC_0_INDEX);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
    }
    else if(duty1 > 0)
    {
        compareValue = 3199 - 3199 * (duty1/100.0);
        DL_TimerA_setCaptureCompareValue(PWM_MOTOR_INST, compareValue, DL_TIMER_CC_0_INDEX);
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
    }
    else 
    {
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
    }


    if(duty2 < 0)
    {
        compareValue = 3199 - 3199 * (-duty2/100.0);
        DL_TimerA_setCaptureCompareValue(PWM_MOTOR_INST, compareValue, DL_TIMER_CC_1_INDEX);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);

    }
    else if(duty2 > 0)
    {
        compareValue = 3199 - 3199 * (duty2/100.0);
        DL_TimerA_setCaptureCompareValue(PWM_MOTOR_INST, compareValue, DL_TIMER_CC_1_INDEX);
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);
    }
    else 
    {
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);
    }
}