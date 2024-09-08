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
float Speed_midset = 30;//预设直线速度
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
PID Velocity_L;
PID trace_hd;

/*速度环PID*/

void Velocity_PID_Init()
{
    Velocity.Kp = -1.33;
    Velocity.Kd = -0.22;
    Velocity.Ki = -0.012;
}

void Velocity_L_PID_Init()
{
    Velocity_L.Kp = -1.34;
    Velocity_L.Kd = -0.22;
    Velocity_L.Ki = -0.012;
}

/*循迹环PID*/
void trace_hd_PID_Init()
{
    trace_hd.Kp = -1.75;
    trace_hd.Kd = -0.65;
    trace_hd.Ki = 0;
}

/*PID初始化*/
void PID_Init()
{
    Velocity_PID_Init();
    Velocity_L_PID_Init();
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
float Velocity_PID_L(float velocity,float velocity_calcu)
{
	/*
	float a = 0.3;
	static float filt_velocity=0;
	static float last_filt_velocity=0;
	*/

    /*
    if(Test_pid_flag==1)//调试模式
    {
        velocity_calcu = Test_Ks/10.0;
        Velocity_L.Kp = -Test_Kp/100.0;
        Velocity_L.Ki = -Test_Ki/1000.0;
        Velocity_L.Kd = -Test_Kd/100.0;
    }
    else//PID参数重载
    {
        Velocity_PID_Init();
    }
    */
    
    if(Test_pid_flag==1)//调试模式
    {
        velocity_calcu = Test_Ks/10.0;
    }
    else//PID参数重载
    {
        Velocity_PID_Init();
    }
    
	Velocity_L.error = velocity - velocity_calcu;//误差值
	Velocity_L.error_difference = Velocity_L.error - Velocity_L.last_error;
	Velocity_L.last_error = Velocity_L.error;
	I_amplitude_limiting(5000,&Velocity_L.error_sum);
	Velocity_L.error_sum += Velocity_L.error;//误差累加

	
	return (Velocity_L.Kp*Velocity_L.error)+(Velocity_L.Kd*Velocity_L.error_difference)+(Velocity_L.Ki*Velocity_L.error_sum);
}

float Velocity_PID_R(float velocity,float velocity_calcu)
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
       Velocity.Ki = -Test_Ki/1000.0;
        Velocity.Kd = -Test_Kd/100.0;
    }
    else//PID参数重载
    {
        Velocity_PID_Init();
    }
/*
    if(Test_pid_flag==1)//调试模式
    {
        velocity_calcu = Test_Ks/10.0;
    }
    else//PID参数重载
    {
        Velocity_PID_Init();
    }
*/
	Velocity.error = velocity - velocity_calcu;//误差值
	Velocity.error_difference = Velocity.error - Velocity.last_error;
	Velocity.last_error = Velocity.error;
	I_amplitude_limiting(5000,&Velocity.error_sum);
	Velocity.error_sum += Velocity.error;//误差累加

	
	return (Velocity.Kp*Velocity.error)+(Velocity.Kd*Velocity.error_difference)+(Velocity.Ki*Velocity.error_sum);
}
char Huidu_Counter()
{
    uint8_t hd_sum = 0;
    
	for(int i=0;i<7;i++)//灰度循迹个数统计
	{
		if(((~Trace_Byte)>>i)&0x01) hd_sum++;
	}

    return hd_sum;
}

