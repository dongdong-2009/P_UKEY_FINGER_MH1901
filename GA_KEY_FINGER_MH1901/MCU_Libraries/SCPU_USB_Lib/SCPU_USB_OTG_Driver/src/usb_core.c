/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : usb_core.c
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 21-October-2014
 * Description          : USB-OTG Core layer.
 *****************************************************************************/
 
/* Include ------------------------------------------------------------------*/
#include "mhscpu.h"
#include "usb_core.h"
#include "usb_bsp.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/	
/* Private macro ------------------------------------------------------------*/	
/* Private variables --------------------------------------------------------*/	
/* Ptivate function prototypes ----------------------------------------------*/	

/******************************************************************************
* Function Name  :
* Description    :
* Input          :
* Output         :
* Return         :
******************************************************************************/
/** @addtogroup USB_OTG_DRIVER
* @{
*/

/** @defgroup USB_CORE 
* @brief This file includes the USB-OTG Core Layer
* @{
*/


/** @defgroup USB_CORE_Private_Defines
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USB_CORE_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 



/** @defgroup USB_CORE_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_CORE_Private_Variables
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_CORE_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_CORE_Private_Functions
* @{
*/ 
/**
* @brief  USB_OTG_DMAInt : The DMA mode init
* @param  pdev : Selected device; usb_dma_structure: usb dma init structure
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_DMAInt(USB_OTG_CORE_HANDLE *pdev, USB_OTG_DMA_Typedef *usb_dma_structure)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DMA_CNTL_TypeDef  DMA_cntl;
    USB_OTG_DMA_ADDR_TypeDef  DMA_addr;
    USB_OTG_DMA_COUNT_TypeDef DMA_count;
    DMA_cntl.d16 = 0x00;
    DMA_addr.d32 = 0x00;
    DMA_count.d32 = 0x00;
    /* Only one DMA channel */
    DMA_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
    DMA_cntl.b.dma_mode = usb_dma_structure->transfer_mode;
    /* Burst Mode 3: INCR16,INCR8,INCR4 or unspecified length */
    DMA_cntl.b.dma_brstm = usb_dma_structure->brstn;
    DMA_cntl.b.dma_ep = usb_dma_structure->dma_ep_num;
    DMA_cntl.b.dma_dir = usb_dma_structure->dir;
    USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
//    /* Note: that the initial memory address written to this register must have a value such
//     * that it's modulo 4 value is equal to 0.That is,DMA_ADDR[1:0] must be equal to 2'b00.The
//     * lower two bits of this register are read and cannot be set by software.
//     */
//    DMA_addr.d32 = (usb_dma_structure->addr & 0xFFFFFFFC);
//    USB_OTG_WRITE_REG32(&pdev->regs.DMAREGS[0]->DMA_ADDR, DMA_addr.d32);
//    /* Note: If DMA is enabled with a count of 0,the bus will not be requested and 
//     * DMA interrupt will be generated.
//     */
//    DMA_count.d32 = usb_dma_structure->count;
//    USB_OTG_WRITE_REG32(&pdev->regs.DMAREGS[0]->DMA_COUNT, DMA_count.d32);
    return status;
}
/**
* @brief  USB_OTG_DMA_DataTransferInt : The DMA mode data transfer init
* @param  pdev : Selected device; usb_dma_structure: usb dma init structure
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_DMA_DataTransferInt(USB_OTG_CORE_HANDLE *pdev, USB_OTG_DMA_Typedef *usb_dma_structure)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DMA_ADDR_TypeDef  DMA_addr;
    USB_OTG_DMA_COUNT_TypeDef DMA_count;
    DMA_addr.d32 = 0x00;
    DMA_count.d32 = 0x00;
//    USB_OTG_DMA_Cmd(pdev,DISABLE);
    /* Note: that the initial memory address written to this register must have a value such
     * that it's modulo 4 value is equal to 0.That is,DMA_ADDR[1:0] must be equal to 2'b00.The
     * lower two bits of this register are read and cannot be set by software.
     */
    DMA_addr.d32 = (usb_dma_structure->addr & 0xFFFFFFFC);
    USB_OTG_WRITE_REG32(&pdev->regs.DMAREGS[0]->DMA_ADDR, DMA_addr.d32);
    /* Note: If DMA is enabled with a count of 0,the bus will not be requested and 
     * DMA interrupt will be generated.
     */
    DMA_count.d32 = usb_dma_structure->count;
    USB_OTG_WRITE_REG32(&pdev->regs.DMAREGS[0]->DMA_COUNT, DMA_count.d32);
    USB_OTG_DMA_Cmd(pdev,ENABLE);
    return status;
}
/**
* @brief  USB_OTG_DMA_Cmd : The DMA mode enable
* @param  pdev : Selected device; NewState: ENABLE&DISABLE
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_DMA_Cmd(USB_OTG_CORE_HANDLE *pdev, FunctionalState NewState)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DMA_CNTL_TypeDef  DMA_cntl;
    #ifdef USE_DEVICE_MODE
    USB_OTG_RXCSRH_IN_PERIPHERAL_TypeDef rx_csrh;
    USB_OTG_TXCSRH_IN_PERIPHERAL_TypeDef tx_csrh;
    #endif
    #ifdef USE_HOST_MODE
    USB_OTG_RXCSRH_IN_HOST_TypeDef rx_csrh;
    USB_OTG_TXCSRH_IN_HOST_TypeDef tx_csrh;
    #endif
    USB_OTG_INTRRXE_TypeDef intr_rxe;
    USB_OTG_INTRTXE_TypeDef intr_txe;
    DMA_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
    if (NewState  != DISABLE)
    {
        if (DMA_cntl.b.dma_dir)         /* In Endpoint */
        {
            /* The relevant interrupt enable bit in the IntrTxE register should be 
             * set to 1(simply so that errors can be detected)
             */
            intr_txe.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTXE);
            intr_txe.d16 |= 0x01 << (DMA_cntl.b.dma_ep & 0x07);
            USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTXE, intr_txe.d16);
            tx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->TXCSRH);
            if (DMA_cntl.b.dma_mode)    /* Mode 1 */
            {
                /* The AutoSet(D15),DMAReqEnab(D12)and the DMAReqMode(D10)bits of the appropriate
                 * TxCSR register should be set to 1.
                 */
                #ifdef USE_DEVICE_MODE
                tx_csrh.b.auto_set = 1;
                tx_csrh.b.dma_req_enab = 1;
                tx_csrh.b.dma_req_mode = 1;
                #endif
                #ifdef USE_HOST_MODE
                tx_csrh.b.auto_set = 1;
                tx_csrh.b.dma_req_enab = 1;
                tx_csrh.b.dma_req_mode = 1;
                #endif
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->TXCSRH,tx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_ENABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            }else{
                /* Note: There is no need to set the MUSBMHDRC to support DMA for this 
                 *       operation.
                 */
                tx_csrh.b.dma_req_enab = 0;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->TXCSRH, tx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_ENABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            }
        }else{                          /* OUT Endpoint */
            /* The relevant interrupt enable bit in the IntrRxE register should be 
             * set to 1
             */
            intr_rxe.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRXE);
            intr_rxe.d16 |= 0x01 << (DMA_cntl.b.dma_ep & 0x07);
            USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRXE, intr_rxe.d16);
            rx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->RXCSRH);
            if (DMA_cntl.b.dma_mode)    /* Mode 1 */
            {
                /* The AutoClear(D15),DMAReqEnab(D13)and DMAReqMode(D11) bits of the appropriate
                 * RxCSR register should be set to 1.In Host mode,the Autoreq(D14) bit should also
                 * be set to 1 and the RqPktCount register should be programmed with the number 
                 * of packets in the transfer.
                 */
                #ifdef USE_DEVICE_MODE
                rx_csrh.b.auto_clear = 1;
                rx_csrh.b.dma_req_enab = 1;
                rx_csrh.b.dma_req_mode = 1;
                #endif
//                #ifdef USE_HOST_MODE
//                rx_csrh.b.auto_clear = 1;
//                rx_csrh.b.dma_req_enab = 1;
//                rx_csrh.b.dma_req_mode = 1;
//                rx_csrh.b.auto_req = 1;
//                #endif
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->RXCSRH,rx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_ENABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            }else{
                /* Note: There is no need to set the MUSBMHDRC to support DMA for this 
                 *       operation.
                 */
                rx_csrh.b.dma_req_enab = 0;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->RXCSRH, rx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_ENABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            } 
        }
    }else{
        if (DMA_cntl.b.dma_dir)
        {
            intr_txe.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTXE);
            intr_txe.d16 &= ~(0x01 << (DMA_cntl.b.dma_ep & 0x07));
            USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTXE, intr_txe.d16);
            tx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->TXCSRH);
            if (DMA_cntl.b.dma_mode)    /* Mode 1 */
            {
                #ifdef USE_DEVICE_MODE
                tx_csrh.b.auto_set = 0;
                tx_csrh.b.dma_req_enab = 0;
                tx_csrh.b.dma_req_mode = 0;
                #endif
                #ifdef USE_HOST_MODE
                tx_csrh.b.auto_set = 0;
                tx_csrh.b.dma_req_enab = 0;
                tx_csrh.b.dma_req_mode = 0;
                #endif
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->TXCSRH,tx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_DISABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            }else{
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_DISABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            } 
        }else{
            intr_rxe.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRXE);
            intr_rxe.d16 &= ~(0x01 << (DMA_cntl.b.dma_ep & 0x07));
            USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRXE, intr_rxe.d16);
            rx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->RXCSRH);
            if (DMA_cntl.b.dma_mode)    /* Mode 1 */
            {
                #ifdef USE_DEVICE_MODE
                rx_csrh.b.auto_clear = 0;
                rx_csrh.b.dma_req_enab = 0;
                rx_csrh.b.dma_req_mode = 0;
                #endif
//                #ifdef USE_HOST_MODE
//                rx_csrh.b.auto_clear = 0;
//                rx_csrh.b.dma_req_enab = 0;
//                rx_csrh.b.dma_req_mode = 0;
//                rx_csrh.b.auto_req = 0;
//                #endif
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[DMA_cntl.b.dma_ep]->RXCSRH,rx_csrh.d8);
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_DISABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            }else{                
                DMA_cntl.b.dma_en = USB_OTG_DMA_MODE_DISABLE;
                USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
            } 
        }
    }
    return status;
}
/**
* @brief  USB_OTG_DMA_IntCmd : The DMA interrupt enable
* @param  pdev : Selected device; NewState: ENABLE&DISABLE
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_DMA_IntCmd(USB_OTG_CORE_HANDLE *pdev, FunctionalState NewState)
{
    USB_OTG_STS status = USB_OTG_OK;
    if (NewState  != DISABLE)
    {
        pdev->regs.DMAREGS[0]->DMA_CNTL |= USB_OTG_DMA_INTR_ENABLE;
    }else{
        pdev->regs.DMAREGS[0]->DMA_CNTL &= USB_OTG_DMA_INTR_DISABLE;
    }
    return status;
}

/**
* @brief  USB_OTG_FifosizeReg
*         Turn Fifo size to Set register value
* @param  fifosiz : Set Fifo size
* @retval None
*/
static uint8_t USB_OTG_FifosizeReg(uint16_t fifosiz)
{
    uint8_t register_value = 0;
    assert_param(IS_FIFO_SIZE(fifosiz));
    
    switch (fifosiz)
    {
        case 8:
            register_value = 0;
            break;
        case 16:
            register_value = 1;
            break;
        case 32:
            register_value = 2;
            break;
        case 64:
            register_value = 3;
            break;
        case 128:
            register_value = 4;
            break;
        case 256:
            register_value = 5;
            break;
        case 512:
            register_value = 6;
            break;
        default:
            break;
    }
    return register_value;
}
/**
* @brief  USB_OTG_FifoStartAddr
*         
* @param  RxFifosize : Rx fifo size; TxFifosize: Tx fifo size
* @retval None
*/
static uint16_t USB_OTG_FifoStartAddr(uint32_t RxFifosize, uint32_t TxFifosize)
{
    uint16_t uint16_start_addr = 0; 
    uint16_start_addr = ((RxFifosize > TxFifosize) ?  RxFifosize : TxFifosize) >> 3;
    return uint16_start_addr;
}
/**
* @brief  USB_OTG_InitDevSpeed :Initializes the DevSpd field of DCFG register 
*         depending the PHY type and the enumeration speed of the device.
* @param  pdev : Selected device
* @retval : None
*/
void USB_OTG_InitDevSpeed(USB_OTG_CORE_HANDLE *pdev , uint8_t speed)
{
//    USB_OTG_POWER_TypeDef power;
//    power.d8 = USB_OTG_READ_REG8();
}

