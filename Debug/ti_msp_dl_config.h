/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_MOTOR */
#define PWM_MOTOR_INST                                                     TIMA0
#define PWM_MOTOR_INST_IRQHandler                               TIMA0_IRQHandler
#define PWM_MOTOR_INST_INT_IRQN                                 (TIMA0_INT_IRQn)
#define PWM_MOTOR_INST_CLK_FREQ                                         80000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_MOTOR_C0_PORT                                             GPIOB
#define GPIO_PWM_MOTOR_C0_PIN                                      DL_GPIO_PIN_8
#define GPIO_PWM_MOTOR_C0_IOMUX                                  (IOMUX_PINCM25)
#define GPIO_PWM_MOTOR_C0_IOMUX_FUNC                 IOMUX_PINCM25_PF_TIMA0_CCP0
#define GPIO_PWM_MOTOR_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_MOTOR_C1_PORT                                             GPIOB
#define GPIO_PWM_MOTOR_C1_PIN                                      DL_GPIO_PIN_9
#define GPIO_PWM_MOTOR_C1_IOMUX                                  (IOMUX_PINCM26)
#define GPIO_PWM_MOTOR_C1_IOMUX_FUNC                 IOMUX_PINCM26_PF_TIMA0_CCP1
#define GPIO_PWM_MOTOR_C1_IDX                                DL_TIMER_CC_1_INDEX



/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMG0)
#define TIMER_1_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                          (1999U)
/* Defines for TIMER_2 */
#define TIMER_2_INST                                                     (TIMG7)
#define TIMER_2_INST_IRQHandler                                 TIMG7_IRQHandler
#define TIMER_2_INST_INT_IRQN                                   (TIMG7_INT_IRQn)
#define TIMER_2_INST_LOAD_VALUE                                          (1999U)




