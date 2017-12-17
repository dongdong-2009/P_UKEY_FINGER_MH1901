/**************************************************************************//**
 * @file     <Device>.h
 * @brief    CMSIS Cortex-M# Core Peripheral Access Layer Header File for
 *           Device <Device>
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list ofC conditions and the following disclaimer.
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


#ifndef MHSCPU_H      /* ToDo: replace '<Device>' with your device name */
#define MHSCPU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* ToDo: replace '<Device>' with your device name; add your doxyGen comment   */
/** @addtogroup <Device>_Definitions <Device> Definitions
  This file defines all structures and symbols for <Device>:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup <Device>_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M# Processor and Core Peripherals
  @{
*/
/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M# Processor Exceptions Numbers ***************************************************/



/* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M3 / Cortex-M4 device       */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
/* ToDo: add here your device specific external interrupt numbers
         according the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       TIM1_IRQHandler   ->   TIM1_IRQn                        */
	DMA_IRQn										= 0,
	USB_IRQn										= 1,
	USBDMA_IRQn										= 2,
	LCD_IRQn										= 3,
	SCI0_IRQn										= 4,
	SCI1_IRQn										= 5,
	SCI2_IRQn										= 6,
	I2C0_IRQn										= 7,
	UART0_IRQn										= 8,
	UART1_IRQn										= 9,
	UART2_IRQn										= 10,
	UART3_IRQn										= 11,
	KCU_IRQn										= 12,
	SPI0_IRQn										= 13,
	SPI1_IRQn										= 14,
	CRYPT0_IRQn										= 15,
	TIM0_0_IRQn										= 16,
	TIM0_1_IRQn										= 17,
	TIM0_2_IRQn										= 18,
	TIM0_3_IRQn										= 19,
	EXTI0_IRQn										= 20,
	EXTI1_IRQn										= 21,
	EXTI2_IRQn										= 22,
	EXTI3_IRQn										= 23,
	EXTI4_IRQn										= 24,
	EXTI5_IRQn										= 25,
	RTC_IRQn										= 26,
	SPI2_IRQn										= 27,
	SENSOR_IRQn										= 28,
	TRNG_IRQn										= 29,
	ADC_IRQn										= 30,
	SSC_IRQn										= 31,
	TIM0_4_IRQn										= 32,
	TIM0_5_IRQn										= 33,
	SPI3_IRQn										= 34
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M# Processor and Core Peripherals */
/* ToDo: set the defines according your Device                                                    */
/* ToDo: define the correct core revision
         __CM0_REV if your device is a CORTEX-M0 device
         __CM3_REV if your device is a CORTEX-M3 device
         __CM4_REV if your device is a CORTEX-M4 device                                           */
//#define __CM3_REV                 0x0201    /*!< Core Revision r2p1                               */
#define __CM3_REV                 0x0200    /*!< Core Revision r2p0                               */
#define __NVIC_PRIO_BITS          4         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
/* ToDo: define __FPU_PRESENT if your devise is a CORTEX-M4                                       */
#define __FPU_PRESENT             0        /*!< FPU present or not                                */

/*@}*/ /* end of group <Device>_CMSIS */


/* ToDo: include the correct core_cm#.h file
         core_cm0.h if your device is a CORTEX-M0 device
         core_cm3.h if your device is a CORTEX-M3 device
         core_cm4.h if your device is a CORTEX-M4 device                                          */
#include "core_cm3.h"                       /* Cortex-M# processor and core peripherals           */
/* ToDo: include your system_<Device>.h file
         replace '<Device>' with your device name                                                 */
#include "system_mhscpu.h"                /* <Device> System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup <Device>_Peripherals <Device> Peripherals
  <Device> Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#include <stdint.h>

/** @addtogroup Exported_types
  * @{
  */  

/*!<Standard Peripheral Library old types (maintained for legacy purpose) */
// typedef int32_t  	s32;
// typedef int16_t 	s16;
// typedef int8_t  	s8;

// typedef const int32_t sc32;  /*!< Read Only */
// typedef const int16_t sc16;  /*!< Read Only */
// typedef const int8_t 	sc8;   /*!< Read Only */

// typedef __IO int32_t  vs32;
// typedef __IO int16_t  vs16;
// typedef __IO int8_t   vs8;

// typedef __I int32_t vsc32;  /*!< Read Only */
// typedef __I int16_t vsc16;  /*!< Read Only */
// typedef __I int8_t 	vsc8;   /*!< Read Only */

// typedef uint32_t  u32;
// typedef uint16_t 	u16;
// typedef uint8_t  	u8;

// typedef const uint32_t 	uc32;  /*!< Read Only */
// typedef const uint16_t 	uc16;  /*!< Read Only */
// typedef const uint8_t 	uc8;   /*!< Read Only */

// typedef __IO uint32_t  	vu32;
// typedef __IO uint16_t 	vu16;
// typedef __IO uint8_t  	vu8;

// typedef __I uint32_t 	vuc32;  /*!< Read Only */
// typedef __I uint16_t 	vuc16;  /*!< Read Only */
// typedef __I uint8_t 	vuc8;   /*!< Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {FALSE = 0, TRUE = !FALSE} Boolean;

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/* ToDo: add here your device specific peripheral access structure typedefs
         following is an example for a timer                                  */
#define BIT0		(0x00000001U)
#define BIT1		(0x00000002U)
#define BIT2		(0x00000004U)
#define BIT3		(0x00000008U)
#define BIT4		(0x00000010U)
#define BIT5		(0x00000020U)
#define BIT6		(0x00000040U)
#define BIT7		(0x00000080U)
#define BIT8		(0x00000100U)
#define BIT9		(0x00000200U)
#define BIT10		(0x00000400U)
#define BIT11		(0x00000800U)
#define BIT12		(0x00001000U)
#define BIT13		(0x00002000U)
#define BIT14		(0x00004000U)
#define BIT15		(0x00008000U)
#define BIT16		(0x00010000U)
#define BIT17		(0x00020000U)
#define BIT18		(0x00040000U)
#define BIT19		(0x00080000U)
#define BIT20		(0x00100000U)
#define BIT21		(0x00200000U)
#define BIT22		(0x00400000U)
#define BIT23		(0x00800000U)
#define BIT24		(0x01000000U)
#define BIT25		(0x02000000U)
#define BIT26		(0x04000000U)
#define BIT27		(0x08000000U)
#define BIT28		(0x10000000U)
#define BIT29		(0x20000000U)
#define BIT30		(0x40000000U)
#define BIT31		(0x80000000U)

#define BIT(n)		(1UL << (n))

typedef struct
{
	__IO uint32_t FREQ_SEL;
    __IO uint32_t CG_CTRL1;
    __IO uint32_t CG_CTRL2;
    __O uint32_t  SOFT_RST1;
    __O uint32_t  SOFT_RST2;
    __IO uint32_t LOCK_R;
	__IO uint32_t PHER_CTRL;
    __IO uint32_t WAKE_EN1;
    __IO uint32_t WAKE_EN2;
    __I uint32_t  WAKE_FLAG1;
    __I uint32_t  WAKE_FLAG2;
    __I uint32_t  HCLK_1MS_VAL;
	__I uint32_t  PCLK_1MS_VAL;
    __IO uint32_t ANA_CTRL;
	__IO uint32_t DMA_CHAN;
    __I uint32_t  SYS_RSCD[(0x108 - 0x3C)>>2];
    __IO uint32_t USBPHY_CR1;
	__IO uint32_t USBPHY_CR2;
	__IO uint32_t USBPHY_CR3;
    __I uint32_t  SYS_RSVD1[(0x3F0 - 0x114)>>2];
    __IO uint32_t CALIB_CSR;        /* CALIB_CSRÔ­¼Ä´æÆ÷ÃûÎªCALIB_SR */
	__IO uint32_t DBG_CR;
	__IO uint32_t CHIP_ID;
} SYSCTRL_TypeDef;

typedef struct
{
	union
	{
		__I  uint32_t RBR;
		__O  uint32_t THR;
		__IO uint32_t DLL;
	} OFFSET_0;
	union
	{
		__IO uint32_t DLH;
		__IO uint32_t IER;
	} OFFSET_4;
	union
	{
		__I uint32_t IIR;
		__O uint32_t FCR;
	} OFFSET_8;
	__IO uint32_t LCR;
	__IO uint32_t MCR;
	__I  uint32_t LSR;
	__I  uint32_t MSR;
	__IO uint32_t SCR;
	__IO uint32_t LPDLL;
	__IO uint32_t LPDLH;
	__I  uint32_t RES0[2];
	union
	{
		__I  uint32_t SRBR[16];
		__O  uint32_t STHR[16];
	} OFFSET_48;
	__IO uint32_t FAR;
	__I  uint32_t TFR;
	__O  uint32_t RFW;
	__I  uint32_t USR;
	__I  uint32_t TFL;
	__I  uint32_t RFL;
	__O  uint32_t SRR;
	__IO uint32_t SRTS;
	__IO uint32_t SBCR;
	__IO uint32_t SDMAM;
	__IO uint32_t SFE;
	__IO uint32_t SRT;
	__IO uint32_t STET;
	__IO uint32_t HTX;
	__O uint32_t DMASA;
	__I  uint32_t RES1[18];
	__I  uint32_t CPR;
	__I  uint32_t UCV;
	__I  uint32_t CTR;
	
} UART_TypeDef;

typedef struct
{
	__IO uint16_t CTRLR0;
	uint16_t RESERVED0;
	__IO uint16_t CTRLR1;
	uint16_t RESERVED1;
	__IO uint32_t SSIENR;
	__IO uint32_t MWCR;
	__IO uint32_t SER;
	__IO uint32_t BAUDR;
	__IO uint32_t TXFTLR;
	__IO uint32_t RXFTLR;
	__IO uint32_t TXFLR;
	__I  uint32_t RXFLR;
	__I  uint32_t SR;
	__IO uint32_t IMR;
	__I  uint32_t ISR;
	__I  uint32_t RISR;
	__I  uint32_t TXOICR;
	__I  uint32_t RXOICR;
	__I  uint32_t RXUICR;
	__I  uint32_t MSTICR;
	__IO uint32_t ICR;
	__IO uint32_t DMACR;
	__IO uint32_t DMATDLR;
	__IO uint32_t DMARDLR;
	__I  uint32_t IDR;
	__I  uint32_t SSI_COMP_VERSION;
	__IO uint32_t DR;
	__IO uint32_t DR_Array[35];
	__IO uint32_t RX_SAMPLE_DLY;	
} SPI_TypeDef;

