/**
  ******************************************************************************
  * @file    usbd_storage_msd.c
  * @author  MCD application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the disk operations functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_mem.h"
#include "usb_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
#define USE_STM3210C_EVAL

/** @defgroup STORAGE 
  * @brief media storage application module
  * @{
  */ 

/** @defgroup STORAGE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Defines
  * @{
  */ 

#define STORAGE_LUN_NBR                  1 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Variables
  * @{
  */ 
/* USB Mass storage Standard Inquiry Data */
const int8_t  STORAGE_Inquirydata[] = {//36

    0x00,          /* Direct Access Device */
    0x00,          /* RMB = 1: Removable Medium */
    0x02,          /* Version: No conformance claim to standard */
    0x02,

    0x1f,          /* Additional Length */
    0x00,          /* SCCS = 1: Storage Controller Component */
    0x00,
    0x00,
    /* Vendor Identification */
    'G', 'A', 'S', 'E', 'C', ' ', ' ', ' ',
    /* Product Identification */
    'G', 'A', 'F', 'K', 'E', 'Y', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* Product Revision Level */
    '1', '.', '0', '0'
  }; 

/**
  * @}
  */ 


/** @defgroup STORAGE_Private_FunctionPrototypes
  * @{
  */ 
int8_t STORAGE_Init (uint8_t lun);

int8_t STORAGE_GetCapacity (uint8_t lun, 
                           uint32_t *block_num, 
                           uint32_t *block_size);

int8_t  STORAGE_IsReady (uint8_t lun);

int8_t  STORAGE_IsWriteProtected (uint8_t lun);

int8_t STORAGE_Read (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_Write (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_GetMaxLun (void);


USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;
#ifndef USE_STM3210C_EVAL   
extern SD_CardInfo SDCardInfo;
#endif
__IO uint32_t count = 0;
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Functions
  * @{
  */ 


/**
  * @brief  Initialize the storage medium
  * @param  lun : logical unit number
  * @retval Status
  */

int8_t STORAGE_Init (uint8_t lun)
{
//#ifndef USE_STM3210C_EVAL 
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//#endif
	
//  if( SD_Init() != 0)
//  {
//    return (-1); 
//  } 
	
  return (0);
  
}

/**
  * @brief  return medium capacity and block size
  * @param  lun : logical unit number
  * @param  block_num :  number of physical block
  * @param  block_size : size of a physical block
  * @retval Status
  */
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
//#ifdef USE_STM3210C_EVAL   
//#ifndef USE_GOLDDRAGON_107 
////  SD_CardInfo SDCardInfo;
////  
////  SD_GetCardInfo(&SDCardInfo);  
//#endif
//#else
////  if(SD_GetStatus() != 0 )
////  {
////    return (-1); 
////  }   
//#endif  

  
    *block_size =  512;  
    *block_num = 512;
//#ifdef USE_GOLDDRAGON_107 
//	*block_num =  MASS_STORAGE_SIZE / 512;
//#else
////  *block_num =  SDCardInfo.CardCapacity / 512;  
//#endif
  
  return (0);
  
}

/**
  * @brief  check whether the medium is ready
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsReady (uint8_t lun)
{
  
//#ifndef USE_STM3210C_EVAL  
//  
//  static int8_t last_status = 0;

//  if(last_status  < 0)
//  {
//    SD_Init();
//    last_status = 0;
//  }
//  
//  if(SD_GetStatus() != 0)
//  {
//    last_status = -1;
//    return (-1); 
//  }  
//#else
////  if( SD_Init() != 0)
////  {
////    return (-1);
////  } 	
//#endif
//  return (0);
}

/**
  * @brief  check whether the medium is write-protected
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
  return  0;
}

/**
  * @brief  Read data from the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to save data
  * @param  blk_addr :  address of 1st block to be read
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
static int8_t fat0_8_buf[4096];
static int8_t fat8_32_buf[512*32];
static int8_t fat16_24_buf[4096];
static int8_t fat64_72_buf[4096];
static int8_t fat504_512_buf[4096];
static int8_t fat1016_1024_buf[4096];

int8_t STORAGE_Read (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{  
    uint8_t *addr;
	if(blk_addr < 8)
	{
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat0_8_buf + blk_addr,blk_len);
	}else if (blk_addr >= 8 && blk_addr < 40) {
		blk_addr -= 8;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat8_32_buf + blk_addr,blk_len);
	} else if (blk_addr >= 16 && blk_addr < 24) {
		blk_addr -= 8;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat16_24_buf + blk_addr,blk_len);
	}else if (blk_addr >= 64 && blk_addr < 72) {
		blk_addr -= 64;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat64_72_buf + blk_addr,blk_len);
	} else if (blk_addr >= 504 && blk_addr < 512) {
		blk_addr -= 504;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat64_72_buf + blk_addr,blk_len);
	}else if (blk_addr >= 1016) {
		blk_addr -= 1016;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(buf,fat1016_1024_buf + blk_addr,blk_len);
	} else {

	}
    return 0;
}
/**
  * @brief  Write data to the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to write from
  * @param  blk_addr :  address of 1st block to be written
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
int8_t STORAGE_Write (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
  
/*
	int i=0;
	for(i = 0; i< 10000000; i++)
	{
	}
*/
//	printf("ew addr:%d size:%d\n",blk_addr, blk_len);
	if(blk_addr < 8)
	{
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat0_8_buf + blk_addr,buf,blk_len);
	}else if (blk_addr >= 8 && blk_addr < 40) {
		blk_addr -= 8;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat8_32_buf + blk_addr,buf,blk_len);
	} else if (blk_addr >= 16 && blk_addr < 24) {
		blk_addr -= 16;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat16_24_buf + blk_addr,buf,blk_len);
	} else if (blk_addr >= 64 && blk_addr < 72) {
		blk_addr -= 64;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat64_72_buf + blk_addr,buf,blk_len);
	} else if (blk_addr >= 504 && blk_addr < 512) {
		blk_addr -= 504;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat64_72_buf + blk_addr,buf,blk_len);
	}else if (blk_addr >= 1016) {
		blk_addr -= 1016;
		blk_addr *= 512;
		blk_len  *= 512;
		memcpy(fat1016_1024_buf + blk_addr,buf,blk_len);
	} else {
//		printf("out write mass rang\n");
	}
	
	
	//printf("STORAGE_Write\n");
  return (0);
}

/**
  * @brief  Return number of supported logical unit
  * @param  None
  * @retval number of logical unit
  */

int8_t STORAGE_GetMaxLun (void)
{
  return (STORAGE_LUN_NBR - 1);
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

