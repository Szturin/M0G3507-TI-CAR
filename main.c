#include "main.h"
#include "Serial.h"
#include "nqei.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#include <stdlib.h>

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

uint8_t status;
float yaw_val;
float yaw_detect;  

static float Velocity_IL,Velocity_IR;//左右轮速度环PID输入
static float Dif_Out;//差速环PID输出

uint8_t Motor_flag;

float M_Speed1;
float M_Speed2;

uint8_t find_lx_flag = 0;//路径定点,自主巡线标志位

//外设列表：
//1.GPIO        LED
//2.PWM         定时器G0,两路PWM
//3.定时器      定时器A0
//4.串口        USART0:调试串口
//5.按键      
//6.OLED      硬件IIC
//7.MPU6050   硬件IIC

/*MPU6050*/
uint8_t beep_flag = 0;//声光提示标志位
uint8_t beep_timer = 0;//声光提示持续时间s
uint8_t Way_Third_Flag;//路线三标志位
uint8_t find_point_flag;//找“点”标志位
uint8_t flag = 0;
uint16_t timer1_counter;

char hmi_speed[]="n1.txt";
char hmi_yaw[]="n2.txt";
char hmi_trace[]="n3.txt";
char hmi_totol[]="n4.txt";
char hmi_imu[]="n5.txt";
char data_HMI[]="";

float yaw_A,yaw_B,yaw_C,yaw_D;

uint32_t Timer_1ms_counter;

uint8_t Close_flag=0;//开环标志位
void Speed_InitPro(uint8_t x)
{
    uint8_t temp;
    uint8_t i;
    for(i=0;i<x;i++)
    {
        Set_Speed(i,i);
    }
}

//“探路”函数
void Find_Proc(void)
{
    Set_Speed(-30,30);
    delay_ms(200);
    Set_Speed(30,-30);
    delay_ms(200);    
}

/*开启系统时钟中断，会导致串口工作异常或者程序卡死*/
int main(void)
{

    /*
    board_init();//MPU6050初始化
    MPU6050_Init();//MPU6050初始化
    while( mpu_dmp_init() )
    {
        printf("dmp error\r\n");
        delay_ms(200);
    }
    printf("Initialization Data Succeed \r\n");
    */

    SYSCFG_DL_init();//syscfg初始化
    Motor_Off();//初始电机关闭，等待按键控制
    //OLED_Init();//OLED初始化
    SYS_UART0_Init();//串口0初始化
    Screen_UART2_Init();//串口2初始化

    BP_Encoder_Init();//编码器初始化
    PID_Init();
    delay_ms(1000);//等待初始化稳定
    Speed_InitPro(Speed_midset*0.8);

	//编码器中断使能
    delay_ms(50);//等待初始化稳定
    TIMER_1_Init();//定时器1初始化
    TIMER_2_Init();

    NVIC_EnableIRQ(UART_JY61P_INST_INT_IRQN);  
    while (1) 
    { 
        int_to_binary_string(Trace_Byte, data_HMI, 8);
        HMI_send_string(hmi_trace, data_HMI);

        float_to_string(Yaw,data_HMI,8);
        HMI_send_string(hmi_yaw, data_HMI);

        float_to_string(yaw_detect,data_HMI,8);
        HMI_send_string(hmi_speed, data_HMI);

        float_to_string(yaw_val,data_HMI,8);
        HMI_send_string(hmi_totol, data_HMI);

        float_to_string(NEncoder.right_motor_total_cnt,data_HMI,8);
        HMI_send_string(hmi_imu, data_HMI);
    }
}

void Beep_Proc()
{
    if(beep_flag == 1)
    {
        DL_GPIO_setPins(GPIO_BEEP_PORT,GPIO_BEEP_PIN_1_PIN);
    }
    else 
    {
        DL_GPIO_clearPins(GPIO_BEEP_PORT,GPIO_BEEP_PIN_1_PIN);
    }
}

