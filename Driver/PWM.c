
#include "ti_msp_dl_config.h"

uint32_t period = 2000; //重装载值

/*
void Motor_PWM_Start()
{
    DL_TimerG_startCounter(PWM_0_INST);//PWM定时器开始计数
}

void Set_PWM_Duty(float Duty, uint8_t channel)
{
    uint32_t CompareValue;
    CompareValue = period - period*Duty;

    switch(channel)
    {
        case 0:
            DL_TimerA_setCaptureCompareValue(PWM_0_INST,CompareValue,DL_TIMER_CC_0_INDEX);//DL_TIMER_CC_0_INDEX:通道选择
            break;
        case 1:
            DL_TimerA_setCaptureCompareValue(PWM_0_INST,CompareValue,DL_TIMER_CC_1_INDEX);
            break;
    }
}

//PWM频率设置
void Set_PWM_Freq(uint32_t freq,uint8_t channel)
{
    period = PWM_0_INST_CLK_FREQ/freq;
    DL_Timer_setLoadValue(PWM_0_INST, period);//装载值设置
}
*/