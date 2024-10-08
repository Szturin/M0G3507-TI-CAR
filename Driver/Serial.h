#ifndef __Serial_H__
#define __Serial_H__

#include "bsp_system.h"
#include "stdio.h"
#include "string.h"

uint8_t Serial_RxPacket[100];

/*PID调试*/
extern float Test_Kp;//比例
extern float Test_Ki;//积分
extern float Test_Kd;//微分
extern float Test_Ks;//目标速度
/*系统调试串口变量*/
extern uint8_t Serial_RxFlag;
extern uint8_t RxState;
extern uint8_t pRxState;//表示当前接收的是第几个变量
extern uint8_t RxData_type;
/*上位机串口变量*/
extern uint8_t Soc_RxFlag;
extern uint8_t RxState_SOC;
extern uint8_t SOC_RxData_type;
extern uint8_t SOC_pRxState;//串口接收数据索引号
/*串口屏串口变量*/
extern uint8_t Screen_RxFlag;
extern uint8_t RxState_Screen;
extern uint8_t Screen_RxData_type;
extern uint8_t Screen_pRxState;//串口接收数据索引号

/*串口中断相关函数*/
void SYS_UART0_Init(void);
void SOC_UART1_Init(void);
void Screen_UART2_Init(void);
void SOC_SendString(char *str);
void Screen_SendString(char *str);

/*串口中断程序声明*/
void UART_0_INST_IRQHandler(void);
void UART_SOC_INST_IRQHandler(void);
void UART_Screen_INST_IRQHandler(void);

/*串口屏函数*/
void HMI_send_string(char* name, char* showdata);
void int_to_binary_string(uint32_t value, char *binary_str, int max_bits);//整数转字符串
void float_to_string(float value, char *str, size_t size);//浮点数转字符串
void HMI_send_number(char* name, int num);
void HMI_send_float(char* name, float num);
void HMI_Wave(char* name, int ch, int val);
void HMI_Wave_Fast(char* name, int ch, int count, int* show_data);

#endif