typedef struct
{
	__IO uint32_t WDT_CR;
	__IO uint32_t RESERVED0;
	__I  uint32_t WDT_CCVR;
	__O  uint32_t WDT_CRR;
	__I  uint32_t WDT_STAT;
	__I  uint32_t WDT_EOI;	
	__I  uint32_t RESERVED1;
	__IO uint32_t WDT_RLD;
	__I  uint32_t RESERVED[53];
	__I  uint32_t WDT_COMP_PARAMS_1;
	__I  uint32_t WDT_COMP_VERSION;
	__I  uint32_t WDT_COMP_TYPE;
} WDT_TypeDef;

typedef struct
{
	__IO uint32_t CRC_CSR;
	__O  uint32_t CRC_INI;
	union
	{
		__I uint32_t DOUT;
		__O uint8_t  DIN;
	} CRC_DATA;
} CRC_TypeDef;

typedef struct
{
	__IO uint32_t LoadCount;
	__I  uint32_t CurrentValue;
	__IO uint32_t ControlReg;
	__IO  uint32_t EOI;
	__I  uint32_t IntStatus;
} TIM_TypeDef;

typedef struct
{
	#define TIM_NUM	6
	TIM_TypeDef TIM[TIM_NUM];
    __I uint8_t RESERVED[0xA0 - TIM_NUM * sizeof(TIM_TypeDef)];
	__I  uint32_t TIM_IntStatus;
	__I  uint32_t TIM_EOI;
	__I  uint32_t TIM_RawIntStatus;
	__I  uint32_t TIM_Comp;
    __IO uint32_t TIM_ReloadCount[TIM_NUM];
} TIM_Module_TypeDef;

typedef struct 
{
    __IO uint32_t ADC_CR;
    __I  uint32_t ADC_SR;
} ADC_TypeDef;

typedef struct
{
	__IO uint32_t IODR;
	__IO uint32_t BSRR;
	__IO uint32_t OEN;
	__IO uint32_t PUE;
} GPIO_TypeDef;

typedef struct
{
	__IO uint32_t INTP_TYPE;
	__IO uint32_t INTP_STA;
} GPIO_INTP_TypeDef;

typedef struct
{
	#define GPIO_NUM	6
	GPIO_TypeDef GPIO[GPIO_NUM];
	__I  uint32_t RSVD0[(0x114 - 0x60)>>2];
	__I  uint32_t INTP[GPIO_NUM];
	__I  uint32_t RSVD1[(0x180 - 0x12C)>>2];
	__IO uint32_t ALT[GPIO_NUM];
	__I  uint32_t RSVD2[(0x200 - 0x198)>>2];
	__IO uint32_t SYS_CR1;
	__I  uint32_t RSVD3[(0x220 - 0x204)>>2];
	__IO uint32_t WAKE_EVEN_TYPE_EN;
	__I  uint32_t RSVD4[(0x228 - 0x224)>>2];
	__IO uint32_t WAKE_EVEN_SRC_L;
	__I  uint32_t RSVD5[(0x800 - 0x22C)>>2];
	GPIO_INTP_TypeDef INTP_TYPE_STA[GPIO_NUM];
} GPIO_MODULE_TypeDef;


typedef struct
{
	__IO uint32_t KEY[32];
	__IO uint32_t BPK_RDY;
    __IO uint32_t BPK_CLR;
    __IO uint32_t BPK_LRA;
    __IO uint32_t BPK_LWA;
    __IO uint32_t BPK_RR;
    __IO uint32_t BPK_LR;
    __O  uint32_t BPK_SCR;
    __I  uint32_t BPK_RSVD0;
	__IO uint32_t RTC_CS;
	__IO uint32_t RTC_REF;
	__IO uint32_t RTC_ARM;
	__I  uint32_t RTC_TIM;
	__O  uint32_t RTC_INTCLR;
	__IO uint32_t OSC32K_CR;
    __IO uint32_t RTC_ATTA_TIM;
	__IO uint32_t RESERVED2;
	__O uint32_t SEN_EXT_TYPE;
	__IO uint32_t SEN_EXT_CFG;
	__IO uint32_t SEN_SOFT_EN;
	__IO uint32_t SEN_STATE;
	__IO uint32_t SEN_BRIDGE;
	__IO uint32_t SEN_SOFT_ATTACK_SOFT_ATTACK;
	__IO uint32_t SEN_SOFT_LOCK;
	__IO uint32_t SEN_ATTACK_CNT;
	__IO uint32_t SEN_REG;
	__IO uint32_t SEN_VG_DETECT;
	__IO uint32_t SEN_RNG_INI;
	__IO uint32_t RESERVED3[(0x0104 - 0x00EC)/4];
	__IO uint32_t SEN_EN[19];
	__IO uint32_t SEN_EXTS_LOCK;
} BPU_MODULE_TypeDef;

typedef struct
{
	__IO uint32_t KEY[32];
	__IO uint32_t BPK_RDY;
    __IO uint32_t BPK_CLR;
    __IO uint32_t BPK_LRA;
    __IO uint32_t BPK_LWA;
    __IO uint32_t BPK_RR;
    __IO uint32_t BPK_LR;
    __O  uint32_t BPK_SCR;
    __I  uint32_t BPK_RSVD0;
} BPK_TypeDef;

typedef struct
{
	__IO uint32_t RTC_CS;
	__IO uint32_t RTC_REF;
	__IO uint32_t RTC_ARM;
	__I  uint32_t RTC_TIM;
	__O  uint32_t RTC_INTCLR;
	__IO uint32_t OSC32K_CR;
    __IO uint32_t RTC_ATTA_TIM;
} RTC_TypeDef;


typedef struct
{
	__O uint32_t SEN_EXT_TYPE;
	__IO uint32_t SEN_EXT_CFG;
	__IO uint32_t SEN_SOFT_EN;
	__IO uint32_t SEN_STATE;
	__IO uint32_t SEN_BRIDGE;
	__IO uint32_t SEN_SOFT_ATTACK;
	__IO uint32_t SEN_SOFT_LOCK;
	__IO uint32_t SEN_ATTACK_CNT;
	__IO uint32_t SEN_REG;
	__IO uint32_t SEN_VG_DETECT;
	__IO uint32_t SEN_RNG_INI;
	__IO uint32_t RESERVED3[(0x0104 - 0x00EC)/4];
	__IO uint32_t SEN_EN[19];
	__IO uint32_t SEN_EXTS_START;
	__IO uint32_t SEN_LOCK;
} SEN_TypeDef;

typedef struct
{
	__IO uint32_t RNG_CSR;
	__IO uint32_t RNG_DATA[2];
	__IO uint32_t RNG_AMA;
	__IO uint32_t RNG_PN;
	__IO uint32_t RNG_INDEX;
} TRNG_TypeDef;

typedef struct
{
	__IO uint32_t IC_CON;
	__IO uint32_t IC_TAR;
	__IO uint32_t IC_SAR;
	__IO uint32_t IC_HS_MADDR;
	__IO uint32_t IC_DATA_CMD;
	__IO uint32_t IC_SS_SCL_HCNT;
	__IO uint32_t IC_SS_SCL_LCNT;
	__IO uint32_t IC_FS_SCL_HCNT;
	__IO uint32_t IC_FS_SCL_LCNT;
	__IO uint32_t IC_HS_SCL_HCNT;
	__IO uint32_t IC_HS_SCL_LCNT;
	__I  uint32_t IC_INTR_STAT;
	__IO uint32_t IC_INTR_MASK;
	__I  uint32_t IC_RAW_INTR_STAT;
	__IO uint32_t IC_RX_TL;
	__IO uint32_t IC_TX_TL;
	__I  uint32_t IC_CLR_INTR;
	__I  uint32_t IC_CLR_RX_UNDER;
	__I  uint32_t IC_CLR_RX_OVER;
	__I  uint32_t IC_CLR_TX_OVER;
	__I  uint32_t IC_CLR_RD_REQ;
	__I  uint32_t IC_CLR_TX_ABRT;
	__I  uint32_t IC_CLR_RX_DONE;
	__I  uint32_t IC_CLR_ACTIVITY;
	__I  uint32_t IC_CLR_STOP_DET;
	__I  uint32_t IC_CLR_START_DET;
	__I  uint32_t IC_CLR_GEN_CALL;
	__IO uint32_t IC_ENABLE;
	__I  uint32_t IC_STATUS;
	__I  uint32_t IC_TXFLR;
	__I  uint32_t IC_RXFLR;
	__IO uint32_t IC_SDA_HOLD;
	__I  uint32_t IC_TX_ABRT_SOURCE;
	__IO uint32_t IC_SLV_DATA_NACK_ONLY;
	__IO uint32_t IC_DMA_CR;
	__IO uint32_t IC_DMA_TDLR;
	__IO uint32_t IC_DMA_RDLR;
	__IO uint32_t IC_SDA_SETUP;
	__IO uint32_t IC_ACK_GENERAL_CALL;
	__I  uint32_t IC_ENABLE_STATUS;
	__IO uint32_t IC_FS_SPKLEN;
	__IO uint32_t IC_HS_SPKLEN;	
} I2C_TypeDef;

typedef struct
{
	__IO uint32_t KCU_CTRL0;
	__IO uint32_t KCU_CTRL1;
	__I  uint32_t KCU_STATUS;
	__I  uint32_t KCU_EVENT;
	__IO uint32_t KCU_RNG;
} KCU_TypeDef;

typedef struct
{
	__IO uint32_t SAR_L;
	__IO uint32_t SAR_H;
	__IO uint32_t DAR_L;
	__IO uint32_t DAR_H;
	__IO uint32_t LLP_L;
	__IO uint32_t LLP_H;
	__IO uint32_t CTL_L;
	__IO uint32_t CTL_H;
	__IO uint32_t SSTAT_L;
	__IO uint32_t SSTAT_H;
	__IO uint32_t DSTAT_L;
	__IO uint32_t DSTAT_H;
	__IO uint32_t SSTATAR_L;
	__IO uint32_t SSTATAR_H;
	__IO uint32_t DSTATAR_L;
	__IO uint32_t DSTATAR_H;
	__IO uint32_t CFG_L;
	__IO uint32_t CFG_H;
	__IO uint32_t SGR_L;
	__IO uint32_t SGR_H;
	__IO uint32_t DSR_L;
	__IO uint32_t DSR_H;
} DMA_TypeDef;