/*循迹环PID控制器*/
float Turn_hd_PID()
{	
    uint8_t temp_hd_sum = 0;
    
    temp_hd_sum = Huidu_Counter();

    if(temp_hd_sum <=2)//差速循迹
    {
        switch(Trace_Byte)
        {
            case 0xE7: // 1110 0111
            case 0xC3: // 1100 0011
                trace_hd.error = 0;
                break;
            case 0x00: // 0000 0000
            case 0x0F: // 0000 1111
            case 0x07: // 0000 0111
                trace_hd.error = -9;
                break;
            case 0xF0: // 1111 0000
            case 0xE0: // 1110 0000
                trace_hd.error = 9;
                break;
            case 0xE3: // 1110 0011
            case 0xF7: // 1111 0111
                trace_hd.error = 2;
                break;
            case 0xC1: // 1100 0001
                trace_hd.error = 1;
                break;
            case 0xF3: // 1111 0011
                trace_hd.error = 4;
                break;
            case 0xF1: // 1111 0001
            case 0xFB: // 1111 1011
                trace_hd.error = 6;
                break;
            case 0xF9: // 1111 1001
            case 0xFD: // 1111 1101
                trace_hd.error = 8;
                break;
            case 0xF8: // 1111 1000
                trace_hd.error = 10;
                break;
            case 0xFC: // 1111 1100
                trace_hd.error = 12;
                break;
            case 0xFE: // 1111 1110
                trace_hd.error = 14;
                break;
            case 0x87: // 1000 0111
                trace_hd.error = -2;
                break;
            case 0xC7: // 1100 0111
                trace_hd.error = -1;
                break;
            case 0xEF: // 1110 1111
                trace_hd.error = -2;
                break;
            case 0xCF: // 1100 1111
                trace_hd.error = -4;
                break;
            case 0x8F: // 1000 1111
            case 0xDF: // 1101 1111
                trace_hd.error = -6;
                break;
            case 0x9F: // 1001 1111
            case 0xBF: // 1011 1111
                trace_hd.error = -8;
                break;
            case 0x1F: // 0001 1111
                trace_hd.error = -10;
                break;
            case 0x3F: // 0011 1111
                trace_hd.error = -12;
                break;
            case 0x7F: // 0111 1111
                trace_hd.error = -14;
                break;
            case 0xFF: // 1111 1111
                trace_hd.error = trace_hd.last_error > 0 ? 16 : -16;
                break;
            default:
                trace_hd.error = 0;
                break;
        }        
    }
    else if(temp_hd_sum >= 6 && temp_hd_sum <= 8)//掉头
    {
        //Angle_PID_Flag = 1;
    }
    else//停车
    {

    }

    trace_hd.error_difference = trace_hd.error - trace_hd.last_error;
    trace_hd.error_sum += trace_hd.error;//误差累加量
    trace_hd.last_error = trace_hd.error;
    I_amplitude_limiting(1000,&trace_hd.error_sum);//误差累加量限幅


    K_trace =  1/16.0 * pow((1-(20/Speed_midset)),0.5);//弯道减速系数
    MID_Speed = Speed_midset * (1 - (uint8_t)(trace_hd.error)*K_trace) * (1 + (uint8_t)(trace_hd.error)*K_trace);//基准速度变换

    return trace_hd.error*trace_hd.Kp  + trace_hd.error_difference * trace_hd.Kd;//PD环循迹，比例+积分
}

/*（七路）循迹环PID控制器*/
float Turn_hd_PID_Seven()
{	
    uint8_t temp_hd_sum = 0;
    
    temp_hd_sum = Huidu_Counter();

    if(temp_hd_sum <=2)//差速循迹
    {
        switch(Trace_Byte)
        {
            //直行
            case 0B11110111: 
                trace_hd.error=0;
                break;
            //左转
            case 0B11100111:
                trace_hd.error=1;
                break;
            case 0B11101111:
                trace_hd.error=2;
                break;
            case 0B11001111:
                trace_hd.error=3;
                break;
            case 0B11011111:
                trace_hd.error=4;
                break;
            case 0B10011111:
                trace_hd.error=5;
                break;
            case 0B10111111:
                trace_hd.error=6;
                break;

            //右转
            case 0B11110011:
                trace_hd.error=-1;
                break;
            case 0B11111011:
                trace_hd.error=-2;
                break;
            case 0B11111001:
                trace_hd.error=-3;
                break;
            case 0B11111101:
                trace_hd.error=-4;
                break;
            case 0B11111100:
                trace_hd.error=-5;
                break;
            case 0B11111110:
                trace_hd.error=-6;
                break;

            default:
                trace_hd.error=0;
                break;
        }        
    }
    else if(temp_hd_sum >= 6 && temp_hd_sum <= 8)//掉头
    {
        //Angle_PID_Flag = 1;
    }
    else//停车
    {

    }

    trace_hd.error_difference = trace_hd.error - trace_hd.last_error;
    trace_hd.error_sum += trace_hd.error;//误差累加量
    trace_hd.last_error = trace_hd.error;
    I_amplitude_limiting(5000,&trace_hd.error_sum);//误差累加量限幅


    K_trace =  1/7.0 * pow((1-(30/Speed_midset)),0.5);//弯道减速系数
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
    float Kp = -1.35;
    float Kd = -9.5;
    float Ki = 0;

    static float error ;
    static float last_error;
    static float error_diff;
    static float error_sum;

    error = yaw -caclu_yaw;
    error_sum += error;

    I_amplitude_limiting(1000,&error_sum);//误差累加量限幅

    error_diff = error-last_error;
    last_error = error;

    
    if (error > 180) // 防止小车转到180度时一直旋转的问题
        error = error - 360;
    if (error < -180)
        error = error + 360;
    

    return Kp * error + Kd * error_diff + Ki*error_sum;
}