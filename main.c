#include "main.h"
#include "Process.h"

uint32_t Key_Number;//按键调试 
uint8_t Key_Val,Key_Down,Key_Up,Key_Old;
uint8_t Key_Timer;

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

uint32_t Timer_1ms_counter;
uint8_t System_Mode;//系统模式

float K_ZX = 0;//减速相关系数

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
        Oled_Proc();
        HMI_Proc();
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
                LX_Select_Proc();
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

                    /*
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
                    */
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