typedef struct
{
	DMA_TypeDef DMA_Channel[4];

	__IO uint32_t RESERVED1[88];
	
	__I  uint32_t RawTfr_L;
	__I  uint32_t RawTfr_H;
	__I  uint32_t RawBlock_L;
	__I  uint32_t RawBlock_H;
	__I  uint32_t RawSrcTran_L;
	__I  uint32_t RawSrcTran_H;
	__I  uint32_t RawDstTran_L;
	__I  uint32_t RawDstTran_H;
	__I  uint32_t RawErr_L;
	__I  uint32_t RawErr_H;
	
	__I  uint32_t StatusTfr_L;
	__I  uint32_t StatusTfr_H;
	__I  uint32_t StatusBlock_L;
	__I  uint32_t StatusBlock_H;
	__I  uint32_t StatusSrcTran_L;
	__I  uint32_t StatusSrcTran_H;
	__I  uint32_t StatusDstTran_L;
	__I  uint32_t StatusDstTran_H;
	__I  uint32_t StatusErr_L;
	__I  uint32_t StatusErr_H;

	__IO uint32_t MaskTfr_L;
	__IO uint32_t MaskTfr_H;
	__IO uint32_t MaskBlock_L;
	__IO uint32_t MaskBlock_H;
	__IO uint32_t MaskSrcTran_L;
	__IO uint32_t MaskSrcTran_H;
	__IO uint32_t MaskDstTran_L;
	__IO uint32_t MaskDstTran_H;
	__IO uint32_t MaskErr_L;
	__IO uint32_t MaskErr_H;
	
	__O  uint32_t ClearTfr_L;
	__O  uint32_t ClearTfr_H;
	__O  uint32_t ClearBlock_L;
	__O  uint32_t ClearBlock_H;
	__O  uint32_t ClearSrcTran_L;
	__O  uint32_t ClearSrcTran_H;
	__O  uint32_t ClearDstTran_L;
	__O  uint32_t ClearDstTran_H;
	__O  uint32_t ClearErr_L;
	__O  uint32_t ClearErr_H;
	
	__I  uint32_t StatusInt_L;
	__I  uint32_t StatusInt_H;
	
	__IO uint32_t ReqSrcReg_L;
	__IO uint32_t ReqSrcReg_H;
	__IO uint32_t ReqDstReg_L;
	__IO uint32_t ReqDstReg_H;
	__IO uint32_t SglReqSrcReg_L;
	__IO uint32_t SglReqSrcReg_H;
	__IO uint32_t SglReqDstReg_L;
	__IO uint32_t SglReqDstReg_H;
	__IO uint32_t LstSrcReg_L;
	__IO uint32_t LstSrcReg_H;
	__IO uint32_t LstDstReg_L;
	__IO uint32_t LstDstReg_H;
	
	__IO uint32_t DmaCfgReg_L;
	__IO uint32_t DmaCfgReg_H;
	__IO uint32_t ChEnReg_L;
	__IO uint32_t ChEnReg_H;
	__I  uint32_t DmaIdReg_L;
	__I  uint32_t DmaIdReg_H;
	__IO uint32_t DmaTestReg_L;
	__IO uint32_t DmaTestReg_H;
	
	__IO uint32_t RESERVED2[4];
	
	__I  uint32_t DMA_COMP_PARAMS_6_L;
	__I  uint32_t DMA_COMP_PARAMS_6_H;
	__I  uint32_t DMA_COMP_PARAMS_5_L;
	__I  uint32_t DMA_COMP_PARAMS_5_H;
	__I  uint32_t DMA_COMP_PARAMS_4_L;
	__I  uint32_t DMA_COMP_PARAMS_4_H;
	__I  uint32_t DMA_COMP_PARAMS_3_L;
	__I  uint32_t DMA_COMP_PARAMS_3_H;
	__I  uint32_t DMA_COMP_PARAMS_2_L;
	__I  uint32_t DMA_COMP_PARAMS_2_H;
	__I  uint32_t DMA_COMP_PARAMS_1_L;
	__I  uint32_t DMA_COMP_PARAMS_1_H;
	__I  uint32_t DMA_Component_ID_Register_L;
	__I  uint32_t DMA_Component_ID_Register_H;

} DMA_MODULE_TypeDef;

typedef struct
{
  __IO uint32_t lcdi_ctrl;
  __IO uint32_t lcdi_cycle;
  __IO uint32_t lcdi_status;
  __IO uint32_t lcdi_data;
} LCD_TypeDef;

typedef struct
{
  __IO uint32_t SCI_DATA;
  __IO uint32_t SCI_CR0;
  __IO uint32_t SCI_CR1;
  __IO uint32_t SCI_CR2;
  __IO uint32_t SCI_IER;
  __IO uint32_t SCI_RETRY;
  __IO uint32_t SCI_TIDE;
  __IO uint32_t SCI_TXCOUNT;
  __IO uint32_t SCI_RXCOUNT;
  __I  uint32_t SCI_FR;
  __IO uint32_t SCI_RXTIME;
  __IO uint32_t SCI_ISTAT;
  __IO uint32_t SCI_STABLE;
  __IO uint32_t SCI_ATIME;
  __IO uint32_t SCI_DTIME;

  __IO uint32_t SCI_ATRSTIME;
  __IO uint32_t SCI_ATRDTIME;
  __IO uint32_t SCI_BLKTIME;
  __IO uint32_t SCI_CHTIME;
  __IO uint32_t SCI_CLKICC;
  __IO uint32_t SCI_BAUD;
  __IO uint32_t SCI_VALUE;
  __IO uint32_t SCI_CHGUARD;
  __IO uint32_t SCI_BLKGUARD;
  __IO uint32_t SCI_SYNCCR;
  __IO uint32_t SCI_SYNCDATA;
  __IO uint32_t SCI_RAWSTAT;
  __IO uint32_t SCI_IIR;
  __I  uint32_t SCI_RES1[4];
  __I  uint32_t SCI_RES2[32];
} SCI_TypeDef;

typedef struct
{
	__IO uint32_t  CFG;
	__IO uint32_t  CS;
	__IO uint32_t  PROT;
	__IO uint32_t *ADDR;
    __IO uint32_t  PDATA;
	__IO uint32_t  RO;
	__IO uint32_t  ROL;
	__IO uint32_t  RO_INFO;
	__IO uint32_t  ROL_INFO;
	__IO uint32_t  HIDE_INFO;
	__IO uint32_t  SCR;
	__IO uint32_t  TIM;
    __IO uint32_t  SRC_LOCK;
    __IO uint32_t  SRC_SHDW;
    __IO uint32_t  SRC_INFO_EN;
} FCU_TypeDef;

typedef struct
{
	__IO uint32_t  SSC_CR1;
	__IO uint32_t  SSC_CR2;
	__IO uint32_t  SSC_CR3;
	__O  uint32_t  RESERVED0[(0x0104-0x000C)/4];
	__IO uint32_t  SSC_SR;
	__IO uint32_t  SSC_SR_CLR;
	__IO uint32_t  SSC_ACK;
	__O  uint32_t  RESERVED1[(0x0184-0x0110)/4];
	__IO uint32_t  DATARAM_SCR;
	__O  uint32_t  RESERVED2[(0x01FC-0x0188)/4];
	__IO uint32_t  BPU_RWC;
	__O  uint32_t  RESERVED3[(0x03EC-0x0200)/4];
	__IO uint32_t  MAIN_SEN_LOCK;
	__IO uint32_t  MAIN_SEN_EN;
} SSC_TypeDef;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group <Device>_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* ToDo: add here your device peripherals base addresses
         following is an example for timer                                    */
/** @addtogroup <Device>_MemoryMap <Device> Memory Mapping
  @{
*/

/* Peripheral and SRAM base address */
#define MHSCPU_FLASH_BASE						(0x01000000UL)                              /*!< (FLASH     ) Base Address */
#define MHSCPU_SRAM_BASE						(0x20000000UL)                              /*!< (SRAM      ) Base Address */
#define MHSCPU_PERIPH_BASE						(0x40000000UL)                              /*!< (Peripheral) Base Address */

#define MHSCPU_EXT_FLASH_BASE					(0x60000000UL)
#define MHSCPU_EXT_RAM_BASE						(0x80000000UL)


#define FLASH_SIZE              		        (1UL << 20)
#define FLASH_BASE              		        (0x01000000UL)

#define OTP_SIZE                		        (1UL << 14)
#define OTP_BASE                		        (0x02000000UL)

#define SRAM_SIZE						        (0x00040000UL)
#define SRAM_BASE						        (0x20000000UL)

/* Peripheral memory map */
#define MHSCPU_AHB_BASE							(MHSCPU_PERIPH_BASE)
#define MHSCPU_APB0_BASE						(MHSCPU_PERIPH_BASE + 0x10000)
#define MHSCPU_APB1_BASE						(MHSCPU_PERIPH_BASE + 0x20000)
#define MHSCPU_APB2_BASE						(MHSCPU_PERIPH_BASE + 0x30000)

#define SSC_BASE								(MHSCPU_AHB_BASE + 0x0000)
#define DMA_BASE								(MHSCPU_AHB_BASE + 0x0800)
#define	USB_BASE								(MHSCPU_AHB_BASE + 0x0C00)
#define LCD_BASE								(MHSCPU_AHB_BASE + 0x1000)
#define FCU_BASE								(MHSCPU_AHB_BASE + 0x1400)

#define SCI0_BASE								(MHSCPU_APB0_BASE)
#define SCI1_BASE								(MHSCPU_APB0_BASE + 0x1000)
#define SCI2_BASE								(MHSCPU_APB0_BASE + 0x2000)
#define I2C0_BASE								(MHSCPU_APB0_BASE + 0x4000)
#define UART0_BASE								(MHSCPU_APB0_BASE + 0x6000)
#define UART1_BASE								(MHSCPU_APB0_BASE + 0x7000)
#define UART2_BASE								(MHSCPU_APB0_BASE + 0x8000)
#define UART3_BASE								(MHSCPU_APB0_BASE + 0x9000)
#define TRNG_BASE								(MHSCPU_APB0_BASE + 0xE000)
#define SYSCTRL_BASE							(MHSCPU_APB0_BASE + 0xF000)

#define SPI_SLAVE_OFFSET_MASTER                 (0x1000)
#define TIMM0_BASE								(MHSCPU_APB1_BASE)
#define TIM0_BASE								(TIMM0_BASE + 0x0000)
#define TIM1_BASE								(TIM0_BASE + 0x0014)
#define TIM2_BASE								(TIM1_BASE + 0x0014)
#define TIM3_BASE								(TIM2_BASE + 0x0014)
#define TIM4_BASE								(TIM3_BASE + 0x0014)
#define TIM5_BASE								(TIM4_BASE + 0x0014)
#define SPIM3_BASE								(MHSCPU_APB1_BASE + 0x2000)
#define ADC_BASE								(MHSCPU_APB1_BASE + 0x4000)
#define KCU_BASE								(MHSCPU_APB1_BASE + 0x5000)
#define GPIO_BASE								(MHSCPU_APB1_BASE + 0x6000)
#define CRC_BASE								(MHSCPU_APB1_BASE + 0x8000)
#define SPIM0_BASE								(MHSCPU_APB1_BASE + 0x9000)
#define SPIS0_BASE								(MHSCPU_APB1_BASE + 0xA000)
#define SPIM1_BASE								(MHSCPU_APB1_BASE + 0xB000)
#define SPIS1_BASE								(MHSCPU_APB1_BASE + 0xC000)
#define SPIM2_BASE								(MHSCPU_APB1_BASE + 0xD000)
#define SPIS2_BASE								(MHSCPU_APB1_BASE + 0xE000)
#define WDG_BASE								(MHSCPU_APB1_BASE + 0xF000)


