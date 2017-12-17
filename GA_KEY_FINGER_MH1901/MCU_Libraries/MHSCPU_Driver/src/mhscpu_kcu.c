#include "mhscpu_kcu.h"

/************ operation definition for KCU  KCU_CTRL1 REGISTER ************/
#define KCU_CTRL0_DEBOUNCE_TIME_Pos									(9)
#define KCU_CTRL0_DEBOUNCE_TIME_Mask								(0x0007U << KCU_CTRL0_DEBOUNCE_TIME_Pos)

#define KCU_CTRL0_PORT_OUT_ENABLE_Pos								(0)
#define KCU_CTRL0_PORT_OUT_ENABLE_Mask								(0x001FU << KCU_CTRL0_PORT_OUT_ENABLE_Pos)

/************ operation definition for KCU  KCU_CTRL1 REGISTER ************/
#define KCU_CTRL1_KCU_RUNNING_Pos									(31)
#define KCU_CTRL1_KCU_RUNNING_Mask									(0x0001U<<KCU_CTRL1_KCU_RUNNING_Pos) 

#define KCU_CTRL1_KBD_EN_Pos										(0)
#define KCU_CTRL1_KBD_EN_Mask										(0x0001U<<KCU_CTRL1_KBD_EN_Pos) 

void KCU_DeInit(void)
{
	SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_KCU, ENABLE);
}

void KCU_Init(KCU_InitTypeDef *KCU_InitStruct)
{
	uint32_t tmpPortSet = 0;
	uint32_t tmpDebounceTimeSet = 0;
	KCU_Cmd(DISABLE);
	while((KCU->KCU_CTRL1 & KCU_CTRL1_KCU_RUNNING_Mask) != 0);
	tmpDebounceTimeSet = KCU_InitStruct->KCU_DebounceTimeLevel << KCU_CTRL0_DEBOUNCE_TIME_Pos;
	tmpPortSet = KCU_InitStruct->KCU_PortOutput & ~(KCU_InitStruct->KCU_PortInput);
	KCU->KCU_CTRL0 = tmpDebounceTimeSet | tmpPortSet;
	KCU->KCU_RNG = KCU_InitStruct->KCU_Rand;
}


void KCU_StructInit(KCU_InitTypeDef *KCU_InitStruct)
{
	KCU_InitStruct->KCU_DebounceTimeLevel = KCU_DebounceTimeLevel_1;
	KCU_InitStruct->KCU_PortInput = KCU_Port_0 | KCU_Port_1 | KCU_Port_2 | KCU_Port_3; 
	KCU_InitStruct->KCU_PortOutput = KCU_Port_4 | KCU_Port_5 | KCU_Port_6 | KCU_Port_7 |KCU_Port_8; 
	KCU_InitStruct->KCU_Rand = 0x5A5A5A5A;
}

void KCU_Cmd(FunctionalState NewState)
{
	if(NewState != DISABLE)
		KCU->KCU_CTRL1 |= KCU_CTRL1_KBD_EN_Mask;
	else
		KCU->KCU_CTRL1 &= ~KCU_CTRL1_KBD_EN_Mask;
}

uint32_t KCU_SetRand(uint32_t Rand)
{

	if (0 != Rand && ~0 != Rand)
	{
		KCU->KCU_RNG = Rand;
		return 0;
	}
	else
	{
		return 1;
	}
}

FlagStatus KCU_GetITStatus(uint32_t KCU_IT)
{
	uint32_t tmpStatus = 0;
	FlagStatus bitstatus = RESET;
	if (KCU_IT & KCU_IT_PUSH)
	{
		tmpStatus |= KCU_STATUS_PUSH_INTERRUPT;
	}
	if (KCU_IT & KCU_IT_RELEASE)
	{
		tmpStatus |= KCU_STATUS_RELEASE_INTERRUPT;
	}
	if (KCU_IT & KCU_IT_OVERRUN)
	{
		tmpStatus |= KCU_STATUS_OVERRUN_INTERRUPT;
	}

	if ((KCU->KCU_STATUS & tmpStatus) != (uint16_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return  bitstatus;
}


void KCU_ITConfig(uint32_t KCU_IT, FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		KCU->KCU_CTRL1 |= KCU_IT;
	}
	else
	{
		KCU->KCU_CTRL1 &= ~KCU_IT;
	}
	
}

void KCU_ClearITPendingBit(uint32_t KCU_IT)
{
	volatile uint32_t clr;
	clr = KCU->KCU_EVENT;
}

uint32_t KCU_GetEvenReg(void)
{
	return KCU->KCU_EVENT;
}
