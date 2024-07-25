#include "main.h"

uint8_t test_trace;//循迹调试
uint32_t val;//按键调试
uint32_t number;//定时器调试       

int turn_err;//循迹偏差值
float PID_Output;//PID控制器输出暂存值

//PS:PWM可调范围，0~100以内任意浮点数
float MT_L = 20.0;//左轮速度初值
float MT_R = 20.0;//右轮速度初值

uint8_t RxData;//串口接收寄存器存储变量
uint8_t MID_Speed;

//外设列表：
//1.GPIO        LED
//2.PWM         定时器G0,两路PWM
//3.定时器      定时器A0
//4.串口        USART0:调试串口
//5.按键      
//6.OLED      硬件IIC
//7.MPU6050   硬件IIC

/*MPU6050*/
uint8_t ret = 1;
float pitch=0,roll=0,yaw=0;

/*开启系统时钟中断，会导致串口工作异常或者程序卡死*/
int main(void)
{
    SYSCFG_DL_init();//syscfg初始化
    Motor_Off();//初始电机关闭，等待按键控制
    TIMER_1_Init();//定时器1初始化
    OLED_Init();//OLED初始化
	//board_init();//MPU6050初始化
    SYS_UART0_Init();//串口0初始化
    SOC_UART1_Init();//串口1初始化
    Screen_UART2_Init();//串口2初始化
    PID_Init();
	//编码器中断使能
    Encoder_Init();
    Turn_hd_PID();

	//MPU6050_Init();//MPU6050初始化
    /*
    while( mpu_dmp_init() )
    {
        printf("dmp error\r\n");
        delay_ms(200);
    }

    printf("Initialization Data Succeed \r\n");
    */
    //Set_Speed(MT_l,MT_R);
    while (1) 
    { 

        //注意，循迹的标号方向与程序处理索引互补

        /*视觉循迹*/
        /*
        PID_Output =  Turn_PID(turn_err,0);
        MT_l = 80  - PID_Output;
        MT_R = 80 + PID_Output;
        Set_Speed(MT_l,MT_R);
        */

        //OLED_ShowBinNum(1, 1, Trace_Byte,8,OLED_6X8);
        OLED_ShowSignedNum(0, 28, MID_Speed, 3, 8);
        OLED_Update();

        /*
        status = mpu_dmp_get_data(&pitch,&roll,&yaw);
        if( status == 0 )
        { 
            printf("MPU6050:%d,%d,%d\n", (int)pitch,(int)roll,(int)yaw);
        }
        delay_ms(20);
        */
        //val = DL_GPIO_readPins(GPIO_SWITCH_PORT,GPIO_SWITCH_USER_SWITCH_S2_PIN);//返回值实际为引脚组成的n位二进制数
    }
}


/*1ms定时器执行函数，执行系统关键计算部分*/
void TIMER_1_INST_IRQHandler(void)
{
    //判断中断事件类型
    static uint8_t counter_10ms;
    static uint8_t counter_2ms;
    static float Velocity_IL,Velocity_IR;//左右轮速度环PID输入
    static float Dif_Out;//差速环PID输出

    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) 
    {
        case DL_TIMER_IIDX_ZERO://如何触发定时器置零中断事件
            /*串口解码程序*/
            SYS_RxPro();
            Screen_RxPro();
            SOC_RxPro();

            /*2ms执行一次传感器检测*/
            if(++counter_2ms==2)
            {
                counter_2ms = 0;
                Get_TraceData();//循迹状态检测
            }
            /*10ms执行一一次数据解算*/
            if(++counter_10ms == 10)
            {
                counter_10ms = 0;
                if(Angle_PID_Flag == 0)//未开启转向环（陀螺仪）
                {
                    printf("%f,%f,%f\r\n",40.0,Motor1_Speed,Motor2_Speed);
                    Dif_Out=Turn_hd_PID();
                    MEASURE_MOTORS_SPEED();//速度测量,限制在10ms
                    /*左右速度环差速预设*/
                    Velocity_IL = MID_Speed -Dif_Out;
                    Velocity_IR = MID_Speed +Dif_Out;
                    MT_L=Velocity_PID(Motor1_Speed,Velocity_IL);
                    MT_R=Velocity_PID(Motor2_Speed,Velocity_IR);
                    /*PWM限幅*/
                    if(MT_R >= 95)
                    {
                        MT_R = 95;
                    }
                    else if(MT_R <= -95)
                    {
                        MT_R = -95;
                    }

                    if(MT_L >= 95)
                    {
                        MT_L = 95;
                    }
                    else if(MT_L <= -95)
                    {
                        MT_L = -95;
                    }
                    //Set_Speed(Velocity_IL,Velocity_IR);//无速度环
                    Set_Speed(MT_L,MT_R);//有速度环
                }
                else //开启转向环
                {
                    
                }
            }
            break;
        default:
            break;
    }
}

/*******************************************************************************************/
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
                break;
            case 2:
                Motor_Off();
                break;
            case 0xAA://PID参数模式切换：调试模式<->固定模式

                Test_pid_flag^=1;
                break;
        }
        DL_UART_Main_transmitData(UART_Screen_INST,Serial_RxPacket[0]);
        Screen_RxFlag=0;
    }
}

/*上位机（SOC）通信解码进程*/
void SOC_RxPro()
{
    /*数据帧的处理部分*/
    if(Soc_RxFlag == 1)
    {
        switch(SOC_RxData_type)//判断数据帧的数据类型
        {
            case 1:
                    turn_err = Serial_RxPacket[0];
                break;
            case 2:
                    turn_err= -Serial_RxPacket[0];
                break;
        }
        Soc_RxFlag=0;	
    }
}