#define BPU_BASE								(MHSCPU_APB2_BASE)

/*@}*/ /* end of group <Device>_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer                                    */

/** @addtogroup <Device>_PeripheralDecl <Device> Peripheral Declaration
  @{
*/
#define SYSCTRL									((SYSCTRL_TypeDef *) SYSCTRL_BASE)

#define UART0									((UART_TypeDef *) UART0_BASE)
#define UART1									((UART_TypeDef *) UART1_BASE)
#define UART2									((UART_TypeDef *) UART2_BASE)
#define UART3									((UART_TypeDef *) UART3_BASE)

#define SPIM0									((SPI_TypeDef *) SPIM0_BASE)
#define SPIS0									((SPI_TypeDef *) SPIS0_BASE)
#define SPIM1									((SPI_TypeDef *) SPIM1_BASE)
#define SPIS1									((SPI_TypeDef *) SPIS1_BASE)
#define SPIM2									((SPI_TypeDef *) SPIM2_BASE)
#define SPIS2									((SPI_TypeDef *) SPIS2_BASE)
#define SPIM3									((SPI_TypeDef *) SPIM3_BASE)

#define I2C0									((I2C_TypeDef *) I2C0_BASE)

#define SCI0                                    ((SCI_TypeDef *) SCI0_BASE)
#define SCI1                                    ((SCI_TypeDef *) SCI1_BASE)
#define SCI2                                    ((SCI_TypeDef *) SCI2_BASE)

#define TIMM0 									((TIM_Module_TypeDef *) TIMM0_BASE)

#define ADC										((ADC_TypeDef *)ADC_BASE) 

#define TRNG									((TRNG_TypeDef *)TRNG_BASE)
#define LCD										((LCD_TypeDef *)LCD_BASE)
#define KCU										((KCU_TypeDef *)KCU_BASE)   
#define CRC										((CRC_TypeDef *)CRC_BASE)

#define DMA										((DMA_MODULE_TypeDef *)DMA_BASE)
#define DMA_Channel_0							((DMA_TypeDef *)DMA_BASE)
#define DMA_Channel_1							((DMA_TypeDef *)(DMA_BASE + 0x58))
#define DMA_Channel_2							((DMA_TypeDef *)(DMA_BASE + 0x58*2))
#define DMA_Channel_3							((DMA_TypeDef *)(DMA_BASE + 0x58*3))

#define GPIO									((GPIO_MODULE_TypeDef *)GPIO_BASE)
#define GPIOA									((GPIO_TypeDef *)GPIO_BASE)
#define GPIOB									((GPIO_TypeDef *)(GPIO_BASE + 0x0010))
#define GPIOC									((GPIO_TypeDef *)(GPIO_BASE + 0x0020))
#define GPIOD									((GPIO_TypeDef *)(GPIO_BASE + 0x0030))
#define GPIOE									((GPIO_TypeDef *)(GPIO_BASE + 0x0040))
#define GPIOF									((GPIO_TypeDef *)(GPIO_BASE + 0x0050))
#define GPIO_GROUP								((GPIO_TypeDef *)GPIO_BASE)
#define GPIO_ALT_GROUP							((__IO uint32_t *)(GPIO_BASE + 0x180))

#define WDT										((WDT_TypeDef *)WDG_BASE)
#define FCU										((FCU_TypeDef *)FCU_BASE)
#define SSC										((SSC_TypeDef *)SSC_BASE)   

#define BPU										((BPU_MODULE_TypeDef *)BPU_BASE)
#define BPK										((BPK_TypeDef *)BPU_BASE)
#define RTC										((RTC_TypeDef *)(BPU_BASE + 0xA0))
#define SENSOR									((SEN_TypeDef *)(BPU_BASE + 0xC0))           

/** @addtogroup Exported_constants
  * @{
  */
  
  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */
    
/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                Universal Asynchronous Receiver Transmitter                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for UART_RBR register  *******************/
#define UART_RBR_RBR                         ((uint32_t)0x01FF)            /*!< Data value */

/*******************  Bit definition for UART_THR register  *******************/
#define UART_THR_THR                         ((uint32_t)0x01FF)            /*!< Data value */

/*******************  Bit definition for UART_DLH register  *******************/
#define UART_DLH_DLH                         ((uint32_t)0x0FF)

/*******************  Bit definition for UART_DLL register  *******************/
#define UART_DLL_DLL                         ((uint32_t)0x0FF)

/*******************  Bit definition for UART_IER register  *******************/
#define UART_IER_ERBFI							((uint32_t)0x0001)
#define UART_IER_ETBEI							((uint32_t)0x0002)
#define UART_IER_ELSI							((uint32_t)0x0004)
#define UART_IER_EDSSI							((uint32_t)0x0008)
#define UART_IER_PTIME							((uint32_t)0x0080)

/*******************  Bit definition for UART_IIR register  *******************/
#define UART_IIR_IID							((uint32_t)0x0007)
#define UART_IIR_IID_0							((uint32_t)0x0001)
#define UART_IIR_IID_1							((uint32_t)0x0002)
#define UART_IIR_IID_2							((uint32_t)0x0004)
#define UART_IIR_IID_3							((uint32_t)0x0008)
#define UART_IIR_FIFOSE							((uint32_t)0x0060)
#define UART_IIR_FIFOSE_0						((uint32_t)0x0020)
#define UART_IIR_FIFOSE_1						((uint32_t)0x0040)

/*******************  Bit definition for UART_FCR register  *******************/
#define UART_FCR_FIFOE							((uint32_t)0x0001)
#define UART_FCR_RFIFOR							((uint32_t)0x0002)
#define UART_FCR_XFIFOR							((uint32_t)0x0004)
#define UART_FCR_DMAM							((uint32_t)0x0008)
#define UART_FCR_TET							((uint32_t)0x0030)
#define UART_FCR_TET_0							((uint32_t)0x0010)
#define UART_FCR_TET_1							((uint32_t)0x0020)
#define UART_FCR_RCVER							((uint32_t)0x00C0)
#define UART_FCR_RCVER_0						((uint32_t)0x0040)
#define UART_FCR_RCVER_1						((uint32_t)0x0080)

/*******************  Bit definition for UART_LCR register  *******************/
#define UART_LCR_DLS							((uint32_t)0x0003)
#define UART_LCR_DLS_0							((uint32_t)0x0001)
#define UART_LCR_DLS_1							((uint32_t)0x0002)
#define UART_LCR_STOP							((uint32_t)0x0004)
#define UART_LCR_PEN							((uint32_t)0x0008)
#define UART_LCR_EPS							((uint32_t)0x0010)
#define UART_LCR_SP								((uint32_t)0x0020)
#define UART_LCR_BC								((uint32_t)0x0040)
#define UART_LCR_DLAB							((uint32_t)0x0080)

/*******************  Bit definition for UART_MCR register  *******************/
#define UART_MCR_DTR							((uint32_t)0x0001)
#define UART_MCR_RTS							((uint32_t)0x0002)
#define UART_MCR_OUT1							((uint32_t)0x0004)
#define UART_MCR_OUT2							((uint32_t)0x0008)
#define UART_MCR_LB								((uint32_t)0x0010)
#define UART_MCR_AFCE							((uint32_t)0x0020)
#define UART_MCR_SIRE							((uint32_t)0x0040)

/*******************  Bit definition for UART_LSR register  *******************/
#define UART_LSR_DR								((uint32_t)0x0001)
#define UART_LSR_OE								((uint32_t)0x0002)
#define UART_LSR_PE								((uint32_t)0x0004)
#define UART_LSR_FE								((uint32_t)0x0008)
#define UART_LSR_BI								((uint32_t)0x0010)
#define UART_LSR_THRE							((uint32_t)0x0020)
#define UART_LSR_TEMT							((uint32_t)0x0040)
#define UART_LSR_PFE							((uint32_t)0x0080)

/*******************  Bit definition for UART_MSR register  *******************/
#define UART_MSR_DCTS								((uint32_t)0x0001)
#define UART_MSR_DDSR								((uint32_t)0x0002)
#define UART_MSR_TERI								((uint32_t)0x0004)
#define UART_MSR_DDCD								((uint32_t)0x0008)
#define UART_MSR_CTS								((uint32_t)0x0010)
#define UART_MSR_DSR								((uint32_t)0x0020)
#define UART_MSR_RI									((uint32_t)0x0040)
#define UART_MSR_DCD								((uint32_t)0x0080)

/*******************  Bit definition for UART_SRBR register  *******************/
#define UART_SRBR_SRBR							((uint32_t)0x01FF)            /*!< Data value */

/*******************  Bit definition for UART_STHR register  *******************/
#define UART_STHR_STHR							((uint32_t)0x01FF)            /*!< Data value */

/*******************  Bit definition for UART_FAR register  *******************/
#define UART_FAR_FAR							((uint32_t)0x0001)

/*******************  Bit definition for UART_TFR register  *******************/
#define UART_TFR_TFR							((uint32_t)0x00FF)

/*******************  Bit definition for UART_RFW register  *******************/
#define UART_RFW_RFWD							((uint32_t)0x00FF)
#define UART_RFW_RFPE							((uint32_t)0x0100)
#define UART_RFW_RFFE							((uint32_t)0x0200)

/*******************  Bit definition for UART_USR register  *******************/
#define UART_USR_BUSY								((uint32_t)0x0001)
#define UART_USR_TFNF								((uint32_t)0x0002)
#define UART_USR_TFE								((uint32_t)0x0004)
#define UART_USR_RFNE								((uint32_t)0x0008)
#define UART_USR_RFF								((uint32_t)0x0010)

/*******************  Bit definition for UART_TFL register  *******************/
#define UART_TFL_TFL								((uint32_t)0x000F)

/*******************  Bit definition for UART_RFL register  *******************/
#define UART_RFL_RFL								((uint32_t)0x000F)

/*******************  Bit definition for UART_SRR register  *******************/
#define UART_SRR_UR									((uint32_t)0x0001)
#define UART_SRR_RFR								((uint32_t)0x0002)
#define UART_SRR_XFR								((uint32_t)0x0004)

/*******************  Bit definition for UART_SRR register  *******************/
#define UART_SRR_UR									((uint32_t)0x0001)

/*******************  Bit definition for UART_SRTS register  *******************/
#define UART_SRTS_SRTS								((uint32_t)0x0001)

/*******************  Bit definition for UART_SBCR register  *******************/
#define UART_SBCR_SBCR								((uint32_t)0x0001)

/*******************  Bit definition for UART_SDMAM register  *******************/
#define UART_SDMAM_SDMAM							((uint32_t)0x0001)

