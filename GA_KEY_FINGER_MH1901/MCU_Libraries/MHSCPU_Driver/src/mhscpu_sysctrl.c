/**************************************************************************//**
 * @file     mhscpu_sysctrl.c
 * @brief    This file provides all the system firmware function.
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
#include "mhscpu_sysctrl.h"
#include "mhscpu_exti.h"


#define SYSCTRL_PHER_CTRL_SPI0_SLV_EN				(BIT24)		/* 0:MASTER  1:SLAVE */
#define SYSCTRL_PHER_CTRL_SPI1_SLV_EN				(BIT25)		/* 0:MASTER  1:SLAVE */
#define SYSCTRL_PHER_CTRL_SPI2_SLV_EN				(BIT26)		/* 0:MASTER  1:SLAVE */

/**
  * @brief  Enables or disables the AHB peripheral clock.
  * @param  SYSCTRL_AHBPeriph: specifies the AHB peripheral to gates its clock.
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_AHBPeriph_DMA
  *     @arg SYSCTRL_AHBPeriph_USB
  *     @arg SYSCTRL_AHBPeriph_LCD
  *     @arg SYSCTRL_AHBPeriph_CRYPT
  *
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCTRL_AHBPeriphClockCmd(uint32_t SYSCTRL_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SYSCTRL_AHB_PERIPH(SYSCTRL_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState != DISABLE)
	{
		SYSCTRL->CG_CTRL2 |= SYSCTRL_AHBPeriph;
	}
	else
	{
		SYSCTRL->CG_CTRL2 &= ~SYSCTRL_AHBPeriph;
	}
}

/**
  * @brief  Enables or disables the APB peripheral clock.
  * @param  SYSCTRL_APBPeriph: specifies the APB peripheral to gates its clock.
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_APBPeriph_TRNG
  *     @arg SYSCTRL_APBPeriph_ADC
  *     @arg SYSCTRL_APBPeriph_CRC
  *     @arg SYSCTRL_APBPeriph_KCU_OSC32K
  *     @arg SYSCTRL_APBPeriph_KCU
  *     @arg SYSCTRL_APBPeriph_BPU
  *     @arg SYSCTRL_APBPeriph_TIMM0
  *     @arg SYSCTRL_APBPeriph_GPIO
  *     @arg SYSCTRL_APBPeriph_I2C0
  *     @arg SYSCTRL_APBPeriph_SCI2
  *     @arg SYSCTRL_APBPeriph_SCI1
  *     @arg SYSCTRL_APBPeriph_SCI0
  *     @arg SYSCTRL_APBPeriph_SPI2
  *     @arg SYSCTRL_APBPeriph_SPI1
  *     @arg SYSCTRL_APBPeriph_SPI0
  *     @arg SYSCTRL_APBPeriph_UART3
  *     @arg SYSCTRL_APBPeriph_UART2
  *     @arg SYSCTRL_APBPeriph_UART1
  *     @arg SYSCTRL_APBPeriph_UART0
  *
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCTRL_APBPeriphClockCmd(uint32_t SYSCTRL_APBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SYSCTRL_APB_PERIPH(SYSCTRL_APBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
	if (NewState != DISABLE)
	{
		SYSCTRL->CG_CTRL1 |= SYSCTRL_APBPeriph;
	}
	else
	{
		SYSCTRL->CG_CTRL1 &= ~SYSCTRL_APBPeriph;
	}
}

/**
  * @brief  Enables or disables the AHB peripheral reset.
  * @param  SYSCTRL_AHBPeriph: specifies the AHB peripheral to reset(CM3 reset/GLB Reset).
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_AHBPeriph_DMA
  *     @arg SYSCTRL_AHBPeriph_USB
  *     @arg SYSCTRL_AHBPeriph_LCD
  *     @arg SYSCTRL_AHBPeriph_CRYPT
  *     @arg SYSCTRL_GLB_RESET
  *     @arg SYSCTRL_CM3_RESET
  *
  * @param  NewState: new state of the specified peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCTRL_AHBPeriphResetCmd(uint32_t SYSCTRL_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SYSCTRL_AHB_PERIPH_RESET(SYSCTRL_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    /* LOCK_R enable reset */
    SYSCTRL->LOCK_R &= ~(SYSCTRL_AHBPeriph & 0xF0000000);
	if (NewState != DISABLE)
	{
		SYSCTRL->SOFT_RST2 |= SYSCTRL_AHBPeriph;
	}
	else
	{
		SYSCTRL->SOFT_RST2 &= ~SYSCTRL_AHBPeriph;
	}
    /* LOCK_R disable reset */
    SYSCTRL->LOCK_R |= (SYSCTRL_AHBPeriph & 0xF0000000);
}
/**
  * @brief  Enables or disables the APB peripheral reset.
  * @param  SYSCTRL_APBPeriph: specifies the APB peripheral to reset.
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_APBPeriph_TRNG
  *     @arg SYSCTRL_APBPeriph_ADC
  *     @arg SYSCTRL_APBPeriph_CRC
  *     @arg SYSCTRL_APBPeriph_KCU_OSC32K
  *     @arg SYSCTRL_APBPeriph_KCU
  *     @arg SYSCTRL_APBPeriph_BPU
  *     @arg SYSCTRL_APBPeriph_TIMM0
  *     @arg SYSCTRL_APBPeriph_GPIO
  *     @arg SYSCTRL_APBPeriph_I2C0
  *     @arg SYSCTRL_APBPeriph_SCI2
  *     @arg SYSCTRL_APBPeriph_SCI1
  *     @arg SYSCTRL_APBPeriph_SCI0
  *     @arg SYSCTRL_APBPeriph_SPI2
  *     @arg SYSCTRL_APBPeriph_SPI1
  *     @arg SYSCTRL_APBPeriph_SPI0
  *     @arg SYSCTRL_APBPeriph_UART3
  *     @arg SYSCTRL_APBPeriph_UART2
  *     @arg SYSCTRL_APBPeriph_UART1
  *     @arg SYSCTRL_APBPeriph_UART0
  *
  * @param  NewState: new state of the specified peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCTRL_APBPeriphResetCmd(uint32_t SYSCTRL_APBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SYSCTRL_APB_PERIPH_RESET(SYSCTRL_APBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
	if (NewState != DISABLE)
	{
		SYSCTRL->SOFT_RST1 |= SYSCTRL_APBPeriph;
	}
	else
	{
		SYSCTRL->SOFT_RST1 &= ~SYSCTRL_APBPeriph;
	}
}

/**
  * @brief  
  * @param  
  * @retval 
  */