//要求1
void LX_Proc_A()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;

    temp_hd_sum =  Huidu_Counter();
    if(LX_state ==0)
    {
        if(temp_hd_sum >=1)
        {
            LX_state = 1;
            Motor_Off();
        }
    }
}

//要求2
void LX_Proc_B()
{
    uint8_t temp_hd_sum;
    static uint8_t LX_state = 0;
    static uint32_t LuChen_Counter;
    
    temp_hd_sum =  Huidu_Counter();

    LuChen_Counter = (NEncoder.left_motor_total_cnt + NEncoder.right_motor_total_cnt)/2;

    if(LX_state ==0)//从A触发
    {
        if(temp_hd_sum >=1)//碰到B
        {
            LX_state = 1;
            Angle_PID_Flag = 0;
            beep_flag = 1;
        }
    }
    else if(LX_state == 1)//从B出发
    {
        if(temp_hd_sum == 0)//碰到C
        {
            yaw_detect -= 188;
            Angle_PID_Flag = 1;
            LX_state = 2;
            beep_flag = 1;
        }        
    }
    else if(LX_state == 2)//从C出发
    {
        if(temp_hd_sum >= 1)//碰到D
        {
            Angle_PID_Flag = 0;
            LX_state = 3;
            beep_flag = 1;     
        }    
    }
    else if(LX_state == 3)//从D出发
    {
        if(temp_hd_sum == 0)//碰到A
        {
            LX_state = 4;
            beep_flag = 1;
            Motor_Off();
        }                        
    }
}

