/**************************************************************************//**
 * @file     mhscpu_sysctrl.h
 * @brief    
 * @version  V1.00
 * @date     11. April 2015
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
#ifndef __MHSCPU_SYSCTRL_H
#define __MHSCPU_SYSCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mhscpu.h"
#include "mhscpu_dma.h"

#define SYSCTRL_FREQ_SEL_POWERMODE_Pos					(16)
#define SYSCTRL_FREQ_SEL_POWERMODE_Mask			        (0x07 << SYSCTRL_FREQ_SEL_POWERMODE_Pos)	 
#define SYSCTRL_FREQ_SEL_POWERMODE_CLOSE_CPU		    (0x00 << SYSCTRL_FREQ_SEL_POWERMODE_Pos) 
#define SYSCTRL_FREQ_SEL_POWERMODE_CLOSE_XTAL12M        (0x01 << SYSCTRL_FREQ_SEL_POWERMODE_Pos) 
#define SYSCTRL_FREQ_SEL_POWERMODE_CLCPU_BFCLXTAL12M    (0x02 << SYSCTRL_FREQ_SEL_POWERMODE_Pos) 
	 
#define SYSCTRL_FREQ_SEL_SYSCLKSRC_Pos		        (10)	
#define SYSCTRL_FREQ_SEL_SYSCLKSRC_Mask			    (0x01 << SYSCTRL_FREQ_SEL_SYSCLKSRC_Pos)	 
#define SYSCTRL_FREQ_SYSCLKSRC_BFPLL				(0x00 << SYSCTRL_FREQ_SEL_SYSCLKSRC_Pos)	 
#define SYSCTRL_FREQ_SYSCLKSRC_AFPLL				(0x01 << SYSCTRL_FREQ_SEL_SYSCLKSRC_Pos)	

#define SYSCTRL_FREQ_SEL_CLKSRC_Pos				    (9)	 
#define SYSCTRL_FREQ_SEL_CLKSRC_Mask			    (0x01 << SYSCTRL_FREQ_SEL_CLKSRC_Pos)	 
#define SYSCTRL_FREQ_CLKSRC_EXT_XTAL				(0x00 << SYSCTRL_FREQ_SEL_CLKSRC_Pos)	 
#define SYSCTRL_FREQ_CLKSRC_CHIP_SOC				(0x01 << SYSCTRL_FREQ_SEL_CLKSRC_Pos)	
	 
#define SYSCTRL_FREQ_SEL_XTAL_Pos					(4)
#define SYSCTRL_FREQ_SEL_XTAL_Mask					(0x03 << SYSCTRL_FREQ_SEL_XTAL_Pos)
#define SYSCTRL_FREQ_SEL_XTAL_72Mhz					(0x00 << SYSCTRL_FREQ_SEL_XTAL_Pos)
#define SYSCTRL_FREQ_SEL_XTAL_96Mhz					(0x01 << SYSCTRL_FREQ_SEL_XTAL_Pos)
#define SYSCTRL_FREQ_SEL_XTAL_108Mhz				(0x02 << SYSCTRL_FREQ_SEL_XTAL_Pos)
#define SYSCTRL_FREQ_SEL_XTAL_120Mhz				(0x03 << SYSCTRL_FREQ_SEL_XTAL_Pos)	 
	 
#define SYSCTRL_FREQ_SEL_HCLK_DIV_EN				((uint32_t)0x0008)	 
	 
#define SYSCTRL_FREQ_SEL_HCLK_DIV_Pos				(2)
#define SYSCTRL_FREQ_SEL_HCLK_DIV_Mask				(0x01 << SYSCTRL_FREQ_SEL_HCLK_DIV_Pos)
#define SYSCTRL_FREQ_SEL_HCLK_DIV_1_2				(0x00 << SYSCTRL_FREQ_SEL_HCLK_DIV_Pos)
#define SYSCTRL_FREQ_SEL_HCLK_DIV_1_4				(0x01 << SYSCTRL_FREQ_SEL_HCLK_DIV_Pos)

#define SYSCTRL_FREQ_SEL_PCLK_DIV_Pos				(0)
#define SYSCTRL_FREQ_SEL_PCLK_DIV_Mask				(0x01 << SYSCTRL_FREQ_SEL_PCLK_DIV_Pos)
#define SYSCTRL_FREQ_SEL_PCLK_DIV_1_2				(0x00 << SYSCTRL_FREQ_SEL_PCLK_DIV_Pos)
#define SYSCTRL_FREQ_SEL_PCLK_DIV_1_4		 		(0x01 << SYSCTRL_FREQ_SEL_PCLK_DIV_Pos)

//#define SYSCTRL_POWER_CTRL_DEEPSLEEP_KEY			(0x55 << 8)
//#define SYSCTRL_POWER_CTRL_CPUSLEEP_KEY				(0x37)

/* AHB 总线外设 */
#define SYSCTRL_AHBPeriph_DMA							((uint32_t)0x20000000)
#define SYSCTRL_AHBPeriph_USB							((uint32_t)0x10000000)
#define SYSCTRL_AHBPeriph_LCD							((uint32_t)0x00000002)
#define SYSCTRL_AHBPeriph_CRYPT						    ((uint32_t)0x00000001)
#define IS_SYSCTRL_AHB_PERIPH(PERIPH)                   ((((PERIPH) & SYSCTRL_AHBPeriph_DMA) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_USB) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_LCD) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_CRYPT) != 0x00))
 