void SYSCTRL_EnterSleep(SleepMode_TypeDef SleepMode)
{
	uint32_t rng;
	assert_param(IS_ALL_SLEEP_MODE(SleepMode));

	if ((uint32_t)SleepMode == SleepMode_CloseCPU)
	{
		SYSCTRL->FREQ_SEL &= ~(SYSCTRL_FREQ_SEL_POWERMODE_Mask);
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_POWERMODE_CLOSE_CPU;
		while (!(SYSCTRL->FREQ_SEL & (~SYSCTRL_FREQ_SEL_POWERMODE_CLOSE_CPU)));
#if   defined ( __CC_ARM )
		__ASM{
            CPSID i;
			NOP;
			NOP;
			NOP;
			NOP;
			WFI;
            CPSIE i;
		};
#elif defined ( __ICCARM__ )
   		__asm(
              "CPSID i\n"
			"NOP\n"
			"NOP\n"
			"NOP\n"
			"NOP\n"
			"WFI\n"
               "CPSIE i\n"
		);             
                              
#endif
	} 
	else if ((uint32_t)SleepMode == DeepSleepMode_FCLCPU_THCLXTAL12M)
	{
		rng = TRNG->RNG_AMA;
		TRNG->RNG_AMA = rng | 0xC0000000;
		{
			//没有这个延时随机数可能关不掉
			int i;
			for (i = 0; i < 10; i++);
		}
		
		SYSCTRL->FREQ_SEL &= ~(SYSCTRL_FREQ_SEL_POWERMODE_Mask);
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_POWERMODE_CLCPU_BFCLXTAL12M;
#if   defined ( __CC_ARM )                
		__ASM{
            CPSID i;
			NOP;
			NOP;
			NOP;
			NOP;
			WFI;
            CPSIE i;
		};
#elif defined ( __ICCARM__ )
              __asm(
            "CPSID i\n"
			"NOP\n"
			"NOP\n"
			"NOP\n"
			"NOP\n"
			"WFI\n"
            "CPSIE i\n"
		);
#endif                
		TRNG->RNG_AMA = rng;
	}
}

