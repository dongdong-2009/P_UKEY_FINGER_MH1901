/*
 * Timer Operations
 * For ms delay and get elapse time 
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for Timer Operation
 */
#include <board.h>




/*
 *  Init timer 0,1,2
 *  @None parameter and return value
 */
void init_timer(void)
{
	TIM_InitTypeDef TIMER_InitStruct;
	SYSCTRL_ClocksTypeDef clocks;

    
	TIMER_InitStruct.TIMx = TIM_0;	
	SYSCTRL_GetClocksFreq(&clocks);
	TIMER_InitStruct.TIM_Period = clocks.PCLK_Frequency/1000;
	TIM_Init(TIMM0, &TIMER_InitStruct);
	TIM_ITConfig(TIMM0, TIM_0, ENABLE);
    TIM_Cmd(TIMM0, TIM_0, ENABLE);
    

    #if 0
	TIMER_InitStruct.TIMx = TIM_1;	
	SYSCTRL_GetClocksFreq(&clocks);
	TIMER_InitStruct.TIM_Period = clocks.PCLK_Frequency/1000000;
	TIM_Init(TIMM0, &TIMER_InitStruct);
	TIM_ITConfig(TIMM0, TIM_1, ENABLE);

	TIMER_InitStruct.TIMx = TIM_2;	
	SYSCTRL_GetClocksFreq(&clocks);
	TIMER_InitStruct.TIM_Period = clocks.PCLK_Frequency/1000;
	TIM_Init(TIMM0, &TIMER_InitStruct);
	TIM_ITConfig(TIMM0, TIM_2, ENABLE);
	TIM_Cmd(TIMM0, TIM_2, ENABLE);		
    #endif
}

