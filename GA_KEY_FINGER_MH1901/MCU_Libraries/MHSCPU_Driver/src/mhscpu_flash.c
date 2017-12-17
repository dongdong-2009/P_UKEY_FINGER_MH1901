/**
  ******************************************************************************
  * @file    stm32f10x_flash.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the FLASH firmware functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mhscpu_conf.h"
#include "mhscpu_flash.h"
#include <core_cm3.h>
#include <core_cmFunc.h>
#include "base_defines.h"


/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/** @defgroup FLASH_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Defines
  * @{
  */ 

/*Flash operate*/
#define OP_SEL_PROGRAM          (0)
#define OP_SEL_ERASE_ALL        (1)
#define OP_SEL_PAGE_ERASE       (2)


/*Flash unlock key*/
#define FLASH_KEY1              (0xABCD00A5)
#define FLASH_KEY2              (0x1234005A)


static uint32_t gu32FLASH_Key1 = 0;
static uint32_t gu32FLASH_Key2 = 0;

static uint32_t (* pfFlash_Done)(void) = (uint32_t (*)(void))0x1A1;

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_FunctionPrototypes
  * @{
  */
  
/**
  * @}
  */

/** @defgroup FLASH_Private_Functions
  * @{
  */

#ifdef NOT_USE_FLASH_DURING_OPERATING
/**
 * If you are sure your software will not access flash during 
 * flash operating, you can use this simple fucntion. During 
 * flash operating any access to flash must be 
 * forbiddden(Inluding MCU fetch instruction and data). 
 * 
 * @author SD_jiaoyinghua (2016-8-11)
 */
void FLASH_Operate(void)
{
    FCU->CS = 1;
}
#else
void FLASH_Operate(void)
{
    __disable_irq();
    __disable_fault_irq();
    pfFlash_Done();
    __enable_fault_irq();
    __enable_irq();

}
#endif


/**
  * @brief  Sets the code latency value.
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  *     @arg FLASH_Latency_2: FLASH Two Latency cycles
  * @retval None
  */
void FLASH_SetLatency(uint32_t u32UsClk)
{
    if (0 == u32UsClk)
    {
        FCU->TIM = (SYSCTRL->HCLK_1MS_VAL + 1000 - 1) / 1000;
    }
    else
    {
        FCU->TIM = u32UsClk;
    }
}


/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1 and Bank2.
  *         - For all other devices it unlocks Bank1 and it is equivalent 
  *           to FLASH_UnlockBank1 function.. 
  * @param  None
  * @retval None
  */
void FLASH_Unlock(void)
{
    gu32FLASH_Key1 = FLASH_KEY1;
    gu32FLASH_Key2 = FLASH_KEY2;
}

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1 and Bank2.
  *         - For all other devices it Locks Bank1 and it is equivalent 
  *           to FLASH_LockBank1 function.
  * @param  None
  * @retval None
  */
void FLASH_Lock(void)
{
    gu32FLASH_Key1 = ~FLASH_KEY1;
    gu32FLASH_Key2 = ~FLASH_KEY2;
    FCU->PROT = FLASH_KEY2;
}

/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t u32Addr)
{
    uint32_t u32RO;
    FLASH_Status status = FLASH_COMPLETE;
    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(u32Addr));

    u32RO = 1UL << (((u32Addr - FLASH_BASE) >> 12) >> 3);
    if (0 != (FCU->RO & u32RO))
    {
        return FLASH_ERROR_WRP;
    }
    
    FCU->ADDR = (uint32_t *)u32Addr;
    FCU->CFG = OP_SEL_PAGE_ERASE;

    //For test.
    FCU->PROT = gu32FLASH_Key1;
    FCU->PROT = gu32FLASH_Key2;

    //Start operate.
    FLASH_Operate();
    while (FCU->CS & 1);

    if (0 != (FCU->CS & 0x02))
    {
        status = FLASH_ERROR_PG;
    }

    return status;
}


