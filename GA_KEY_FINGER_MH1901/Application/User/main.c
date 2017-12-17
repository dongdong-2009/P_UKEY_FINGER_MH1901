/*
 * User Application Entrance
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    08/31/17     Create this file for user application entrance
 */

#include <string.h>
#include <stdio.h>
#include <mhscpu.h>
#include <base_defines.h>
#include <board.h>
#include <mhscpu_flash.h>
#include <libapi.h>
#include "usbd_desc.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "hal_flash_interface.h"
#include "hal_flash_data.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;



void TIM0_0_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIMM0, TIM_0);
    timer0_isr();
}




/*
 * Read template totals for test
 */
void test_read_template_totals(void)
{
        U8 res = 0;
        
        res = lib_api((LIB_READ_ID_LIST<<4));
        debugPrint("condlist1 = %02x \r\n",res&0x0f);
    
        res = lib_api((LIB_READ_ID_LIST<<4)|0x01);
        debugPrint("condlist2 = %02x \r\n",res&0x0f);
}

/*
 * Delete fingerprint ID
 *
 */
 void delete_fingerprint_id(const U8 fp_id)
 {
	U8 res;
    U8 id = fp_id;
	
	

	res=lib_api((LIB_DELETE_ID<<4)|(id<<1)|0x01);
	if( (res&0x01) == 0)
	{
		debugPrint("delete is ok  id =%d\r\n",(res&0x0f)>>1);
	}
	else
	{
		debugPrint("delete is failed  id =%d\r\n",(res&0x0f)>>1);
	}
}

/*
 * Clean all fingerprint
 *
 */
 void clean_all_fingerprint(void)
 {

     U8 res = 0;
     
 
     res = lib_api((LIB_DELETE_ID<<4));
     if( (res&0x01) == 0)
     {
         debugPrint("empty is ok  id =%d\r\n",(res&0x0f)>>1);
     }
     else
     {
         debugPrint("empty is failed  id =%d\r\n",(res&0x0f)>>1);
     }
 }

 /*
  * Test Enroll
  *
  *
  */
 void test_enroll(U8 fp_id)
 {
	U8 res = 0;
    U8 enroll_id = fp_id;
	

		while(1)
		{
			res = lib_api(LIB_PRE_ENEOLL<<4);
			if( (res&0x01) == 0)
				break;
		}




		while(1)
		{
			res=lib_api((LIB_ENROLL<<4)|(enroll_id<<1));
			if( (res&0x01) == 0)
			{
				debugPrint("single enroll is ok \r\n");
			}
			else
			{
				debugPrint("single enroll is failed\r\n");
			}
			break;
		}
		
		while(1)
		{
			res=lib_api(LIB_IS_FINGERLEAVE<<4);
			if( (res&0x01) == 0)
				break;
		}

}


/*
 * Match Fingerprint
 */
U8 match_fingerprint(void)
{
	U8 res = 0;


	while(1)
	{
		res = lib_api(LIB_PRE_MATCH<<4);
		if( (res&0x01) == 0)
			break;
	}

	while(1)
	{
		res = lib_api((LIB_MATCH<<4));
		if( (res&0x01) == 0)
		{
			debugPrint("match is ok id=%d \r\n",(res&0x0f)>>1);
		}
		else
		{
			debugPrint("match is failed id=%d \r\n",(res&0x0f)>>1);
		}
		break;
	}

    return res;
}





void save_enroll(const U8 fp_id)
{
	U8 res = 0;


	res=lib_api((LIB_ENROLL<<4)|(fp_id<<1)|0x01);
	if( (res&0x01) == 0)
	{
		debugPrint("post_enroll is ok \r\n");
	}
	else
	{
		debugPrint("post_enroll is failed\r\n");
	}
}









int main(void)
{
	int i;
    int okay = 0;
    int error = 0;
 	USBPHY_CR1_TypeDef usbphy_cr1;

	unsigned char buffer[16]={0x55};
	//init clock
    init_clock();

	//init timer	
    init_timer();

	//init nvic
	NVIC_Configuration();

	//init gpio
	GPIO_Configuration();

	//init uart	
	uartInit(UART0_BAUDRATE);

	//init lib
    lib_init();
    lib_debug_print_en(DISABLE);

	// init usb  
	usbphy_cr1.d32 = MHSCPU_READ_REG32(&SYSCTRL->USBPHY_CR1);
	usbphy_cr1.b.commononn = 0;
	usbphy_cr1.b.stop_ck_for_suspend = 0;
	MHSCPU_WRITE_REG32(&SYSCTRL->USBPHY_CR1,usbphy_cr1.d32);

	memset(&USB_OTG_dev, 0x00, sizeof(USB_OTG_dev));
	USBD_Init(&USB_OTG_dev,
		#ifdef USE_USB_OTG_HS
			  USB_OTG_HS_CORE_ID,
		#else
			  USB_OTG_FS_CORE_ID, 
		#endif
			  &USR_desc,
			  &USBD_MSC_cb, 
			  &USRD_cb);

	//init flash	
	HAL_Flash_Interface_Init();

	memset(buffer,0x55,16);
	WriteFlash(buffer,DEV_AUTH_KEY+1,1);

	writeonebyte(DEV_AUTH_KEY,buffer[0]);

	memset(buffer,0xAA,16);
	writeonebyte(DEV_AUTH_KEY,buffer[0]);	
	FLASH_WriteBlock(DEV_AUTH_KEY,buffer,3);		



	

	//init rand
	mh_rand_init();

	//init cos
	cos_init(1);		

	debugPrint("Freqsel:%x\r\n",*(volatile unsigned int *)0x4001F000);	
	clean_all_fingerprint();



	
while (1)
{
	okay = error = 0;
	
    test_read_template_totals();
   // delete_fingerprint_id(0);

	  
    test_enroll(0);
    test_enroll(0);
    test_enroll(0);
    save_enroll(0);

    test_enroll(1);
    test_enroll(1);
    test_enroll(1);
    save_enroll(1);

	#if 1
    test_enroll(2);
    test_enroll(2);
    test_enroll(2);
    save_enroll(2);

    test_enroll(3);
    test_enroll(3);
    test_enroll(3);
    save_enroll(3);

    test_enroll(4);
    test_enroll(4);
    test_enroll(4);
    save_enroll(4);
  #endif

    for ( i=0; i<200; i++ )
    {
            if (!(match_fingerprint()&0x01))
            {
                okay++;
            }else{
                error++;
            }
    }

    debugPrint("okay:%d,error:%d\r\n",okay,error);

    

    test_read_template_totals();
    
    delete_fingerprint_id(1);
    
    test_read_template_totals();
    //clean_all_fingerprint();
    //test_read_template_totals();

    debugPrint("*###When there is nothing,you can do anything,Perez,Irisil!###*\r\n");
}
}