/**
  * @brief  Get WKUP_FLAG1 register value
  * @param  None 
  * @retval The value of WKUP_FLAG1 register
  */
uint32_t SYSCTRL_GetWakeupFlag1Status(void)
{
 	return (SYSCTRL->WAKE_FLAG1 & 0x03F5C70F);
}
/**
  * @brief  Get WKUP_FLAG2 register value
  * @param  None
  * @retval The value of WKUP_FLAG2 register
  */
uint32_t SYSCTRL_GetWakeupFlag2Status(void)
{
 	return (SYSCTRL->WAKE_FLAG2 & 0x00003F0F);
}
/**
  * @brief  Initialize the WAKE_EN1&WAKE_EN2 registers to their default values.
  * @param  None
  * @retval None
  */
void SYSCTRL_WakeIntDeInit(void)
{
	SYSCTRL->WAKE_EN1 = 0x03F5C70F;
    SYSCTRL->WAKE_EN2 = 0x00003F0E;
}

/**
  * @brief  Configuration wake-up source
  * @param  SYSCTRL_WakeInt: specifies the wakeup source.
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_WakeInt1_EXTI_INT5
  *     @arg SYSCTRL_WakeInt1_EXTI_INT4
  *     @arg SYSCTRL_WakeInt1_EXTI_INT3
  *     @arg SYSCTRL_WakeInt1_EXTI_INT2
  *     @arg SYSCTRL_WakeInt1_EXTI_INT1
  *     @arg SYSCTRL_WakeInt1_EXTI_INT0
  *     @arg SYSCTRL_WakeInt1_EXTI_ALL
  *     @arg SYSCTRL_WakeInt1_I2C0
  *     @arg SYSCTRL_WakeInt1_SCI2
  *     @arg SYSCTRL_WakeInt1_SCI1
  *     @arg SYSCTRL_WakeInt1_SCI0
  *     @arg SYSCTRL_WakeInt1_SPI2
  *     @arg SYSCTRL_WakeInt1_SPI1
  *     @arg SYSCTRL_WakeInt1_SPI0
  *     @arg SYSCTRL_WakeInt1_UART3
  *     @arg SYSCTRL_WakeInt1_UART2
  *     @arg SYSCTRL_WakeInt1_UART1
  *     @arg SYSCTRL_WakeInt1_UART0
  *
  * @param  NewState: new state of the specified wakeup source.
  *   This parameter can be: ENABLE or DISABLE. 
  * @retval None
  */
void SYSCTRL_WakeUpInt1Config(uint32_t SYSCTRL_WakeInt, FunctionalState NewState)
{
	if (DISABLE != NewState)
	{
		SYSCTRL->WAKE_EN1 |= SYSCTRL_WakeInt;
	}
	else
	{
		SYSCTRL->WAKE_EN1 &= ~SYSCTRL_WakeInt;
	}
}

/**
  * @brief  Configuration wake-up source
  * @param  SYSCTRL_WakeInt: specifies the wakeup source.
  *   
  *   For @b this parameter can be any combination
  *   of the following values:        
  *     @arg SYSCTRL_WakeInt2_TIM0_5
  *     @arg SYSCTRL_WakeInt2_TIM0_4
  *     @arg SYSCTRL_WakeInt2_TIM0_3
  *     @arg SYSCTRL_WakeInt2_TIM0_2
  *     @arg SYSCTRL_WakeInt2_TIM0_1
  *     @arg SYSCTRL_WakeInt2_TIM0_0
  *     @arg SYSCTRL_WakeInt2_USB
  *     @arg SYSCTRL_WakeInt2_RTC
  *     @arg SYSCTRL_WakeInt2_KEYBOARD
  *
  * @param  NewState: new state of the specified wakeup source.
  *   This parameter can be: ENABLE or DISABLE. 
  * @retval None
  */