/**
  * @brief  Erases all FLASH pages.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    if (0 != FCU->RO)
    {
        return FLASH_ERROR_WRP;
    }

    FCU->ADDR = (__IO uint32_t *)FLASH_BASE;
    FCU->CFG = OP_SEL_ERASE_ALL;

    //For test.
    FCU->PROT = gu32FLASH_Key1;
    FCU->PROT = gu32FLASH_Key2;

    //Start operate.
    FLASH_Operate();
    while (FCU->CS & 1);

    if (0 != (FCU->CS & 0x02))
    {
        status = FLASH_ERROR_PG;
    }

    return status;
}


/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t u32Addr, uint32_t u32Data)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(u32Addr) || IS_OTP_ADDRESS(u32Addr));
    //Word align
    assert_param(0 == (u32Addr & 0x03));

    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else
    {
        pu32RO = &FCU->RO_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);

    if (0 != (*pu32RO & u32Tmp))
    {
        return FLASH_ERROR_WRP;
    }

    FCU->CFG = OP_SEL_PROGRAM;
    FCU->PDATA = u32Data;
    FCU->ADDR = (__IO uint32_t *)u32Addr;

    FCU->PROT = gu32FLASH_Key1;
    FCU->PROT = gu32FLASH_Key2;

    FLASH_Operate();
    while (FCU->CS & 1);

    if (0 != (FCU->CS & 2) || *(uint32_t *)u32Addr != u32Data)
    {
        status = FLASH_ERROR_PG;
    }

    return status;
}

/**
  * @brief  Write protects the desired pages
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg For @b STM32_Low-density_devices: value between FLASH_WRProt_Pages0to3 and FLASH_WRProt_Pages28to31  
  *     @arg For @b STM32_Medium-density_devices: value between FLASH_WRProt_Pages0to3
  *       and FLASH_WRProt_Pages124to127
  *     @arg For @b STM32_High-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to255
  *     @arg For @b STM32_Connectivity_line_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to127    
  *     @arg For @b STM32_XL-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to511
  *     @arg FLASH_WRProt_AllPages
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
void FLASH_Protect(uint32_t u32Addr)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;

    
    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else if (IS_OTP_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        FCU->RO = ~0UL;
        FCU->RO_INFO = ~0UL;
        return;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);;
    *pu32RO |= u32Tmp;

    return;
}

uint32_t FLASH_IsProtect(uint32_t u32Addr)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;

    
    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else if (IS_OTP_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        return 0;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);

    return (0 != (*pu32RO & u32Tmp));
}

void FLASH_SetProtect(uint8_t u8Type, uint32_t u32RO)
{
    __IO uint32_t *pu32RO;

    
    if (0 == u8Type)
    {
        pu32RO = &FCU->RO;
    }
    else
    {
        pu32RO = &FCU->RO_INFO;
    }

    *pu32RO |= u32RO;

    return;
}

uint32_t FLASH_GetProtect(uint8_t u8Type)
{
    uint32_t u32RO;

    if (0 == u8Type)
    {
        u32RO = FCU->RO;
    }
    else
    {
        u32RO = FCU->RO_INFO;
    }

    return u32RO;
}

void FLASH_SetProtectLock(uint8_t u8Type, uint32_t u32Lock)
{
    __IO uint32_t *pu32RO;
    __IO uint32_t *pu32ROL;
    
    if (0 == u8Type)
    {
        pu32RO = &FCU->RO;
        pu32ROL = &FCU->ROL;
    }
    else
    {
        pu32RO = &FCU->RO_INFO;
        pu32ROL = &FCU->ROL_INFO;
    }

    *pu32RO |= u32Lock;
    *pu32ROL |= u32Lock;

    return;
}


uint32_t FLASH_GetProtectLock(uint8_t u8Type)
{
    uint32_t u32ROL;

    if (0 == u8Type)
    {
        u32ROL = FCU->ROL;
    }
    else
    {
        u32ROL = FCU->ROL_INFO;
    }

    return u32ROL;
}

void FLASH_UnProtect(uint32_t u32Addr)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;

    
    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else if (IS_OTP_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        return;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);;
    *pu32RO &= ~u32Tmp;

    return;
}

void FLASH_SetUnProtect(uint8_t u8Type, uint32_t u32RO)
{
    __IO uint32_t *pu32RO;
    
    if (0 == u8Type)
    {
        pu32RO = &FCU->RO;
    }
    else
    {
        pu32RO = &FCU->RO_INFO;
    }

    *pu32RO &= ~u32RO;

    return;
}



void FLASH_ProtectLock(uint32_t u32Addr)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;
    __IO uint32_t *pu32ROL;
    
    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        pu32ROL = &FCU->ROL;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else if (IS_OTP_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO_INFO;
        pu32ROL = &FCU->ROL_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        FCU->RO = ~0UL;
        FCU->ROL = ~0UL;
        FCU->RO_INFO = ~0UL;
        FCU->ROL_INFO = ~0UL;
        return;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);
    *pu32RO |= u32Tmp;
    *pu32ROL |= u32Tmp;

    return;
}

uint32_t FLASH_IsProtectLock(uint32_t u32Addr)
{
    uint8_t u8Size;
    uint32_t u32Tmp;
    __IO uint32_t *pu32RO;
    __IO uint32_t *pu32ROL;
    
    if (IS_FLASH_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO;
        pu32ROL = &FCU->ROL;
        u32Tmp = FLASH_BASE;
        u8Size = 3;
    }
    else if (IS_OTP_ADDRESS(u32Addr))
    {
        pu32RO = &FCU->RO_INFO;
        pu32ROL = &FCU->ROL_INFO;
        u32Tmp = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        return 0;
    }

    u32Tmp = 1UL << (((u32Addr - u32Tmp) >> 12) >> u8Size);;
    *pu32RO |= u32Tmp;
    *pu32ROL |= u32Tmp;

    return (0 != (*pu32ROL & u32Tmp));
}

FLASH_Status FLASH_WriteBlock(uint32_t u32Addr, void *pvBuf, uint32_t len)
{
    uint8_t j, u8Size;
    uint32_t i, u32Tmp, u32Base;
    uint8_t *pu8Start = pvBuf, *pu8End = (uint8_t *)pvBuf + len;
    uint32_t volatile *pu32Addr = (uint32_t volatile *)u32Addr;
    __IO uint32_t *pRO;

    if (0 == len)
    {
        return FLASH_COMPLETE;
    }
    
    assert_param((IS_FLASH_ADDRESS(u32Addr) && IS_FLASH_ADDRESS(u32Addr + len - 1)) || 
                 (IS_OTP_ADDRESS(u32Addr) && IS_OTP_ADDRESS(u32Addr + len - 1)));
    assert_param(0 == (u32Addr & 0x03));
    
    if (IS_OTP_ADDRESS(u32Addr))
    {
        pRO = &FCU->RO_INFO;
        u32Base = OTP_BASE;
        u8Size = 0;
    }
    else
    {
        pRO = &FCU->RO;
        u32Base = FLASH_BASE;
        u8Size = 3;
    }

    for (i = u32Addr; i < u32Addr + len; i += (0x1000 << u8Size))
    {
        u32Tmp = 1UL << (((i - u32Base) >> 12) >> u8Size);
        if (0 != (*pRO & u32Tmp))
        {
            return FLASH_ERROR_WRP;
        }
    }

    //clear erro flag.
    FCU->CS = 0;
    FCU->ADDR = pu32Addr;
    FCU->PDATA = *pu32Addr;
    
    while (pu8Start < pu8End)
    {
        i = pu8End - pu8Start;
        i = i > 4 ? 4 : i;
        u32Tmp = 0;
        for ( j = 0; j < i; j++)
        {
            u32Tmp |= (pu8Start[j] << (j << 3));
        }
        pu8Start += 4;

        while (FCU->CS & 1);
        if (FCU->PDATA != *FCU->ADDR || 0 != (FCU->CS & 0x02))
        {
            return FLASH_ERROR_PG;
        }
        FCU->CFG = OP_SEL_PROGRAM;
        FCU->PDATA = u32Tmp;
        FCU->ADDR = pu32Addr++;

        //For test.
        FCU->PROT = gu32FLASH_Key1;
        FCU->PROT = gu32FLASH_Key2;
        FLASH_Operate();
    }

    while (FCU->CS & 1);
    if (FCU->PDATA != *FCU->ADDR || 0 != (FCU->CS & 0x02))
    {
        return FLASH_ERROR_PG;
    }

    return FLASH_COMPLETE;
}


void ReadFlash(U8 *buffer, U32 addr, U32 length)
{
	register U32 i;
	
	for(i=0;i<length;i++)
	{
		*(buffer+i)=(*(U8 *)(addr+i));
	}
	
}

// ---------------------------------------------------------------------  
// Func : WriteFlash()
// Description : Write Flash by Byte with Page Erase. Page span is 
// allowed. The most length for one call is 2048 bytes.
// ---------------------------------------------------------------------  
// Input
// buffer : data to be written 
// addr : write start address
// length : write data length
//-----------------------------------------------------------------
// Output
// No
// ---------------------------------------------------------------------  


void WriteFlash(unsigned char *data_buf, unsigned int addr, unsigned int len)
{
	U32 addr_start,addr_end;
	// the following addresses are DWORD aligned
	U32 head_page_addr,tail_page_addr, head_page_offset;
	// page buffer
    U32 mem[512];
    // a pointer
	U8 *memtp;	
	
	register U32 i, j;
	 
	U32 n, flash_addr, data_length, data_offset;
	
	
	data_length = len;
	
	// the two addesses define an area of new DWORD
	// in this area no FLASH data need to copy in
	
	// start address aligned forward
	// |    |    | XX | XX | XX | XX | XX | XX |
	//                 --->
	// |    |    | xx | xx |      XXXXXXXX     | 
	addr_start = (addr + 0x3) & 0x1FFFFFC;
	
	// end address aligned backward
	// | XX | XX | XX | XX | XX | XX |    |    | 
	//                      <---
	// |      XXXXXXXX     | xx | xx |    |    | 
	addr_end = (addr + len - 1) & 0x1FFFFFC;
	
	// page address to determine spaces to be erased
	head_page_addr = addr & 0x1FFF000;
	tail_page_addr = addr_end & 0x1FFF000;
	
	// relative address to the head page of the new data start point
	head_page_offset = addr & 0x0FFF;
	
	if(head_page_addr == tail_page_addr)     // within 1 page
	{
		// copy FLASH data in the page before the new DWORD area
		flash_addr = head_page_addr;
		n = (addr_start - flash_addr) >> 2;
		
		for(i = 0; i < n; i ++,	flash_addr +=4)
		{
			mem[i] = *(unsigned int *)flash_addr;
		}
		
		// copy FLASH data int the page after new DWORD area
		flash_addr = addr_end;
		n = ((addr_end & 0xFFC)>>2);
		for(i = n; i < 512; i ++,	flash_addr +=4)
		{
			mem[i] = *(unsigned int *)flash_addr;
		}
		
		// write new data into page buffer, maybe overwrite some FLASH bytes
		memtp = (unsigned char *)(mem) + head_page_offset;
	
		for(i = 0; i < len; i++)
		{
			*(memtp + i) = *(data_buf + i);
		}
		
		// erase one FLASH page
		FLASH_ErasePage(head_page_addr);
		
		// write the data in page buffer into one FLASH page
		for(i = 0; i < 512; i++)
		{
			if(mem[i] ^ 0xffffffff) FLASH_WriteBlock(head_page_addr + i, &mem[i],4);
		}
	}
	else         // span pages (in 2 pages)
	{	
		// copy FLASH data in the 1st page before the new DWORD area
		flash_addr = head_page_addr;
		n = (addr_start - head_page_addr) >> 2;
		
		for(i = 0; i < n; i ++, flash_addr += 4)
		{
			mem[i] = *(unsigned int *)flash_addr;
		}
		
		// write new data in the 1st page to the page buffer
		memtp = (unsigned char *)(mem)+ head_page_offset;
		n = tail_page_addr - addr;
		// number of the remained bytes for the page 2
		data_length -= n;			
		data_offset = n;
		
		// write new bytes in the page 1
		for(i = 0; i < n; i++)
		{
			*(memtp + i) = *(data_buf + i);
		}
		
		// erase one flash page
		FLASH_ErasePage(addr_start);
		
		// write page buffer into the page 1
		for(i = 0; i < 512; i++)
		{
			if(mem[i] ^ 0xffffffff) FLASH_WriteBlock(head_page_addr + i, &mem[i],4);
		}
		
		// copy flash data to be reserved in the page 2
		flash_addr = addr_end;
		n = ((addr_end & 0x7fc)>>2);
		for(i = n; i < 512; i ++,	flash_addr +=4)
		{
			mem[i] = *(unsigned int *)flash_addr;
		}
		
		// write new bytes into the page buffer
		memtp = (unsigned char *)mem;
		
		for(i = 0, j = data_offset; i < data_length; i++, j++)
		{
			*(memtp + i) = *(data_buf + j);
		}	
		
		// erase the 2nd page
		FLASH_ErasePage(addr_end);
		
		// write the 2nd page
		for(i = 0; i < 512; i++)
		{
			if(mem[i] ^ 0xffffffff) FLASH_WriteBlock(tail_page_addr + i, &mem[i],4);
		}		
	}
	
	
}

void writeonebyte(unsigned int addr,unsigned char status)
{
	unsigned int addr1,temp,temp1=0xffffffff;
	unsigned char i,offset;

	temp=*(unsigned int*)(addr & 0x01fffffc);
	addr1=addr & 0x01fffffc;
	
	offset=addr&0x03;
	offset=3-offset;
	temp1&=status;
	temp1|=0xffffff00;
	for(i=0;i<offset*8;i++)
	{
		temp1=(temp1<<1)|0x01;
	}
	temp&=temp1;
	FLASH_WriteBlock(addr1,&temp,4);
	
}





/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
