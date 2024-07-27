#include "ti_msp_dl_config.h"
#include "datatype.h"
#include "user.h"
#include "nqei.h"

encoder NEncoder={
	.left_motor_period_ms=20,
	.right_motor_period_ms=20,
};
systime timer_qei1,timer_qei0;

motor_config trackless_motor={
	.left_encoder_dir_config=left_motor_encoder_dir_default,	//��������������
	.right_encoder_dir_config=right_motor_encoder_dir_default,//��������������
	.left_motion_dir_config=left_motion_dir_default,					//����˶���������
	.right_motion_dir_config=right_motion_dir_default,				//����˶���������
	.wheel_radius_cm=tire_radius_cm_default,									//�����ֵİ뾶
	.pulse_num_per_circle=pulse_cnt_per_circle_default,				//������ת��һȦʱ�������������ۼ�ֵ
	.servo_median_value1=servo_median_value1_default,					//�����е��ֵ1
	.servo_median_value2=servo_median_value2_default,					//�����е��ֵ2
};


/***************************************
������:	void QEI0_IRQHandler(void)
˵��: QEI0�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void QEI0_IRQHandler(void)
{
	//NEncoder.right_motor_dir=((DL_GPIO_readPins(PORTB_PORT,  PORTB_RIGHT_DIR_PIN))!=0?1:-1);
	NEncoder.right_motor_dir=((PORTB_PORT->DIN31_0 & PORTB_RIGHT_DIR_PIN)!=0?1:-1);
	if(trackless_motor.right_encoder_dir_config==0)
	{		
		if(NEncoder.right_motor_dir==1)	NEncoder.right_motor_period_cnt++;
		else NEncoder.right_motor_period_cnt--;
		NEncoder.right_motor_total_cnt+=NEncoder.right_motor_dir;	
	}
	else
	{
		if(NEncoder.right_motor_dir==1)	NEncoder.right_motor_period_cnt--;
		else NEncoder.right_motor_period_cnt++;
		NEncoder.right_motor_total_cnt-=NEncoder.right_motor_dir;				
	}	
}

/***************************************
������:	void QEI1_IRQHandler(void)
˵��: QEI1�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void QEI1_IRQHandler(void)
{
	//NEncoder.left_motor_dir=((DL_GPIO_readPins(PORTB_PORT,  PORTB_LEFT_DIR_PIN))!=0?-1:1);
	NEncoder.left_motor_dir=((PORTB_PORT->DIN31_0 & PORTB_LEFT_DIR_PIN)!=0?-1:1);
	if(trackless_motor.left_encoder_dir_config==0)
	{	
		if(NEncoder.left_motor_dir==1)	NEncoder.left_motor_period_cnt++;
		else NEncoder.left_motor_period_cnt--;
		NEncoder.left_motor_total_cnt+=NEncoder.left_motor_dir;
	}
	else
	{
		if(NEncoder.left_motor_dir==1)	NEncoder.left_motor_period_cnt--;
		else NEncoder.left_motor_period_cnt++;
		NEncoder.left_motor_total_cnt-=NEncoder.left_motor_dir;
	}		
}

/***************************************
������:	void get_left_motor_speed(void)
˵��: ��ȡ�������ʵ���ٶ�ֵ
���:	��
����:	��
��ע:	����λʱ���ڵ�������,ת����rpm��cm/s
����:	��������
***************************************/
float get_left_motor_speed(void)
{
	static uint16_t cnt1=0;cnt1++;
	if(cnt1>=4)
	{
		cnt1=0;
		NEncoder.left_motor_period_ms=20;
		//���ٶ�ת����תÿ����
		NEncoder.left_motor_speed_rpm=60*(NEncoder.left_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.left_motor_period_ms*0.001f);	
		NEncoder.left_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.left_motor_speed_rpm/60.0f);
		NEncoder.left_motor_period_cnt=0;
	}
	return NEncoder.left_motor_speed_cmps;
}

/***************************************
������:	void get_right_motor_speed(void)
˵��: ��ȡ�ұ�����ʵ���ٶ�ֵ
���:	��
����:	��
��ע:	����λʱ���ڵ�������,ת����rpm��cm/s
����:	��������
***************************************/
float get_right_motor_speed(void)
{
	static uint16_t cnt2=0;cnt2++;
	if(cnt2>=4)
	{
		cnt2=0;
		NEncoder.right_motor_period_ms=20;
		//���ٶ�ת����תÿ����
		NEncoder.right_motor_speed_rpm=60*(NEncoder.right_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.right_motor_period_ms*0.001f);
		NEncoder.right_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.right_motor_speed_rpm/60);
		
		NEncoder.right_motor_period_cnt=0;	
	}
	return NEncoder.right_motor_speed_cmps;
}

/***************************************
������:	void motor_total_cnt_reset(void)
˵��: �����������λ
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void motor_total_cnt_reset(void)
{
	NEncoder.left_motor_total_cnt =0;
	NEncoder.right_motor_total_cnt=0;
}



/*
//�ⲿ�ж�
void GROUP1_IRQHandler(void)
{
	if(DL_Interrupt_getStatusGroup(DL_INTERRUPT_GROUP_1,DL_INTERRUPT_GROUP1_GPIOB))
	{
		if(DL_GPIO_getEnabledInterruptStatus(PORTB_PORT, PORTB_RIGHT_PULSE_PIN))
		{
			QEI0_IRQHandler();
			DL_GPIO_clearInterruptStatus(PORTB_PORT, PORTB_RIGHT_PULSE_PIN);
		}		
		
		if(DL_GPIO_getEnabledInterruptStatus(PORTB_PORT, PORTB_LEFT_PULSE_PIN))
		{
			QEI1_IRQHandler();
			DL_GPIO_clearInterruptStatus(PORTB_PORT, PORTB_LEFT_PULSE_PIN);
		}
		DL_Interrupt_clearGroup(DL_INTERRUPT_GROUP_1, DL_INTERRUPT_GROUP1_GPIOB);		
	}
}
*/