/* APB 总线外设 */
#define SYSCTRL_APBPeriph_TRNG							((uint32_t)0x80000000)
#define SYSCTRL_APBPeriph_ADC							((uint32_t)0x40000000)
#define SYSCTRL_APBPeriph_CRC							((uint32_t)0x20000000)
#define SYSCTRL_APBPeriph_KCU_OSC32K					((uint32_t)0x10000000)
#define SYSCTRL_APBPeriph_KCU							((uint32_t)0x08000000)
#define SYSCTRL_APBPeriph_BPU							((uint32_t)0x04000000)
#define SYSCTRL_APBPeriph_TIMM0							((uint32_t)0x00200000)
#define SYSCTRL_APBPeriph_GPIO							((uint32_t)0x00100000)
#define SYSCTRL_APBPeriph_I2C0							((uint32_t)0x00040000)
#define SYSCTRL_APBPeriph_SCI2							((uint32_t)0x00010000)
#define SYSCTRL_APBPeriph_SCI1							((uint32_t)0x00008000)
#define SYSCTRL_APBPeriph_SCI0							((uint32_t)0x00004000)
#define SYSCTRL_APBPeriph_SPI3							((uint32_t)0x00000800)
#define SYSCTRL_APBPeriph_SPI2							((uint32_t)0x00000400)
#define SYSCTRL_APBPeriph_SPI1							((uint32_t)0x00000200)
#define SYSCTRL_APBPeriph_SPI0							((uint32_t)0x00000100)
#define SYSCTRL_APBPeriph_UART3						    ((uint32_t)0x00000008)
#define SYSCTRL_APBPeriph_UART2						    ((uint32_t)0x00000004)
#define SYSCTRL_APBPeriph_UART1						    ((uint32_t)0x00000002)
#define SYSCTRL_APBPeriph_UART0						    ((uint32_t)0x00000001)
#define IS_SYSCTRL_APB_PERIPH(PERIPH)                   ((((PERIPH) & 0xFC75CF0F) != 0x00) && ((PERIPH) != 0x00))

/* SOFT_RST2*/
#define SYSCTRL_GLB_RESET                               ((uint32_t)0x80000000)
#define SYSCTRL_CM3_RESET                               ((uint32_t)0x40000000)
#define IS_SYSCTRL_AHB_PERIPH_RESET(PERIPH)             ((((PERIPH) & SYSCTRL_AHBPeriph_DMA) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_USB) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_LCD) != 0x00) ||  \
                                                         (((PERIPH) & SYSCTRL_AHBPeriph_CRYPT) != 0x00) || \
                                                         (((PERIPH) & SYSCTRL_GLB_RESET) != 0x00) || \
                                                         (((PERIPH) & SYSCTRL_CM3_RESET) != 0x00))
                                                         
                                                         
#define IS_SYSCTRL_APB_PERIPH_RESET(PERIPH)             ((((PERIPH) & 0xFC75CF0F) != 0x00) && ((PERIPH) != 0x00))                                                      

/* Wake_EN1 */
#define SYSCTRL_WakeInt1_EXTI_INT5					(BIT25)
#define SYSCTRL_WakeInt1_EXTI_INT4					(BIT24)
#define SYSCTRL_WakeInt1_EXTI_INT3					(BIT23)
#define SYSCTRL_WakeInt1_EXTI_INT2					(BIT22)
#define SYSCTRL_WakeInt1_EXTI_INT1					(BIT21)
#define SYSCTRL_WakeInt1_EXTI_INT0					(BIT20)
#define SYSCTRL_WakeInt1_EXTI_ALL					(BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20)
#define SYSCTRL_WakeInt1_I2C0						(BIT18)
#define SYSCTRL_WakeInt1_SCI2						(BIT16)
#define SYSCTRL_WakeInt1_SCI1						(BIT15)
#define SYSCTRL_WakeInt1_SCI0						(BIT14)
#define SYSCTRL_WakeInt1_SPI2						(BIT10)
#define SYSCTRL_WakeInt1_SPI1						(BIT9)
#define SYSCTRL_WakeInt1_SPI0						(BIT8)
#define SYSCTRL_WakeInt1_UART3						(BIT3)
#define SYSCTRL_WakeInt1_UART2						(BIT2)
#define SYSCTRL_WakeInt1_UART1						(BIT1)
#define SYSCTRL_WakeInt1_UART0						(BIT0)
#define SYSCTRL_WakeInt1_ALL						(0x3F5C70F)
/* Wake_EN2 */
#define SYSCTRL_WakeInt2_TIM0_5						(BIT13)
#define SYSCTRL_WakeInt2_TIM0_4						(BIT12)
#define SYSCTRL_WakeInt2_TIM0_3						(BIT11)
#define SYSCTRL_WakeInt2_TIM0_2						(BIT10)
#define SYSCTRL_WakeInt2_TIM0_1						(BIT9)
#define SYSCTRL_WakeInt2_TIM0_0						(BIT8)
#define SYSCTRL_WakeInt2_USB						(BIT3)
#define SYSCTRL_WakeInt2_RTC						(BIT2)
#define SYSCTRL_WakeInt2_KEYBOARD					(BIT1)
/* 仅存在于Wake_Flag2寄存器 */
#define SYSCTRL_WakeInt2_SENSOR						(BIT0)
#define SYSCTRL_WakeInt2_ALL						(0x3F0F)
/**
  * @}
  */