/*******************  Bit definition for UART_SFE register  *******************/
#define UART_SFE_SFE								((uint32_t)0x0001)

/*******************  Bit definition for UART_SRT register  *******************/
#define UART_SRT_SRT								((uint32_t)0x0003)
#define UART_SRT_SRT_0								((uint32_t)0x0001)
#define UART_SRT_SRT_1								((uint32_t)0x0002)

/*******************  Bit definition for UART_STET register  *******************/
#define UART_STET_STET								((uint32_t)0x0003)
#define UART_STET_STET_0							((uint32_t)0x0001)
#define UART_STET_STET_1							((uint32_t)0x0002)

/*******************  Bit definition for UART_HTX register  *******************/
#define UART_HTX_HTX								((uint32_t)0x0001)

/*******************  Bit definition for UART_DMASA register  *******************/
#define UART_DMASA_DMASA							((uint32_t)0x0001)

/******************************************************************************/
/*                                                                            */
/*                General Purpose and Alternate Function I/O                  */
/*                                                                            */
/******************************************************************************/

/*!<******************  Bit definition for GPIO_IODR register  *******************/
#define GPIO_IODR_ODR0                        ((uint16_t)0x00000001)            /*!< Port output data, bit 0 */
#define GPIO_IODR_ODR1                        ((uint16_t)0x00000002)            /*!< Port output data, bit 1 */
#define GPIO_IODR_ODR2                        ((uint16_t)0x00000004)            /*!< Port output data, bit 2 */
#define GPIO_IODR_ODR3                        ((uint16_t)0x00000008)            /*!< Port output data, bit 3 */
#define GPIO_IODR_ODR4                        ((uint16_t)0x00000010)            /*!< Port output data, bit 4 */
#define GPIO_IODR_ODR5                        ((uint16_t)0x00000020)            /*!< Port output data, bit 5 */
#define GPIO_IODR_ODR6                        ((uint16_t)0x00000040)            /*!< Port output data, bit 6 */
#define GPIO_IODR_ODR7                        ((uint16_t)0x00000080)            /*!< Port output data, bit 7 */
#define GPIO_IODR_ODR8                        ((uint16_t)0x00000100)            /*!< Port output data, bit 8 */
#define GPIO_IODR_ODR9                        ((uint16_t)0x00000200)            /*!< Port output data, bit 9 */
#define GPIO_IODR_ODR10                       ((uint16_t)0x00000400)            /*!< Port output data, bit 10 */
#define GPIO_IODR_ODR11                       ((uint16_t)0x00000800)            /*!< Port output data, bit 11 */
#define GPIO_IODR_ODR12                       ((uint16_t)0x00001000)            /*!< Port output data, bit 12 */
#define GPIO_IODR_ODR13                       ((uint16_t)0x00002000)            /*!< Port output data, bit 13 */
#define GPIO_IODR_ODR14                       ((uint16_t)0x00004000)            /*!< Port output data, bit 14 */
#define GPIO_IODR_ODR15                       ((uint16_t)0x00008000)            /*!< Port output data, bit 15 */

#define GPIO_IODR_IDR0                        ((uint16_t)0x00010000)            /*!< Port input data, bit 0 */
#define GPIO_IODR_IDR1                        ((uint16_t)0x00020000)            /*!< Port input data, bit 1 */
#define GPIO_IODR_IDR2                        ((uint16_t)0x00040000)            /*!< Port input data, bit 2 */
#define GPIO_IODR_IDR3                        ((uint16_t)0x00080000)            /*!< Port input data, bit 3 */
#define GPIO_IODR_IDR4                        ((uint16_t)0x00100000)            /*!< Port input data, bit 4 */
#define GPIO_IODR_IDR5                        ((uint16_t)0x00200000)            /*!< Port input data, bit 5 */
#define GPIO_IODR_IDR6                        ((uint16_t)0x00400000)            /*!< Port input data, bit 6 */
#define GPIO_IODR_IDR7                        ((uint16_t)0x00800000)            /*!< Port input data, bit 7 */
#define GPIO_IODR_IDR8                        ((uint16_t)0x01000000)            /*!< Port input data, bit 8 */
#define GPIO_IODR_IDR9                        ((uint16_t)0x02000000)            /*!< Port input data, bit 9 */
#define GPIO_IODR_IDR10                       ((uint16_t)0x04000000)            /*!< Port input data, bit 10 */
#define GPIO_IODR_IDR11                       ((uint16_t)0x08000000)            /*!< Port input data, bit 11 */
#define GPIO_IODR_IDR12                       ((uint16_t)0x10000000)            /*!< Port input data, bit 12 */
#define GPIO_IODR_IDR13                       ((uint16_t)0x20000000)            /*!< Port input data, bit 13 */
#define GPIO_IODR_IDR14                       ((uint16_t)0x40000000)            /*!< Port input data, bit 14 */
#define GPIO_IODR_IDR15                       ((uint16_t)0x80000000)            /*!< Port input data, bit 15 */

/******************  Bit definition for GPIO_BSRR register  *******************/
#define GPIO_BSRR_BS0                        ((uint32_t)0x00000001)        /*!< Port x Set bit 0 */
#define GPIO_BSRR_BS1                        ((uint32_t)0x00000002)        /*!< Port x Set bit 1 */
#define GPIO_BSRR_BS2                        ((uint32_t)0x00000004)        /*!< Port x Set bit 2 */
#define GPIO_BSRR_BS3                        ((uint32_t)0x00000008)        /*!< Port x Set bit 3 */
#define GPIO_BSRR_BS4                        ((uint32_t)0x00000010)        /*!< Port x Set bit 4 */
#define GPIO_BSRR_BS5                        ((uint32_t)0x00000020)        /*!< Port x Set bit 5 */
#define GPIO_BSRR_BS6                        ((uint32_t)0x00000040)        /*!< Port x Set bit 6 */
#define GPIO_BSRR_BS7                        ((uint32_t)0x00000080)        /*!< Port x Set bit 7 */
#define GPIO_BSRR_BS8                        ((uint32_t)0x00000100)        /*!< Port x Set bit 8 */
#define GPIO_BSRR_BS9                        ((uint32_t)0x00000200)        /*!< Port x Set bit 9 */
#define GPIO_BSRR_BS10                       ((uint32_t)0x00000400)        /*!< Port x Set bit 10 */
#define GPIO_BSRR_BS11                       ((uint32_t)0x00000800)        /*!< Port x Set bit 11 */
#define GPIO_BSRR_BS12                       ((uint32_t)0x00001000)        /*!< Port x Set bit 12 */
#define GPIO_BSRR_BS13                       ((uint32_t)0x00002000)        /*!< Port x Set bit 13 */
#define GPIO_BSRR_BS14                       ((uint32_t)0x00004000)        /*!< Port x Set bit 14 */
#define GPIO_BSRR_BS15                       ((uint32_t)0x00008000)        /*!< Port x Set bit 15 */

#define GPIO_BSRR_BR0                        ((uint32_t)0x00010000)        /*!< Port x Reset bit 0 */
#define GPIO_BSRR_BR1                        ((uint32_t)0x00020000)        /*!< Port x Reset bit 1 */
#define GPIO_BSRR_BR2                        ((uint32_t)0x00040000)        /*!< Port x Reset bit 2 */
#define GPIO_BSRR_BR3                        ((uint32_t)0x00080000)        /*!< Port x Reset bit 3 */
#define GPIO_BSRR_BR4                        ((uint32_t)0x00100000)        /*!< Port x Reset bit 4 */
#define GPIO_BSRR_BR5                        ((uint32_t)0x00200000)        /*!< Port x Reset bit 5 */
#define GPIO_BSRR_BR6                        ((uint32_t)0x00400000)        /*!< Port x Reset bit 6 */
#define GPIO_BSRR_BR7                        ((uint32_t)0x00800000)        /*!< Port x Reset bit 7 */
#define GPIO_BSRR_BR8                        ((uint32_t)0x01000000)        /*!< Port x Reset bit 8 */
#define GPIO_BSRR_BR9                        ((uint32_t)0x02000000)        /*!< Port x Reset bit 9 */
#define GPIO_BSRR_BR10                       ((uint32_t)0x04000000)        /*!< Port x Reset bit 10 */
#define GPIO_BSRR_BR11                       ((uint32_t)0x08000000)        /*!< Port x Reset bit 11 */
#define GPIO_BSRR_BR12                       ((uint32_t)0x10000000)        /*!< Port x Reset bit 12 */
#define GPIO_BSRR_BR13                       ((uint32_t)0x20000000)        /*!< Port x Reset bit 13 */
#define GPIO_BSRR_BR14                       ((uint32_t)0x40000000)        /*!< Port x Reset bit 14 */
#define GPIO_BSRR_BR15                       ((uint32_t)0x80000000)        /*!< Port x Reset bit 15 */

/******************  Bit definition for GPIO_OEN register  *******************/
#define GPIO_OEN_0							((uint32_t)0x00000001)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_1							((uint32_t)0x00000002)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_2							((uint32_t)0x00000004)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_3							((uint32_t)0x00000008)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_4							((uint32_t)0x00000010)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_5							((uint32_t)0x00000020)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_6							((uint32_t)0x00000040)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_7							((uint32_t)0x00000080)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_8							((uint32_t)0x00000100)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_9							((uint32_t)0x00000200)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_10							((uint32_t)0x00000400)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_11							((uint32_t)0x00000800)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_12							((uint32_t)0x00001000)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_13							((uint32_t)0x00002000)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_14							((uint32_t)0x00004000)        /*!< 1:Port Input 0:Port Output */
#define GPIO_OEN_15							((uint32_t)0x00008000)        /*!< 1:Port Input 0:Port Output */

/******************  Bit definition for GPIO_PUE register  *******************/
#define GPIO_PUE_0							((uint32_t)0x00000001)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_1							((uint32_t)0x00000002)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_2							((uint32_t)0x00000004)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_3							((uint32_t)0x00000008)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_4							((uint32_t)0x00000010)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_5							((uint32_t)0x00000020)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_6							((uint32_t)0x00000040)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_7							((uint32_t)0x00000080)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_8							((uint32_t)0x00000100)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_9							((uint32_t)0x00000200)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_10							((uint32_t)0x00000400)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_11							((uint32_t)0x00000800)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_12							((uint32_t)0x00001000)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_13							((uint32_t)0x00002000)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_14							((uint32_t)0x00004000)        /*!< 1:PullUp Enable 0:PullUp Disable */
#define GPIO_PUE_15							((uint32_t)0x00008000)        /*!< 1:PullUp Enable 0:PullUp Disable */