void SYSCTRL_WakeUpInt2Config(uint32_t SYSCTRL_WakeInt, FunctionalState NewState)
{
	if (DISABLE != NewState)
	{
		SYSCTRL->WAKE_EN2 |= SYSCTRL_WakeInt;
	}
	else
	{
		SYSCTRL->WAKE_EN2 &= ~SYSCTRL_WakeInt;
	}
}


/**
  * @brief  Select System clock source
  * @param  source_select：System clock source value。
  * @retval None
  */
void SYSCTRL_SYSCLKSourceSelect(SYSCLK_SOURCE_TypeDef source)
{
    assert_param(IS_SYSCLK_SOURCE(source));
    switch (source)
    {
        case SELECT_EXT12M:
            SYSCTRL->FREQ_SEL &= ~BIT(9);
            break;
        case SELECT_INC12M:
            SYSCTRL->FREQ_SEL |= BIT(9);
            break;
    }
}

/*  * @brief  Set System clock Freq
  * @param  SYSCLK_Freq：System clock set value。
  * @retval None
  */
void SYSCTRL_PLLConfig(SYSCTRL_PLL_TypeDef SYSCLK_Freq)
{
	assert_param(IS_PLL_FREQ(SYSCLK_Freq));
	switch((uint32_t)SYSCLK_Freq)
	{
	case SYSCTRL_PLL_120MHz:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_XTAL_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_XTAL_120Mhz;
		break;
	case SYSCTRL_PLL_108MHz:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_XTAL_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_XTAL_108Mhz;
		break;
	case SYSCTRL_PLL_96MHz:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_XTAL_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_XTAL_96Mhz;
		break;
	case SYSCTRL_PLL_72MHz:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_XTAL_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_XTAL_72Mhz;
		break;
	}
}

/**
  * @brief  Set System HCLK Div 
  * @param  HCLK_Div：Div value
  * @retval None
  */
void SYSCTRL_HCLKConfig(uint32_t HCLK_Div)
{
	assert_param(IS_GET_SYSCTRL_HCLK_DIV(HCLK_Div));
	switch(HCLK_Div)
	{
	case SYSCTRL_HCLK_Div2:
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_HCLK_DIV_EN;
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_HCLK_DIV_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_HCLK_DIV_1_2;
		break;
	case SYSCTRL_HCLK_Div4:
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_HCLK_DIV_EN;
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_HCLK_DIV_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_HCLK_DIV_1_4;
		break;
	case SYSCTRL_HCLK_Div_None:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_HCLK_DIV_EN;
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_HCLK_DIV_Mask;
		break;
	}
}

/**
  * @brief  Set System PCLK Div 
  * @param  PCLK_Div：Div value
  * @retval None
  */
void SYSCTRL_PCLKConfig(uint32_t PCLK_Div)
{
	assert_param(IS_GET_SYSCTRL_PCLK_DIV(PCLK_Div));
	switch(PCLK_Div)
	{
	case SYSCTRL_PCLK_Div2:
		SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_PCLK_DIV_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_PCLK_DIV_1_2;
		break;
	case SYSCTRL_PCLK_Div4:
        SYSCTRL->FREQ_SEL &= ~SYSCTRL_FREQ_SEL_PCLK_DIV_Mask;
		SYSCTRL->FREQ_SEL |= SYSCTRL_FREQ_SEL_PCLK_DIV_1_4;
		break;
	}
}

/**
  * @brief  Get clock's Frequency
  * @param  SYSCTRL_Clocks: 用于记录获取的时钟频率
  * @retval NONE
  */
