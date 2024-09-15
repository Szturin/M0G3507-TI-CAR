#include "Process.h"

/**************************串口进程*********************************/
/*系统串口(UART1)解码进程*/
void SYS_RxPro()
{
    /*PID调试*/
    if(Serial_RxFlag)
    {
        switch(RxData_type)//判断数据帧的数据类型
        {
            case 1:
                if(pRxState == 1)
                {
                    Test_Kp = Serial_RxPacket[0]-0x30;
                }
                else if(pRxState == 2)
                {
                    Test_Kp = (Serial_RxPacket[0]-0x30)*10 + (Serial_RxPacket[1]-0x30);
                }
                else if(pRxState == 3)
                {
                    Test_Kp = (Serial_RxPacket[0]-0x30)*100 + (Serial_RxPacket[1]-0x30)*10 + (Serial_RxPacket[2]-0x30);
                }
                break;
            case 2:
                if(pRxState == 1)
                {
                    Test_Ki = Serial_RxPacket[0]-0x30;
                }
                else if(pRxState == 2)
                {
                    Test_Ki = (Serial_RxPacket[0]-0x30)*10 + (Serial_RxPacket[1]-0x30);
                }
                else if(pRxState == 3)
                {
                    Test_Ki = (Serial_RxPacket[0]-0x30)*100 + (Serial_RxPacket[1]-0x30)*10 + (Serial_RxPacket[2]-0x30);
                }				
                break;
            case 3:
                if(pRxState == 1)
                {
                    Test_Kd = Serial_RxPacket[0]-0x30;
                }
                else if(pRxState == 2)
                {
                    Test_Kd = (Serial_RxPacket[0]-0x30)*10 + (Serial_RxPacket[1]-0x30);
                }
                else if(pRxState == 3)
                {

                    Test_Kd = (Serial_RxPacket[0]-0x30)*100 + (Serial_RxPacket[1]-0x30)*10 + (Serial_RxPacket[2]-0x30);
                }						
                break;
            case 4:
                if(pRxState == 1)
                {
                    Test_Ks = Serial_RxPacket[0]-0x30;
                }
                else if(pRxState == 2)
                {
                    Test_Ks = (Serial_RxPacket[0]-0x30)*10 + (Serial_RxPacket[1]-0x30);
                }
                else if(pRxState == 3)
                {
                    Test_Ks = (Serial_RxPacket[0]-0x30)*100 + (Serial_RxPacket[1]-0x30)*10 + (Serial_RxPacket[2]-0x30);
                }						
                break;
        }
        Serial_RxFlag=0;	
        pRxState=0;
    }
}

/*串口屏通信解码进程*/
void Screen_RxPro()
{
    if(Screen_RxFlag == 1)
    {
        switch(Serial_RxPacket[0])
        {
            case 1:
                Motor_On();
                Angle_PID_Flag = 1;

                if(System_Mode == 0 | System_Mode == 1)
                {
                    yaw_detect = yaw_val;
                }
                else
                {
                    yaw_detect = yaw_val - LX_3_IMU_ANGEL_1;
                }

                Motor_flag = 1;
                break;
            case 2:
                Serial_JY61P_Zero_Yaw();
                Motor_Off();
                break;
            case 3://PID参数模式切换：调试模式<->固定模式
                Test_pid_flag=1;
            case 4:
                Test_pid_flag=0;
                break;
        }
        DL_UART_Main_transmitData(UART_Screen_INST,Serial_RxPacket[0]);
        Screen_RxFlag=0;
    }
}

/********************特殊功能进程**********************/
void Beep_Proc()
{
    if(beep_flag == 1 || beep_key_flag == 1)
    {
        DL_GPIO_setPins(GPIO_BEEP_PORT,GPIO_BEEP_PIN_1_PIN);
    }
    else 
    {
        DL_GPIO_clearPins(GPIO_BEEP_PORT,GPIO_BEEP_PIN_1_PIN);
    }
}

/*OlED进程*/
void Oled_Proc()
{
    OLED_ShowString(0, 8,"Mode:",8);
    OLED_ShowNum(42, 8, System_Mode+1, 1 ,8);
    OLED_ShowBinNum(0, 28, Trace_Byte, 8 ,8);
    OLED_ShowSignedNum(0, 45,Yaw,3,8);
    OLED_Update();    
}

/*串口屏进程*/
void HMI_Proc()
{
    int_to_binary_string(Trace_Byte, data_HMI, 8);
    HMI_send_string(hmi_trace, data_HMI);//串口屏打印循迹状态

    float_to_string(Yaw,data_HMI,8);
    HMI_send_string(hmi_yaw, data_HMI);//串口屏打印陀螺仪实际的Yaw角度

    float_to_string(M_Speed_L,data_HMI,8);
    HMI_send_string(hmi_speed, data_HMI);//串口屏打印速度

    float_to_string(System_Mode,data_HMI,8);
    HMI_send_string(hmi_imu, data_HMI);//串口屏打印单片机记忆的Yaw角度

    float_to_string(NEncoder.right_motor_total_cnt,data_HMI,8);
    HMI_send_string(hmi_totol, data_HMI);//串口屏打印总的脉冲数
}

/*按键*/
void Key_Proc(void)//按键检测程序
{
	if(Key_Timer) return;//Key_Timer=0时执行下面的语句
	Key_Timer=1;

	//按键扫描部分//
	Key_Val=Key_Read();
	Key_Down=Key_Val & (Key_Old ^ Key_Val);
	Key_Up=~Key_Val & (Key_Old ^ Key_Val);
	Key_Old=Key_Val;

    switch(Key_Down)
    {
        case 1:
            beep_key_flag = 1;
            if(++System_Mode == 4)
            {
                System_Mode = 0;
            }
            break;
        case 2:
            Motor_On();
            Angle_PID_Flag = 1;
            Speed_midset = Speed_ZX;
            if(System_Mode == 0 | System_Mode == 1)
            {
                yaw_detect = yaw_val;
            }
            else 
            {
                yaw_detect = yaw_val - LX_3_IMU_ANGEL_1;
            }

            Motor_flag = 1;
            break;
        case 3:
            Motor_flag = 0;
            Serial_JY61P_Zero_Yaw();
            Motor_Off();            
        break;
    }
}
