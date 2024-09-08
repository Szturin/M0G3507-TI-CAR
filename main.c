#include "main.h"
#include "OLED.h"
#include "Serial.h"
#include "nqei.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#include <stdlib.h>

uint32_t Key_Number;//按键调试 
uint8_t Key_Val,Key_Down,Key_Up,Key_Old;

//PS:PWM可调范围，0~100以内任意浮点数
float MT_L = 20.0;//左轮速度初值
float MT_R = 20.0;//右轮速度初值

uint8_t RxData;//串口接收寄存器存储变量
uint8_t MID_Speed;//基准速度

float yaw_val;//记录Yaw角
float yaw_detect;//预测yaw角

static float Velocity_IL,Velocity_IR;//左右轮速度环PID输入
static float Dif_Out;//差速环PID输出

uint8_t Motor_flag;//电机使能标志位

float M_Speed_L;//左轮测速
float M_Speed_R;//右轮测速

uint32_t Timer_Angel_Sleep;
uint8_t Timer_Angel_Sleep_flag;

//外设列表：
//1.GPIO        LED
//2.PWM         定时器G0,两路PWM
//3.定时器      定时器A0
//4.串口        USART0:调试串口
//5.按键      
//6.OLED      硬件IIC
//7.MPU6050   硬件IIC
//8.串口屏
//9.陀螺仪串口

uint8_t beep_flag = 0;//声光提示标志位
uint8_t beep_key_flag;//按键提示音
uint8_t beep_timer = 0;//声光提示持续时间

uint8_t flag = 0;
char hmi_speed[]="n1.txt";//测速
char hmi_yaw[]="n2.txt";//测角度
char hmi_trace[]="n3.txt";//测循迹
char hmi_totol[]="n4.txt";//测里程
char hmi_imu[]="n5.txt";//测记忆角度
char data_HMI[]="";//串口屏数据存储空间
uint8_t Key_Timer;

uint32_t Timer_1ms_counter;
uint8_t System_Mode;

uint8_t WD_Protect_Flag;
uint8_t WD_Protect_time;
uint8_t WD_Protect_err;

float K_ZX = 0;
/*初始加速函数*/
void Speed_InitPro(uint8_t x)
{
    uint8_t temp;
    uint8_t i;
    for(i=0;i<x;i++)
    {
        Set_Speed(i,i);
    }
}

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

void LX_Proc_Select()
{
    switch(System_Mode)
    {
        case 0:
            LX_Proc_1();
            break;
        case 1:
            LX_Proc_2();
            break;
        case 2:
            LX_Proc_3();
            break;
        case 3:
            LX_Proc_4();
            break;
    }
}
/*开启系统时钟中断，会导致串口工作异常或者程序卡死*/
int main(void)
{
    SYSCFG_DL_init();//syscfg初始化
    delay_ms(100);//等待初始化稳定
    OLED_Init();//OLED初始化
    OLED_ShowString(16,8,"OK",8);
    delay_ms(100);//等待陀螺仪稳定
    Motor_Off();//初始电机关闭，等待按键控制
    
    SYS_UART0_Init();//串口0初始化
    Screen_UART2_Init();//串口2初始化

    BP_Encoder_Init();//编码器初始化
    PID_Init();
    delay_ms(1000);//等待初始化稳定
    //Speed_InitPro(Speed_midset*0.8);

	//编码器中断使能
    delay_ms(50);//等待初始化稳定
    TIMER_1_Init();//定时器1初始化
    TIMER_2_Init();

    NVIC_EnableIRQ(UART_JY61P_INST_INT_IRQN);  
    while (1) 
    { 
        OLED_ShowString(0, 8,"Mode:",8);
        OLED_ShowNum(42, 8, System_Mode+1, 1 ,8);
        OLED_ShowBinNum(0, 28, Trace_Byte, 8 ,8);
        OLED_ShowSignedNum(0, 45,Yaw,3,8);
        OLED_Update();


        /*
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
        */
    }
}


/*5ms定时函数，执行复杂计算任务*/
void TIMER_2_INST_IRQHandler(void)
{
switch (DL_TimerG_getPendingInterrupt(TIMER_2_INST)) 
    {
        case DL_TIMER_IIDX_ZERO://如何触发定时器置零中断事件
            if(beep_flag == 1)
            {
                if(++beep_timer==100)
                {
                    beep_timer=0;
                    beep_flag = 0;
                }
            }

            if(beep_key_flag == 1)
            {
                if(++beep_timer == 30)
                {
                    beep_timer = 0;
                    beep_key_flag = 0;
                }   
            }

            //倍频鉴相电路测速
            M_Speed_R = get_right_motor_speed();
            M_Speed_L = get_left_motor_speed();
            break;
        default:
            break;
    }    
}

