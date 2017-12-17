/**************************************************************************//**
 * @file     mhscpu_ssc.h
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
#ifndef __MHSCPU_SSC_H
#define __MHSCPU_SSC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mhscpu.h"

typedef enum
{ 
	SSC_RandFreq_1				= 0x3F,	//���ԴƵ�ʲ���Ƶ
	SSC_RandFreq_1_2 			= 0xA0,	//���ԴƵ��2��Ƶ
	SSC_RandFreq_1_4 			= 0xFF,	//���ԴƵ��4��Ƶ
	SSC_RandFreqOff 			= 0xA5	//���ܹر�
}SSC_RandFreqTypeDef; 
#define IS_SSC_RAND_FREQ(FREQ) (((FREQ) == SSC_RandFreq_1) || ((FREQ) == SSC_RandFreq_1_2) || \
								((FREQ) == SSC_RandFreq_1_4) || ((FREQ) == SSC_RandFreqOff) )


typedef enum
{
	SSC_ITDisable				= 0x00,	//��ȫ�쳣��Ӧ�жϹر�
	SSC_ITEnable				= 0x01,	//��ȫ�쳣��Ӧ�жϴ�
	SSC_ITEnableWithKeyClean 	= 0x02,	//��ȫ�쳣��Ӧ�жϴ򿪣��ڲ����ж�ͬʱ������Կ��������
}SSC_ITModeTypeDef;
#define IS_SSC_IT_MODE(MODE) (((MODE) == SSC_ITDisable) || ((MODE) == SSC_ITEnable) || \
								((MODE) == SSC_ITEnableWithKeyClean))


#define	SSC_ITSysXTAL12M					BIT(18)	//ϵͳ��12Mʱ�ӱ�־
#define	SSC_ITSysGlitch						BIT(17)	//����Դë�̱�־
#define	SSC_ITSysVolHigh					BIT(16)	//����Դ��ѹ��־
#define	SSC_ITSysVolLow						BIT(15)	//����ԴǷѹ��־
#define	SSC_ITIntRetValueNotExpect			BIT(12)	//�жϷ�����򷵻ط�ƥ��ֵ
#define	SSC_ITExeInstructionWhenTCleaned	BIT(11)	//��PSR��T����λ��0ʱ������ִ��ָ��
#define	SSC_ITExeUndefinedInstruction		BIT(10)	//ִ�зǶ���ָ��
#define	SSC_ITExeCoprocessorInstruction		BIT(9)	//ִ��Э������ָ��
#define	SSC_ITMPUStackAccessException 		BIT(8)	//MPU�·���ջ�쳣
#define	SSC_ITStackAccessException			BIT(7)	//����ջ�쳣
#define	SSC_ITIBUSAccessException			BIT(6)	//IBUS�����쳣
#define	SSC_ITMPUDataAccessException		BIT(5)	//MPU��ָ������ݷ����쳣
#define	SSC_ITDivZero						BIT(4)	//��0�����쳣
#define	SSC_ITParityError					(BIT(3)|BIT(2)|BIT(1)|BIT(0))		//��żУ������쳣
#define	SSC_ITParityErrorRegBank			BIT(3)	//��żУ��Ĵ���BANK����
#define	SSC_ITParityErrorMPU				BIT(2)	//��żУ��MPU�´���
#define	SSC_ITParityErrorIntCtrl			BIT(1)	//��żУ���жϿ��ƴ���
#define	SSC_ITParityErrorInstructionFlow	BIT(0)	//��żУ��ָ��������


typedef struct
{
	SSC_RandFreqTypeDef IBUS_DBUS_ReadPolarityReversalFreq;		//IBUS_DBUS�����ݼ��Է�ת����
	SSC_RandFreqTypeDef SBUS_ReadPolarityReversalFreq;			//SBUS�����ݼ��Է�ת����
	SSC_RandFreqTypeDef DBUS_SBUS_WritePolarityReversalFreq;	//DBUS_SBUSд���ݼ��Է�ת����
	SSC_RandFreqTypeDef RandBranchInsertFreq;					//�����֧����ʹ��
	SSC_RandFreqTypeDef IgnoreClockGateFreq;					//����ʱ���ſ�ʹ��
	FunctionalState RegisterHeapClean;							//�Ĵ��������ʹ��
	FunctionalState ParityCheck;								//��żУ��ʹ��
}SSC_InitTypeDef;


/*
 *	��BPK��Ϊ4��ÿ��256����Ϊ��λ���ö�дȨ��
 *	SSC_BPKAccessCtrBlock_0Ϊ��ʼ0��ַ��
 */