/**
* @brief  USB_OTG_EpFifoConfiguration : Configure Epn's FIFO Size
* @param  pdev : Selected device
* @retval None
*/
static void USB_OTG_EpFifoConfiguration(USB_OTG_CORE_HANDLE *pdev,
                                        uint8_t ep_num, 
                                        uint32_t Trx_fifo_startaddr, 
                                        uint16_t rx_fifo_size,
                                        uint16_t tx_fifo_size)
{
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef    txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef    rxcsrl;
    txcsrl.d8 = 0;
    rxcsrl.d8 = 0;
    txcsrl.b.flush_fifo = 1;
    rxcsrl.b.flush_fifo = 1;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INDEX, ep_num);
    #ifdef USE_DEVICE_MODE
    /* set Rx FIFO size */
    USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[ep_num]->RXMAXP, pdev->dev.out_ep[ep_num].maxpacket);
    USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->RXFIFOADD,  \
                        Trx_fifo_startaddr >> 3);
    USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->RXFIFOSZ,USB_OTG_FifosizeReg(rx_fifo_size));
    /* Flush the FIFOs */
    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep_num]->RXCSRL, rxcsrl.d8);
    /* set Tx FIFO size */
    USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[ep_num]->TXMAXP, pdev->dev.in_ep[ep_num].maxpacket);
	USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->TXFIFOADD,  \
                        (Trx_fifo_startaddr + rx_fifo_size) >> 3);
    USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->TXFIFOSZ,USB_OTG_FifosizeReg(tx_fifo_size));
    /* Flush the FIFOs */
    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep_num]->TXCSRL, txcsrl.d8);
    #endif
    #ifdef USE_HOST_MODE
    
    #endif
}
/**
* @brief  USB_OTG_EnableCommonInt
*         Initializes the commmon interrupts, used in both device and host modes
* @param  pdev : Selected device
* @retval None
*/
static void USB_OTG_EnableCommonInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_INTRUSBE_TypeDef intr_usbe;
    intr_usbe.d8 = 0;
    /* Clear any pending interrupts */
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSB, 0xFF);
    /* Enable the interrupts in the INTMSK */
    intr_usbe.b.en_resume = 1;
    intr_usbe.b.en_suspend = 1; 
  
#ifdef USE_OTG_MODE
    intr_usbe.b.en_sess_req = 1;
    intr_usbe.b.en_discon = 1;
#endif
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSBE, intr_usbe.d8);
    /* Enable Epn_Rx interrupr(n = 1,2,3,4,5,6,7) */
    USB_OTG_WRITE_REG16( &pdev->regs.COMMREGS->INTRRXE, 0xFE);
    /* Enable Epn_Tx interrupr(n = 0,1,2,3,4,5,6,7) */
    USB_OTG_WRITE_REG16( &pdev->regs.COMMREGS->INTRTXE, 0xFF);
}

/**
* @brief  USB_OTG_CoreReset : Soft reset of the core
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
static USB_OTG_STS USB_OTG_CoreReset(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    __IO USB_OTG_POWER_TypeDef  power;

    power.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->POWER);
    /* Core Soft Reset */
    power.b.en_suspendM = 1;
    power.b.reset = 1;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
    /* The CPU should keep the Reset bit set for at least 20 ms to ensure correct 
     * resetting of the target device.
     */
    USB_OTG_BSP_mDelay(25);
    power.b.reset = 0;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
    /* Wait for 3 PHY Clocks*/
    USB_OTG_BSP_uDelay(3);
    return status;
}

/**
* @brief  USB_OTG_WritePacket : Writes a packet into the Tx FIFO associated 
*         with the EP
* @param  pdev : Selected device
* @param  src : source pointer
* @param  ch_ep_num : end point number
* @param  bytes : No. of bytes
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_WritePacket(USB_OTG_CORE_HANDLE *pdev, 
                                uint8_t             *src, 
                                uint8_t             ch_ep_num, 
                                uint16_t            len)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DMA_Typedef usb_dma_structure;
    USB_OTG_DMA_CNTL_TypeDef dma_cntl;
    uint32_t i= 0;
    #ifdef USE_DEVICE_MODE
    if (pdev->cfg.dma_enable == 0)
    {
        for (i = 0; i < len; i++)
        {
            pdev->regs.FIFO[ch_ep_num]->byte = *src++;
        }
    } else{
        if ((ch_ep_num != (USB_OTG_DEV_DMA_EP_NUM & 0x07)) || \
            (ch_ep_num == 0) || \
            (!pdev->dev.in_ep[ch_ep_num].dma_addr))
            
        {
            for (i = 0; i < len; i++)
            {
                pdev->regs.FIFO[ch_ep_num]->byte = *src++;
            }
        }else{
            dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
            usb_dma_structure.addr = pdev->dev.in_ep[ch_ep_num].dma_addr;
            usb_dma_structure.count = len;
            USB_OTG_DMA_DataTransferInt(pdev,&usb_dma_structure);
            dma_cntl.b.dma_brstm = 0;
            dma_cntl.b.dma_ep = 1;
            dma_cntl.b.dma_ie = 1;
            dma_cntl.b.dma_mode = 0;
            dma_cntl.b.dma_dir = 1;
            dma_cntl.b.dma_en = 1;
            USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL,dma_cntl.d16);
        }
    }
    #endif
    #ifdef USE_HOST_MODE
    if (pdev->cfg.dma_enable == 0)
    {
        for (i = 0; i < len; i++)
        {
            pdev->regs.FIFO[ch_ep_num]->byte = *src++;
        }
    } 
    #endif
    return status;
}


/**
* @brief  USB_OTG_ReadPacket : Reads a packet from the Rx FIFO
* @param  pdev : Selected device
* @param  dest : Destination Pointer
* @param  bytes : No. of bytes
* @retval None
*/
void *USB_OTG_ReadPacket(USB_OTG_CORE_HANDLE *pdev, 
                         uint8_t *dest, 
                         uint8_t ch_ep_num, 
                         uint16_t len)
{
    uint32_t i=0;
    uint32_t count32b = (len + 3) / 4;
    uint32_t *data_buff = (uint32_t *)dest;
    USB_OTG_FIFO *fifo = pdev->regs.FIFO[ch_ep_num];
    for (i = 0; i < count32b; i++, data_buff++)
    {
        *data_buff = pdev->regs.FIFO[ch_ep_num]->word;
    }
    /* Return the buffer pointer because if the transfer is composed of several
     packets, the data of the next packet must be stored following the 
     previous packet's data */
    return ((void *)dest);
}