/*1ms定时器执行函数，执行系统关键计算部分*/
void TIMER_1_INST_IRQHandler(void)
{
    //判断中断事件类型
    static uint8_t counter_10ms;
    static uint8_t counter_2ms;

    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) 
    {
        case DL_TIMER_IIDX_ZERO://如何触发定时器置零中断事件
            /*串口解码程序*/
            /*2ms执行一次传感器检测*/
            Key_Proc();
            if(++Key_Timer == 10){Key_Timer = 0;}

            if(Timer_Angel_Sleep_flag == 1)//陀螺仪阻塞保护
            {
                Timer_Angel_Sleep++;
            }
            else
            {
                Timer_Angel_Sleep_flag = 0;
            }

            if(++counter_2ms==2)
            {
                counter_2ms = 0;                    
                Get_TraceData();//循迹状态检测
                Timer_1ms_counter++;
                LX_Proc_Select();
                Beep_Proc();
                //LX_Proc_B();
                /*串口处理*/
                SYS_RxPro();
                Screen_RxPro();
            }

            /*5ms执行一一次数据解算*/
            if(++counter_10ms == 10)
            {
                counter_10ms = 0;
                if(Angle_PID_Flag == 0)//未开启转向环（陀螺仪）
                {
                    //Dif_Out=Turn_hd_PID();
                    Dif_Out = Turn_hd_PID();
                    /*左右速度环差速预设*/
                    //printf("speed:%f,%f,%f\r\n",Test_Ks/10,M_Speed_L,M_Speed_R);

                    //printf("JiaoDu:%f,%f,%f\r\n",Roll,Yaw,Pitch);
                    Velocity_IL = MID_Speed -Dif_Out;
                    Velocity_IR = MID_Speed + Dif_Out;

                    if(Test_pid_flag == 0)
                    {
                        MT_L=Velocity_PID_L(M_Speed_L,Velocity_IL);
                        MT_R=Velocity_PID_R(M_Speed_R,Velocity_IR);
                    }
                    else
                    {
                        MT_L=Velocity_PID_L(M_Speed_L,Test_Ks/10);
                        MT_R=Velocity_PID_R(M_Speed_R,Test_Ks/10);                        
                    }

                    /*PWM限幅*/
                    if(MT_R > 100)
                    {
                        MT_R = 100;
                    }
                    else if(MT_R < -100)
                    {
                        MT_R = -100;
                    }

                    if(MT_L > 100)
                    {
                        MT_L = 100;
                    }
                    else if(MT_L < -100)
                    {
                        MT_L = -100;
                    }

                   // Set_Speed(MT_L,MT_R);//有速度环
                    Set_Speed(Velocity_IL,Velocity_IR);//无速度环
                    //Set_Speed(100,-30);//无速度环
                    yaw_val =Yaw;
                }
                else
                {
                    Dif_Out = Turn_imu_PID(Yaw, yaw_detect);
                    Velocity_IL = Speed_midset -Dif_Out; 
                    Velocity_IR = Speed_midset +Dif_Out;
                    Set_Speed(Velocity_IL,Velocity_IR);//速度环+陀螺仪转向环
                    // Set_Speed(100,-30);//无速度环
                }
            }
            break;
        default:
            break;
    }
}

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

/*******************************路线进程***********************************/
//要求1
void LX_Proc_1()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;

    temp_hd_sum =  Huidu_Counter();

    if(LX_state ==0)
    {
        Speed_midset = 30 - NEncoder.right_motor_total_cnt/8000 * 15.0;

        if(temp_hd_sum >=1)
        {
            beep_flag = 1;
            LX_state = 1;
            Motor_Off();
        }
    }
}

//要求2
void LX_Proc_2()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;
    static uint32_t LuChen_Counter;
    
    temp_hd_sum =  Huidu_Counter();

    //LuChen_Counter = (NEncoder.left_motor_total_cnt + NEncoder.right_motor_total_cnt)/2;

    if(LX_state ==0)//从A触发
    {
        Speed_midset = 40;
        if(temp_hd_sum >=1)//碰到B
        {
            Speed_midset = 35;
            LX_state = 1;
            Angle_PID_Flag = 0;
            beep_flag = 1;
            motor_total_cnt_reset();
        }
    }
    else if(LX_state == 1)//从B出发
    {
        if(NEncoder.right_motor_total_cnt >= 5000)
        {
            if(temp_hd_sum == 0)//碰到C
            {
                Speed_midset = 40;
                yaw_detect -= LX_2_IMU_ANGEL;
                Angle_PID_Flag = 1;
                LX_state = 2;
                beep_flag = 1;
            }                   
        }
    }
    else if(LX_state == 2)//从C出发
    {
        if(temp_hd_sum >= 1)//碰到D
        {
            motor_total_cnt_reset();
            Speed_midset = 35;
            Angle_PID_Flag = 0;
            LX_state = 3;
            beep_flag = 1;     
        }    
    }
    else if(LX_state == 3)//从D出发
    {
        if(NEncoder.right_motor_total_cnt >= 5000)
        {
            if(temp_hd_sum == 0)//碰到A
            {
                LX_state = 4;
                beep_flag = 1;
                Motor_Off();
            }                 
        }
                   
    }
}