/******************  Bit definition for GPIO_ALT register  *******************/
#define GPIO_ALT_PORT0						(BIT0 | BIT1)
#define GPIO_ALT_PORT0_0					(BIT0)
#define GPIO_ALT_PORT0_1					(BIT1)
#define GPIO_ALT_PORT1						(BIT2 | BIT3)
#define GPIO_ALT_PORT1_0					(BIT2)
#define GPIO_ALT_PORT1_1					(BIT3)
#define GPIO_ALT_PORT2						(BIT4 | BIT5)
#define GPIO_ALT_PORT2_0					(BIT4)
#define GPIO_ALT_PORT2_1					(BIT5)
#define GPIO_ALT_PORT3						(BIT6 | BIT7)
#define GPIO_ALT_PORT3_0					(BIT6)
#define GPIO_ALT_PORT3_1					(BIT7)
#define GPIO_ALT_PORT4						(BIT8 | BIT9)
#define GPIO_ALT_PORT4_0					(BIT8)
#define GPIO_ALT_PORT4_1					(BIT9)
#define GPIO_ALT_PORT5						(BIT10 | BIT11)
#define GPIO_ALT_PORT5_0					(BIT10)
#define GPIO_ALT_PORT5_1					(BIT11)
#define GPIO_ALT_PORT6						(BIT12 | BIT13)
#define GPIO_ALT_PORT6_0					(BIT12)
#define GPIO_ALT_PORT6_1					(BIT13)
#define GPIO_ALT_PORT7						(BIT14 | BIT15)
#define GPIO_ALT_PORT7_0					(BIT14)
#define GPIO_ALT_PORT7_1					(BIT15)
#define GPIO_ALT_PORT8						(BIT16 | BIT17)
#define GPIO_ALT_PORT8_0					(BIT16)
#define GPIO_ALT_PORT8_1					(BIT17)
#define GPIO_ALT_PORT9						(BIT18 | BIT19)
#define GPIO_ALT_PORT9_0					(BIT18)
#define GPIO_ALT_PORT9_1					(BIT19)
#define GPIO_ALT_PORT10						(BIT20 | BIT21)
#define GPIO_ALT_PORT10_0					(BIT20)
#define GPIO_ALT_PORT10_1					(BIT21)
#define GPIO_ALT_PORT11						(BIT22 | BIT23)
#define GPIO_ALT_PORT11_0					(BIT22)
#define GPIO_ALT_PORT11_1					(BIT23)
#define GPIO_ALT_PORT12						(BIT24 | BIT25)
#define GPIO_ALT_PORT12_0					(BIT24)
#define GPIO_ALT_PORT12_1					(BIT25)
#define GPIO_ALT_PORT13						(BIT26 | BIT27)
#define GPIO_ALT_PORT13_0					(BIT26)
#define GPIO_ALT_PORT13_1					(BIT27)
#define GPIO_ALT_PORT14						(BIT28 | BIT29)
#define GPIO_ALT_PORT14_0					(BIT28)
#define GPIO_ALT_PORT14_1					(BIT29)
#define GPIO_ALT_PORT15						(BIT30 | BIT31)
#define GPIO_ALT_PORT15_0					(BIT30)
#define GPIO_ALT_PORT15_1					(BIT31)

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface                         */
/*                                                                            */
/******************************************************************************/

/*****************  Bit definition for SPI_CTRLR0 register  *******************/
#define SPI_CTRLR0_DFS						((uint32_t)0x000F)
#define SPI_CTRLR0_DFS_0					((uint32_t)0x0001)
#define SPI_CTRLR0_DFS_1					((uint32_t)0x0002)
#define SPI_CTRLR0_DFS_2					((uint32_t)0x0004)
#define SPI_CTRLR0_DFS_3					((uint32_t)0x0008)
#define SPI_CTRLR0_FRF						((uint32_t)0x0030)
#define SPI_CTRLR0_FRF_0					((uint32_t)0x0010)
#define SPI_CTRLR0_FRF_1					((uint32_t)0x0020)
#define SPI_CTRLR0_SCPH						((uint32_t)0x0040)
#define SPI_CTRLR0_SCPOL					((uint32_t)0x0080)
#define SPI_CTRLR0_TMOD						((uint32_t)0x0300)
#define SPI_CTRLR0_TMOD_0					((uint32_t)0x0100)
#define SPI_CTRLR0_TMOD_1					((uint32_t)0x0200)
#define SPI_CTRLR0_SLV_OE					((uint32_t)0x0400)
#define SPI_CTRLR0_SRL						((uint32_t)0x0800)
#define SPI_CTRLR0_CFS						((uint32_t)0xF000)

/*****************  Bit definition for SPI_CTRLR1 register  *******************/
#define SPI_CTRLR0_NDF						((uint32_t)0xFFFF)

/*****************  Bit definition for SPI_SSIENR register  *******************/
#define SPI_SSIENR_SSIENR					((uint32_t)0x0001)

/*****************  Bit definition for SPI_MWCR register  *********************/
#define SPI_MWCR_MWMOD						((uint32_t)0x0001)
#define SPI_MWCR_MDD						((uint32_t)0x0002)
#define SPI_MWCR_MHS						((uint32_t)0x0004)

/*****************  Bit definition for SPI_SER register  **********************/
#define SPI_SER_SER							((uint32_t)0x000F)
#define SPI_SER_0							((uint32_t)0x0001)
#define SPI_SER_1							((uint32_t)0x0002)
#define SPI_SER_2							((uint32_t)0x0004)
#define SPI_SER_3							((uint32_t)0x0008)

/*****************  Bit definition for SPI_BAUDR register  ********************/
#define SPI_BAUDR_BAUDR						((uint32_t)0xFFFF)

/*****************  Bit definition for SPI_TXFTLR register  *******************/
#define SPI_TXFTLR_TFT						((uint32_t)0x000F)
/*****************  Bit definition for SPI_RXFTLR register  *******************/
#define SPI_RXFTLR_RFT						((uint32_t)0x000F)
/*****************  Bit definition for SPI_TXFLR register  ********************/
#define SPI_TXFLR_TXTFL						((uint32_t)0x001F)
/*****************  Bit definition for SPI_RXFLR register  ********************/
#define SPI_RXFLR_RXTFL						((uint32_t)0x001F)

/*****************  Bit definition for SPI_SR register  ***********************/
#define SPI_SR_BUSY							((uint32_t)0x0001)
#define SPI_SR_TFNF							((uint32_t)0x0002)
#define SPI_SR_TFE							((uint32_t)0x0004)
#define SPI_SR_RFNE							((uint32_t)0x0008)
#define SPI_SR_RFF							((uint32_t)0x0010)
#define SPI_SR_TXE							((uint32_t)0x0020)
#define SPI_SR_DCOL							((uint32_t)0x0040)
/*****************  Bit definition for SPI_IMR register  **********************/
#define SPI_IMR_TXEIM						((uint32_t)0x0001)
#define SPI_IMR_TXOIM						((uint32_t)0x0002)
#define SPI_IMR_RXUIM						((uint32_t)0x0004)
#define SPI_IMR_RXOIM						((uint32_t)0x0008)
#define SPI_IMR_RXFIM						((uint32_t)0x0010)
#define SPI_IMR_MSTIM						((uint32_t)0x0020)
/*****************  Bit definition for SPI_ISR register  **********************/
#define SPI_ISR_TXEIS						((uint32_t)0x0001)
#define SPI_ISR_TXOIS						((uint32_t)0x0002)
#define SPI_ISR_RXUIS						((uint32_t)0x0004)
#define SPI_ISR_RXOIS						((uint32_t)0x0008)
#define SPI_ISR_RXFIS						((uint32_t)0x0010)
#define SPI_ISR_MSTIS						((uint32_t)0x0020)
/*****************  Bit definition for SPI_RISR register  *********************/
#define SPI_RISR_TXEIR						((uint32_t)0x0001)
#define SPI_RISR_TXOIR						((uint32_t)0x0002)
#define SPI_RISR_RXUIR						((uint32_t)0x0004)
#define SPI_RISR_RXOIR						((uint32_t)0x0008)
#define SPI_RISR_RXFIR						((uint32_t)0x0010)
#define SPI_RISR_MSTIR						((uint32_t)0x0020)
/*****************  Bit definition for SPI_TXOICR register  *******************/
#define SPI_TXOICR_TXOICR					((uint32_t)0x0001)
/*****************  Bit definition for SPI_RXOICR register  *******************/
#define SPI_RXOICR_RXOICR					((uint32_t)0x0001)
/*****************  Bit definition for SPI_RXUICR register  *******************/
#define SPI_RXUICR_RXUICR					((uint32_t)0x0001)
/*****************  Bit definition for SPI_MSTICR register  *******************/
#define SPI_MSTICR_MSTICR					((uint32_t)0x0001)

/*****************  Bit definition for SPI_DMACR register  ********************/
#define SPI_DMACR_RDMAE						((uint32_t)0x0001)
#define SPI_DMACR_TDMAE						((uint32_t)0x0002)
/*****************  Bit definition for SPI_DMATDLR register  ******************/
#define SPI_DMATDLR_DMATDLR					((uint32_t)0x000F)
/*****************  Bit definition for SPI_DMARDLR register  ******************/
#define SPI_DMATDLR_DMARDLR					((uint32_t)0x000F)
/*****************  Bit definition for SPI_DMARDLR register  ******************/
#define SPI_DMATDLR_DMARDLR					((uint32_t)0x000F)

/*****************  Bit definition for SPI_DR register  ***********************/
#define SPI_DR_DR							((uint32_t)0xFFFF)
/**************  Bit definition for SPI_RX_SAMPLE_DLY register  ***************/
#define SPI_RX_SAMPLE_DLY					((uint32_t)0xFFFF)


/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface                    */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for IC_CON register  *********************/
#define I2C_IC_CON_MASTER_MODE					((uint32_t)0x0001)
#define I2C_IC_CON_SPEED						((uint32_t)0x0006)
#define I2C_IC_CON_SPEED_0						((uint32_t)0x0002)
#define I2C_IC_CON_SPEED_1						((uint32_t)0x0004)
#define I2C_IC_CON_10BITADDR_SLAVE				((uint32_t)0x0008)
#define I2C_IC_CON_10BITADDR_MASTER				((uint32_t)0x0010)
#define I2C_IC_CON_RESTART_EN					((uint32_t)0x0020)
#define I2C_IC_CON_SLAVE_DISABLE				((uint32_t)0x0040)

/*******************  Bit definition for IC_TAR register  *********************/
#define I2C_IC_TAR_TAR							((uint32_t)0x03FF)
#define I2C_IC_TAR_GC_OR_START					((uint32_t)0x0400)
#define I2C_IC_TAR_SPECIAL						((uint32_t)0x0800)
#define I2C_IC_TAR_10BITADDR_MASTER				((uint32_t)0x1000)

/*******************  Bit definition for IC_SAR register  *********************/
#define I2C_IC_SAR_SAR							((uint32_t)0x03FF)

