
#include "Path.h"

//路线选择进程
void LX_Select_Proc()
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