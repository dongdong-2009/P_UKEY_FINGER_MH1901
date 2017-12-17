/*
 * Timer Operations
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for Timer Function Declars and MACRO Defines
 */
#ifndef _TIMER_H
#define _TIMER_H

extern volatile  UINT32  tick_count;

void init_timer(void);
void TIM0_0_IRQHandler(void);
void TIM0_1_IRQHandler(void);
void TIM0_2_IRQHandler(void);
void Timer0_2_mdelay(uint32_t delay);
void timerDelayMs(UINT32 ms);
void timerDelayUs(UINT32 us);
void timerRestart(void);
UINT32  timerElapsedUs(void);











#endif /* End of _TIMER_H */