//要求3
void LX_Proc_C()
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
            if(fabsf(Yaw - yaw_detect) <= 1.0)
            {
                beep_flag = 1;
                LX_state = 11;
            }            
        }
    }
    else if(LX_state == 11)
    {
        Speed_midset=30;

        if(temp_hd_sum >=1)//碰到C
        {
            LX_state = 1;
            Angle_PID_Flag = 0;
            beep_flag = 1;
            motor_total_cnt_reset();
        }        
    }
    else if(LX_state == 1)//从C出发
    {
        if(NEncoder.right_motor_total_cnt > 4000)
        {
            if(temp_hd_sum == 0)//碰到B
            {
                //yaw_detect = -(180 + 2 * abs(yaw_detect)-30);//30:补偿值
                yaw_detect = yaw_val + 56;
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
            Angle_PID_Flag = 0;
            LX_state = 3;
            beep_flag = 1;     
            motor_total_cnt_reset();
        }    
    }
    else if(LX_state == 3)//从D出发
    {
        if(NEncoder.right_motor_total_cnt > 4000)
        {
            if(temp_hd_sum == 0)//碰到A
            {
                LX_state = 0;
                Speed_midset=0;
                Angle_PID_Flag = 1;
                yaw_detect = yaw_val - 54.5;                
                beep_flag = 1;
                if(++i>=4)
                {
                    LX_state = 4;
                    Motor_Off();
                }
            }              
        }             
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
                   // DL_GPIO_setPins(GPIO_BEEP, uint32_t pins)
                    beep_timer=0;
                    beep_flag = 0;
                }
            }

            //倍频鉴相电路
            M_Speed2 = get_right_motor_speed();
            M_Speed1 = get_left_motor_speed();
            /*外部中断测速
            MEASURE_MOTORS_SPEED();
            M_Speed2 = Motor2_Speed;
            M_Speed1 = Motor1_Speed;
            */
            //status = mpu_dmp_get_data(&pitch,&roll,&yaw);
            //float_to_string(yaw,data_HMI,8);
            //HMI_send_string(hmi_imu, data_HMI);

                /*
            if(Angle_PID_Flag) //开启转向环
            {
                Velocity_IL = -Turn_imu_PID(yaw,yaw_detect);
                Velocity_IR = +Turn_imu_PID(yaw,yaw_detect);

                if(abs((int)(yaw  - yaw_detect)) <=20)
                {
                    Angle_PID_Flag = 0;
                }
                
                Set_Speed(Velocity_IL,Velocity_IR);
            }
            */
            //number++;
            //status = mpu_dmp_get_data(&pitch,&roll,&yaw);
            //printf("MPU6050:%d,%d,%d\n", (int)pitch,(int)roll,(int)yaw);
            //status = mpu_dmp_get_data(&pitch,&roll,&yaw);
            //status = mpu_dmp_get_data(&pitch,&roll,&yaw);
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
            
            if(++counter_2ms==2)
            {
                counter_2ms = 0;                    
                Get_TraceData();//循迹状态检测
                Timer_1ms_counter++;
                LX_Proc_C();
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
                    //printf("speed:%f,%f,%f\r\n",Test_Ks/10,M_Speed1,M_Speed2);
                    printf("JiaoDu:%f,%f,%f\r\n",Roll,Yaw,Pitch);
                    Velocity_IL = MID_Speed -Dif_Out;
                    Velocity_IR = MID_Speed + Dif_Out;
                    //MT_L=Velocity_PID(M_Speed1,Velocity_IL);
                    //MT_R=Velocity_PID(M_Speed2,1000);

                    if(Test_pid_flag == 0)
                    {
                        MT_L=Velocity_PID_L(M_Speed1,Velocity_IL);
                        MT_R=Velocity_PID_R(M_Speed2,Velocity_IR);
                    }
                    else
                    {
                        MT_L=Velocity_PID_L(M_Speed1,Test_Ks/10);
                        MT_R=Velocity_PID_R(M_Speed2,Test_Ks/10);                        
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

                   // Set_Speed(30,30);//无速度环
                    //Set_Speed(100,100);//无速度环
                   // Set_Speed(MT_L,MT_R);//有速度环
                    Set_Speed(Velocity_IL,Velocity_IR);//无速度环
                    //yaw_val = yaw;
                    yaw_val =Yaw;
                }
                else
                {
                    Dif_Out = Turn_imu_PID(Yaw, yaw_detect);

                    Velocity_IL = Speed_midset -Dif_Out; 
                    Velocity_IR = Speed_midset +Dif_Out;

                    Set_Speed(Velocity_IL,Velocity_IR);//速度环+陀螺仪转向环
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
                Angle_PID_Flag = 1;
                yaw_detect = yaw_val - 43;
                Motor_flag = 1;
                //Close_flag = 1;
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


/*上位机（SOC）通信解码进程*/
/*
void SOC_RxPro()
{
    //数据帧的处理部分
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
    else if(Soc_RxFlag == 2)
    {
        Motor_On();
        Soc_RxFlag=0;
    }
}
*/


/*
extern float Pitch,Roll,Yaw;
extern FPSCounter FPS;



int main(void)
{
    SYSCFG_DL_init();                           // 应用sysconfig的初始化
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);      // 配置定时器的NVIC
    DL_TimerG_startCounter(TIMER_0_INST);       // 初始化定时器
    OLED_Init();		                        //初始化OLED

    NVIC_EnableIRQ(UART_JY61P_INST_INT_IRQN);           //使能中断


    while(1){
        if (!DL_GPIO_readPins(GPIO_Key_PORT, GPIO_Key_PIN_S2_PIN)) {
            Serial_JY61P_Zero_Yaw();
        }
        OLED_ClearArea(0,0,79,40);
        OLED_Printf(0, 0, OLED_6X8, "Roll:%5.2f",Roll);
        OLED_Printf(0, 10, OLED_6X8, "Pitch:%5.2f",Pitch);
        OLED_Printf(0, 20, OLED_6X8, "Yaw:%5.2f",Yaw);
        OLED_Update();
        
        
    }

}

//定时器中断函数
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            if(FPS.Count<100){FPS.Count++;}else{FPS.Count=0;OLED_Printf(92,55,OLED_6X8,"FPS:%2d",FPS.Value);FPS.Value=0;}   //在右上角显示帧率
           
            break;


        default:
            break;
    }
}
*/