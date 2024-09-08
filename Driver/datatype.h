#ifndef __DATATYPE_H
#define __DATATYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef   signed          char int8;
typedef unsigned          char _u8;
typedef unsigned          char uint8;
typedef unsigned          char byte;
typedef   signed short    int int16;
typedef unsigned short    int uint16;
typedef unsigned short    int _u16;
typedef unsigned long     int _u32; 

enum 
{
	_ROL=0,
	_PIT,
	_YAW
};

enum 
{
	ROL=0,
	PIT,
	YAW
};

typedef struct
{
  float x;
  float y;
  float z;
}vector3f;

typedef struct
{
  float x;
  float y;
}vector2f;


typedef struct
{
	uint8_t bit1	:1;
	uint8_t bit2	:1;
	uint8_t bit3	:1;
	uint8_t bit4	:1;
	uint8_t bit5	:1;
	uint8_t bit6	:1;
	uint8_t bit7	:1;
	uint8_t bit8	:1;
	uint8_t bit9	:1;
	uint8_t bit10	:1;
	uint8_t bit11	:1;
	uint8_t bit12	:1;
	uint8_t bit13	:1;
	uint8_t bit14	:1;
	uint8_t bit15	:1;
	uint8_t bit16	:1;
}gray_flags;

typedef struct
{
  volatile float last_time;
  volatile float current_time;
  volatile float period;
  volatile uint16_t period_int;//单位ms
}systime;

typedef struct
{
	uint8_t update_flag;
  uint16_t buf[8];
	uint8_t online_flag;
}mppm;



typedef struct
{
	int8_t left_encoder_dir_config,right_encoder_dir_config;//编码器方向配置
	int8_t left_motion_dir_config	,right_motion_dir_config; //电机运动方向配置
	float wheel_radius_cm;				//轮胎半径,单位为cm
	uint16_t pulse_num_per_circle;//轮胎转动一圈累计的脉冲数量
	uint16_t servo_median_value1,servo_median_value2;
}motor_config;

typedef struct
{
	int16_t left_motor_cnt,right_motor_cnt;//单个采样周期内的脉冲数量
	int8_t left_motor_dir,right_motor_dir; //运动方向
	float left_motor_speed_rpm,right_motor_speed_rpm;//转速单位转每分钟
	float left_motor_gyro_rps,right_motor_gyro_rps;//转速单位rad/s
	float left_motor_speed_cmps,right_motor_speed_cmps;//转速c单位为cm/s
	float left_motor_period_ms,right_motor_period_ms;
	
	int32_t left_motor_total_cnt,right_motor_total_cnt;
	int32_t left_motor_period_cnt,right_motor_period_cnt;
	uint8_t left_motor_cnt_clear,right_motor_cnt_clear;
	
}encoder;

#endif


