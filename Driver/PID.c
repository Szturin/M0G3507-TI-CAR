#include "PID.h"
#include "ti_msp_dl_config.h"
#include "Serial.h"
#include "main.h"
#include "math.h"

uint8_t Trace_Byte;//循迹总状态
uint8_t Angle_PID_Flag =0;//角度环开启标志位
uint8_t CV_flag=0;//视觉循迹开启标志位
uint8_t Test_pid_flag=0;//PID调试标志位
float K_trace = 0.0615;//减速系数
float Speed_midset = 90;//预设直线速度

/*灰度GPIO口宏定义*/
#define Read_Huidu_IO1	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_0_PIN) == GPIO_TRACE_PIN_TRACE_0_PIN)?1:0)
#define Read_Huidu_IO2	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_1_PIN) == GPIO_TRACE_PIN_TRACE_1_PIN)?1:0)
#define Read_Huidu_IO3	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_2_PIN) == GPIO_TRACE_PIN_TRACE_2_PIN)?1:0)
#define Read_Huidu_IO4	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_3_PIN) == GPIO_TRACE_PIN_TRACE_3_PIN)?1:0)
#define Read_Huidu_IO5	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_4_PIN) == GPIO_TRACE_PIN_TRACE_4_PIN)?1:0)
#define Read_Huidu_IO6	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_5_PIN) == GPIO_TRACE_PIN_TRACE_5_PIN)?1:0)
#define Read_Huidu_IO7	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_6_PIN) == GPIO_TRACE_PIN_TRACE_6_PIN)?1:0)
#define Read_Huidu_IO8	 ((DL_GPIO_readPins(GPIO_TRACE_PORT,GPIO_TRACE_PIN_TRACE_7_PIN) == GPIO_TRACE_PIN_TRACE_7_PIN)?1:0)

/*PID结构体定义*/
typedef struct 
{
	float Kp;
	float Ki;
	float Kd;
	float error;
	float last_error;
	float error_sum;
	float error_difference;
	float filt_velocity;
	float last_filt_velocity;
	float velocity_sum;
}PID;

/*结构体初始化*/
PID Velocity;
PID trace_hd;

/*速度环PID*/
void Velocity_PID_Init()
{
    Velocity.Kp = -1.2;
    Velocity.Kd = -0.01;
    Velocity.Ki = -0.02;
}

/*循迹环PID*/
void trace_hd_PID_Init()
{
    trace_hd.Kp = 4.5;
    trace_hd.Kd = 20;
    trace_hd.Ki = 0;
}

/*PID初始化*/
void PID_Init()
{
    Velocity_PID_Init();
    trace_hd_PID_Init();
}

/*限幅函数*/
void I_amplitude_limiting(float number,float *Error_sum)
{
	if(*Error_sum > number)
	{
		*Error_sum = number;
	}
	
	if(*Error_sum <- number)
	{
		*Error_sum = -number;
	}
}

/*速度环PID控制器*/
float Velocity_PID(float velocity,float velocity_calcu)
{
	/*
	float a = 0.3;
	static float filt_velocity=0;
	static float last_filt_velocity=0;
	*/
    if(Test_pid_flag==1)//调试模式
    {
        velocity_calcu = Test_Ks/10.0;
        Velocity.Kp = -Test_Kp/100.0;
        Velocity.Ki = -Test_Ki/100.0;
        Velocity.Kd = -Test_Kd/100.0;
    }
    else//PID参数重载
    {
        Velocity_PID_Init();
    }

	Velocity.error = velocity - velocity_calcu;//误差值
	Velocity.error_difference = Velocity.error - Velocity.last_error;
	Velocity.last_error = Velocity.error;
	I_amplitude_limiting(5000,&Velocity.error_sum);
	Velocity.error_sum += Velocity.error;//误差累加

	
	return (Velocity.Kp*Velocity.error)+(Velocity.Kd*Velocity.error_difference)+(Velocity.Ki*Velocity.error_sum);
}

/*循迹环PID控制器*/
float Turn_hd_PID()
{	
    uint8_t hd_sum = 0;
    
	for(int i=0;i<7;i++)//灰度循迹个数统计
	{
		if(((~Trace_Byte)>>i)&0x01) hd_sum++;
	}

    if(hd_sum <=2)//差速循迹
    {
        switch(Trace_Byte)
        {
            //直行
            case 0B11100111: 
                trace_hd.error=0;
                break;
            //左转
            case 0B11101111:
                trace_hd.error=1;
                break;
            case 0B11001111:
                trace_hd.error=2;
                break;
            case 0B11011111:
                trace_hd.error=3;
                break;
            case 0B10011111:
                trace_hd.error=4;
                break;
            case 0B10111111:
                trace_hd.error=5;
                break;
            case 0B00111111:
                trace_hd.error=6;
                break;
            case 0B01111111:
                trace_hd.error=7;
                break;
            //右转
            case 0B11110111:
                trace_hd.error=-1;
                break;
            case 0B11110011:
                trace_hd.error=-2;
                break;
            case 0B11111011:
                trace_hd.error=-3;
                break;
            case 0B11111001:
                trace_hd.error=-4;
                break;
            case 0B11111101:
                trace_hd.error=-5;
                break;
            case 0B11111100:
                trace_hd.error=-6;
                break;
            case 0B11111110:
                trace_hd.error=-7;
                break;    
        }        
    }
    else if(hd_sum >= 6 && hd_sum <= 8)//掉头
    {
        Angle_PID_Flag = 1;
    }
    else//停车
    {

    }
    trace_hd.error_difference = trace_hd.error - trace_hd.last_error;
    trace_hd.error_sum += trace_hd.error;//误差累加量
    trace_hd.last_error = trace_hd.error;
    I_amplitude_limiting(5000,&trace_hd.error_sum);//误差累加量限幅

    K_trace =  1/7.0 * pow((1-(45/Speed_midset)),0.5);//弯道减速系数
    MID_Speed = Speed_midset * (1 - (uint8_t)(trace_hd.error)*K_trace) * (1 + (uint8_t)(trace_hd.error)*K_trace);//基准速度变换

    return trace_hd.error*trace_hd.Kp  + trace_hd.error_difference * trace_hd.Kd;//PD环循迹，比例+积分
}

void Get_TraceData()
{
    Trace_Byte = (Read_Huidu_IO1<<7) + (Read_Huidu_IO2 << 6) + (Read_Huidu_IO3 << 5) + (Read_Huidu_IO4 <<4) 
                 + (Read_Huidu_IO5 << 3) +(Read_Huidu_IO6 <<2) +(Read_Huidu_IO7 <<1) + Read_Huidu_IO8;//8路灰度循迹状态
}

//视觉循迹PID
float Turn_cv_PID(int measure, int caclu)
{
    static float Kp = 0.08;
    static float Ki = 0;
    static float Kd = 0;
    static float last_err = 0;
    static float err_sum = 0;
    static float err = 0;
    static float err_difference = 0;

    err = measure - caclu;
    err_difference = err - last_err;
    last_err = err;
    err_sum += err_difference;
    return Kp * err + Ki * err_sum + Kd * err_difference;
}

/*转向环PID控制器*/
float Turn_imu_PID(int yaw, int caclu_yaw)
{	
    float Kp = 0.10;
    float Kd = 0.20;
    static float error ;
    static float last_error;
    static float error_diff;

    error = yaw -caclu_yaw;
    error_diff = error-last_error;
    last_error = error;

    return Kp * error + Kd * error_diff;
}