//要求3
void LX_Proc_3()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;
    static uint8_t i;


    temp_hd_sum =  Huidu_Counter();

    if(LX_state ==0)//从A触发
    {
        Speed_midset=0;
        if(Angle_PID_Flag)
        {
            Timer_Angel_Sleep_flag = 1;

            if(fabsf(Yaw - yaw_detect) <= 1.5)
            {
                Speed_midset=Speed_ZX-15;
                beep_flag = 1;
                LX_state = 11;
            }            
        }
    }
    else if(LX_state == 11)
    {

        if(temp_hd_sum >=1)//碰到C
        {
            Speed_midset=Speed_WD-10;
            LX_state = 1;
            Angle_PID_Flag = 0;
            beep_flag = 1;
            motor_total_cnt_reset();
        }        
    }
    else if(LX_state == 1)//从C出发
    {
        if(NEncoder.right_motor_total_cnt > 5000)
        {
            if(temp_hd_sum == 0)//碰到B
            {
                Speed_midset=Speed_ZX-15;
                yaw_detect = yaw_val + LX_4_IMU_ANGEL_1 + 0.5;
                Angle_PID_Flag = 1;
                LX_state = 2;
                beep_flag = 1;
            }                    
        }
    }
    else if(LX_state == 2)//从B出发
    {
        if(temp_hd_sum >= 1)//碰到D
        {
            Speed_midset=Speed_WD-10;
            Angle_PID_Flag = 0;
            LX_state = 3;
            beep_flag = 1;     
            motor_total_cnt_reset();
        }    
    }
    else if(LX_state == 3)//从D出发
    {
        if(NEncoder.right_motor_total_cnt > 5000)
        {
            if(temp_hd_sum == 0)//碰到A
            {
                beep_flag = 1;
                LX_state = 4;
                Motor_Off();
            }              
        }             
    }
}

//要求4
void LX_Proc_4()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;
    static uint8_t i;

    temp_hd_sum =  Huidu_Counter();
    K_ZX = 1-(20/Speed_ZX);

    if(LX_state ==0)//从A触发
    {
        Speed_midset=0;

        if(Angle_PID_Flag)
        {
            Timer_Angel_Sleep_flag = 1;
            if(fabsf(Yaw - yaw_detect) <= 1.5)
            {
                motor_total_cnt_reset();

                Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX * K_ZX);

                beep_flag = 1;
                LX_state = 11;
            }            
        }
    }
    else if(LX_state == 11)
    {

        Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX *K_ZX);

        if(NEncoder.right_motor_total_cnt > 4000)
        {
            if(temp_hd_sum >=1)//碰到C
            {
                Speed_midset=Speed_WD;
                LX_state = 1;
                Angle_PID_Flag = 0;
                beep_flag = 1;
                motor_total_cnt_reset();
            }    
        }
     
    }
    else if(LX_state == 1)//从C出发
    {
        if(NEncoder.right_motor_total_cnt > 5000)
        {
            if(temp_hd_sum == 0)//碰到B
            {
                motor_total_cnt_reset();
                Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX * K_ZX);
                yaw_detect = yaw_val + LX_4_IMU_ANGEL_1 + 0.65;
                Angle_PID_Flag = 1;
                LX_state = 2;
                beep_flag = 1;
            }                    
        }
    }
    else if(LX_state == 2)//从B出发
    {
        Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX * K_ZX);
        if(NEncoder.right_motor_total_cnt > 4000)
        {
            if(temp_hd_sum >= 1)//碰到D
            {
                Speed_midset=Speed_WD;
                Angle_PID_Flag = 0;
                LX_state = 3;
                beep_flag = 1;     
                motor_total_cnt_reset();

                if(i == 3)
                {
                    Speed_midset = Speed_WD - 5;
                }
            }              
        }
    }
    else if(LX_state == 3)//从D出发
    {
        if(NEncoder.right_motor_total_cnt > 5000)
        {
            if(temp_hd_sum == 0)//碰到A
            {
                motor_total_cnt_reset();
                Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX * K_ZX);
                LX_state = 12;
                Angle_PID_Flag = 1;
                yaw_detect = yaw_val - LX_4_IMU_ANGEL_1 + 1.0;                
                beep_flag = 1;
                if(++i>=4)
                {
                    LX_state = 4;
                    Motor_Off();
                }
            }              
        }             
    }
    else if(LX_state == 12)//从A出发
    {
        Speed_midset = Speed_ZX - (NEncoder.right_motor_total_cnt/12000 * Speed_ZX * K_ZX);

        if(NEncoder.right_motor_total_cnt > 4000)
        {
            if(temp_hd_sum >= 1)//碰到C
            {
                Speed_midset=Speed_WD;
                Angle_PID_Flag = 0;
                LX_state = 1;
                beep_flag = 1;     
                motor_total_cnt_reset();
            }       

        }
    }
}