/**
* @brief  USB_OTG_SelectCore 
*         Initialize core registers address.
* @param  pdev : Selected device
* @param  coreID : USB OTG Core ID
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_SelectCore(USB_OTG_CORE_HANDLE *pdev, 
                               USB_OTG_CORE_ID_TypeDef coreID)
{
    uint32_t i , baseAddress = 0;
    USB_OTG_STS status = USB_OTG_OK;
    #ifdef  USB_OTG_DMA_ENABLE
        pdev->cfg.dma_enable       = 1; 
    #else
        pdev->cfg.dma_enable       = 0;
    #endif
//    pdev->cfg.dma_enable       = 0;
    /* at startup the core is in FS mode */
    pdev->cfg.speed            = USB_OTG_SPEED_FULL;
    pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;    
  
    /* initialize device cfg following its address */
    if (coreID == USB_OTG_FS_CORE_ID)
    {
        baseAddress                = USB_OTG_FS_BASE_ADDR;
        pdev->cfg.coreID           = USB_OTG_FS_CORE_ID;
        pdev->cfg.host_channels    = 8 ;
        pdev->cfg.dev_endpoints    = 4 ;
        pdev->cfg.TotalFifoSize    = 512; /* in 8-bits */
        pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;     

        #ifdef USB_OTG_FS_SOF_OUTPUT_ENABLED    
        pdev->cfg.Sof_output       = 1;    
        #endif 

        #ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT    
        pdev->cfg.low_power        = 1;    
        #endif     
    }
    else if (coreID == USB_OTG_HS_CORE_ID)
    {
        baseAddress                = USB_OTG_FS_BASE_ADDR;
        pdev->cfg.coreID           = USB_OTG_HS_CORE_ID;    
        pdev->cfg.host_channels    = 8 ;
        pdev->cfg.dev_endpoints    = 4 ;
        pdev->cfg.TotalFifoSize    = 512;/* in 8-bits */

        #ifdef USB_OTG_ULPI_PHY_ENABLED
            pdev->cfg.phy_itface       = USB_OTG_ULPI_PHY;
        #else    
        #ifdef USB_OTG_EMBEDDED_PHY_ENABLED
            pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;
        #endif  
        #endif      

        #ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED    
            pdev->cfg.dma_enable       = 1;    
        #endif

        #ifdef USB_OTG_HS_SOF_OUTPUT_ENABLED    
            pdev->cfg.Sof_output       = 1;    
        #endif 

        #ifdef USB_OTG_HS_LOW_PWR_MGMT_SUPPORT    
            pdev->cfg.low_power        = 1;    
        #endif 
    }
  
    /* Common USB Registers */
    pdev->regs.COMMREGS = (USB_OTG_COMMREGS *)(baseAddress +\
                                USB_OTG_COMMON_GLOBAL_REG_OFFSET);
    /* Indexed CSR */
    pdev->regs.INDEXREGS = (USB_OTG_INDEXREGS *)(baseAddress +\
                                 USB_OTG_INDEXED_CSR_REG_OFFSET);
    /* FIFOS */
    for (i = 0; i < NUM_EP_FIFO; i++)
    {
        pdev->regs.FIFO[i] = (USB_OTG_FIFO *)(baseAddress +\
           USB_OTG_EP_FIFO_GLOBAL_REG_OFFSET + i * USB_OTG_EP_FIFO_REG_OFFSET);
    }
    /* DynFIFO + Version */
    pdev->regs.DYNFIFOREGS = (USB_OTG_DYNFIFOREGS *)(baseAddress +\
                                   USB_OTG_DYNFIFO_REG_OFFSET);
    /* ULPI & Addnl. Config. registers */
    pdev->regs.ULPIREGS = (USB_OTG_ULPIREGS *)(baseAddress +\
                                   USB_OTG_ULPI_REG_OFFSET);
    /* TADDR Epn (n = 0 - 15). 0x80 - 0xFF */
    for (i = 0; i < NUM_EP_FIFO; i++)
    {
        pdev->regs.TADDRREGS[i] = (USB_OTG_TADDRREGS *)(baseAddress +\
           USB_OTG_TADDR_GLOBAL_REG_OFFSET + i * USB_OTG_TADDR_REG_OFFSET);
    }
    /* CSR EPn (n = 0 - 15). 0x100 - 0x1FF */
    for (i = 0; i < NUM_EP_FIFO; i++)
    {
        pdev->regs.CSRREGS[i] = (USB_OTG_CSRREGS *)(baseAddress +\
           USB_OTG_EP_CSR_GLOBAL_REG_OFFSET + i * USB_OTG_EP_CSR_REG_OFFSET);
    }
    /* Optional DMA Registers */
    for (i = 0; i < NUM_DMA_CHANNEL; i++)
    {
        pdev->regs.DMAREGS[i] = (USB_OTG_DMAREGS *)(baseAddress +\
                                    USB_OTG_DMA_GLOBAL_REG_OFFSET + i * USB_OTG_DMA_REG_OFFSET);
    }
    /* Extended Registers */
    pdev->regs.EXTREGS = (USB_OTG_EXTREGS *)(baseAddress +\
                                   USB_OTG_EXTENED_REG_OFFSET);
    /* LPM Registers */
    pdev->regs.LPMREGS = (USB_OTG_LPMREGS *)(baseAddress +\
                                   USB_OTG_LPM_REG_OFFSET);
  
    return status;
}


/**
* @brief  USB_OTG_CoreInit
*         Initializes the USB_OTG controller registers and prepares the core
*         device mode or host mode operation.
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_CoreInit(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
//    USB_OTG_GUSBCFG_TypeDef  usbcfg;
//    USB_OTG_GCCFG_TypeDef    gccfg;
//    USB_OTG_GAHBCFG_TypeDef  ahbcfg;

//    usbcfg.d32 = 0;
//    gccfg.d32 = 0;
//    ahbcfg.d32 = 0;
//  
//    /* FS interface*/ 
//    /* Reset after a PHY select and set Host mode */
//    USB_OTG_CoreReset(pdev);
//    /* Deactivate the power down*/
//    gccfg.d32 = 0;
//    gccfg.b.pwdn = 1;
//    
//    gccfg.b.vbussensingA = 1 ;
//    gccfg.b.vbussensingB = 1 ;     
//#ifndef VBUS_SENSING_ENABLED
//    gccfg.b.disablevbussensing = 1; 
//#endif    
//    
//    if(pdev->cfg.Sof_output)
//    {
//        gccfg.b.sofouten = 1;  
//    }
//    
//    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GCCFG, gccfg.d32);
//    USB_OTG_BSP_mDelay(20);
//    /* case the HS core is working in FS mode */
//    if(pdev->cfg.dma_enable == 1)
//    {
//        ahbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GAHBCFG);
//        ahbcfg.b.hburstlen = 5; /* 64 x 32-bits*/
//        ahbcfg.b.dmaenable = 1;
//        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32);
//    }
//    /* initialize OTG features */
//#ifdef  USE_OTG_MODE
//    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
//    usbcfg.b.hnpcap = 1;
//    usbcfg.b.srpcap = 1;
//    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
//    USB_OTG_EnableCommonInt(pdev);
//#endif
    return status;
}
/**
* @brief  USB_OTG_EnableGlobalInt
*         Enables the controller's Global Int in the ISER reg
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EnableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    /* USB -- INT_ISER[1] */
    NVIC->ISER[0] = 0x02;  /* Set Bit1 */
    return status;
}


/**
* @brief  USB_OTG_DisableGlobalInt
*         Disables the controller's Global Int in the ICER reg
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    /* USB -- INT_ICER[1] */
    NVIC->ICER[0] = 0x00000002;  /* Clear Bit1 */
    return status;
}


/**
* @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
* @param  pdev : Selected device
* @param  num : FO num
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_FlushTxFifo (USB_OTG_CORE_HANDLE *pdev , uint32_t num )
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_TXCSRL_IN_HOST_TypeDef txcsrl;
    txcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.TXCSRL);
    if (txcsrl.b.tx_pkt_rdy)
        txcsrl.b.flush_fifo = 1;
    USB_OTG_WRITE_REG8( &pdev->regs.INDEXREGS->CSRL.TXCSRL, txcsrl.d8 );
    /* Wait for 3 PHY Clocks*/
    USB_OTG_BSP_uDelay(3);
    return status;
}


/**
* @brief  USB_OTG_FlushRxFifo : Flush a Rx FIFO
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_FlushRxFifo( USB_OTG_CORE_HANDLE *pdev )
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_RXCSRL_IN_HOST_TypeDef rxcsrl;
    rxcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->RXCSRL);
    if (rxcsrl.b.rx_pkt_rdy)
        rxcsrl.b.flush_fifo = 1;
    USB_OTG_WRITE_REG8( &pdev->regs.INDEXREGS->RXCSRL, rxcsrl.d8 );
    /* Wait for 3 PHY Clocks*/
    USB_OTG_BSP_uDelay(3);
    return status;
}


/**
* @brief  USB_OTG_SetCurrentMode : Set ID line
* @param  pdev : Selected device
* @param  mode :  (Host/device)
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_SetCurrentMode(USB_OTG_CORE_HANDLE *pdev , uint8_t mode)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_POWER_TypeDef  power;
    USB_OTG_DEVCTL_TypeDef devctl;
    
    power.d8 = 0;
    devctl.d8 = USB_OTG_READ_REG8(&pdev->regs.DYNFIFOREGS->DEVCTL);
    
    if ( mode == HOST_MODE)
    {
        power.b.en_suspendM = 1;
        USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
        devctl.b.host_mode = 1;
        devctl.b.session = 1;
        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->DEVCTL, devctl.d8);
    }
    else if ( mode == DEVICE_MODE)
    {
        devctl.b.host_mode = 0;
        power.b.en_suspendM = 1;
        devctl.b.session = 0;
        power.b.soft_conn = 1;
        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->DEVCTL, devctl.d8);
        USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
        #ifdef USE_DEVICE_MODE
        pdev->dev.out_ep[0].xfer_buff = pdev->dev.setup_packet;
        pdev->dev.out_ep[0].xfer_len = 8;
        #endif
    }
    USB_OTG_BSP_mDelay(50);
    return status;
}


/**
* @brief  USB_OTG_GetMode : Get current mode
* @param  pdev : Selected device
* @retval current mode
*/
uint8_t USB_OTG_GetMode(USB_OTG_CORE_HANDLE *pdev)
{
    return ((USB_OTG_READ_REG8(&pdev->regs.DYNFIFOREGS->DEVCTL) & 0x4) >> 2);
}


/**
* @brief  USB_OTG_IsDeviceMode : Check if it is device mode
* @param  pdev : Selected device
* @retval num_in_ep
*/
uint8_t USB_OTG_IsDeviceMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_GetMode(pdev) != HOST_MODE);
}


/**
* @brief  USB_OTG_IsHostMode : Check if it is host mode
* @param  pdev : Selected device
* @retval num_in_ep
*/
uint8_t USB_OTG_IsHostMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_GetMode(pdev) == HOST_MODE);
}


/**
* @brief  USB_OTG_ReadCoreItr : returns the Core Interrupt register
* @param  pdev : Selected device
* @retval Status
*/
uint8_t USB_OTG_ReadCoreItr(USB_OTG_CORE_HANDLE *pdev)
{
    uint8_t v = 0;
    v = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSB);
    v &= USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSBE);
    return v;
}