/*******************  Bit definition for IC_HS_MADDR register  ****************/
#define I2C_IC_HS_MADDR_MAR						((uint32_t)0x0007)
#define I2C_IC_HS_MADDR_MAR_0					((uint32_t)0x0001)
#define I2C_IC_HS_MADDR_MAR_1					((uint32_t)0x0002)
#define I2C_IC_HS_MADDR_MAR_2					((uint32_t)0x0004)

/*******************  Bit definition for IC_DATA_CMD register  ****************/
#define I2C_IC_DATA_CMD_DAT						((uint32_t)0x00FF)
#define I2C_IC_DATA_CMD_CMD						((uint32_t)0x0100)
#define I2C_IC_DATA_CMD_STOP					((uint32_t)0x0200)
#define I2C_IC_DATA_CMD_RESTART					((uint32_t)0x0400)

/*******************  Bit definition for IC_SS_SCL_HCNT register  *************/
#define I2C_IC_SS_SCL_HCNT_HCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_SS_SCL_LCNT register  *************/
#define I2C_IC_SS_SCL_LCNT_LCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_FS_SCL_HCNT register  *************/
#define I2C_IC_FS_SCL_HCNT_HCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_FS_SCL_LCNT register  *************/
#define I2C_IC_FS_SCL_LCNT_LCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_HS_SCL_HCNT register  *************/
#define I2C_IC_HS_SCL_HCNT_HCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_HS_SCL_LCNT register  *************/
#define I2C_IC_HS_SCL_LCNT_LCNT						((uint32_t)0xFFFF)

/*******************  Bit definition for IC_INTR_STAT register  ***************/
#define I2C_IC_INTR_STAT_R_RX_UNDER					((uint32_t)0x0001)
#define I2C_IC_INTR_STAT_R_RX_OVER					((uint32_t)0x0002)
#define I2C_IC_INTR_STAT_R_RX_FULL					((uint32_t)0x0004)
#define I2C_IC_INTR_STAT_R_TX_OVER					((uint32_t)0x0008)
#define I2C_IC_INTR_STAT_R_TX_EMPTY					((uint32_t)0x0010)
#define I2C_IC_INTR_STAT_R_RD_REQ					((uint32_t)0x0020)
#define I2C_IC_INTR_STAT_R_TX_ABRT					((uint32_t)0x0040)
#define I2C_IC_INTR_STAT_R_RX_DONE					((uint32_t)0x0080)
#define I2C_IC_INTR_STAT_R_ACTIVITY					((uint32_t)0x0100)
#define I2C_IC_INTR_STAT_R_STOP_DET					((uint32_t)0x0200)
#define I2C_IC_INTR_STAT_R_START_DET				((uint32_t)0x0400)
#define I2C_IC_INTR_STAT_R_GEN_CALL					((uint32_t)0x0800)

/*******************  Bit definition for IC_INTR_MASK register  ***************/
#define I2C_IC_INTR_MASK_M_RX_UNDER					((uint32_t)0x0001)
#define I2C_IC_INTR_MASK_M_RX_OVER					((uint32_t)0x0002)
#define I2C_IC_INTR_MASK_M_RX_FULL					((uint32_t)0x0004)
#define I2C_IC_INTR_MASK_M_TX_OVER					((uint32_t)0x0008)
#define I2C_IC_INTR_MASK_M_TX_EMPTY					((uint32_t)0x0010)
#define I2C_IC_INTR_MASK_M_RD_REQ					((uint32_t)0x0020)
#define I2C_IC_INTR_MASK_M_TX_ABRT					((uint32_t)0x0040)
#define I2C_IC_INTR_MASK_M_RX_DONE					((uint32_t)0x0080)
#define I2C_IC_INTR_MASK_M_ACTIVITY					((uint32_t)0x0100)
#define I2C_IC_INTR_MASK_M_STOP_DET					((uint32_t)0x0200)
#define I2C_IC_INTR_MASK_M_START_DET				((uint32_t)0x0400)
#define I2C_IC_INTR_MASK_M_GEN_CALL					((uint32_t)0x0800)

/*******************  Bit definition for IC_RAW_INTR_STAT register  ***********/
#define I2C_IC_RAW_INTR_STAT_RX_UNDER				((uint32_t)0x0001)
#define I2C_IC_RAW_INTR_STAT_RX_OVER				((uint32_t)0x0002)
#define I2C_IC_RAW_INTR_STAT_RX_FULL				((uint32_t)0x0004)
#define I2C_IC_RAW_INTR_STAT_TX_OVER				((uint32_t)0x0008)
#define I2C_IC_RAW_INTR_STAT_TX_EMPTY				((uint32_t)0x0010)
#define I2C_IC_RAW_INTR_STAT_RD_REQ					((uint32_t)0x0020)
#define I2C_IC_RAW_INTR_STAT_TX_ABRT				((uint32_t)0x0040)
#define I2C_IC_RAW_INTR_STAT_RX_DONE				((uint32_t)0x0080)
#define I2C_IC_RAW_INTR_STAT_ACTIVITY				((uint32_t)0x0100)
#define I2C_IC_RAW_INTR_STAT_STOP_DET				((uint32_t)0x0200)
#define I2C_IC_RAW_INTR_STAT_START_DET				((uint32_t)0x0400)
#define I2C_IC_RAW_INTR_STAT_GEN_CALL				((uint32_t)0x0800)

/*******************  Bit definition for IC_RX_TL register  *******************/
#define I2C_IC_RX_TL_TL								((uint32_t)0x00FF)

/*******************  Bit definition for IC_TX_TL register  *******************/
#define I2C_IC_TX_TL_TL								((uint32_t)0x00FF)

/*******************  Bit definition for IC_CLR_INTR register  ****************/
#define I2C_IC_CLR_INTR								((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_RX_UNDER register  ************/
#define I2C_IC_CLR_RX_UNDER							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_RX_OVER register  *************/
#define I2C_IC_CLR_RX_OVER							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_TX_OVER register  *************/
#define I2C_IC_CLR_TX_OVER							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_RD_REQ register  **************/
#define I2C_IC_CLR_RD_REQ							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_TX_ABRT register  *************/
#define I2C_IC_CLR_TX_ABRT							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_RX_DONE register  *************/
#define I2C_IC_CLR_RX_DONE							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_ACTIVITY register  ************/
#define I2C_IC_CLR_ACTIVITY							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_STOP_DET register  ************/
#define I2C_IC_CLR_STOP_DET							((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_START_DET register  ***********/
#define I2C_IC_CLR_START_DET						((uint32_t)0x0001)

/*******************  Bit definition for IC_CLR_GEN_CALL register  ************/
#define I2C_IC_CLR_GEN_CALL							((uint32_t)0x0001)

/*******************  Bit definition for IC_ENABLE  register  *****************/
#define I2C_IC_ENABLE_ENABLE 						((uint32_t)0x0001)
#define I2C_IC_ENABLE_ABORT 						((uint32_t)0x0002)

/*******************  Bit definition for IC_STATUS  register  *****************/
#define I2C_IC_STATUS_ACTIVITY						((uint32_t)0x0001)
#define I2C_IC_STATUS_TFNF							((uint32_t)0x0002)
#define I2C_IC_STATUS_TFE							((uint32_t)0x0004)
#define I2C_IC_STATUS_RFNE							((uint32_t)0x0008)
#define I2C_IC_STATUS_RFF							((uint32_t)0x0010)
#define I2C_IC_STATUS_MST_ACTIVITY					((uint32_t)0x0020)
#define I2C_IC_STATUS_SLV_ACTIVITY					((uint32_t)0x0040)
/*******************  Bit definition for IC_TXFLR  register  ******************/
#define I2C_IC_TXFLR_TXFLR							(8)

/*******************  Bit definition for IC_RXFLR  register  ******************/
#define I2C_IC_RXFLR_RXFLR							(8)

/*******************  Bit definition for IC_SDA_HOLD  register  ***************/
#define I2C_IC_SDA_HOLD								((uint32_t)0xFFFF)

/*******************  Bit definition for IC_TX_ABRT_SOURCE  register  *********/
#define I2C_IC_TX_ABRT_SOURCE_7B_ADDR_NOACK			((uint32_t)0x00000001)
#define I2C_IC_TX_ABRT_SOURCE_10ADDR1_NOACK			((uint32_t)0x00000002)
#define I2C_IC_TX_ABRT_SOURCE_10ADDR2_NOACK			((uint32_t)0x00000004)
#define I2C_IC_TX_ABRT_SOURCE_TXDATA_NOACK			((uint32_t)0x00000008)
#define I2C_IC_TX_ABRT_SOURCE_GCALL_NOACK			((uint32_t)0x00000010)
#define I2C_IC_TX_ABRT_SOURCE_GCALL_READ			((uint32_t)0x00000020)
#define I2C_IC_TX_ABRT_SOURCE_HS_ACKDET				((uint32_t)0x00000040)
#define I2C_IC_TX_ABRT_SOURCE_SBYTE_ACKDET			((uint32_t)0x00000080)
#define I2C_IC_TX_ABRT_SOURCE_HS_NORSTRT			((uint32_t)0x00000100)
#define I2C_IC_TX_ABRT_SOURCE_SBYTE_NORSTRT			((uint32_t)0x00000200)
#define I2C_IC_TX_ABRT_SOURCE_10B_RD_NORSTRT		((uint32_t)0x00000400)
#define I2C_IC_TX_ABRT_SOURCE_MASTER_DIS			((uint32_t)0x00000800)
#define I2C_IC_TX_ABRT_SOURCE_LOST					((uint32_t)0x00001000)
#define I2C_IC_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO		((uint32_t)0x00002000)
#define I2C_IC_TX_ABRT_SOURCE_SLV_ARBLOST			((uint32_t)0x00004000)
#define I2C_IC_TX_ABRT_SOURCE_SLVRD_INTX			((uint32_t)0x00008000)
#define I2C_IC_TX_ABRT_SOURCE_USER_ABRT				((uint32_t)0x00010000)
#define I2C_IC_TX_ABRT_SOURCE_TX_FLUSH_CNT			((uint32_t)0xFF000000)

/*******************  Bit definition for IC_SLV_DATA_NACK_ONLY  register  *****/
#define I2C_IC_SLV_DATA_NACK_ONLY					((uint32_t)0x0001)

/*******************  Bit definition for IC_DMA_TDLR  register  ***************/
#define I2C_IC_DMA_TDLR_TDLR						((uint32_t)0x000F)

/*******************  Bit definition for IC_DMA_RDLR  register  ***************/
#define I2C_IC_DMA_TDLR_TDLR						((uint32_t)0x000F)

/*******************  Bit definition for IC_SDA_SETUP  register  **************/
#define I2C_IC_SDA_SETUP							((uint32_t)0x00FF)

