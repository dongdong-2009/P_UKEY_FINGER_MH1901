/*
 * Board initialization
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/12/17     Create this file for entire platform Operation
 */

#include <board.h>

typedef void (*init_fnc_t)(void);
void initcall_run_list(init_fnc_t init_sequence[]);


/* Define a function array for init the peripheral device */
init_fnc_t init_sequence[] = {

	init_clock,
	//init_spi0_controller,
	//init_timer,
	NVIC_Configuration,
    //init_afs121n,
	GPIO_Configuration,
    NULL
};


/*
 * IRQ Configuration
 * @None parameter and return value
 *
 *
 */

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);

	/*NVIC_InitStructure.NVIC_IRQChannel = UART0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	*/

	NVIC_InitStructure.NVIC_IRQChannel = TIM0_0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	/*
	NVIC_InitStructure.NVIC_IRQChannel = TIM0_1_IRQn;
 	NVIC_Init(&NVIC_InitStructure);

    
	NVIC_InitStructure.NVIC_IRQChannel = TIM0_2_IRQn;
 	NVIC_Init(&NVIC_InitStructure);*/
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
	
	/* Enable USB Interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_ClearPendingIRQ(TRNG_IRQn);
	NVIC_InitStructure.NVIC_IRQChannel = TRNG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);

#if CRYPT_INT
	NVIC_ClearPendingIRQ(CRYPT0_IRQn);
	NVIC_InitStructure.NVIC_IRQChannel = CRYPT0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);
#endif



    
    
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif



/*
 * GPIO Configuration
 * @None parameter and return value
 *
 */

void GPIO_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
	GPIO_InitTypeDef GPIO_InitStruct;

    EXTI_DeInit();

	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	EXTI_ClearITPendingBit(EXTI_Line2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
    
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Remap = GPIO_Remap_1;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

    EXTI_ClearITPendingBit(EXTI_Line2);
    NVIC_ClearPendingIRQ(EXTI2_IRQn);
 
    //EXTI_LineConfig(EXTI_Line2, DEF_BIT_11, EXTI_Trigger_Falling);

	EXTI_ClearITPendingBit(EXTI_Line2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
 
	/*NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);*/
  (void)NVIC_InitStructure;


  
    /* Config the CS0 for SPI0 
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Remap = GPIO_Remap_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);*/
}




/*
 * Init the clock of all peripheral devices
 * @ No Parameters
 * @ No Return Values
 *
 */
void init_clock(void)
{
    UINT32 i = 0;


	SYSCTRL_SYSCLKSourceSelect(SELECT_EXT12M);
	SYSCTRL_PLLConfig(SYSCTRL_PLL_120MHz);	
    //SYSCTRL_PLLConfig(SYSCTRL_PLL_108MHz);
    
    SYSCTRL_HCLKConfig(SYSCTRL_HCLK_Div_None);
    SYSCTRL_PCLKConfig(SYSCTRL_PCLK_Div4);


	SYSCTRL_APBPeriphClockCmd(SYSCTRL_APBPeriph_UART0 | SYSCTRL_APBPeriph_SPI0 | \
                               SYSCTRL_APBPeriph_GPIO|SYSCTRL_APBPeriph_TIMM0,ENABLE);
	/* Please don't reset the GPIO, otherwise the JLINK cann't use */
	SYSCTRL_APBPeriphResetCmd(SYSCTRL_APBPeriph_UART0 | SYSCTRL_APBPeriph_SPI0\
							   |SYSCTRL_APBPeriph_TIMM0 ,ENABLE);


    SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_USB,ENABLE);
    SYSCTRL_AHBPeriphResetCmd(SYSCTRL_AHBPeriph_USB,ENABLE);


	SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_CRYPT, ENABLE);
	SYSCTRL_AHBPeriphResetCmd(SYSCTRL_AHBPeriph_CRYPT, ENABLE);

	SYSCTRL_APBPeriphClockCmd(SYSCTRL_APBPeriph_TRNG, ENABLE);




  
    for (i = 0; i < 0xA000; i++)
    {
        __asm("nop");
    }
}







/*
 * Init all peripheral devices
 * @ No Parameters
 * @ No Return Values
 *
 */
void init_all_peripheral_devices(void)
{
    init_fnc_t *init_fnc_ptr         = (void *)0;
    
    /* Initialization function set */
    for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr)
    {
        (*init_fnc_ptr)();
    }
}



/*
 * For external call
 */
void hardwareInit(void)
{
    init_all_peripheral_devices();
}


void EXTI2_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
}

