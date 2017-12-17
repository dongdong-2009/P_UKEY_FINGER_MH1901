#ifndef __MHSCPU_KCU_H
#define __MHSCPU_KCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mhscpu.h"

typedef struct
{
	uint32_t KCU_DebounceTimeLevel;
	uint32_t KCU_PortInput;
	uint32_t KCU_PortOutput;
	uint32_t KCU_Rand;
}KCU_InitTypeDef;

#define KCU_DebounceTimeLevel_0				((uint32_t)0x0000)
#define KCU_DebounceTimeLevel_1				((uint32_t)0x0001)
#define KCU_DebounceTimeLevel_2				((uint32_t)0x0002)
#define KCU_DebounceTimeLevel_3				((uint32_t)0x0003)
#define KCU_DebounceTimeLevel_4				((uint32_t)0x0004)
#define KCU_DebounceTimeLevel_5				((uint32_t)0x0005)
#define KCU_DebounceTimeLevel_6				((uint32_t)0x0006)
#define KCU_DebounceTimeLevel_7				((uint32_t)0x0007)

#define KCU_Port_0							((uint32_t)0x0001)
#define KCU_Port_1							((uint32_t)0x0002)
#define KCU_Port_2							((uint32_t)0x0004)
#define KCU_Port_3							((uint32_t)0x0008)
#define KCU_Port_4							((uint32_t)0x0010)
#define KCU_Port_5							((uint32_t)0x0020)
#define KCU_Port_6							((uint32_t)0x0040)
#define KCU_Port_7							((uint32_t)0x0080)
#define KCU_Port_8							((uint32_t)0x0100)

#define KCU_IT_PUSH							((uint32_t)0x0002)
#define KCU_IT_RELEASE						((uint32_t)0x0004)
#define KCU_IT_OVERRUN						((uint32_t)0x0008)

void KCU_DeInit(void);
void KCU_Init(KCU_InitTypeDef *KCU_InitStruct);
void KCU_StructInit(KCU_InitTypeDef *KCU_InitStruct);
void KCU_Cmd(FunctionalState NewState);	 
uint32_t KCU_SetRand(uint32_t rand);
void KCU_ITConfig(uint32_t KCU_IT, FunctionalState NewState);
FlagStatus KCU_GetITStatus(uint32_t KCU_IT);
void KCU_ClearITPendingBit(uint32_t KCU_IT);
uint32_t KCU_GetEvenReg(void);		//read Even register to clear The Interrup
	 
#ifdef __cplusplus
}
#endif

#endif 