/* Defines for I2C_OLED */
#define I2C_OLED_INST                                                       I2C1
#define I2C_OLED_INST_IRQHandler                                 I2C1_IRQHandler
#define I2C_OLED_INST_INT_IRQN                                     I2C1_INT_IRQn
#define I2C_OLED_BUS_SPEED_HZ                                             400000
#define GPIO_I2C_OLED_SDA_PORT                                             GPIOA
#define GPIO_I2C_OLED_SDA_PIN                                     DL_GPIO_PIN_30
#define GPIO_I2C_OLED_IOMUX_SDA                                   (IOMUX_PINCM5)
#define GPIO_I2C_OLED_IOMUX_SDA_FUNC                    IOMUX_PINCM5_PF_I2C1_SDA
#define GPIO_I2C_OLED_SCL_PORT                                             GPIOA
#define GPIO_I2C_OLED_SCL_PIN                                     DL_GPIO_PIN_29
#define GPIO_I2C_OLED_IOMUX_SCL                                   (IOMUX_PINCM4)
#define GPIO_I2C_OLED_IOMUX_SCL_FUNC                    IOMUX_PINCM4_PF_I2C1_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_SOC */
#define UART_SOC_INST                                                      UART1
#define UART_SOC_INST_IRQHandler                                UART1_IRQHandler
#define UART_SOC_INST_INT_IRQN                                    UART1_INT_IRQn
#define GPIO_UART_SOC_RX_PORT                                              GPIOA
#define GPIO_UART_SOC_TX_PORT                                              GPIOA
#define GPIO_UART_SOC_RX_PIN                                       DL_GPIO_PIN_9
#define GPIO_UART_SOC_TX_PIN                                       DL_GPIO_PIN_8
#define GPIO_UART_SOC_IOMUX_RX                                   (IOMUX_PINCM20)
#define GPIO_UART_SOC_IOMUX_TX                                   (IOMUX_PINCM19)
#define GPIO_UART_SOC_IOMUX_RX_FUNC                    IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_SOC_IOMUX_TX_FUNC                    IOMUX_PINCM19_PF_UART1_TX
#define UART_SOC_BAUD_RATE                                              (115200)
#define UART_SOC_IBRD_40_MHZ_115200_BAUD                                    (21)
#define UART_SOC_FBRD_40_MHZ_115200_BAUD                                    (45)
/* Defines for UART_Screen */
#define UART_Screen_INST                                                   UART2
#define UART_Screen_INST_IRQHandler                             UART2_IRQHandler
#define UART_Screen_INST_INT_IRQN                                 UART2_INT_IRQn
#define GPIO_UART_Screen_RX_PORT                                           GPIOA
#define GPIO_UART_Screen_TX_PORT                                           GPIOA
#define GPIO_UART_Screen_RX_PIN                                   DL_GPIO_PIN_22
#define GPIO_UART_Screen_TX_PIN                                   DL_GPIO_PIN_21
#define GPIO_UART_Screen_IOMUX_RX                                (IOMUX_PINCM47)
#define GPIO_UART_Screen_IOMUX_TX                                (IOMUX_PINCM46)
#define GPIO_UART_Screen_IOMUX_RX_FUNC                 IOMUX_PINCM47_PF_UART2_RX
#define GPIO_UART_Screen_IOMUX_TX_FUNC                 IOMUX_PINCM46_PF_UART2_TX
#define UART_Screen_BAUD_RATE                                           (115200)
#define UART_Screen_IBRD_40_MHZ_115200_BAUD                                 (21)
#define UART_Screen_FBRD_40_MHZ_115200_BAUD                                 (45)
/* Defines for UART_JY61P */
#define UART_JY61P_INST                                                    UART3
#define UART_JY61P_INST_IRQHandler                              UART3_IRQHandler
#define UART_JY61P_INST_INT_IRQN                                  UART3_INT_IRQn
#define GPIO_UART_JY61P_RX_PORT                                            GPIOB
#define GPIO_UART_JY61P_TX_PORT                                            GPIOB
#define GPIO_UART_JY61P_RX_PIN                                     DL_GPIO_PIN_3
#define GPIO_UART_JY61P_TX_PIN                                     DL_GPIO_PIN_2
#define GPIO_UART_JY61P_IOMUX_RX                                 (IOMUX_PINCM16)
#define GPIO_UART_JY61P_IOMUX_TX                                 (IOMUX_PINCM15)
#define GPIO_UART_JY61P_IOMUX_RX_FUNC                  IOMUX_PINCM16_PF_UART3_RX
#define GPIO_UART_JY61P_IOMUX_TX_FUNC                  IOMUX_PINCM15_PF_UART3_TX
#define UART_JY61P_BAUD_RATE                                            (115200)
#define UART_JY61P_IBRD_80_MHZ_115200_BAUD                                  (43)
#define UART_JY61P_FBRD_80_MHZ_115200_BAUD                                  (26)





/* Port definition for Pin Group GPIO_BEEP */
#define GPIO_BEEP_PORT                                                   (GPIOB)

/* Defines for PIN_1: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_BEEP_PIN_1_PIN                                     (DL_GPIO_PIN_27)
#define GPIO_BEEP_PIN_1_IOMUX                                    (IOMUX_PINCM58)
/* Port definition for Pin Group GPIO_SWITCH */
#define GPIO_SWITCH_PORT                                                 (GPIOB)

