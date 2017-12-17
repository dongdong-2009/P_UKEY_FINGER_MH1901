


#include "mhscpu.h"
#include "mhscpu_it.h"
#include <string.h>
#include <stdio.h>
#include "usb_dcd_int.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern __ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	printf("SysTick_Handler\r\n");
}

void USB_IRQHandler(void)
{
    USBD_OTG_ISR_Handler (&USB_OTG_dev);
    NVIC_ClearPendingIRQ(USB_IRQn);
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */




/*



void RTC_IRQHandler(void)
{
	printf("RTC Alarm\r\n");
	//RTC_SetAlarm(10000);
	RTC_ClearITPendingBit();
	NVIC_ClearPendingIRQ(RTC_IRQn);
}

void SENSOR_IRQHandler(void)
{
		uint32_t index = 0;
	uint32_t Key[16];
	uint32_t Buf[16];
	
	uint32_t state = SENSOR_GetITStatus();
	
	printf("\r\nreadkey:");
	BPK_ReadKey(Buf,16,0);
	
	for(index = 0;index < sizeof(Key)/sizeof(Key[0]);index++)
			printf("%08X ", Buf[index]);
	
	for(index = 0;index < sizeof(Key)/sizeof(Key[0]);index++)
			Key[index] = index;	
	printf("\r\nwrite key:");
	BPK_WriteKey(Key,16,0);
	
	printf("\r\nreadkey:");
	BPK_ReadKey(Buf,16,0);
	
	for(index = 0;index < sizeof(Key)/sizeof(Key[0]);index++)
			printf("%08X ", Buf[index]);
	
	SENSOR_ClearIT();
	NVIC_ClearPendingIRQ(SENSOR_IRQn);
}

*/




/**/

