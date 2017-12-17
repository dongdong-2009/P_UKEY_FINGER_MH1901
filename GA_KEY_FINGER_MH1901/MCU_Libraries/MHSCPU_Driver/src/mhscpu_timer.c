#include "mhscpu_timer.h"


void TIM_DeInit(TIM_Module_TypeDef * TIMMx)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	switch((uint32_t)TIMMx)
	{
	case ((uint32_t)TIMM0):
		SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_TIMM0, ENABLE);
	}
}

void TIM_Init(TIM_Module_TypeDef* TIMMx, TIM_InitTypeDef* TIM_InitStruct)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIM_InitStruct->TIMx));

	TIM_Cmd(TIMMx, TIM_InitStruct->TIMx, DISABLE);
	
	TIMMx->TIM[TIM_InitStruct->TIMx].ControlReg = 0;
	TIMMx->TIM[TIM_InitStruct->TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_MODE;
	TIMMx->TIM[TIM_InitStruct->TIMx].ControlReg &= ~TIMER_CONTROL_REG_TIMER_PWM;

	TIMMx->TIM[TIM_InitStruct->TIMx].LoadCount = TIM_InitStruct->TIM_Period;
}

void TIM_PWMInit(TIM_Module_TypeDef* TIMMx, TIM_PWMInitTypeDef* TIM_PWMInitStruct)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PWM_PERIPH(TIM_PWMInitStruct->TIMx));

	TIM_Cmd(TIMMx, TIM_PWMInitStruct->TIMx, DISABLE);

	TIMMx->TIM[TIM_PWMInitStruct->TIMx].ControlReg = 0;
	TIMMx->TIM[TIM_PWMInitStruct->TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_MODE;
	TIMMx->TIM[TIM_PWMInitStruct->TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_PWM;
	TIMMx->TIM[TIM_PWMInitStruct->TIMx].LoadCount = TIM_PWMInitStruct->TIM_LowLevelPeriod;
	TIMMx->TIM_ReloadCount[TIM_PWMInitStruct->TIMx] = TIM_PWMInitStruct->TIM_HighLevelPeriod;
}

void TIM_Cmd(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx, FunctionalState NewState)
{
	//assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	//assert_param(IS_TIM_PERIPH(TIMx));
	//assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState != DISABLE)
	{
		TIMMx->TIM[TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_ENABLE;
	}
	else
	{
		TIMMx->TIM[TIMx].ControlReg &= ~TIMER_CONTROL_REG_TIMER_ENABLE;
	}
}

void TIM_ModeConfig(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx, TIM_ModeTypeDef TIM_Mode)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));
	assert_param(IS_TIM_MODE(TIM_Mode));
	if (TIM_Mode_General == TIM_Mode)
	{
		TIMMx->TIM[TIMx].ControlReg &= ~TIMER_CONTROL_REG_TIMER_PWM;
	} 
	else if(TIM_Mode_PWM == TIM_Mode)
	{
		TIMMx->TIM[TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_PWM;
	}
}

void TIM_SetPeriod(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx, uint32_t Period)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));
	TIMMx->TIM[TIMx].LoadCount = Period;
}


void TIM_SetPWMPeriod(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx, uint32_t PWM_LowLevelPeriod, uint32_t PWM_HighLevelPeriod)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));
	TIMMx->TIM[TIMx].LoadCount = PWM_LowLevelPeriod;
	TIMMx->TIM_ReloadCount[TIMx] = PWM_HighLevelPeriod;
}

void TIM_ITConfig(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx, FunctionalState NewState)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState != DISABLE)
		TIMMx->TIM[TIMx].ControlReg &= ~TIMER_CONTROL_REG_TIMER_INTERRUPT;
	else
		TIMMx->TIM[TIMx].ControlReg |= TIMER_CONTROL_REG_TIMER_INTERRUPT;
}

void TIM_ClearITPendingBit(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx)
{
	volatile uint32_t clr;
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));
	clr = TIMMx->TIM[TIMx].EOI;
}

ITStatus TIM_GetITStatus(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx)
{
	ITStatus bitstatus = RESET;
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));

	if ((TIMMx->TIM[TIMx].IntStatus & TIMER_INT_STATUS_INTERRUPT) != RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return bitstatus;
}

uint32_t TIM_GetAllITStatus(TIM_Module_TypeDef* TIMMx)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	return TIMMx->TIM_IntStatus;
}

ITStatus TIM_GetRawITStatus(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx)
{
	ITStatus bitstatus = RESET; 
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	assert_param(IS_TIM_PERIPH(TIMx));

	if (((TIMMx->TIM_RawIntStatus) & (0x01 << TIMx)) != RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return bitstatus;
}

uint32_t TIM_GetAllRawITStatus(TIM_Module_TypeDef* TIMMx)
{
	assert_param(IS_TIM_MODULE_PERIPH(TIMMx));
	return TIMMx->TIM_RawIntStatus;
}


uint32_t get_current_count_value(TIM_Module_TypeDef* TIMMx, TIM_NumTypeDef TIMx)
{
		return TIMMx->TIM[TIMx].CurrentValue;
}