/* Defines for USER_SWITCH_1: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIO_SWITCH_USER_SWITCH_1_PIN                           (DL_GPIO_PIN_21)
#define GPIO_SWITCH_USER_SWITCH_1_IOMUX                          (IOMUX_PINCM49)
/* Defines for USER_SWITCH_3: GPIOB.23 with pinCMx 51 on package pin 22 */
#define GPIO_SWITCH_USER_SWITCH_3_PIN                           (DL_GPIO_PIN_23)
#define GPIO_SWITCH_USER_SWITCH_3_IOMUX                          (IOMUX_PINCM51)
/* Defines for USER_SWITCH_2: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GPIO_SWITCH_USER_SWITCH_2_PIN                           (DL_GPIO_PIN_24)
#define GPIO_SWITCH_USER_SWITCH_2_IOMUX                          (IOMUX_PINCM52)
/* Defines for PIN_FL2: GPIOA.7 with pinCMx 14 on package pin 49 */
#define GPIO_MOTOR_PIN_FL2_PORT                                          (GPIOA)
#define GPIO_MOTOR_PIN_FL2_PIN                                   (DL_GPIO_PIN_7)
#define GPIO_MOTOR_PIN_FL2_IOMUX                                 (IOMUX_PINCM14)
/* Defines for PIN_FL1: GPIOA.4 with pinCMx 9 on package pin 44 */
#define GPIO_MOTOR_PIN_FL1_PORT                                          (GPIOA)
#define GPIO_MOTOR_PIN_FL1_PIN                                   (DL_GPIO_PIN_4)
#define GPIO_MOTOR_PIN_FL1_IOMUX                                  (IOMUX_PINCM9)
/* Defines for PIN_FR1: GPIOA.3 with pinCMx 8 on package pin 43 */
#define GPIO_MOTOR_PIN_FR1_PORT                                          (GPIOA)
#define GPIO_MOTOR_PIN_FR1_PIN                                   (DL_GPIO_PIN_3)
#define GPIO_MOTOR_PIN_FR1_IOMUX                                  (IOMUX_PINCM8)
/* Defines for PIN_FR2: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_MOTOR_PIN_FR2_PORT                                          (GPIOB)
#define GPIO_MOTOR_PIN_FR2_PIN                                  (DL_GPIO_PIN_14)
#define GPIO_MOTOR_PIN_FR2_IOMUX                                 (IOMUX_PINCM31)
/* Defines for PIN_FSTBY: GPIOB.19 with pinCMx 45 on package pin 16 */
#define GPIO_MOTOR_PIN_FSTBY_PORT                                        (GPIOB)
#define GPIO_MOTOR_PIN_FSTBY_PIN                                (DL_GPIO_PIN_19)
#define GPIO_MOTOR_PIN_FSTBY_IOMUX                               (IOMUX_PINCM45)
/* Defines for PIN_TRACE_0: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_TRACE_PIN_TRACE_0_PORT                                      (GPIOB)
#define GPIO_TRACE_PIN_TRACE_0_PIN                              (DL_GPIO_PIN_13)
#define GPIO_TRACE_PIN_TRACE_0_IOMUX                             (IOMUX_PINCM30)
/* Defines for PIN_TRACE_1: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_TRACE_PIN_TRACE_1_PORT                                      (GPIOB)
#define GPIO_TRACE_PIN_TRACE_1_PIN                              (DL_GPIO_PIN_15)
#define GPIO_TRACE_PIN_TRACE_1_IOMUX                             (IOMUX_PINCM32)
/* Defines for PIN_TRACE_2: GPIOB.16 with pinCMx 33 on package pin 4 */
#define GPIO_TRACE_PIN_TRACE_2_PORT                                      (GPIOB)
#define GPIO_TRACE_PIN_TRACE_2_PIN                              (DL_GPIO_PIN_16)
#define GPIO_TRACE_PIN_TRACE_2_IOMUX                             (IOMUX_PINCM33)
/* Defines for PIN_TRACE_3: GPIOA.12 with pinCMx 34 on package pin 5 */
#define GPIO_TRACE_PIN_TRACE_3_PORT                                      (GPIOA)
#define GPIO_TRACE_PIN_TRACE_3_PIN                              (DL_GPIO_PIN_12)
#define GPIO_TRACE_PIN_TRACE_3_IOMUX                             (IOMUX_PINCM34)
/* Defines for PIN_TRACE_4: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_TRACE_PIN_TRACE_4_PORT                                      (GPIOA)
#define GPIO_TRACE_PIN_TRACE_4_PIN                              (DL_GPIO_PIN_13)
#define GPIO_TRACE_PIN_TRACE_4_IOMUX                             (IOMUX_PINCM35)
/* Defines for PIN_TRACE_5: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_TRACE_PIN_TRACE_5_PORT                                      (GPIOA)
#define GPIO_TRACE_PIN_TRACE_5_PIN                              (DL_GPIO_PIN_14)
#define GPIO_TRACE_PIN_TRACE_5_IOMUX                             (IOMUX_PINCM36)
/* Defines for PIN_TRACE_6: GPIOA.15 with pinCMx 37 on package pin 8 */
#define GPIO_TRACE_PIN_TRACE_6_PORT                                      (GPIOA)
#define GPIO_TRACE_PIN_TRACE_6_PIN                              (DL_GPIO_PIN_15)
#define GPIO_TRACE_PIN_TRACE_6_IOMUX                             (IOMUX_PINCM37)
/* Defines for PIN_TRACE_7: GPIOA.16 with pinCMx 38 on package pin 9 */
#define GPIO_TRACE_PIN_TRACE_7_PORT                                      (GPIOA)
#define GPIO_TRACE_PIN_TRACE_7_PIN                              (DL_GPIO_PIN_16)
#define GPIO_TRACE_PIN_TRACE_7_IOMUX                             (IOMUX_PINCM38)
/* Port definition for Pin Group GPIO_GW */
#define GPIO_GW_PORT                                                     (GPIOA)