/**
* @brief  USB_OTG_ReadOtgItr : returns the USB_OTG Interrupt register
* @param  pdev : Selected device
* @retval Status
*/
uint8_t USB_OTG_ReadOtgItr (USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSB));
}
#ifdef USE_HOST_MODE
/**
* @brief  USB_OTG_CoreInitHost : Initializes USB_OTG controller+ for host mode
* @param  pdev : Selected device
* @retval status
*/
USB_OTG_STS USB_OTG_CoreInitHost(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS                     status = USB_OTG_OK;
    uint32_t i, 
             Trx_fifo_size = 0, 
             Ttx_fifo_size = 0;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef    txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef    rxcsrl;    
    
    txcsrl.d8 = 0;
    rxcsrl.d8 = 0;
    txcsrl.b.flush_fifo = 1;
    rxcsrl.b.flush_fifo = 1;
    /* Test */	
	pdev->regs.CSRREGS[1]->TXMAXP = 0x0040;
	pdev->regs.CSRREGS[1]->TXCSRL = 0x48; // ClrDataTog, FlushFIFO.
	pdev->regs.CSRREGS[1]->TXCSRH = 0x28; // TxMode, FrcDataTog.
	
	// FIFO tot size = 512.
	// 0-63, TP0, 64-127, TP1 Txd, 128-191, TP1, RXD.
	pdev->regs.COMMREGS->INDEX = 1;
	pdev->regs.DYNFIFOREGS->TXFIFOSZ = 3; // 64
	pdev->regs.DYNFIFOREGS->TXFIFOADD = 8; 
	pdev->regs.DYNFIFOREGS->RXFIFOSZ = 3; // 64
	pdev->regs.DYNFIFOREGS->RXFIFOADD = 16;
	pdev->regs.COMMREGS->INDEX = 0;

    if (pdev->cfg.coreID == USB_OTG_FS_CORE_ID  )
    {  
//		/* Set Full speed phy */
//        USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_FULL);
//        /* FIFO total size = 512 bytes */
//        /* EP0 */
//        USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INDEX, 0);
//        /* set Rx FIFO size */
//        USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->RXFIFOADD, 0);
//        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->RXFIFOSZ,USB_OTG_FifosizeReg(RX0_FIFO_FS_SIZE));
//        /* set Tx FIFO size */
//        USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->TXFIFOADD,0);
//        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->TXFIFOSZ,USB_OTG_FifosizeReg(TX0_FIFO_FS_SIZE));
//        /* EP1 */
//        Trx_fifo_size = ((RX0_FIFO_FS_SIZE > TX0_FIFO_FS_SIZE) ?  RX0_FIFO_FS_SIZE : TX0_FIFO_FS_SIZE);
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    1,
//                                    Trx_fifo_size,
//                                    RX1_FIFO_FS_SIZE,
//                                    TX1_FIFO_FS_SIZE);
//        /* EP2 */
//        Trx_fifo_size += RX1_FIFO_FS_SIZE + TX1_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    2,
//                                    Trx_fifo_size,
//                                    RX2_FIFO_FS_SIZE,
//                                    TX2_FIFO_FS_SIZE);
//        
//        /* EP3 */ 
//        Trx_fifo_size += RX2_FIFO_FS_SIZE + TX2_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    3,
//                                    Trx_fifo_size,
//                                    RX3_FIFO_FS_SIZE,
//                                    TX3_FIFO_FS_SIZE);
//        /* EP4 */ 
//        Trx_fifo_size += RX3_FIFO_FS_SIZE + TX3_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    4,
//                                    Trx_fifo_size,
//                                    RX4_FIFO_FS_SIZE,
//                                    TX4_FIFO_FS_SIZE);
//        /* EP5 */ 
//        Trx_fifo_size += RX4_FIFO_FS_SIZE + TX4_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    5,
//                                    Trx_fifo_size,
//                                    RX5_FIFO_FS_SIZE,
//                                    TX5_FIFO_FS_SIZE);
//        /* EP6 */ 
//        Trx_fifo_size += RX5_FIFO_FS_SIZE + TX5_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    6,
//                                    Trx_fifo_size,
//                                    RX6_FIFO_FS_SIZE,
//                                    TX6_FIFO_FS_SIZE);
//        /* EP7 */ 
//        Trx_fifo_size += RX6_FIFO_FS_SIZE + TX6_FIFO_FS_SIZE;
//        USB_OTG_EpFifoConfiguration(pdev,
//                                    7,
//                                    Trx_fifo_size,
//                                    RX7_FIFO_FS_SIZE,
//                                    TX7_FIFO_FS_SIZE);
    }  
    /* Clear all pending Device Interrupts */
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INTRUSBE, 0x00);
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INTRUSB, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRXE, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRX, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTXE, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTX, 0x00);
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INDEX, 0);
    if (pdev->cfg.dma_enable == 1)
    {
 
    }
    USB_OTG_EnableHostInt(pdev);
    return status;
}

/**
* @brief  USB_OTG_IsEvenFrame 
*         This function returns the frame number for sof packet
* @param  pdev : Selected device
* @retval Frame number
*/
uint8_t USB_OTG_IsEvenFrame (USB_OTG_CORE_HANDLE *pdev) 
{
    return !(USB_OTG_READ_REG16(&pdev->regs.COMMREGS->FRAME) & 0x1);
}

/**
* @brief  USB_OTG_DriveVbus : set/reset vbus
* @param  pdev : Selected device
* @param  state : VBUS state
* @retval None
*/
void USB_OTG_DriveVbus (USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
//  USB_OTG_HPRT0_TypeDef     hprt0;
//  
//  hprt0.d32 = 0;
//  
//  /* enable disable the external charge pump */
//  USB_OTG_BSP_DriveVBUS(pdev, state);
//  
//  /* Turn on the Host port power. */
//  hprt0.d32 = USB_OTG_ReadHPRT0(pdev);
//  if ((hprt0.b.prtpwr == 0 ) && (state == 1 ))
//  {
//    hprt0.b.prtpwr = 1;
//    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
//  }
//  if ((hprt0.b.prtpwr == 1 ) && (state == 0 ))
//  {
//    hprt0.b.prtpwr = 0;
//    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
//  }
//  
//  USB_OTG_BSP_mDelay(200);
}
/**
* @brief  USB_OTG_EnableHostInt: Enables the Host mode interrupts
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EnableHostInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS       status = USB_OTG_OK;
    USB_OTG_INTRUSBE_TypeDef intr_usbe;
    USB_OTG_DMA_INTR_TypeDef dma_intr;
    /* Disable all interrupts. */
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSBE, 0);
    /* Clear any pending interrupts */
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSB, 0);
    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pdev);
    
    if (pdev->cfg.dma_enable == 0)
    {
        dma_intr.d8 = 0;
        /* Only One DMA Intr register */
        USB_OTG_WRITE_REG8( &pdev->regs.DMAREGS[0]->DMA_INTR, dma_intr.d8);
    }
  
    /* Enable interrupts matching to the Host mode ONLY */
    intr_usbe.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSBE);
    intr_usbe.b.en_VBus_error = 1;
    intr_usbe.b.en_sess_req = 1;
    intr_usbe.b.en_discon = 1;
    intr_usbe.b.en_conn = 1;
    intr_usbe.b.en_sof = 1;
    intr_usbe.b.en_reset_babble = 1;
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSBE, intr_usbe.d8);
    return status;
}

/**
* @brief  USB_OTG_InitFSLSPClkSel : Initializes the FSLSPClkSel field of the 
*         HCFG register on the PHY type
* @param  pdev : Selected device
* @param  freq : clock frequency
* @retval None
*/
void USB_OTG_InitFSLSPClkSel(USB_OTG_CORE_HANDLE *pdev , uint8_t freq)
{
//  USB_OTG_HCFG_TypeDef   hcfg;
//  
//  hcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.HREGS->HCFG);
//  hcfg.b.fslspclksel = freq;
//  USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HCFG, hcfg.d32);
}


/**
* @brief  USB_OTG_ReadHPRT0 : Reads HPRT0 to modify later
* @param  pdev : Selected device
* @retval HPRT0 value
*/
uint32_t USB_OTG_ReadHPRT0(USB_OTG_CORE_HANDLE *pdev)
{
//  USB_OTG_HPRT0_TypeDef  hprt0;
//  
//  hprt0.d32 = USB_OTG_READ_REG32(pdev->regs.HPRT0);
//  hprt0.b.prtena = 0;
//  hprt0.b.prtconndet = 0;
//  hprt0.b.prtenchng = 0;
//  hprt0.b.prtovrcurrchng = 0;
//  return hprt0.d32;
}


/**
* @brief  USB_OTG_ReadHostAllChannels_intr : Register PCD Callbacks
* @param  pdev : Selected device
* @retval Status
*/
uint8_t USB_OTG_ReadHostAllChannels_intr (USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_READ_REG8 (&pdev->regs.COMMREGS->INTRUSB));
}


/**
* @brief  USB_OTG_ResetPort : Reset Host Port
* @param  pdev : Selected device
* @retval status
* @note : (1)The application must wait at least 10 ms (+ 10 ms security)
*   before clearing the reset bit.
*/
uint32_t USB_OTG_ResetPort(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_POWER_TypeDef power;
    power.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->POWER);
    power.b.en_suspendM = 1;
    power.b.reset = 1;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
    USB_OTG_BSP_mDelay(20);
    power.b.en_suspendM = 1;
    power.b.reset = 0;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
    return 1;
}