/*******************  Bit definition for IC_ACK_GENERAL_CALL  register  *******/
#define I2C_IC_ACK_GENERAL_CALL						((uint32_t)0x0001)

/*******************  Bit definition for IC_ENABLE_STATUS  register  **********/
#define I2C_IC_ENABLE_STATUS_IC_EN					((uint32_t)0x0001)
#define I2C_IC_ENABLE_STATUS_SLV_RX_ABORTED			((uint32_t)0x0001)
#define I2C_IC_ENABLE_STATUS_SLV_FIFO_FILLED_AND_FLUSHED		((uint32_t)0x0001)

/*******************  Bit definition for IC_FS_SPKLEN  register  **************/
#define I2C_IC_FS_SPKLEN_SPKLEN						((uint32_t)0x00FF)

/*******************  Bit definition for IC_HS_SPKLEN  register  **************/
#define I2C_IC_HS_SPKLEN_SPKLEN						((uint32_t)0x00FF)

/*******************  Bit definition for IC_COMP_PARAM_1  register  ***********/
#define I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH			((uint32_t)0x0003)
#define I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_0		((uint32_t)0x0001)
#define I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_1		((uint32_t)0x0002)
#define I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE			((uint32_t)0x000C)
#define I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_0		((uint32_t)0x0004)
#define I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_1		((uint32_t)0x0008)
#define I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES			((uint32_t)0x0010)
#define I2C_IC_COMP_PARAM_1_INTR_IO					((uint32_t)0x0020)
#define I2C_IC_COMP_PARAM_1_HAS_DMA					((uint32_t)0x0040)
#define I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS		((uint32_t)0x0080)
#define I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH			((uint32_t)0x0000FF00)
#define I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH			((uint32_t)0x00FF0000)

/*******************  Bit definition for IC_COMP_VERSION  register  ***********/
#define I2C_IC_COMP_VERSION							((uint32_t)0xFFFFFFFF)

/*******************  Bit definition for IC_COMP_TYPE  register  **************/
#define I2C_IC_COMP_TYPE							((uint32_t)0xFFFFFFFF)


/******************************************************************************/
/*                                                                            */
/*						Keyboard Control Unit Block			                  */
/*                                                                            */
/******************************************************************************/
/*****************  Bit definition for KCU_CTRL0 register  ********************/
#define KCU_CTRL0_OUT_EN							((uint32_t)0x01FF)
#define KCU_CTRL0_OUT_EN_0							((uint32_t)0x0001)
#define KCU_CTRL0_OUT_EN_1							((uint32_t)0x0002)
#define KCU_CTRL0_OUT_EN_2							((uint32_t)0x0004)
#define KCU_CTRL0_OUT_EN_3							((uint32_t)0x0008)
#define KCU_CTRL0_OUT_EN_4							((uint32_t)0x0010)
#define KCU_CTRL0_OUT_EN_5							((uint32_t)0x0020)
#define KCU_CTRL0_OUT_EN_6							((uint32_t)0x0040)
#define KCU_CTRL0_OUT_EN_7							((uint32_t)0x0080)
#define KCU_CTRL0_OUT_EN_8							((uint32_t)0x0100)
#define KCU_CTRL0_DEBOUNCE_RANGE					((uint32_t)0x0E00)
#define KCU_CTRL0_DEBOUNCE_RANGE_0					((uint32_t)0x0200)
#define KCU_CTRL0_DEBOUNCE_RANGE_1					((uint32_t)0x0400)
#define KCU_CTRL0_DEBOUNCE_RANGE_2					((uint32_t)0x0800)
#define KCU_CTRL1_KBD_EN							((uint32_t)0x0001)
#define KCU_CTRL1_PUSH_INTERRUPT					((uint32_t)0x0002)
#define KCU_CTRL1_RELEASE_INTERRUPT					((uint32_t)0x0004)
#define KCU_CTRL1_OVERRUN_INTERRUPT					((uint32_t)0x0008)
#define KCU_CTRL1_KCU_RUNING						((uint32_t)0x8000)
/*****************  Bit definition for KCU_CTRL1 register  ********************/
#define KCU_STATUS_INTERRUPT						((uint32_t)0x0001)
#define KCU_STATUS_OVERRUN_INTERRUPT				((uint32_t)0x0002)
#define KCU_STATUS_PUSH_INTERRUPT					((uint32_t)0x0004)
#define KCU_STATUS_RELEASE_INTERRUPT				((uint32_t)0x0008)
/*****************  Bit definition for KCU_STATUS register  *******************/
#define KCU_STATUS_INTERRUPT						((uint32_t)0x0001)
#define KCU_STATUS_OVERRUN_INTERRUPT				((uint32_t)0x0002)
#define KCU_STATUS_PUSH_INTERRUPT					((uint32_t)0x0004)
#define KCU_STATUS_RELEASE_INTERRUPT				((uint32_t)0x0008)
#define KCU_STATUS_EVENT_0_NEW						((uint32_t)0x0010)
#define KCU_STATUS_EVENT_0_PUSH						((uint32_t)0x0020)
#define KCU_STATUS_EVENT_1_NEW						((uint32_t)0x0040)
#define KCU_STATUS_EVENT_1_PUSH						((uint32_t)0x0080)
#define KCU_STATUS_EVENT_2_NEW						((uint32_t)0x0100)
#define KCU_STATUS_EVENT_2_PUSH						((uint32_t)0x0200)
#define KCU_STATUS_EVENT_3_NEW						((uint32_t)0x0400)
#define KCU_STATUS_EVENT_3_PUSH						((uint32_t)0x0800)
/*****************  Bit definition for KCU_EVENT register  ********************/
#define KCU_EVENT_EVENT_0_INPUT_NUM					((uint32_t)0x0000000F)
#define KCU_EVENT_EVENT_0_OUTPUT_NUM				((uint32_t)0x000000F0)
#define KCU_EVENT_EVENT_1_INPUT_NUM					((uint32_t)0x00000F00)
#define KCU_EVENT_EVENT_1_OUTPUT_NUM				((uint32_t)0x0000F000)
#define KCU_EVENT_EVENT_2_INPUT_NUM					((uint32_t)0x000F0000)
#define KCU_EVENT_EVENT_2_OUTPUT_NUM				((uint32_t)0x00F00000)
#define KCU_EVENT_EVENT_3_INPUT_NUM					((uint32_t)0x0F000000)
#define KCU_EVENT_EVENT_3_OUTPUT_NUM				((uint32_t)0xF0000000)


/******************************************************************************/
/*                                                                            */
/*						Timer Control Unit Block			                  */
/*                                                                            */
/******************************************************************************/
/**********  Bit definition for TIMER_CONTROL_REG register  *******************/
#define TIMER_CONTROL_REG_TIMER_ENABLE				(0x0001U)
#define TIMER_CONTROL_REG_TIMER_MODE				(0x0002U) 
#define TIMER_CONTROL_REG_TIMER_INTERRUPT			(0x0004U)
#define TIMER_CONTROL_REG_TIMER_PWM					(0x0008U)
/*****************  Bit definition for IntStatus register  ********************/
#define TIMER_INT_STATUS_INTERRUPT					(0x0001U)


/******************************************************************************/
/*                                                                            */
/*							WDT Control Unit Block			                  */
/*                                                                            */
/******************************************************************************/
/*****************  Bit definition for WDT_CR register  ***********************/
#define WDT_CR_WDT_EN								((uint32_t)0x0001)
#define WDT_CR_RMOD									((uint32_t)0x0002)
/*****************  Bit definition for WDT_CCVR register  *********************/
#define WDT_CCVR_CCVR								((uint32_t)0xFFFFFFFF)
/*****************  Bit definition for WDT_CRR register  **********************/
#define WDT_CRR_CRR									((uint32_t)0x00FF)
/*****************  Bit definition for WDT_STAT register  *********************/
#define WDT_STAT_INT								((uint32_t)0x0001)
/*****************  Bit definition for WDT_EOI register  **********************/
#define WDT_EOI_EOI									((uint32_t)0x0001)
/*****************  Bit definition for WDT_RLD register  **********************/
#define WDT_RLD_RLD									((uint32_t)0xFFFFFFFF)


/******************************************************************************/
/*                                                                            */
/*							RNG Control Unit Block			                  */
/*                                                                            */
/******************************************************************************/

/************ bit definition for TRNG RNG_CSR REGISTER ************/
#define TRNG_RNG_CSR_INTP_EN_Mask					BIT(4)
#define TRNG_RNG_CSR_ATTACK_TRNG1_Mask				BIT(3)
#define TRNG_RNG_CSR_ATTACK_TRNG0_Mask				BIT(2)
#define TRNG_RNG_CSR_S128_TRNG1_Mask				BIT(1)
#define TRNG_RNG_CSR_S128_TRNG0_Mask				BIT(0)

/************ bit definition for TRNG RNG_AMA REGISTER ************/
#define TRNG_RNG_AMA_PD_TRNG1_Mask					BIT(31)
#define TRNG_RNG_AMA_PD_TRNG0_Mask					BIT(30)
#define TRNG_RNG_AMA_ANA_OUT_TRNG1_Mask				BIT(29)
#define TRNG_RNG_AMA_ANA_OUT_TRNG0_Mask				BIT(28)


#define MHSCPU_READ_REG8(reg)                       (*(__IO uint8_t *) reg)
#define MHSCPU_READ_REG16(reg)                      (*(__IO uint16_t *) reg)
#define MHSCPU_READ_REG32(reg)                      (*(__IO uint32_t *) reg)
#define MHSCPU_WRITE_REG8(reg, value)               (*(__IO uint8_t *) reg = value)
#define MHSCPU_WRITE_REG16(reg, value)              (*(__IO uint16_t *) reg = value)
#define MHSCPU_WRITE_REG32(reg, value)              (*(__IO uint32_t *) reg = value)
#define MHSCPU_MODIFY_REG8(reg, clear_mask, set_mask)   \
        MHSCPU_WRITE_REG8(reg, (((MHSCPU_READ_REG8(reg)) & ~clear_mask) | set_mask))
#define MHSCPU_MODIFY_REG16(reg, clear_mask, set_mask)   \
        MHSCPU_WRITE_REG16(reg, (((MHSCPU_READ_REG16(reg)) & ~clear_mask) | set_mask))
#define MHSCPU_MODIFY_REG32(reg, clear_mask, set_mask)   \
        MHSCPU_WRITE_REG32(reg, (((MHSCPU_READ_REG32(reg)) & ~clear_mask) | set_mask))
/*@}*/ /* end of group <Device>_PeripheralDecl */

/*@}*/ /* end of group <Device>_Definitions */

#ifdef USE_STDPERIPH_DRIVER
  #include "mhscpu_conf.h"
#endif

#ifdef __cplusplus
}
#endif

#endif  /* <Device>_H */
