

#ifndef __MHSCPU_TRNG_H
#define __MHSCPU_TRNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mhscpu.h"

/** @defgroup RNG_Exported_Types
  * @{
  */
typedef enum{
    TRNG0,
    TRNG1
} TRNG_ChannelTypeDef;
#define IS_TRNG_CHANNEL(CHANNEL)  	(((CHANNEL) == TRNG0) || \
									((CHANNEL) == TRNG1))


#define TRNG_IT_RNG0_S128           ((uint32_t)0x00000001)
#define TRNG_IT_RNG1_S128           ((uint32_t)0x00000002)
#define TRNG_IT_RNG0_ATTACK         ((uint32_t)0x00000004)     
#define TRNG_IT_RNG1_ATTACK         ((uint32_t)0x00000008)  
#define IS_TRNG_GET_IT(IT)  		(((IT) == TRNG_IT_RNG0_S128) || \
									((IT) == TRNG_IT_RNG1_S128) || \
									((IT) == TRNG_IT_RNG0_ATTACK) || \
									((IT) == TRNG_IT_RNG1_ATTACK))

/** @defgroup RNG_Exported_Functions
  * @{
  */
void TRNG_Start(TRNG_ChannelTypeDef TRNGx);
void TRNG_Stop(TRNG_ChannelTypeDef TRNGx);
uint32_t TRNG_Get(uint32_t rand[4], TRNG_ChannelTypeDef TRNGx);
void TRNG_SetPseudoRandom(uint32_t PseudoRandom);
void TRNG_DirectOutANA(TRNG_ChannelTypeDef TRNGx, FunctionalState NewState);

void TRNG_ITConfig(FunctionalState NewState);
ITStatus TRNG_GetITStatus(uint32_t TRNG_IT);
void TRNG_ClearITPendingBit(uint32_t TRNG_IT);
uint8_t TRNG_Getdeep(TRNG_ChannelTypeDef TRNGx);

#ifdef __cplusplus
}
#endif

#endif 

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
