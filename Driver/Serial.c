#include "stdio.h"
#include "string.h"
#include "ti_msp_dl_config.h"
#include "main.h"

uint8_t Serial_RxPacket[100];
/*PID调试*/
float Test_Kp;//比例
float Test_Ki;//积分
float Test_Kd;//微分
float Test_Ks;//目标速度

/*系统调试串口变量*/
uint8_t Serial_RxFlag;
uint8_t RxState = 0;
uint8_t pRxState;//表示当前接收的是第几个变量
uint8_t RxData_type;

/*上位机串口变量*/
uint8_t Soc_RxFlag;
uint8_t RxState_SOC = 0;
uint8_t SOC_RxData_type;
uint8_t SOC_pRxState;//串口接收数据索引号

/*串口屏串口变量*/
uint8_t Screen_RxFlag;
uint8_t RxState_Screen;
uint8_t Screen_RxData_type;
uint8_t Screen_pRxState;//串口接收数据索引号

uint8_t Serial_RxPacket[100];

//系统串口初始化
void SYS_UART0_Init(void)
{
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    DL_UART_clearInterruptStatus(UART_0_INST,DL_UART_INTERRUPT_RX);//清除中断标志位
}

//SOC通信串口初始化
void SOC_UART1_Init(void)
{
	NVIC_ClearPendingIRQ(UART_SOC_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_SOC_INST_INT_IRQN);
    DL_UART_clearInterruptStatus(UART_SOC_INST,DL_UART_INTERRUPT_RX);//清除中断标志位
}

//发送字符串给SOC
void SOC_SendString(char *str)
{
    while(*str != '\0')
    {
        DL_UART_Main_transmitDataBlocking(UART_SOC_INST, *str++);
    }
}

//串口屏通信串口初始化
void Screen_UART2_Init(void)
{
	NVIC_ClearPendingIRQ(UART_Screen_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_Screen_INST_INT_IRQN);
    DL_UART_clearInterruptStatus(UART_Screen_INST,DL_UART_INTERRUPT_RX);//清除中断标志位
}

//发送字符串给串口屏
void Screen_SendString(char *str)
{
    while(*str != '\0')
    {
        DL_UART_Main_transmitDataBlocking(UART_Screen_INST, *str++);
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {//判断中断的类型，DL_UART_Main_getPendingInterrupt(UART_0_INST)调用具有清空标志位的功能？
        case DL_UART_MAIN_IIDX_RX://如果触发串口接收事件
       RxData = DL_UART_Main_receiveData(UART_0_INST);
		if(RxState == 0)//帧头检测
		{
			if(RxData == '#' && Serial_RxFlag == 0)
			{
				RxState=1;
			}
		}
		else if(RxState == 1)//数据类型检测s
		{
			switch(RxData)
			{
				case 'P'://x+
					RxState=2;
					RxData_type=1;
					break;
				case 'I'://x-
					RxState=2;
					RxData_type=2;
					break;
				case 'D'://x-
					RxState=2;
					RxData_type=3;
					break;
				case 'S':
					RxState=2;
					RxData_type=4;
					break;					
			}
		}
		else if(RxState==2)//数据，接收范围限制在单字节，足够完成任务要求
		{
			if(RxData == '!')
			{
				Serial_RxFlag=1;
				RxState=0;			
			}
			else
			{
				Serial_RxPacket[pRxState]=RxData;
				pRxState++;		
			}
		}
            break;
        default:
            break;
    }
}


void UART_SOC_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_SOC_INST))//判断中断的类型，DL_UART_Main_getPendingInterrupt(UART_0_INST)调用具有清空标志位的功能？
    {
        case DL_UART_MAIN_IIDX_RX://如果触发串口接收事件
            RxData = DL_UART_Main_receiveData(UART_SOC_INST);
            if(RxState_SOC == 0)//帧头检测
            {
                if(RxData == 0xFF && Soc_RxFlag == 0)//上位机的数据帧
                {
                    RxState_SOC=1;
                }
            }
            else if(RxState_SOC == 1)//数据类型检测s
            {
                switch(RxData)
                {
                    case 0x2B ://+
                        RxState_SOC=2;
                        SOC_RxData_type=1;
                        break;
                    case 0x2D://-
                        RxState_SOC=2;
                        SOC_RxData_type=2;
                        break;	
                }
            }
            else if(RxState_SOC==2)//数据，接收范围限制在单字节，足够完成任务要求
            {
                Serial_RxPacket[0]=RxData;
                RxState_SOC=3;
            }
            else if( RxState_SOC == 3)
            {
                if(RxData==0XFB)
                {
                    Soc_RxFlag=1;
                    RxState_SOC=0;		                    
                }
                // DL_UART_Main_transmitData(UART_SOC_INST,(uint8_t)turn_err);
            }
            break;
        default:
            break;
    }
}


void UART_Screen_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_Screen_INST))
    {
        case DL_UART_MAIN_IIDX_RX://如果触发串口接收事件
            RxData = DL_UART_Main_receiveData(UART_Screen_INST);
            if(RxState_Screen == 0)//帧头检测
            {
                if(RxData == 0xEF && Screen_RxFlag == 0)
                {
                    RxState_Screen = 1;//串口屏的数据帧
                }
            }
            else if(RxState_Screen == 1)
            {
                Serial_RxPacket[0]=RxData;
                RxState_Screen = 2;
            }
            else if(RxState_Screen == 2)
            {
                if(RxData == 0xEE)
                {
                    Screen_RxFlag=1;
                    RxState_Screen=0;
                }
            }
            break;
        default:
            break;
    }
}