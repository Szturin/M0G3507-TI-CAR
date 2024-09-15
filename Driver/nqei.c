#include "nqei.h"

encoder NEncoder={
	.left_motor_period_ms=20,
	.right_motor_period_ms=20,
};
systime timer_qei1,timer_qei0;

motor_config trackless_motor={
	.left_encoder_dir_config=left_motor_encoder_dir_default,	        //??????????????
	.right_encoder_dir_config=right_motor_encoder_dir_default,          //??????????????
	.left_motion_dir_config=left_motion_dir_default,					//???????????????
	.right_motion_dir_config=right_motion_dir_default,				    //???????????????
	.wheel_radius_cm=tire_radius_cm_default,							//????????
	.pulse_num_per_circle=pulse_cnt_per_circle_default,				    //????????????????????????????
};


/***************************************
??????:	void QEI0_IRQHandler(void)
???: QEI0?��????????
????:	??
????:	??
???:	??
????:	????????
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
??????:	void QEI1_IRQHandler(void)
???: QEI1?��????????
????:	??
????:	??
???:	??
????:	????????
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
??????:	void get_left_motor_speed(void)
???: ??????????????????
????:	??
????:	??
???:	????��????????????,?????rpm??cm/s
????:	????????
***************************************/
float get_left_motor_speed(void)
{
	static uint16_t cnt1=0;cnt1++;
    float period_temp;
	if(cnt1>=4)
	{
		cnt1=0;
		NEncoder.left_motor_period_ms=20;
		//????????????????
		NEncoder.left_motor_speed_rpm=60*(NEncoder.left_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.left_motor_period_ms*0.001f);	
		NEncoder.left_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.left_motor_speed_rpm/60.0f);

       //period_temp = NEncoder.left_motor_period_cnt;
		NEncoder.left_motor_period_cnt=0;

	}
   // period_temp = NEncoder.left_motor_period_cnt;
   // NEncoder.left_motor_period_cnt=0;
	return NEncoder.left_motor_speed_rpm;
   // return period_temp;
}

/***************************************
??????:	void get_right_motor_speed(void)
???: ?????????????????
????:	??
????:	??
???:	????��????????????,?????rpm??cm/s
????:	????????
***************************************/
float get_right_motor_speed(void)
{
	static uint16_t cnt2=0;cnt2++;
    float period_temp;
	if(cnt2>=4)
	{
		cnt2=0;
		NEncoder.right_motor_period_ms=20;
		//????????????????
		NEncoder.right_motor_speed_rpm=60*(NEncoder.right_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.right_motor_period_ms*0.001f);
		NEncoder.right_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.right_motor_speed_rpm/60);
		//period_temp = NEncoder.right_motor_period_cnt;
		NEncoder.right_motor_period_cnt=0;	
	}
    //period_temp = NEncoder.right_motor_period_cnt;
    //NEncoder.right_motor_period_cnt=0;
    //return period_temp;
	return NEncoder.right_motor_speed_rpm;
   // return period_temp;
}

/***************************************
??????:	void motor_total_cnt_reset(void)
???: ????????????��
????:	??
????:	??
???:	??
????:	????????
***************************************/
void motor_total_cnt_reset(void)
{
	NEncoder.left_motor_total_cnt =0;
	NEncoder.right_motor_total_cnt=0;
}


void BP_Encoder_Init()
{
	NVIC_ClearPendingIRQ(PORTB_INT_IRQN);
	//NVIC_EnableIRQ(PORTA_INT_IRQN);//GPIOA?��????
	NVIC_EnableIRQ(PORTB_INT_IRQN);//GPIOB?��????
}




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