/**
* @brief  USB_OTG_HC_Init : Prepares a host channel for transferring packets
* @param  pdev : Selected device
* @param  hc_num : channel number
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_HC_Init(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS status = USB_OTG_OK;
    uint32_t intr_enable = 0;
    USB_OTG_TYPE0_TypeDef  type0;
    USB_OTG_TXTYPE_TypeDef tx_type;
    USB_OTG_RXTYPE_TypeDef rx_type;
    USB_OTG_RXCSRH_IN_HOST_TypeDef  rx_csrh;
    USB_OTG_RXCSRL_IN_HOST_TypeDef  rx_csrl;
    USB_OTG_TXCSRH_IN_HOST_TypeDef  tx_csrh;
    USB_OTG_TXCSRL_IN_HOST_TypeDef  tx_csrl;
    type0.d8 = 0x00;
    tx_type.d8 = 0x00;
    rx_type.d8 = 0x00;
    rx_csrh.d8 = 0x00;
    rx_csrl.d8 = 0x00;
    tx_csrh.d8 = 0x00;
    tx_csrl.d8 = 0x00;
    switch (pdev->host.hc[hc_num].ep_type) 
    {
        case EP_TYPE_CTRL:
            type0.b.speed = pdev->host.hc[hc_num].speed;
            /* Speed */
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->TXPE.TYPE0, type0.d8);
            break;
        case EP_TYPE_BULK:
            if (pdev->host.hc[hc_num].ep_is_in) 
            {
                rx_type.b.speed = pdev->host.hc[hc_num].speed;
                rx_type.b.protocol = EP_TYPE_BULK;
                rx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXTYPE, rx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXINTERVAL, 3);
                /* If the CPU sets this bit, the ReqPkt bit will be automatically set when the RxPktRdy bit is cleared. 
                 * Note: This bit is automatically cleared when a short packet is received. 
                 */
                rx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH);
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                rx_csrh.b.auto_req = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH, rx_csrh.d8);
                /* When the endpoint is first configured, the endpoint data toggle should be set to 0 */
                rx_csrl.b.clr_data_tog = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                /* Clear FIFO */
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                if (rx_csrl.b.rx_pkt_rdy)
                {
                    rx_csrl.b.flush_fifo = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                    if (rx_csrl.b.rx_pkt_rdy)
                    {
                        rx_csrl.b.flush_fifo = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    }
                }
            } 
            else 
            {
                tx_type.b.speed = pdev->host.hc[hc_num].speed;
                tx_type.b.protocol = EP_TYPE_BULK;
                tx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXTYPE, tx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXINTERVAL, 3);
            }
            break;
        case EP_TYPE_INTR:
            if (pdev->host.hc[hc_num].ep_is_in) 
            {
                rx_type.b.speed = pdev->host.hc[hc_num].speed;
                rx_type.b.protocol = EP_TYPE_INTR;
                rx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXTYPE, rx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXINTERVAL, 1);
                /* If the CPU sets this bit, the ReqPkt bit will be automatically set when the RxPktRdy bit is cleared. 
                 * Note: This bit is automatically cleared when a short packet is received. 
                 */
                rx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH);
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
//                rx_csrh.b.auto_req = 1;
//                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH, rx_csrh.d8);
                /* When the endpoint is first configured, the endpoint data toggle should be set to 0 */
                rx_csrl.b.clr_data_tog = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                /* Clear FIFO */
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                if (rx_csrl.b.rx_pkt_rdy)
                {
                    rx_csrl.b.flush_fifo = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                    if (rx_csrl.b.rx_pkt_rdy)
                    {
                        rx_csrl.b.flush_fifo = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    }
                }
            } 
            else 
            {
                tx_type.b.speed = pdev->host.hc[hc_num].speed;
                tx_type.b.protocol = EP_TYPE_INTR;
                tx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXTYPE, tx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXINTERVAL, 1);
            }
            break;
        case EP_TYPE_ISOC:
            if (pdev->host.hc[hc_num].ep_is_in) 
            {
                rx_type.b.speed = pdev->host.hc[hc_num].speed;
                rx_type.b.protocol = EP_TYPE_ISOC;
                rx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXTYPE, rx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXINTERVAL, 1);
                /* If the CPU sets this bit, the ReqPkt bit will be automatically set when the RxPktRdy bit is cleared. 
                 * Note: This bit is automatically cleared when a short packet is received. 
                 */
                rx_csrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH);
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                rx_csrh.b.auto_req = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRH, rx_csrh.d8);
                /* When the endpoint is first configured, the endpoint data toggle should be set to 0 */
                rx_csrl.b.clr_data_tog = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                /* Clear FIFO */
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                if (rx_csrl.b.rx_pkt_rdy)
                {
                    rx_csrl.b.flush_fifo = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                    if (rx_csrl.b.rx_pkt_rdy)
                    {
                        rx_csrl.b.flush_fifo = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                    }
                }
            } 
            else 
            {
                tx_type.b.speed = pdev->host.hc[hc_num].speed;
                tx_type.b.protocol = EP_TYPE_ISOC;
                tx_type.b.target_EP_number = pdev->host.hc[hc_num].ep_num;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXTYPE, tx_type.d8);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXMAXP, \
                                                             pdev->host.hc[hc_num].max_packet);
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXINTERVAL, 1);
            }
            break;
    }
    return status;
}


/**
* @brief  USB_OTG_HC_StartXfer : Start transfer
* @param  pdev : Selected device
* @param  hc_num : channel number
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_HC_StartXfer(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS status = USB_OTG_OK;
    uint16_t                 len_words = 0;   
    uint16_t num_packets;
    uint16_t max_hc_pkt_count;
    USB_OTG_CSR0L_IN_HOST_TypeDef csr0l;
    USB_OTG_CSR0H_IN_HOST_TypeDef csr0h;
    USB_OTG_TXCSRL_IN_HOST_TypeDef tx_csrl;
    USB_OTG_RXCSRL_IN_HOST_TypeDef rx_csrl;
    max_hc_pkt_count = 256;
  
    /* Compute the expected number of packets associated to the transfer */
    if (pdev->host.hc[hc_num].xfer_len > 0)
    {
        num_packets = (pdev->host.hc[hc_num].xfer_len + \
                       pdev->host.hc[hc_num].max_packet - 1) / pdev->host.hc[hc_num].max_packet;

        if (num_packets > max_hc_pkt_count)
        {
            num_packets = max_hc_pkt_count;
            pdev->host.hc[hc_num].xfer_len = num_packets * \
                                             pdev->host.hc[hc_num].max_packet;
        }
    }
    else    /* Data-length is zero */
    {   
        num_packets = 1;
    }
    if (pdev->host.hc[hc_num].ep_is_in)
    {
        pdev->host.hc[hc_num].xfer_len = num_packets * \
                                         pdev->host.hc[hc_num].max_packet;
    }
      
    if (pdev->cfg.dma_enable == 1)
    {

    }
    if (pdev->cfg.dma_enable == 0) /* Slave mode */
    {  
//        if((pdev->host.hc[hc_num].ep_is_in == 0) && 
//           (pdev->host.hc[hc_num].xfer_len > 0))
        if(pdev->host.hc[hc_num].ep_is_in == 0)
        {
            switch(pdev->host.hc[hc_num].ep_type) 
            {
            /* Non periodic transfer */
            case EP_TYPE_CTRL:
            case EP_TYPE_BULK:
                if (pdev->host.hc[hc_num].ep_num)
                {
                    tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL);
                    if (pdev->host.hc[hc_num].xfer_len == 0)
                    {
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            0);
                        pdev->host.hc[hc_num].xfer_count = 0;
                        tx_csrl.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                    }
                    else if (pdev->host.hc[hc_num].xfer_len >= 64)
                    {
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            64);
                        pdev->host.hc[hc_num].xfer_count += 64;
                        tx_csrl.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                    }else {
                        /* Write packet into the Tx FIFO. */
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            pdev->host.hc[hc_num].xfer_len);
                        pdev->host.hc[hc_num].xfer_count = pdev->host.hc[hc_num].xfer_len;
                        tx_csrl.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                    }
                }else{
                    csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
                    if (pdev->host.hc[hc_num].xfer_len == 0)
                    {
                        /* Write packet into the Tx FIFO. */
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            0);
                        pdev->host.hc[hc_num].xfer_count += pdev->host.hc[hc_num].xfer_len;
                        csr0l.b.tx_pkt_rdy = 1;
                        csr0l.b.setup_pkt = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                    }
                    else if (pdev->host.hc[hc_num].xfer_len >= 64)
                    {
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            64);
                        pdev->host.hc[hc_num].xfer_count += 64;
                        csr0l.b.tx_pkt_rdy = 1;
                        csr0l.b.setup_pkt = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                    }else {
                        /* Write packet into the Tx FIFO. */
                        USB_OTG_WritePacket(pdev, 
                                            pdev->host.hc[hc_num].xfer_buff, 
                                            pdev->host.hc[hc_num].ep_num,
                                            pdev->host.hc[hc_num].xfer_len);
                        pdev->host.hc[hc_num].xfer_count += pdev->host.hc[hc_num].xfer_len;
                        csr0l.b.tx_pkt_rdy = 1;
                        csr0l.b.setup_pkt = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                    }
                }
                break;
            /* Periodic transfer */
            case EP_TYPE_INTR:
            case EP_TYPE_ISOC:
                tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL);
                if (pdev->host.hc[hc_num].xfer_len == 0)
                {
                    /* Write packet into the Tx FIFO. */
                    USB_OTG_WritePacket(pdev, 
                                        pdev->host.hc[hc_num].xfer_buff, 
                                        pdev->host.hc[hc_num].ep_num,
                                        0);
                    pdev->host.hc[hc_num].xfer_count += pdev->host.hc[hc_num].xfer_len;
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                }
                else if (pdev->host.hc[hc_num].xfer_len >= 64)
                {
                    USB_OTG_WritePacket(pdev, 
                                        pdev->host.hc[hc_num].xfer_buff, 
                                        pdev->host.hc[hc_num].ep_num,
                                        64);
                    pdev->host.hc[hc_num].xfer_count += 64;
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                }else {
                    /* Write packet into the Tx FIFO. */
                    USB_OTG_WritePacket(pdev, 
                                        pdev->host.hc[hc_num].xfer_buff, 
                                        pdev->host.hc[hc_num].ep_num,
                                        pdev->host.hc[hc_num].xfer_len);
                    pdev->host.hc[hc_num].xfer_count = pdev->host.hc[hc_num].xfer_len;
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->TXCSRL, tx_csrl.d8);
                }
                break;

            default:
                break;
            }
        }
        else 
        {
            switch(pdev->host.hc[hc_num].ep_type) 
            {
            /* Non periodic transfer */
            case EP_TYPE_CTRL:
                csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
                csr0h.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRH.CSR0H);
                csr0l.b.req_pkt = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                break;
            case EP_TYPE_BULK:
                /* Periodic transfer */
            case EP_TYPE_INTR:
            case EP_TYPE_ISOC:
                rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL);
                rx_csrl.b.ReqPkt = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[pdev->host.hc[hc_num].ep_num]->RXCSRL, rx_csrl.d8);
                break;
            default:
                break;
            }
        }
    }
  return status;
}