#define SSC_BPKAccessCtrBlock_0				(0x01)
#define SSC_BPKAccessCtrBlock_1				(0x02)
#define SSC_BPKAccessCtrBlock_2				(0x04)
#define SSC_BPKAccessCtrBlock_3				(0x08)

typedef enum
{
	SSC_BPKReadOnly		= 0x01,		//BPK��ֻ��
	SSC_BPKWriteOnly	= 0x02,		//BPK��ֻд
	SSC_BPKReadWrite	= 0x03		//BPK���д
}SSC_BPKAccessCtrlTypeDef;

#define IS_BPK_ACCESS_CTRL(CTRL) (((CTRL) == SSC_BPKReadOnly) || ((CTRL) == SSC_BPKWriteOnly) || \
								((CTRL) == SSC_BPKReadWrite))

#define SSC_SENSOR_XTAL12M							((uint32_t)0x00000001)
#define SSC_SENSOR_VOL_LOW							((uint32_t)0x00000002)
#define SSC_SENSOR_VOL_HIGH							((uint32_t)0x00000004)
#define SSC_SENSOR_VOLGLITCH						((uint32_t)0x00000008)
#define IS_SSC_SENSOR(SENSOR)						((((SENSOR) & (uint32_t)0xFFFFFFF0) == 0x00) && ((SENSOR) != (uint32_t)0x00))

typedef enum
{
	SSC_SENSOR_CPUReset  = 0,
	SSC_SENSOR_Interrupt = 1
}SSC_SENSOR_RespModeTypeDef;
#define IS_SSC_SENSOR_RESP_MODE(Mode)				((Mode) == SSC_SENSOR_CPUReset ||\
													(Mode) == SSC_SENSOR_Interrupt)

/**
  * @method	SSC_Init
  * @brief	SSC��ȫ���Գ�ʼ��
  * @param	SSC_InitTypeDef SSC_InitStruct
  * @retval void
  */
void SSC_Init(SSC_InitTypeDef SSC_InitStruct);
/**
  * @method	SSC_ITConfig
  * @brief	SSC��ȫ�ж���Ӧ����
  * @param	uint32_t SSC_IT
  * @param	SSC_ITModeTypeDef SSC_ITMode
  * @retval void
  */
void SSC_ITConfig(uint32_t SSC_IT, SSC_ITModeTypeDef SSC_ITMode);
/**
  * @method	SSC_GetITStatus
  * @brief	SSC��ȫ�ж�״̬
  * @param	uint32_t SSC_IT
  * @retval ITStatus
  */
ITStatus SSC_GetITStatus(uint32_t SSC_IT);
/**
  * @method	SSC_ClearITPendingBit
  * @brief	SSC��ȫ�ж����
  * @param	uint32_t SSC_IT
  * @retval void
  */
void SSC_ClearITPendingBit( uint32_t SSC_IT);

/**
  * @method	SSC_SetDataRAMScrambler
  * @brief	��������RAM����
  * @param	uint32_t Scrambler
  * @retval void
  */
void SSC_SetDataRAMScrambler(uint32_t Scrambler);
/**
  * @method	SSC_BPKAccessCtrConfig
  * @brief	����BPK����Ȩ��
  * @param	uint32_t SSC_BPKAccessCtrBlock
  * @param	SSC_BPKAccessCtrlTypeDef SSC_BPKAccessCtr
  * @retval void
  */
void SSC_BPKAccessCtrlConfig(uint32_t SSC_BPKAccessCtrBlock, SSC_BPKAccessCtrlTypeDef SSC_BPKAccessCtr);

/**
  * @method	SSC_SENSOR_Enable
  * @brief	����ϵͳSensor
  * @param	SSC_SENSOR
  * @retval 
  */
uint32_t SSC_SENSORCmd(uint32_t SSC_SENSOR, FunctionalState NewState);

/**
  * @method	SSC_SENSORLock
  * @brief	����ϵͳSensor����״̬
  * @param	SSC_SENSOR
  * @retval 
  */
void SSC_SENSORLock(uint32_t SSC_SENSOR);


/**
  * @method	SSC_SENSOR_AttackRespMode
  * @brief	ϵͳSensor��Ӧ��ʽ
  * @param	SSC_SENSOR_RespMode
  * @retval 
  */
void SSC_SENSORAttackRespMode(SSC_SENSOR_RespModeTypeDef SSC_SENSOR_RespMode);

#ifdef __cplusplus
}
#endif

#endif 

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
