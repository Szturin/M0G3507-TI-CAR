#include "motor.h"

/**
  * @brief			电机启动
  * @param		    None
  * @retval 		None
  */
void Motor_On(void)
{
    DL_GPIO_setPins(GPIO_MOTOR_PIN_FSTBY_PORT, GPIO_MOTOR_PIN_FSTBY_PIN);//置位对应端口->引脚电平,STBY
}

/**
  * @brief		    电机关闭
  * @param		    None
  * @retval 		None		
  */
void Motor_Off(void)
{
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FSTBY_PORT, GPIO_MOTOR_PIN_FSTBY_PIN);//清除对应端口->引脚电平，STBY
    //逻辑口电平清除
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
    DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);
}

/**
  * @brief			速度设置
  * @param		    左右轮占空比（百分值）
  * @retval 	    None
  */
void Set_Speed(float duty1,float duty2)
{
    uint32_t compareValue = 0;//比较值最大为3199，最小值为0
    if(duty1 < 0)
    {
        //PWM定时器向下计数，比较值计算
        compareValue = 3199 - 3199 * (-duty1/100.0);
        //比较值设置
        DL_TimerA_setCaptureCompareValue(PWM_MOTOR_INST, compareValue, DL_TIMER_CC_0_INDEX); 
        //电机逻辑引脚设置
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FL2_PORT, GPIO_MOTOR_PIN_FL2_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FL1_PORT, GPIO_MOTOR_PIN_FL1_PIN);
       
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
        DL_GPIO_setPins(GPIO_MOTOR_PIN_FR2_PORT, GPIO_MOTOR_PIN_FR2_PIN);
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_FR1_PORT, GPIO_MOTOR_PIN_FR1_PIN);
        

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