/**
* @brief  USB_OTG_HC_Halt : Halt channel
* @param  pdev : Selected device
* @param  hc_num : channel number
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_HC_Halt(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
  USB_OTG_STS status = USB_OTG_OK;
//  USB_OTG_HNPTXSTS_TypeDef            nptxsts;
//  USB_OTG_HPTXSTS_TypeDef             hptxsts;
//  USB_OTG_HCCHAR_TypeDef              hcchar;
//  
//  nptxsts.d32 = 0;
//  hptxsts.d32 = 0;
//  hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
//  hcchar.b.chen = 1;
//  hcchar.b.chdis = 1;
//  
//  /* Check for space in the request queue to issue the halt. */
//  if (hcchar.b.eptype == HCCHAR_CTRL || hcchar.b.eptype == HCCHAR_BULK)
//  {
//    nptxsts.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->HNPTXSTS);
//    if (nptxsts.b.nptxqspcavail == 0)
//    {
//      hcchar.b.chen = 0;
//    }
//  }
//  else
//  {
//    hptxsts.d32 = USB_OTG_READ_REG32(&pdev->regs.HREGS->HPTXSTS);
//    if (hptxsts.b.ptxqspcavail == 0)
//    {
//      hcchar.b.chen = 0;
//    }
//  }
//  USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, hcchar.d32);
  return status;
}

/**
* @brief  Issue a ping token
* @param  None
* @retval : None
*/
USB_OTG_STS USB_OTG_HC_DoPing(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
  USB_OTG_STS               status = USB_OTG_OK;
//  USB_OTG_HCCHAR_TypeDef    hcchar;
//  USB_OTG_HCTSIZn_TypeDef   hctsiz;  
//  
//  hctsiz.d32 = 0;
//  hctsiz.b.dopng = 1;
//  hctsiz.b.pktcnt = 1;
//  USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCTSIZ, hctsiz.d32);
//  
//  hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
//  hcchar.b.chen = 1;
//  hcchar.b.chdis = 0;
//  USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, hcchar.d32);
  return status;  
}

/**
* @brief  Stop the device and clean up fifo's
* @param  None
* @retval : None
*/
void USB_OTG_StopHost(USB_OTG_CORE_HANDLE *pdev)
{
//  USB_OTG_HCCHAR_TypeDef  hcchar;
//  uint32_t                i;
//  
//  USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HAINTMSK , 0);
//  USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HAINT,      0xFFFFFFFF);
//  /* Flush out any leftover queued requests. */
//  
//  for (i = 0; i < pdev->cfg.host_channels; i++)
//  {
//    hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[i]->HCCHAR);
//    hcchar.b.chen = 0;
//    hcchar.b.chdis = 1;
//    hcchar.b.epdir = 0;
//    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[i]->HCCHAR, hcchar.d32);
//  }
//  
//  /* Flush the FIFO */
//  USB_OTG_FlushRxFifo(pdev);
//  USB_OTG_FlushTxFifo(pdev ,  0x10 );  
}
#endif
#ifdef USE_DEVICE_MODE
/*         PCD Core Layer       */
/**
* @brief  USB_OTG_CoreInitDev : Initializes the USB_OTG controller registers 
*         for device mode
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_CoreInitDev (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS  status = USB_OTG_OK;
    uint32_t i, 
             Trx_fifo_startaddr = 0;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef    txcsrl;
    USB_OTG_TXCSRH_IN_PERIPHERAL_TypeDef    txcsrh;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef    rxcsrl;   
    USB_OTG_RXCSRH_IN_PERIPHERAL_TypeDef    rxcsrh;      
    USB_OTG_DMA_Typedef    USB_OTG_DMA_structure;
    txcsrl.d8 = 0;
    rxcsrl.d8 = 0;
    txcsrl.b.flush_fifo = 1;
    rxcsrl.b.flush_fifo = 1;
    if (pdev->cfg.coreID == USB_OTG_FS_CORE_ID  )
    {  
        /* Set Full speed phy */
        USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_FULL);
        /* FIFO total size = 512 bytes */
        /* EP0 */
        USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INDEX, 0);
        /* set Rx FIFO size */
        USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->RXFIFOADD, 0);
        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->RXFIFOSZ,USB_OTG_FifosizeReg(RX0_FIFO_FS_SIZE));
        /* set Tx FIFO size */
        USB_OTG_WRITE_REG16(&pdev->regs.DYNFIFOREGS->TXFIFOADD,0);
        USB_OTG_WRITE_REG8(&pdev->regs.DYNFIFOREGS->TXFIFOSZ,USB_OTG_FifosizeReg(TX0_FIFO_FS_SIZE));
        /* EP1 */
        Trx_fifo_startaddr = ((RX0_FIFO_FS_SIZE > TX0_FIFO_FS_SIZE) ?  RX0_FIFO_FS_SIZE : TX0_FIFO_FS_SIZE);
        USB_OTG_EpFifoConfiguration(pdev,
                                    1,
                                    Trx_fifo_startaddr,
                                    RX1_FIFO_FS_SIZE,
                                    TX1_FIFO_FS_SIZE);
        /* EP2 */
        Trx_fifo_startaddr += RX1_FIFO_FS_SIZE + TX1_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    2,
                                    Trx_fifo_startaddr,
                                    RX2_FIFO_FS_SIZE,
                                    TX2_FIFO_FS_SIZE);
        
        /* EP3 */ 
        Trx_fifo_startaddr += RX2_FIFO_FS_SIZE + TX2_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    3,
                                    Trx_fifo_startaddr,
                                    RX3_FIFO_FS_SIZE,
                                    TX3_FIFO_FS_SIZE);
        /* EP4 */ 
        Trx_fifo_startaddr += RX3_FIFO_FS_SIZE + TX3_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    4,
                                    Trx_fifo_startaddr,
                                    RX4_FIFO_FS_SIZE,
                                    TX4_FIFO_FS_SIZE);
        /* EP5 */ 
        Trx_fifo_startaddr += RX4_FIFO_FS_SIZE + TX4_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    5,
                                    Trx_fifo_startaddr,
                                    RX5_FIFO_FS_SIZE,
                                    TX5_FIFO_FS_SIZE);
        /* EP6 */ 
        Trx_fifo_startaddr += RX5_FIFO_FS_SIZE + TX5_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    6,
                                    Trx_fifo_startaddr,
                                    RX6_FIFO_FS_SIZE,
                                    TX6_FIFO_FS_SIZE);
        /* EP7 */ 
        Trx_fifo_startaddr += RX6_FIFO_FS_SIZE + TX6_FIFO_FS_SIZE;
        USB_OTG_EpFifoConfiguration(pdev,
                                    7,
                                    Trx_fifo_startaddr,
                                    RX7_FIFO_FS_SIZE,
                                    TX7_FIFO_FS_SIZE);
    }  
    /* Clear all pending Device Interrupts */
    USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSB);
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INTRUSBE, 0x00);
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INTRUSB, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRXE, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRRX, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTXE, 0x00);
    USB_OTG_WRITE_REG16(&pdev->regs.COMMREGS->INTRTX, 0x00);
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INDEX, 0);
    if (pdev->cfg.dma_enable == 1)
    {
        if (USB_OTG_DEV_DMA_EP_NUM & 0x80)
        {
            txcsrh.d8 = USB_OTG_READ_REG8( &pdev->regs.CSRREGS[USB_OTG_DEV_DMA_EP_NUM & 0x07]->TXCSRH);
            txcsrh.b.dma_req_enab = 0;
            USB_OTG_WRITE_REG8( &pdev->regs.CSRREGS[USB_OTG_DEV_DMA_EP_NUM & 0x07]->TXCSRH,txcsrh.d8);
        }else{
            rxcsrh.d8 = USB_OTG_READ_REG8( &pdev->regs.CSRREGS[USB_OTG_DEV_DMA_EP_NUM & 0x07]->RXCSRH);
        }
        /* Burst Mode 3: INCR16,INCR8,INCR4 or unspecified length */
        USB_OTG_DMA_structure.brstn = USB_OTG_DMA_BRSTN_MODE0;
        /* Default use Mode0 */
        #ifdef USE_USB_DMA_MODE1
        USB_OTG_DMA_structure.transfer_mode = USB_OTG_DMA_MODE1_TRANSFER;
        #else
        USB_OTG_DMA_structure.transfer_mode = USB_OTG_DMA_MODE0_TRANSFER;
        #endif
        if (USB_OTG_DEV_DMA_EP_NUM & 0x80)  /* In endpoint */
        {
            USB_OTG_DMA_structure.dir = USB_OTG_DMA_READ_Endpoint;
        }else{
            USB_OTG_DMA_structure.dir = USB_OTG_DMA_WRITE_Endpoint;
        }
        USB_OTG_DMA_structure.dma_ep_num = USB_OTG_DEV_DMA_EP_NUM & 0x07;
        /* DMA Function Init */
        USB_OTG_DMAInt(pdev, &USB_OTG_DMA_structure);
    }
    USB_OTG_EnableDevInt(pdev);
    return status;
}
/**
* @brief  USB_OTG_EnableDevInt : Enables the Device mode interrupts
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EnableDevInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_INTRUSBE_TypeDef intr_usbe;
    USB_OTG_DMA_CNTL_TypeDef  DMA_cntl;
    /* Disable all interrupts. */
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSBE, 0);
    /* Clear any pending interrupts */
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSB, 0);
    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pdev);
    
    if (pdev->cfg.dma_enable == 1)
    {
        DMA_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
        DMA_cntl.b.dma_ie = USB_OTG_DMA_INTR_ENABLE;
        /* Enable DMA Interrupt */
        USB_OTG_WRITE_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL, DMA_cntl.d16);
    }
  
    intr_usbe.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSBE);
    intr_usbe.b.en_discon = 1;
    intr_usbe.b.en_sof = 1;
    intr_usbe.b.en_reset_babble = 1;
//	intr_usbe.b.en_suspend = 1;
//	intr_usbe.b.en_resume = 1;
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->INTRUSBE, intr_usbe.d8);
    return status;
}