/** @defgroup SYSCTRL_Exported_Types
  * @{
  */

typedef struct
{
  uint32_t PLL_Frequency;     /*!< returns PLL frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK frequency expressed in Hz */
  uint32_t PCLK_Frequency;    /*!< returns PCLK frequency expressed in Hz */
}SYSCTRL_ClocksTypeDef;

/**
  * @}
  */


typedef enum
{
	SleepMode_CloseCPU = 0,
	DeepSleepMode_FCLCPU_THCLXTAL12M = 2
}SleepMode_TypeDef;
#define IS_ALL_SLEEP_MODE(MODE)						(((MODE) == SleepMode_CloseCPU) || \
													 ((MODE) == DeepSleepMode_FCLCPU_THCLXTAL12M)) 
typedef enum 
{
    SELECT_EXT12M,
    SELECT_INC12M
} SYSCLK_SOURCE_TypeDef;
#define IS_SYSCLK_SOURCE(FREQ)						(((FREQ) == SELECT_EXT12M) || \
													((FREQ) == SELECT_INC12M))
typedef enum
{
	SYSCTRL_PLL_120MHz     = (uint32_t)0x01,
	SYSCTRL_PLL_108MHz     = (uint32_t)0x02,
	SYSCTRL_PLL_96MHz      = (uint32_t)0x03,
	SYSCTRL_PLL_72MHz      = (uint32_t)0x04
}SYSCTRL_PLL_TypeDef;
#define IS_PLL_FREQ(FREQ)							(((FREQ) == SYSCTRL_PLL_120MHz) || \
													((FREQ) == SYSCTRL_PLL_108MHz) || \
													((FREQ) == SYSCTRL_PLL_96MHz) || \
													((FREQ) == SYSCTRL_PLL_72MHz))

#define SYSCTRL_HCLK_Div_None					    ((uint32_t)0x00)
#define SYSCTRL_HCLK_Div2							((uint32_t)0x01)
#define SYSCTRL_HCLK_Div4							((uint32_t)0x02)
#define IS_GET_SYSCTRL_HCLK_DIV(DIV)				(((DIV) == SYSCTRL_HCLK_Div_None) || \
													((DIV) == SYSCTRL_HCLK_Div2) || \
													((DIV) == SYSCTRL_HCLK_Div4)) 

#define SYSCTRL_PCLK_Div2							((uint32_t)0x01)
#define SYSCTRL_PCLK_Div4							((uint32_t)0x02)
#define IS_GET_SYSCTRL_PCLK_DIV(DIV)				(((DIV) == SYSCTRL_PCLK_Div2) || \
													((DIV) == SYSCTRL_PCLK_Div4)) 
													


/********************* System control APIs ***************************************/
void SYSCTRL_AHBPeriphClockCmd(uint32_t SYSCTRL_AHBPeriph, FunctionalState NewState);
void SYSCTRL_AHBPeriphResetCmd(uint32_t SYSCTRL_AHBPeriph, FunctionalState NewState);
void SYSCTRL_APBPeriphClockCmd(uint32_t SYSCTRL_APBPeriph, FunctionalState NewState);
void SYSCTRL_APBPeriphResetCmd(uint32_t SYSCTRL_APBPeriph, FunctionalState NewState);

void SYSCTRL_EnterSleep(SleepMode_TypeDef SleepMode);
void SYSCTRL_WakeIntDeInit(void);
void SYSCTRL_WakeUpInt1Config(uint32_t SYSCTRL_WakeInt, FunctionalState NewState);
void SYSCTRL_WakeUpInt2Config(uint32_t SYSCTRL_WakeInt, FunctionalState NewState);
uint32_t SYSCTRL_GetWakeupFlag1Status(void);
uint32_t SYSCTRL_GetWakeupFlag2Status(void);

void SYSCTRL_SYSCLKSourceSelect(SYSCLK_SOURCE_TypeDef source_select);
void SYSCTRL_PLLConfig(SYSCTRL_PLL_TypeDef PLL_Freq);
void SYSCTRL_HCLKConfig(uint32_t HCLK_Div);
void SYSCTRL_PCLKConfig(uint32_t PCLK_Div);

void SYSCTRL_GetClocksFreq(SYSCTRL_ClocksTypeDef* SYSCTRL_Clocks);

#ifdef __cplusplus
}
#endif

#endif 

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
