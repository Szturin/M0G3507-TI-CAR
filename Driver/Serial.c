#include "Serial.h"
#define BUFFER_SIZE 256  // 定义缓冲区大小

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

void int_to_binary_string(uint32_t value, char *binary_str, int max_bits)
{
    // 生成二进制字符串
    for (int i = max_bits - 1; i >= 0; --i)
    {
        binary_str[i] = (value & (1U << i)) ? '1' : '0';
    }
    binary_str[max_bits] = '\0'; // 确保字符串以 null 结尾
}

// 将浮点数转换为字符串
void float_to_string(float value, char *str, size_t size)
{
    snprintf(str, size, "%.2f", value); // 格式化为两位小数
}

void HMI_send_string(char *name, char *showdata)
{
    char buffer[BUFFER_SIZE];
    int length;

    // 构造要发送的字符串，确保缓冲区足够大
    length = snprintf(buffer, sizeof(buffer), "%s=\"%s\"\xff\xff\xff", name, showdata);

    // 检查是否发生了缓冲区溢出
    if (length >= sizeof(buffer))
    {
        // 缓冲区溢出处理，例如可以通过截断字符串或者增加缓冲区大小来解决
        // 这里只是简单地截断字符串以确保不会发送超出缓冲区的内容
        buffer[sizeof(buffer) - 1] = '\0'; // 确保字符串以 null 结尾
    }

    // 使用 Screen_SendString 发送构造好的字符串
    Screen_SendString(buffer);
}

void HMI_send_number(char* name, int num)
{
    char buffer[BUFFER_SIZE];
    int length = snprintf(buffer, sizeof(buffer), "%s=%d\xff\xff\xff", name, num);
    if (length >= BUFFER_SIZE)
    {
        // 缓冲区溢出处理
    }
    Screen_SendString(buffer);
}

void HMI_send_float(char* name, float num)
{
    char buffer[BUFFER_SIZE];
    int num_int = (int)(num * 100);
    int length = snprintf(buffer, sizeof(buffer), "%s=%d\xff\xff\xff", name, num_int);
    if (length >= BUFFER_SIZE)
    {
        // 缓冲区溢出处理
    }
    Screen_SendString(buffer);
}

void HMI_Wave(char* name, int ch, int val)
{
    char buffer[BUFFER_SIZE];
    int length = snprintf(buffer, sizeof(buffer), "add %s,%d,%d\xff\xff\xff", name, ch, val);
    if (length >= BUFFER_SIZE)
    {
        // 缓冲区溢出处理
    }
    Screen_SendString(buffer);
}

void HMI_Wave_Fast(char* name, int ch, int count, int* show_data)
{
    char buffer[BUFFER_SIZE];
    int length = snprintf(buffer, sizeof(buffer), "addt %s,%d,%d\xff\xff\xff", name, ch, count);
    if (length >= BUFFER_SIZE)
    {
        // 缓冲区溢出处理
    }
    Screen_SendString(buffer);

    delay_ms(100);

    for (int i = 0; i < count; i++)
    {
        // 发送每个字符
        DL_UART_Main_transmitDataBlocking(UART_Screen_INST, (char)show_data[i]);
    }
    // 发送结束标志
    Screen_SendString("\xff\xff\xff");
}

void HMI_Wave_Clear(char* name, int ch)
{
    char buffer[BUFFER_SIZE];
    int length = snprintf(buffer, sizeof(buffer), "cle %s,%d\xff\xff\xff", name, ch);
    if (length >= BUFFER_SIZE)
    {
        // 缓冲区溢出处理
    }
    Screen_SendString(buffer);
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

/*
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
                else if(RxData == 0xEF)
                {
                    RxState_SOC = 11;
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
            else if(RxState_SOC == 11)
            {
                Serial_RxPacket[0]=RxData;
                RxState_SOC = 12;
            }
            else if(RxState_SOC == 12)
            {
                if(RxData == 0xEB)
                {
                    RxState_SOC = 13;
                    Soc_RxFlag=2;
                }
            }
            break;
        default:
            break;
    }
}
*/