/**
* @brief  USB_OTG_GetDeviceSpeed
*         Get the device speed from the device status register
* @param  None
* @retval status
*/
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev)
{
//  USB_OTG_DSTS_TypeDef  dsts;
    enum USB_OTG_SPEED speed = USB_SPEED_UNKNOWN;
  
  
//  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
//  
//  switch (dsts.b.enumspd)
//  {
//  case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
//    speed = USB_SPEED_HIGH;
//    break;
//  case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
//  case DSTS_ENUMSPD_FS_PHY_48MHZ:
//    speed = USB_SPEED_FULL;
//    break;
//    
//  case DSTS_ENUMSPD_LS_PHY_6MHZ:
//    speed = USB_SPEED_LOW;
//    break;
//  }
    speed = USB_SPEED_FULL;
    return speed;
}
/**
* @brief  enables EP0 OUT to receive SETUP packets and configures EP0
*   for transmitting packets
* @param  None
* @retval USB_OTG_STS : status
*/
USB_OTG_STS  USB_OTG_EP0Activate(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS             status = USB_OTG_OK;
//  USB_OTG_DSTS_TypeDef    dsts;
//  USB_OTG_DEPCTL_TypeDef  diepctl;
//  USB_OTG_DCTL_TypeDef    dctl;
//  
//  dctl.d32 = 0;
//  /* Read the Device Status and Endpoint 0 Control registers */
//  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
//  diepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL);
//  /* Set the MPS of the IN EP based on the enumeration speed */
//  switch (dsts.b.enumspd)
//  {
//  case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
//  case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
//  case DSTS_ENUMSPD_FS_PHY_48MHZ:
//    diepctl.b.mps = DEP0CTL_MPS_64;
//    break;
//  case DSTS_ENUMSPD_LS_PHY_6MHZ:
//    diepctl.b.mps = DEP0CTL_MPS_8;
//    break;
//  }
//  USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL, diepctl.d32);
//  dctl.b.cgnpinnak = 1;
//  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, dctl.d32);
  return status;
}


/**
* @brief  USB_OTG_EPActivate : Activates an EP
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPActivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_INTRRXE_TypeDef intr_rxe;
    USB_OTG_INTRTXE_TypeDef intr_txe;
//    __IO uint16_t *addr;
    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
//        addr = &pdev->regs.COMMREGS->INTRTXE;
        intr_txe.d16 = 1 << ep->num;
        USB_OTG_MODIFY_REG16(&pdev->regs.COMMREGS->INTRTXE, 0, intr_txe.d16);
    }
    else
    {
//        addr = &pdev->regs.COMMREGS->INTRRXE;
        intr_rxe.d16 = 1 << ep->num;
        USB_OTG_MODIFY_REG16(&pdev->regs.COMMREGS->INTRTXE, 0, intr_rxe.d16);
    }  
    return status;
}


/**
* @brief  USB_OTG_EPDeactivate : Deactivates an EP
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_INTRRXE_TypeDef intr_rxe;
    USB_OTG_INTRTXE_TypeDef intr_txe;
//    __IO uint16_t *addr;
    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
//        addr = &pdev->regs.COMMREGS->INTRTXE;
        intr_txe.d16 = 1 << ep->num;
        USB_OTG_MODIFY_REG16(&pdev->regs.COMMREGS->INTRTXE, intr_txe.d16, 0);
    }
    else
    {
//        addr = &pdev->regs.COMMREGS->INTRRXE;
        intr_rxe.d16 = 1 << ep->num;
        USB_OTG_MODIFY_REG16(&pdev->regs.COMMREGS->INTRTXE, intr_rxe.d16, 0);
    }  
    return status;
}


/**
* @brief  USB_OTG_EPStartXfer : Handle the setup for data xfer for an EP and 
*         starts the xfer
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPStartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef tx_csrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef rx_csrl;
    USB_OTG_RXCOUNT_TypeDef  rx_count;
    USB_OTG_DMA_CNTL_TypeDef dma_cntl;
    /* IN endpoint */
    if (ep->is_in == 1)
    {
        if (pdev->cfg.dma_enable == 1)
        {
            dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
            if ((dma_cntl.b.dma_mode == USB_OTG_DMA_MODE1_TRANSFER) && ((USB_OTG_DEV_DMA_EP_NUM & 0x07) == ep->num))
            {
                
            }
            if ((dma_cntl.b.dma_mode == USB_OTG_DMA_MODE0_TRANSFER) && ((USB_OTG_DEV_DMA_EP_NUM & 0x07) == ep->num))
            {
                ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                /* Zero Length Packet? */
                if (ep->rem_data_len == 0)
                {
//                    pdev->dev.in_ep[ep->num].dma_addr = 0;
//                    USB_OTG_WritePacket(pdev, 
//                                        ep->xfer_buff + ep->xfer_count, 
//                                        ep->num, 
//                                        0);
//                    ep->xfer_count = ep->xfer_len;
//                    ep->rem_data_len = 0; 
//                    tx_csrl.b.tx_pkt_rdy = 1;
//                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL,tx_csrl.d8);
                }
                else
                {
                    if (ep->rem_data_len > ep->maxpacket)
                    {
                        pdev->dev.in_ep[ep->num].dma_addr = (uint32_t)&ep->xfer_buff + ep->xfer_count;
                        USB_OTG_WritePacket(pdev, 
                                            ep->xfer_buff + ep->xfer_count, 
                                            ep->num, 
                                            ep->maxpacket); 
                        ep->xfer_count += ep->maxpacket;
                        if (ep->xfer_len >= ep->xfer_count)
                        {
                            ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                        }     
                        else     
                        {
                            ep->rem_data_len = 0;
                            ep->xfer_count = ep->xfer_len;
                        }                    
                    }
                    else
                    {
                        pdev->dev.in_ep[ep->num].dma_addr = (uint32_t)ep->xfer_buff + ep->xfer_count;
                        USB_OTG_WritePacket(pdev, 
                                            ep->xfer_buff + ep->xfer_count, 
                                            ep->num, 
                                            ep->rem_data_len); 
                        ep->xfer_count = ep->xfer_len; 
                        ep->rem_data_len = 0;                        
                    }
                }
            }
        }
        if ((pdev->cfg.dma_enable == 0) || \
            ((USB_OTG_DEV_DMA_EP_NUM & 0x07) != ep->num))
        {
            tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL);
            ep->rem_data_len = ep->xfer_len - ep->xfer_count;
            /* Zero Length Packet? */
            if (ep->rem_data_len == 0)
            {
                USB_OTG_WritePacket(pdev, 
                                    ep->xfer_buff + ep->xfer_count, 
                                    ep->num, 
                                    0);
                ep->xfer_count = ep->xfer_len;
                ep->rem_data_len = 0; 
                tx_csrl.b.tx_pkt_rdy = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL,tx_csrl.d8);
            }
            else
            {
                if (ep->rem_data_len > ep->maxpacket)
                {
                    USB_OTG_WritePacket(pdev, 
                                        ep->xfer_buff + ep->xfer_count, 
                                        ep->num, 
                                        ep->maxpacket); 
                    ep->xfer_count += ep->maxpacket;
                    if (ep->xfer_len >= ep->xfer_count)
                    {
                        ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                    }     
                    else     
                    {
                        ep->rem_data_len = 0;
                        ep->xfer_count = ep->xfer_len;
                    }                    
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL,tx_csrl.d8);
                }
                else
                {
                    USB_OTG_WritePacket(pdev, 
                                        ep->xfer_buff + ep->xfer_count, 
                                        ep->num, 
                                        ep->rem_data_len); 
                    ep->xfer_count = ep->xfer_len; 
                    ep->rem_data_len = 0;                
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL,tx_csrl.d8);
                }
            }
        }
    }
    else
    {
        /* OUT endpoint */
        if (pdev->cfg.dma_enable == 1)
        {
            dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
            if ((dma_cntl.b.dma_mode == USB_OTG_DMA_MODE1_TRANSFER) && (dma_cntl.b.dma_ep == ep->num))
            {
                
            }
        }
        if ((!pdev->cfg.dma_enable) || \
            (dma_cntl.b.dma_ep != ep->num) || \
            (dma_cntl.b.dma_mode == USB_OTG_DMA_MODE0_TRANSFER))
        {
            rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL);
            if (ep->xfer_len == 0)
            {
                ep->rem_data_len = 0;
                ep->xfer_count = ep->xfer_len;
                rx_csrl.b.rx_pkt_rdy = 0;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL,rx_csrl.d8);
            }
            else
            {
                rx_count.d16 = USB_OTG_READ_REG16(&pdev->regs.CSRREGS[ep->num]->RXCOUNT);
                USB_OTG_ReadPacket(pdev, 
                                   ep->xfer_buff + ep->xfer_count, 
                                   ep->num, 
                                   rx_count.d16);
                ep->xfer_count += rx_count.d16;
                if (ep->xfer_len >= ep->xfer_count)
                {
                    ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                }
                else
                {
                    ep->rem_data_len = 0;
                    ep->xfer_count = ep->xfer_len;
                }
                rx_csrl.b.rx_pkt_rdy = 0;
                USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL,rx_csrl.d8);
            }
        }
    }
    return status;
}


/**
* @brief  USB_OTG_EP0StartXfer : Handle the setup for a data xfer for EP0 and 
*         starts the xfer
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS                 status = USB_OTG_OK;
    USB_OTG_COUNT0_TypeDef      count0;
    USB_OTG_CSR0L_IN_PERIPHERAL_TypeDef csr0l;
    uint32_t fifoemptymsk = 0;

    count0.d8 = 0;
    csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
    /* IN endpoint */
    if (ep->is_in == 1)
    {
        ep->rem_data_len = ep->xfer_len - ep->xfer_count;
        /* Zero Length Packet? */
        if (ep->rem_data_len == 0)
        {
            USB_OTG_WritePacket(pdev, 
                                ep->xfer_buff, 
                                0, 
                                ep->xfer_len);
            ep->xfer_count = ep->xfer_len;
            ep->rem_data_len = 0;
            csr0l.b.tx_pkt_rdy = 1;
            csr0l.b.data_end = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,csr0l.d8);
        }
        else
        {
            if (ep->rem_data_len > ep->maxpacket)
            {
                USB_OTG_WritePacket(pdev, 
                                    ep->xfer_buff + ep->xfer_count, 
                                    0, 
                                    ep->maxpacket); 
                ep->xfer_count += ep->maxpacket;
                ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                csr0l.b.tx_pkt_rdy = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,csr0l.d8);
            }
            else
            {
                USB_OTG_WritePacket(pdev, 
                                    ep->xfer_buff + ep->xfer_count, 
                                    0, 
                                    ep->rem_data_len);  
                ep->xfer_count = ep->xfer_len;    
                ep->rem_data_len = 0;                
                csr0l.b.tx_pkt_rdy = 1;
                csr0l.b.data_end = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,csr0l.d8);
            }
        }
        if (pdev->cfg.dma_enable == 1)
        {

        }

        if (pdev->cfg.dma_enable == 0)
        {
            /* Enable the Tx FIFO Empty Interrupt for this EP */
            if (ep->xfer_len > 0)
            {
            }
        }
    }
    else
    {
        /* Program the transfer size and packet count as follows:
        * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
        * pktcnt = N           */
        if (ep->xfer_len == 0)
        {

        }
        else
        {

        }
        if (pdev->cfg.dma_enable == 1)
        {
        }
    }
    return status;
}