void SYSCTRL_GetClocksFreq(SYSCTRL_ClocksTypeDef* SYSCTRL_Clocks)
{
    /* 获取系统时钟 */
	if (SYSCTRL_FREQ_SEL_XTAL_120Mhz == (SYSCTRL->FREQ_SEL & SYSCTRL_FREQ_SEL_XTAL_Mask))
	{
		SYSCTRL_Clocks->PLL_Frequency = 120000000;
	} 
	else if (SYSCTRL_FREQ_SEL_XTAL_108Mhz == (SYSCTRL->FREQ_SEL & SYSCTRL_FREQ_SEL_XTAL_Mask))
	{
		SYSCTRL_Clocks->PLL_Frequency = 108000000;
	}
	else if (SYSCTRL_FREQ_SEL_XTAL_96Mhz == (SYSCTRL->FREQ_SEL & SYSCTRL_FREQ_SEL_XTAL_Mask))
	{
		SYSCTRL_Clocks->PLL_Frequency = 96000000;
	}
	else if (SYSCTRL_FREQ_SEL_XTAL_72Mhz == (SYSCTRL->FREQ_SEL & SYSCTRL_FREQ_SEL_XTAL_Mask))
	{
		SYSCTRL_Clocks->PLL_Frequency = 72000000;
	}
	
    /* HCLK */
	SYSCTRL_Clocks->HCLK_Frequency = SYSCTRL->HCLK_1MS_VAL * 1000;
    
    /* PCLK */
	SYSCTRL_Clocks->PCLK_Frequency = SYSCTRL->PCLK_1MS_VAL * 1000;
}

/************ operation definition for DMA SYSCTRL REGISTER ************/
#define SYSCTRL_PHER_CTRL_DMA_CH0_IF_Pos					(0)
#define SYSCTRL_PHER_CTRL_DMA_CH0_IF_Mask					(0x1FU<<SYSCTRL_PHER_CTRL_DMA_CH0_IF_Pos)

#define SYSCTRL_PHER_CTRL_DMA_CH1_IF_Pos					(8)
#define SYSCTRL_PHER_CTRL_DMA_CH1_IF_Mask					(0x1FU<<SYSCTRL_PHER_CTRL_DMA_CH1_IF_Pos)

#define SYSCTRL_PHER_CTRL_DMA_CH2_IF_Pos					(16)
#define SYSCTRL_PHER_CTRL_DMA_CH2_IF_Mask					(0x1FU<<SYSCTRL_PHER_CTRL_DMA_CH2_IF_Pos)

#define SYSCTRL_PHER_CTRL_DMA_CH3_IF_Pos					(24)
#define SYSCTRL_PHER_CTRL_DMA_CH3_IF_Mask					(0x1FU<<SYSCTRL_PHER_CTRL_DMA_CH3_IF_Pos)

#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_I2C0_TX				(0x00)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_I2C0_RX				(0x01)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART0_TX				(0x02)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART0_RX				(0x03)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART1_TX				(0x04)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART1_RX				(0x05)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART2_TX				(0x06)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART2_RX				(0x07)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART3_TX				(0x08)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART3_RX				(0x09)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI0_TX				(0x0A)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI0_RX				(0x0B)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI1_TX				(0x0C)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI1_RX				(0x0D)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI2_TX				(0x0E)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI2_RX				(0x0F)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI3_TX				(0x10)
#define SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI3_RX				(0x11)

