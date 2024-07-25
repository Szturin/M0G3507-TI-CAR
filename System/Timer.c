#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "string.h"
#include "main.h"

void TIMER_1_Init()
{
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);//定时器A0中断使能
    DL_TimerA_startCounter(TIMER_1_INST);//定时器A0开始计数，PS:标注定时器类型
}