/**
* @brief  USB_OTG_EPSetStall : Set the EP STALL
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPSetStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_CSR0L_IN_PERIPHERAL_TypeDef csr0l;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef rxcsrl;
 
    if (ep->is_in == 1)
    {
        if (ep->num)
        {
            txcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL);
            /* set the stall bit */
            txcsrl.b.send_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL, txcsrl.d8);
        }
        else
        {
            csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
            /* set the stall bit */
            csr0l.b.send_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
        }
    }
    else    /* OUT Endpoint */
    {
        if (ep->num)
        {
            rxcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL);
            /* set the stall bit */
            rxcsrl.b.send_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL, rxcsrl.d8);
        }
        else
        {
            csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
            /* set the stall bit */
            csr0l.b.send_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
        }
    }
    return status;
}


/**
* @brief  Clear the EP STALL
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPClearStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;

    USB_OTG_CSR0L_IN_PERIPHERAL_TypeDef csr0l;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef rxcsrl;
 
    if (ep->is_in == 1)
    {
        if (ep->num)
        {
            txcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL);
            /* set the stall bit */
            txcsrl.b.sent_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->TXCSRL, txcsrl.d8);
        }
        else
        {
            csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
            /* set the stall bit */
            csr0l.b.sent_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
        }
    }
    else    /* OUT Endpoint */
    {
        if (ep->num)
        {
            rxcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL);
            /* set the stall bit */
            rxcsrl.b.sent_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[ep->num]->RXCSRL, rxcsrl.d8);
        }
        else
        {
            csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
            /* set the stall bit */
            csr0l.b.sent_stall = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
        }
    }
    return status;
}


/**
* @brief  USB_OTG_ReadDevAllOutEp_itr : returns OUT endpoint interrupt bits
* @param  pdev : Selected device
* @retval OUT endpoint interrupt bits
*/
uint16_t USB_OTG_ReadDevAllOutEp_itr(USB_OTG_CORE_HANDLE *pdev)
{
  uint16_t v;
  v  = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRX);
  v &= USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRXE);
  return ((v & 0xFE));
}


/**
* @brief  USB_OTG_ReadDevOutEP_itr : returns Device OUT EP Interrupt register
* @param  pdev : Selected device
* @param  ep : end point number
* @retval Device OUT EP Interrupt register
*/
uint16_t USB_OTG_ReadDevOutEP_itr(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  uint16_t v;
  v  = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRX &(0x01 << epnum));
  v &= USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRRXE);
  return v;
}


/**
* @brief  USB_OTG_ReadDevAllInEPItr : Get int status register
* @param  pdev : Selected device
* @retval int status register
*/
uint16_t USB_OTG_ReadDevAllInEPItr(USB_OTG_CORE_HANDLE *pdev)
{
  uint16_t v;
  v = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTX);
  v &= USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTXE);
  return (v & 0xFF);
}

/**
* @brief  configures EPO to receive SETUP packets
* @param  None
* @retval : None
*/
void USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev)
{
//  USB_OTG_DEP0XFRSIZ_TypeDef  doeptsize0;
//  doeptsize0.d32 = 0;
//  doeptsize0.b.supcnt = 3;
//  doeptsize0.b.pktcnt = 1;
//  doeptsize0.b.xfersize = 8 * 3;
//  USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPTSIZ, doeptsize0.d32 );
//  
//  if (pdev->cfg.dma_enable == 1)
//  {
//    USB_OTG_DEPCTL_TypeDef  doepctl;
//    doepctl.d32 = 0;
//    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPDMA, 
//                        (uint32_t)&pdev->dev.setup_packet);
//    
//    /* EP enable */
//    doepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[0]->DOEPCTL);
//    doepctl.b.epena = 1;
//    doepctl.d32 = 0x80008000;
//    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPCTL, doepctl.d32);
//  }
}

/**
* @brief  USB_OTG_RemoteWakeup : active remote wakeup signalling
* @param  None
* @retval : None
*/
void USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_POWER_TypeDef power;
    /* Note: If CLK has been stopped,it will need be restarted before 
     * this write can occur. 
     */
    power.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->POWER); 
    power.b.resume = 1;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
    /* The software should leave then this bit set for approximately 10ms
     * (minimum of 2ms, a maximum of 15ms) before resetting it to 0.    
     */
    USB_OTG_BSP_mDelay(10);
    power.b.resume = 0;
    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->POWER, power.d8);
}


/**
* @brief  USB_OTG_UngateClock : active USB Core clock
* @param  None
* @retval : None
*/
void USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pdev)
{
//  if(pdev->cfg.low_power)
//  {
//    
//    USB_OTG_DSTS_TypeDef     dsts;
//    USB_OTG_PCGCCTL_TypeDef  power; 
//    
//    dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
//    
//    if(dsts.b.suspsts == 1)
//    {
//      /* un-gate USB Core clock */
//      power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
//      power.b.gatehclk = 0;
//      power.b.stoppclk = 0;
//      USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
//      
//    }
//  }
}

/**
* @brief  Stop the device and clean up fifo's
* @param  None
* @retval : None
*/
void USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pdev)
{
//  uint32_t i;
//  
//  pdev->dev.device_status = 1;
//  
//  for (i = 0; i < pdev->cfg.dev_endpoints ; i++)
//  {
//    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFF);
//    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
//  }
//  
//  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, 0 );
//  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, 0 );
//  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, 0 );
//  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFF );  
//  
//  /* Flush the FIFO */
//  USB_OTG_FlushRxFifo(pdev);
//  USB_OTG_FlushTxFifo(pdev ,  0x10 );  
}

/**
* @brief  returns the EP Status
* @param  pdev : Selected device
*         ep : endpoint structure
* @retval : EP status
*/

uint32_t USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,USB_OTG_EP *ep)
{
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef  txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef  rxcsrl;
    __IO uint8_t *depctl_addr;
    uint32_t Status = 0;  

    txcsrl.d8 = 0;
    rxcsrl.d8 = 0;
    if (ep->is_in == 1)
    {
        depctl_addr = &(pdev->regs.CSRREGS[ep->num]->TXCSRL);
        txcsrl.d8 = USB_OTG_READ_REG8(depctl_addr);

        if (txcsrl.b.sent_stall == 1)  
            Status = USB_OTG_EP_TX_STALL;
        else if (txcsrl.b.under_run == 1)
            Status = USB_OTG_EP_TX_NAK;
        else 
            Status = USB_OTG_EP_TX_VALID;     
    }
    else
    {
        depctl_addr = &(pdev->regs.CSRREGS[ep->num]->RXCSRL);
        rxcsrl.d8 = USB_OTG_READ_REG8(depctl_addr);
        if (rxcsrl.b.sent_stall == 1)  
            Status = USB_OTG_EP_RX_STALL;
        else if ((rxcsrl.b.data_error == 1) || (rxcsrl.b.over_run == 1))
            Status = USB_OTG_EP_RX_NAK;
        else 
            Status = USB_OTG_EP_RX_VALID; 
    } 

    /* Return the current status */
    return Status;
}

/**
* @brief  Set the EP Status
* @param  pdev : Selected device
*         Status : new Status
*         ep : EP structure
* @retval : None
*/
void USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep , uint32_t Status)
{
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef  txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef  rxcsrl;
    __IO uint8_t *depctl_addr;

    txcsrl.d8 = 0;
    rxcsrl.d8 = 0;
    /* Process for IN endpoint */
    if (ep->is_in == 1)
    {
        depctl_addr = &(pdev->regs.CSRREGS[ep->num]->TXCSRL);
        txcsrl.d8 = USB_OTG_READ_REG8(depctl_addr);

        if (Status == USB_OTG_EP_TX_STALL)  
        {
            USB_OTG_EPSetStall(pdev, ep); 
            return;
        }
        else if (Status == USB_OTG_EP_TX_NAK)
        {
//            txcsrl.b.snak = 1;
        }
        else if (Status == USB_OTG_EP_TX_VALID)
        {
            if ((txcsrl.b.sent_stall == 1) || (txcsrl.b.send_stall == 1))
            {  
                ep->even_odd_frame = 0;
                USB_OTG_EPClearStall(pdev, ep);
                return;
            }      
            txcsrl.b.send_stall = 0;
            txcsrl.b.sent_stall = 0; 
            txcsrl.b.under_run = 0;
        }
        else if (Status == USB_OTG_EP_TX_DIS)
        {
//            txcsrl.b.usbactep = 0;
        }
        USB_OTG_WRITE_REG8(depctl_addr, txcsrl.d8); 
    } 
    else /* Process for OUT endpoint */
    {
        depctl_addr = &(pdev->regs.CSRREGS[ep->num]->RXCSRL);
        rxcsrl.d8 = USB_OTG_READ_REG8(depctl_addr);    

        if (Status == USB_OTG_EP_RX_STALL)  {
            rxcsrl.b.send_stall = 1;
        }
        else if (Status == USB_OTG_EP_RX_NAK)
        {
//            rxcsrl.b.over_run = 1;
//            rxcsrl.b.data_error = 1;
        }
        else if (Status == USB_OTG_EP_RX_VALID)
        {
            if ((rxcsrl.b.send_stall == 1) || (rxcsrl.b.sent_stall == 1))
            {  
                ep->even_odd_frame = 0;
                USB_OTG_EPClearStall(pdev, ep);
                return;
            }  
            rxcsrl.b.send_stall = 0;
            rxcsrl.b.sent_stall = 0;    
            rxcsrl.b.data_error = 0;
            rxcsrl.b.over_run = 0;
        }
        else if (Status == USB_OTG_EP_RX_DIS)
        {
//            txcsrl.b.usbactep = 0;    
        }
        USB_OTG_WRITE_REG8(depctl_addr, rxcsrl.d8); 
    }
}

#endif
/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/
/************************ (C) COPYRIGHT 2014 Megahuntmicro ****END OF FILE****/