/* Defines for GPIO_SCL: GPIOA.1 with pinCMx 2 on package pin 34 */
#define GPIO_GW_GPIO_SCL_PIN                                     (DL_GPIO_PIN_1)
#define GPIO_GW_GPIO_SCL_IOMUX                                    (IOMUX_PINCM2)
/* Defines for GPIO_SDA: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_GW_GPIO_SDA_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_GW_GPIO_SDA_IOMUX                                    (IOMUX_PINCM1)
/* Port definition for Pin Group PORTB */
#define PORTB_PORT                                                       (GPIOB)

/* Defines for RIGHT_PULSE: GPIOB.4 with pinCMx 17 on package pin 52 */
// pins affected by this interrupt request:["RIGHT_PULSE","LEFT_PULSE"]
#define PORTB_INT_IRQN                                          (GPIOB_INT_IRQn)
#define PORTB_INT_IIDX                          (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define PORTB_RIGHT_PULSE_IIDX                               (DL_GPIO_IIDX_DIO4)
#define PORTB_RIGHT_PULSE_PIN                                    (DL_GPIO_PIN_4)
#define PORTB_RIGHT_PULSE_IOMUX                                  (IOMUX_PINCM17)
/* Defines for LEFT_PULSE: GPIOB.5 with pinCMx 18 on package pin 53 */
#define PORTB_LEFT_PULSE_IIDX                                (DL_GPIO_IIDX_DIO5)
#define PORTB_LEFT_PULSE_PIN                                     (DL_GPIO_PIN_5)
#define PORTB_LEFT_PULSE_IOMUX                                   (IOMUX_PINCM18)
/* Defines for RIGHT_DIR: GPIOB.6 with pinCMx 23 on package pin 58 */
#define PORTB_RIGHT_DIR_PIN                                      (DL_GPIO_PIN_6)
#define PORTB_RIGHT_DIR_IOMUX                                    (IOMUX_PINCM23)
/* Defines for LEFT_DIR: GPIOB.7 with pinCMx 24 on package pin 59 */
#define PORTB_LEFT_DIR_PIN                                       (DL_GPIO_PIN_7)
#define PORTB_LEFT_DIR_IOMUX                                     (IOMUX_PINCM24)
/* Defines for A: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Encoder_A_PORT                                                   (GPIOA)
#define Encoder_A_PIN                                           (DL_GPIO_PIN_17)
#define Encoder_A_IOMUX                                          (IOMUX_PINCM39)
/* Defines for B: GPIOA.18 with pinCMx 40 on package pin 11 */
#define Encoder_B_PORT                                                   (GPIOA)
#define Encoder_B_PIN                                           (DL_GPIO_PIN_18)
#define Encoder_B_IOMUX                                          (IOMUX_PINCM40)
/* Defines for C: GPIOB.17 with pinCMx 43 on package pin 14 */
#define Encoder_C_PORT                                                   (GPIOB)
#define Encoder_C_PIN                                           (DL_GPIO_PIN_17)
#define Encoder_C_IOMUX                                          (IOMUX_PINCM43)
/* Defines for D: GPIOB.18 with pinCMx 44 on package pin 15 */
#define Encoder_D_PORT                                                   (GPIOB)
#define Encoder_D_PIN                                           (DL_GPIO_PIN_18)
#define Encoder_D_IOMUX                                          (IOMUX_PINCM44)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_MOTOR_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_TIMER_2_init(void);
void SYSCFG_DL_I2C_OLED_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_SOC_init(void);
void SYSCFG_DL_UART_Screen_init(void);
void SYSCFG_DL_UART_JY61P_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