void SYSCTRL_DMA_ChannelConfig(DMA_TypeDef* DMA_Channelx, uint32_t DMA_Peripheral, uint32_t DMA_DIR)
{
	uint32_t tmpDMA_CHx_IFx = 0;
	
	if (1 == DMA_DIR)
	{
		switch (DMA_Peripheral)
		{
		case I2C0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_I2C0_RX;		
			break;
		case UART0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART0_RX;		
			break;
		case UART1_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART1_RX;		
			break;
		case UART2_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART2_RX;		
			break;
		case UART3_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART3_RX;		
			break;
		case SPIS0_BASE:
		case SPIM0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI0_RX;		
			break;
		case SPIS1_BASE:
		case SPIM1_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI1_RX;		
			break;
		case SPIS2_BASE:
		case SPIM2_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI2_RX;		
			break;
		case SPIM3_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI3_RX;		
			break;
		}
	}
	else if (2 == DMA_DIR)
	{
		switch (DMA_Peripheral)
		{
		case I2C0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_I2C0_TX;		
			break;
		case UART0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART0_TX;		
			break;
		case UART1_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART1_TX;		
			break;
		case UART2_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART2_TX;		
			break;
		case UART3_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_UART3_TX;		
			break;
		case SPIS0_BASE:
		case SPIM0_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI0_TX;		
			break;
		case SPIS1_BASE:
		case SPIM1_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI1_TX;		
			break;
		case SPIS2_BASE:
		case SPIM2_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI2_TX;		
			break;
		case SPIM3_BASE:
			tmpDMA_CHx_IFx = SYSCTRL_PHER_CTRL_DMA_CHx_IF_SPI3_TX;		
			break;
		}
	}
	if(DMA_Channelx == DMA_Channel_0)
	{
		SYSCTRL->DMA_CHAN &= ~SYSCTRL_PHER_CTRL_DMA_CH0_IF_Mask;
		SYSCTRL->DMA_CHAN |= tmpDMA_CHx_IFx << SYSCTRL_PHER_CTRL_DMA_CH0_IF_Pos;
	}
	else if(DMA_Channelx == DMA_Channel_1)
	{
		SYSCTRL->DMA_CHAN &= ~SYSCTRL_PHER_CTRL_DMA_CH1_IF_Mask;
		SYSCTRL->DMA_CHAN |= tmpDMA_CHx_IFx << SYSCTRL_PHER_CTRL_DMA_CH1_IF_Pos;
	}
	else if(DMA_Channelx == DMA_Channel_2)
	{
		SYSCTRL->DMA_CHAN &= ~SYSCTRL_PHER_CTRL_DMA_CH2_IF_Mask;
		SYSCTRL->DMA_CHAN |= tmpDMA_CHx_IFx << SYSCTRL_PHER_CTRL_DMA_CH2_IF_Pos;
	}
	else if(DMA_Channelx == DMA_Channel_3)
	{
		SYSCTRL->DMA_CHAN &= ~SYSCTRL_PHER_CTRL_DMA_CH3_IF_Mask;
		SYSCTRL->DMA_CHAN |= tmpDMA_CHx_IFx << SYSCTRL_PHER_CTRL_DMA_CH3_IF_Pos;
	}
}

void SYSCTRL_SPI_Reset(SPI_TypeDef* SPIx)
{

	switch((uint32_t)SPIx)
	{
	case SPIM0_BASE:
	case SPIS0_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_SPI0, ENABLE);
		break;
	case SPIM1_BASE:
	case SPIS1_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_SPI1, ENABLE);
		break;
	case SPIM2_BASE:
	case SPIS2_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_SPI2, ENABLE);
		break;
	case SPIM3_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_SPI3, ENABLE);
		break;
	}
}

void SYSCTRL_SPI_MasterSlaveModeSet(SPI_TypeDef* SPIx)
{

	switch((uint32_t)SPIx)
	{
	case SPIM0_BASE:
		SYSCTRL->PHER_CTRL &= ~SYSCTRL_PHER_CTRL_SPI0_SLV_EN;
		break;
	case SPIS0_BASE:
		SYSCTRL->PHER_CTRL |= SYSCTRL_PHER_CTRL_SPI0_SLV_EN;
		break;
	case SPIM1_BASE:
		SYSCTRL->PHER_CTRL &= ~SYSCTRL_PHER_CTRL_SPI1_SLV_EN;
		break;
	case SPIS1_BASE:
		SYSCTRL->PHER_CTRL |= SYSCTRL_PHER_CTRL_SPI1_SLV_EN;
		break;
	case SPIM2_BASE:
		SYSCTRL->PHER_CTRL &= ~SYSCTRL_PHER_CTRL_SPI2_SLV_EN;
		break;
	case SPIS2_BASE:
		SYSCTRL->PHER_CTRL |= SYSCTRL_PHER_CTRL_SPI2_SLV_EN;
		break;
	}
}

void SYSCTRL_UART_Reset(UART_TypeDef* UARTx)
{
	switch((uint32_t)UARTx)
	{
	case UART0_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_UART0, ENABLE);
		break;
	case UART1_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_UART1, ENABLE);
		break;
	case UART2_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_UART2, ENABLE);
		break;
	case UART3_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_UART3, ENABLE);
		break;
	}
}

void SYSCTRL_I2S_Reset(I2C_TypeDef* I2Cx)
{

	switch((uint32_t)I2Cx)
	{
	case I2C0_BASE:
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_I2C0, ENABLE);
		break;
	}
}

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
