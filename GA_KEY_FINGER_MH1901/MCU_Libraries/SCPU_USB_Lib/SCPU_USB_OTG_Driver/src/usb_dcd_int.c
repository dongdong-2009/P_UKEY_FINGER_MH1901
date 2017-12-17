/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : usb_dcd_int.c
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 21-October-2014
 * Description          : Peripheral Device interrupt subroutines.
 *****************************************************************************/
 
/* Include ------------------------------------------------------------------*/
#include "usb_dcd_int.h"
//#include "usb_std.h"
/** @addtogroup USB_OTG_DRIVER
* @{
*/

/** @defgroup USB_DCD_INT 
* @brief This file contains the interrupt subroutines for the Device mode.
* @{
*/


/** @defgroup USB_DCD_INT_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_DCD_INT_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 



/** @defgroup USB_DCD_INT_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_DCD_INT_Private_Variables
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USB_DCD_INT_Private_FunctionPrototypes
* @{
*/ 
/* static functions */
static uint16_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);

/* Interrupt Handlers */
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint16_t ep_intr);
static uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint16_t ep_intr);
static uint32_t DCD_HandleSof_ISR(USB_OTG_CORE_HANDLE *pdev);

static uint32_t DCD_HandleRxStatusQueueLevel_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev , uint32_t epnum);

static uint32_t DCD_HandleUsbReset_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleEnumDone_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleResume_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleUSBSuspend_ISR(USB_OTG_CORE_HANDLE *pdev);

static uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev);
#ifdef VBUS_SENSING_ENABLED
static uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pdev);
#endif

/**
* @}
*/ 


/** @defgroup USB_DCD_INT_Private_Functions
* @{
*/ 

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED  
/**
* @brief  USBD_OTG_EP1OUT_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
  
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef  rxcsrl;
    USB_OTG_INTRRX_TypeDef  intr_rx;
    USB_OTG_INTRRXE_TypeDef intr_rxe; 
    USB_OTG_RXCOUNT_TypeDef rxcounter;
    
    intr_rx.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTX);
    intr_rx.d16&= USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTXE);
    rxcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[1]->TXCSRL);
    /* Transfer complete */
    if ( rxcsrl.b.rx_pkt_rdy )
    {
        if (pdev->cfg.dma_enable == 1)
        {
            rxcounter.d16 = USB_OTG_READ_REG16(&pdev->regs.CSRREGS[1]->RXCOUNT);
            /*ToDo : handle more than one single MPS size packet */
            pdev->dev.out_ep[1].xfer_count = rxcounter.d16;
        }    
        /* Inform upper layer: data ready */
        /* RX COMPLETE */
        USBD_DCD_INT_fops->DataOutStage(pdev , 1);
        /* Clear the bit in intr_rx for this interrupt */
        CLEAR_OUT_EP_INTR(1, rx_pkt_rdy);
    }

    if (rxcsrl.b.sent_stall)
    {
        CLEAR_OUT_EP_INTR(1, sent_stall);
    }
    if (rxcsrl.b.send_stall)
    {
        CLEAR_OUT_EP_INTR(1, send_stall);
    }
    if (rxcsrl.b.data_error)
    {
        CLEAR_OUT_EP_INTR(1, data_error);
    }
    if (rxcsrl.b.over_run)
    {
        CLEAR_OUT_EP_INTR(1, over_run);
    }
    
    return 1;
}

/**
* @brief  USBD_OTG_EP1IN_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
  
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef  txcsrl;
    USB_OTG_INTRTX_TypeDef  intr_tx;
    USB_OTG_INTRTXE_TypeDef intr_txe;
    USB_OTG_RXFIFOSZ_TypeDef txfifosz; 

    intr_tx.d16 = USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTX);
    intr_tx.d16&= USB_OTG_READ_REG16(&pdev->regs.COMMREGS->INTRTXE);
    txcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[1]->TXCSRL);

    if ( !txcsrl.b.tx_pkt_rdy )
    {
        CLEAR_IN_EP_INTR(1, tx_pkt_rdy);
        /* TX COMPLETE */
        USBD_DCD_INT_fops->DataInStage(pdev , 1);
    }
    if ( txcsrl.b.sent_stall )
    {
        CLEAR_IN_EP_INTR(1, sent_stall);
    }
    if (txcsrl.b.send_stall)
    {
        CLEAR_IN_EP_INTR(1, send_stall);
    }
    if (txcsrl.b.under_run)
    {
        CLEAR_IN_EP_INTR(1, under_run);
    }
    
    if (!txcsrl.b.fifo_not_empty)
    {
        DCD_WriteEmptyTxFifo(pdev , 1);
        CLEAR_IN_EP_INTR(1, fifo_not_empty);
    }
    return 1;
}
#endif
/**
* @brief  USBD_OTG_DMA_ISR_Handler
*         handles all USB DMA Interrupts
* @param  pdev: device instance
* @retval status
*/
uint32_t USBD_OTG_DMA_ISR_Handler (USB_OTG_CORE_HANDLE *pdev, uint8_t dma_intr_value)
{
    uint32_t retval = 0;
    USB_OTG_DMA_INTR_TypeDef dma_intr;
    USB_OTG_DMA_CNTL_TypeDef dma_cntl;
//    USB_OTG_DMA_COUNT_TypeDef dma_count;
    USB_OTG_DMA_Typedef usb_otg_dma_structure;
    #ifdef USE_DEVICE_MODE
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef rx_csrl;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef tx_csrl;
    #endif
//    #ifdef USE_HOST_MODE
//    USB_OTG_RXCSRL_IN_HOST_TypeDef rx_csrl;
//    USB_OTG_TXCSRL_IN_HOST_TypeDef tx_csrl;
//    #endif
    dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
    dma_intr.d8 = USB_OTG_READ_REG8(&pdev->regs.DMAREGS[0]->DMA_INTR);
//    dma_count.d32 = USB_OTG_READ_REG32(&pdev->regs.DMAREGS[0]->DMA_COUNT);
    switch (dma_intr_value & 0x01)
    {
        /* Channel 0 Interrup */
        case 1:
            if (dma_cntl.b.dma_mode)    /* Mode 1*/
            {
                if (dma_cntl.b.dma_dir)
                {
                    /* In Endpoint */
                    tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->TXCSRL);
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->TXCSRL, tx_csrl.d8);
                    if (pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len > pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count)
                    {
                        usb_otg_dma_structure.addr = *pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                      pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                        usb_otg_dma_structure.count = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                      pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                        pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len;
                        pdev->dev.in_ep[dma_cntl.b.dma_ep].rem_data_len = 0;
                        USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                    }else{
                        USB_OTG_DMA_Cmd(pdev,DISABLE);
                    }
                }else{
                    /* OUT Endpoint */
                    rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->RXCSRL);
                    rx_csrl.b.rx_pkt_rdy = 0;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->RXCSRL, rx_csrl.d8);
                    if (pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len > pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count)
                    {
                        usb_otg_dma_structure.addr = *pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                      pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                        usb_otg_dma_structure.count = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                      pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                        pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len;
                        pdev->dev.out_ep[dma_cntl.b.dma_ep].rem_data_len = 0;
                        USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                    }else{
                        USB_OTG_DMA_Cmd(pdev,DISABLE);
                    }
                }
            }else{                      /* mode 0 */
                if (dma_cntl.b.dma_dir)
                {
                    /* In Endpoint */
                    tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->TXCSRL);
                    tx_csrl.b.tx_pkt_rdy = 1;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->TXCSRL, tx_csrl.d8);
                    if (pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len > pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count)
                    {
                        if (pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len - pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count >= \
                            pdev->dev.in_ep[dma_cntl.b.dma_ep].maxpacket)
                        {
                            usb_otg_dma_structure.addr = *pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                          pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                            usb_otg_dma_structure.count = pdev->dev.in_ep[dma_cntl.b.dma_ep].maxpacket;
                            pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                                            pdev->dev.in_ep[dma_cntl.b.dma_ep].maxpacket;
                            pdev->dev.in_ep[dma_cntl.b.dma_ep].rem_data_len = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len -
                                                                              pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                            USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                        }else{
                            if (pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len - pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count > 0)
                            {
                                usb_otg_dma_structure.addr = *pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                              pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                                usb_otg_dma_structure.count = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                              pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count;
                                pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.in_ep[dma_cntl.b.dma_ep].xfer_len;
                                pdev->dev.in_ep[dma_cntl.b.dma_ep].rem_data_len = 0;
                                USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                            }else{
                                USB_OTG_DMA_Cmd(pdev,DISABLE);
                            }
                        }
                    }
                }else{
                    /* OUT Endpoint */
                    rx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->RXCSRL);
                    rx_csrl.b.rx_pkt_rdy = 0;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[dma_cntl.b.dma_ep]->RXCSRL, rx_csrl.d8);
                    if (pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len > pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count)
                    {
                        if (pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len - pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count >= \
                            pdev->dev.out_ep[dma_cntl.b.dma_ep].maxpacket)
                        {
                            usb_otg_dma_structure.addr = *pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                          pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                            usb_otg_dma_structure.count = pdev->dev.out_ep[dma_cntl.b.dma_ep].maxpacket;
                            pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                                            pdev->dev.out_ep[dma_cntl.b.dma_ep].maxpacket;
                            pdev->dev.out_ep[dma_cntl.b.dma_ep].rem_data_len = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len -
                                                                              pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                            USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                        }else{
                            if (pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len - pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count > 0)
                            {
                                usb_otg_dma_structure.addr = *pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_buff + \
                                                              pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                                usb_otg_dma_structure.count = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len - \
                                                              pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count;
                                pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_count = pdev->dev.out_ep[dma_cntl.b.dma_ep].xfer_len;
                                pdev->dev.out_ep[dma_cntl.b.dma_ep].rem_data_len = 0;
                                USB_OTG_DMA_DataTransferInt(pdev, &usb_otg_dma_structure);
                            }else{
                                USB_OTG_DMA_Cmd(pdev,DISABLE);
                            }
                        }
                    }
                }
            }
            break;
        default:
            dma_intr.d8 = 0x00;
            USB_OTG_WRITE_REG8(&pdev->regs.DMAREGS[0]->DMA_INTR,dma_intr.d8);
            break;
    }
    return retval;
}
/**
* @brief  USBD_OTG_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
uint8_t SetAddress_Flag = 0;
uint8_t Address_Value = 0;
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_INTRUSB_TypeDef  gintr_status;
    USB_OTG_INTRTX_TypeDef   intr_tx;
    USB_OTG_INTRRX_TypeDef   intr_rx;
    USB_OTG_CSR0L_IN_PERIPHERAL_TypeDef  csr0l;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef txcsrl;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef rxcsrl;
    uint32_t retval = 0;
    USB_OTG_EP *ep;
    uint8_t rx_DataLength = 0;
    if (USB_OTG_IsDeviceMode(pdev)) /* ensure that we are in device mode */
    {
        gintr_status.d8 = USB_OTG_ReadCoreItr(pdev);
		intr_rx.d16 = USB_OTG_ReadDevAllOutEp_itr(pdev);
		intr_tx.d16 = USB_OTG_ReadDevAllInEPItr(pdev);
        if ((!gintr_status.d8) && (!intr_rx.d16) &&(!intr_tx.d16))/* avoid spurious interrupt */
        {
            return 0;
        }

        if (gintr_status.b.resume)
        {
            retval |= DCD_HandleResume_ISR(pdev);
        }

        if (gintr_status.b.suspend)
        {
            retval |= DCD_HandleUSBSuspend_ISR(pdev);
        }
        if (gintr_status.b.sof)
        {
            retval |= DCD_HandleSof_ISR(pdev);
        }
        if (gintr_status.b.reset_babble)
        {
            retval |= DCD_HandleUsbReset_ISR(pdev);
        }  
        if (gintr_status.b.sess_req)
        {
            retval |= DCD_SessionRequest_ISR(pdev);
        }
        if (gintr_status.b.VBus_error)
        {
        }
        if (intr_tx.d16 & 0x01)
        {
            csr0l.d8 = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L);
            if (csr0l.b.sent_stall)
            {
                DCD_EP_ClrStall(pdev, 0x80);
            }
//            printf("CSR0L = 0X%0x\n", csr0l.d8);
            if (csr0l.b.setup_end)
            {
                csr0l.b.serviced_setupend = 1;
                USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
            }
            if (SetAddress_Flag)
            {
                pdev->regs.COMMREGS->FADDR = Address_Value;
                SetAddress_Flag = 0;
            }
            if (csr0l.b.rx_pkt_rdy)
            {
                /* Read Packet */
                rx_DataLength = USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->COUNT.COUNT0);
                if ((rx_DataLength < pdev->dev.out_ep[0].maxpacket) || \
                    (pdev->dev.out_ep[0].xfer_count + rx_DataLength == pdev->dev.out_ep[0].xfer_len))
                {
                    /* Copy the setup packet received in FIFO into the setup buffer in RAM */
                    USB_OTG_ReadPacket(pdev , 
                                       pdev->dev.out_ep[0].xfer_buff + pdev->dev.out_ep[0].xfer_count,
                                       0, 
                                       rx_DataLength);
                    pdev->dev.out_ep[0].xfer_count += USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->COUNT.COUNT0);
                    if (pdev->dev.out_ep[0].xfer_buff != pdev->dev.setup_packet)
                    {
                        if (pdev->dev.out_ep[0].xfer_len == 7)
                        {
                            csr0l.b.serviced_rxpktrdy = 1;
                            USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                            
                            pdev->dev.out_ep[0].xfer_buff = pdev->dev.setup_packet;
                            pdev->dev.out_ep[0].xfer_count = 0;
                            pdev->dev.out_ep[0].xfer_len = 8;
                            /* RX COMPLETE */
                            USBD_DCD_INT_fops->DataOutStage(pdev , 0);
                            pdev->dev.device_state = USB_OTG_EP0_STATUS_IN;
                            DCD_EP_Tx (pdev,
                                       0,
                                       NULL, 
                                       0); 
//                            /* RX COMPLETE */
//                            USBD_DCD_INT_fops->DataOutStage(pdev , 0);
                        }
                    }
                    else{
                        csr0l.b.serviced_rxpktrdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                        /* deal with receive setup packet */
                        USBD_DCD_INT_fops->SetupStage(pdev);
                    }
                }else{
                    /* Copy the setup packet received in FIFO into the setup buffer in RAM */
                    USB_OTG_ReadPacket(pdev , 
//                                       pdev->dev.setup_packet + pdev->dev.out_ep[0].xfer_count, 
                                       pdev->dev.out_ep[0].xfer_buff + pdev->dev.out_ep[0].xfer_count,
                                       0, 
                                       rx_DataLength);
                    pdev->dev.out_ep[0].xfer_count += USB_OTG_READ_REG8(&pdev->regs.INDEXREGS->COUNT.COUNT0);
//                    csr0l.b.serviced_rxpktrdy = 1;
//                    USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
                }
            }
            else if (!csr0l.b.tx_pkt_rdy)
            {
                if (pdev->dev.in_ep[0].xfer_count != pdev->dev.in_ep[0].xfer_len)
                {
                    if (pdev->dev.in_ep[0].xfer_len - pdev->dev.in_ep[0].xfer_count > pdev->dev.in_ep[0].maxpacket)
                    {
                        USB_OTG_WritePacket (pdev, 
                                             pdev->dev.in_ep[0].xfer_buff + pdev->dev.in_ep[0].xfer_count,
                                             0, 
                                             pdev->dev.in_ep[0].maxpacket);
                        pdev->dev.in_ep[0].xfer_count += pdev->dev.in_ep[0].maxpacket;
                        csr0l.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,csr0l.d8);
                    }else{
                        USB_OTG_WritePacket (pdev, 
                                             pdev->dev.in_ep[0].xfer_buff + pdev->dev.in_ep[0].xfer_count,  
                                             0,
                                             pdev->dev.in_ep[0].xfer_len - pdev->dev.in_ep[0].xfer_count);
                        pdev->dev.in_ep[0].xfer_count = pdev->dev.in_ep[0].xfer_len;
                        csr0l.b.tx_pkt_rdy = 1;
                        csr0l.b.data_end = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,csr0l.d8);
                    }
                    
                }
            }
        }
        if ((intr_rx.d16 & 0xFE))
        {
            retval |= DCD_HandleOutEP_ISR(pdev, intr_rx.d16 & 0xFE);
        }    

        if (intr_tx.d16 & 0xFE)
        {
            retval |= DCD_HandleInEP_ISR(pdev, intr_tx.d16 & 0xFE);
        }
    }
    return retval;
}
#ifdef VBUS_SENSING_ENABLED
/**
* @brief  DCD_SessionRequest_ISR
*         Indicates that the USB_OTG controller has detected a connection
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev)
{
//    USB_OTG_INTRUSB_TypeDef  intr_usb; 
//    USBD_DCD_INT_fops->DevConnected (pdev);

//    intr_usb.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSB);
//    /* Clear interrupt */
//    intr_usb.b.sess_req = 0;
//    USB_OTG_WRITE_REG8(&pdev->regs.COMMREGS->INTRUSB, intr_usb.d8);   
    return 1;
}

/**
* @brief  DCD_OTG_ISR
*         Indicates that the USB_OTG controller has detected an OTG event:
*                 used to detect the end of session i.e. disconnection
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pdev)
{
 
    return 1;
}
#endif
/**
* @brief  DCD_HandleResume_ISR
*         Indicates that the USB_OTG controller has detected a resume or
*                 remote Wake-up sequence
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleResume_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    if(pdev->cfg.low_power)
    {
        /* un-gate USB Core clock */
    }
    /* Inform upper layer by the Resume Event */
    USBD_DCD_INT_fops->Resume (pdev);

    return 1;
}

/**
* @brief  USB_OTG_HandleUSBSuspend_ISR
*         Indicates that SUSPEND state has been detected on the USB
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleUSBSuspend_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_INTRUSB_TypeDef  intr_usb;
    __IO uint8_t prev_status = 0;

    prev_status = pdev->dev.device_status;
    USBD_DCD_INT_fops->Suspend (pdev);      

    intr_usb.d8 = USB_OTG_READ_REG8(&pdev->regs.COMMREGS->INTRUSB);
    if((pdev->cfg.low_power) && (intr_usb.b.suspend == 1)  && 
       (pdev->dev.connection_status == 1) && 
       (prev_status  == USB_OTG_CONFIGURED))
    {
        /*  switch-off the clocks */
        
        /* Request to enter Sleep mode after exit from current ISR */
        SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
    }
    return 1;
}

/**
* @brief  DCD_HandleInEP_ISR
*         Indicates that an IN EP has a pending Interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint16_t ep_intr)
{
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef  txcsrl;
    USB_OTG_TXCSRH_IN_PERIPHERAL_TypeDef  txcsrh;   
    uint16_t diepint;
    uint16_t epnum = 0;
    USB_OTG_DMA_CNTL_TypeDef dma_cntl;
    while ( ep_intr )
    {
        if (ep_intr & 0x01) /* In ITR */
        {
//            diepint = DCD_ReadDevInEP(pdev , epnum); /* Get In ITR status */
            txcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRL);
            txcsrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRH);
            if ( !txcsrl.b.tx_pkt_rdy )
            {
                if (pdev->cfg.dma_enable == 1)
                {
                    dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
                    if ((USB_OTG_DEV_DMA_EP_NUM & 0x07) == epnum)
                    {
                        if (dma_cntl.b.dma_mode == USB_OTG_DMA_MODE1_TRANSFER)
                        {
                        
                        }else{
                            if (pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count >= pdev->dev.in_ep[epnum].maxpacket)
                            {
                                pdev->dev.in_ep[epnum].dma_addr = (uint32_t)&pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count;
                                USB_OTG_WritePacket (pdev, 
                                                     pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count,
                                                     epnum, 
                                                     pdev->dev.in_ep[epnum].maxpacket);
                                pdev->dev.in_ep[epnum].xfer_count += pdev->dev.in_ep[epnum].maxpacket;
                                pdev->dev.in_ep[epnum].rem_data_len = pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count;
                            }else{ 
                                if (pdev->dev.in_ep[epnum].rem_data_len > 0)
                                {
                                    pdev->dev.in_ep[epnum].rem_data_len = pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count;
                                    txcsrh.b.dma_req_enab = 0;
                                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRH,txcsrh.d8);
                                    pdev->dev.in_ep[epnum].xfer_count = pdev->dev.in_ep[epnum].xfer_len;
                                    pdev->dev.in_ep[epnum].rem_data_len = 0;
                                    USB_OTG_WritePacket (pdev, 
                                                         pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count,  
                                                         epnum,
                                                         pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count);
                                }
                                /* TX COMPLETE */
                                USBD_DCD_INT_fops->DataInStage(pdev , epnum);
                            }
                        }
                    }
                }   
                if ((!pdev->cfg.dma_enable) || 
                    ((USB_OTG_DEV_DMA_EP_NUM & 0x07) != epnum))
                {
                    if (pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count >= pdev->dev.in_ep[epnum].maxpacket)
                    {
                        USB_OTG_WritePacket (pdev, 
                                             pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count,
                                             epnum, 
                                             pdev->dev.in_ep[epnum].maxpacket);
                        pdev->dev.in_ep[epnum].xfer_count += pdev->dev.in_ep[epnum].maxpacket;
                        pdev->dev.in_ep[epnum].rem_data_len = pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count;
                        txcsrl.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,txcsrl.d8);
                    }else{
                        USB_OTG_WritePacket (pdev, 
                                             pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count,  
                                             epnum,
                                             pdev->dev.in_ep[epnum].xfer_len - pdev->dev.in_ep[epnum].xfer_count);
                        pdev->dev.in_ep[epnum].xfer_count = pdev->dev.in_ep[epnum].xfer_len;
                        pdev->dev.in_ep[epnum].rem_data_len = 0;
                        txcsrl.b.tx_pkt_rdy = 1;
                        USB_OTG_WRITE_REG8(&pdev->regs.INDEXREGS->CSRL.CSR0L,txcsrl.d8);
                        /* TX COMPLETE */
                        USBD_DCD_INT_fops->DataInStage(pdev , epnum);
                    }
                }        
            }
            if ( txcsrl.b.sent_stall )
            {
                CLEAR_IN_EP_INTR(epnum, sent_stall);
            }
            if (txcsrl.b.send_stall)
            {
                CLEAR_IN_EP_INTR(epnum, send_stall);
            }
            if (txcsrl.b.under_run)
            {
                CLEAR_IN_EP_INTR(epnum, under_run);
            }     
            if (!txcsrl.b.fifo_not_empty)
            {

                DCD_WriteEmptyTxFifo(pdev , epnum);
            }
        }
        epnum++;
        ep_intr >>= 1;
    }
    
    return 1;
}

/**
* @brief  DCD_HandleOutEP_ISR
*         Indicates that an OUT EP has a pending Interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint16_t ep_intr)
{
//    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
    USB_OTG_RXCSRL_IN_PERIPHERAL_TypeDef  rxcsrl;
    USB_OTG_RXCSRH_IN_PERIPHERAL_TypeDef  rxcsrh;
    USB_OTG_RXCOUNT_TypeDef  rx_count;
    uint32_t epnum = 1;
    uint16_t doepint = 0;
    ep_intr >>= 1;
    while ( ep_intr )
    {
        if (ep_intr & 0x1)
        {
            rxcsrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->RXCSRL);
            rxcsrh.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->RXCSRH);
            /* Transfer complete */
            if ( rxcsrl.b.rx_pkt_rdy )
            {
                /* Inform upper layer: data ready */
                rx_count.d16 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->RXCOUNT);
                if (rx_count.d16 >= pdev->dev.out_ep[epnum].maxpacket)
                {
                    USB_OTG_ReadPacket (pdev, 
                                        pdev->dev.out_ep[epnum].xfer_buff + pdev->dev.out_ep[epnum].xfer_count,
                                        epnum, 
                                        pdev->dev.out_ep[epnum].maxpacket);
                    pdev->dev.out_ep[epnum].xfer_count += pdev->dev.out_ep[epnum].maxpacket;
//                    rxcsrl.b.rx_pkt_rdy = 0;
//                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[epnum]->RXCSRL,rxcsrl.d8);
//                    if (pdev->dev.out_ep[epnum].xfer_count >= pdev->dev.out_ep[epnum].xfer_len)
//                    {
                        /* RX COMPLETE */
                        USBD_DCD_INT_fops->DataOutStage(pdev , epnum);
//                    }
                }
                else
                {
                    USB_OTG_ReadPacket (pdev, 
                                        pdev->dev.out_ep[epnum].xfer_buff + pdev->dev.out_ep[epnum].xfer_count,
                                        epnum, 
                                        rx_count.d16);
                    pdev->dev.out_ep[epnum].xfer_count += rx_count.d16;
                    if (pdev->dev.out_ep[epnum].xfer_len >=  pdev->dev.out_ep[epnum].xfer_count)
                    {
                        pdev->dev.out_ep[epnum].xfer_len = pdev->dev.out_ep[epnum].xfer_count;
                    }
                    else
                    {
                        pdev->dev.out_ep[epnum].xfer_count = pdev->dev.out_ep[epnum].xfer_len;
                    }
                    rxcsrl.b.rx_pkt_rdy = 0;
                    USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[epnum]->RXCSRL, rxcsrl.d8);
                    /* RX COMPLETE */
                    USBD_DCD_INT_fops->DataOutStage(pdev , epnum);
                }
                if (pdev->cfg.dma_enable == 1)
                {

                }   
            }
            /* Endpoint disable  */
            if ( rxcsrl.b.sent_stall )
            {
                /* Clear the bit in RXCSRL for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, sent_stall);
            }
            if (rxcsrl.b.send_stall)
            {
                /* Clear the bit in RXCSRL for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, send_stall);
            }
            if (rxcsrl.b.over_run)
            {
                /* Clear the bit in RXCSRL for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, over_run);
            }
            if (rxcsrl.b.data_error)
            {
                /* Clear the bit in RXCSRL for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, data_error);
            }
        }
        epnum++;
        ep_intr >>= 1;
    }
    return 1;
}

/**
* @brief  DCD_HandleSof_ISR
*         Handles the SOF Interrupts
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleSof_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USBD_DCD_INT_fops->SOF(pdev);
    return 1;
}

/**
* @brief  DCD_HandleRxStatusQueueLevel_ISR
*         Handles the Rx Status Queue Level Interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleRxStatusQueueLevel_ISR(USB_OTG_CORE_HANDLE *pdev)
{
//    USB_OTG_GINTMSK_TypeDef  int_mask;
//    USB_OTG_DRXSTS_TypeDef   status;
//    USB_OTG_EP *ep;

//    /* Disable the Rx Status Queue Level interrupt */
//    int_mask.d32 = 0;
//    int_mask.b.rxstsqlvl = 1;
//    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32, 0);

//    /* Get the Status from the top of the FIFO */
//    status.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP );

//    ep = &pdev->dev.out_ep[status.b.epnum];

//    switch (status.b.pktsts)
//    {
//        case STS_GOUT_NAK:
//            break;
//        case STS_DATA_UPDT:
//            if (status.b.bcnt)
//            {
//              USB_OTG_ReadPacket(pdev,ep->xfer_buff, status.b.bcnt);
//              ep->xfer_buff += status.b.bcnt;
//              ep->xfer_count += status.b.bcnt;
//            }
//            break;
//        case STS_XFER_COMP:
//            break;
//        case STS_SETUP_COMP:
//            break;
//        case STS_SETUP_UPDT:
//            /* Copy the setup packet received in FIFO into the setup buffer in RAM */
//            USB_OTG_ReadPacket(pdev , pdev->dev.setup_packet, 8);
//            ep->xfer_count += status.b.bcnt;
//            break;
//        default:
//            break;
//    }

//    /* Enable the Rx Status Queue Level interrupt */
//    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0, int_mask.d32);

    return 1;
}

/**
* @brief  DCD_WriteEmptyTxFifo
*         check FIFO for the next packet to be loaded
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev, uint32_t epnum)
{
    USB_OTG_EP *ep;
    USB_OTG_TXCSRL_IN_PERIPHERAL_TypeDef tx_csrl;
    uint32_t len = 0;
    uint32_t len32b;
    USB_OTG_DMA_CNTL_TypeDef dma_cntl;
    ep = &pdev->dev.in_ep[epnum];    

    len = ep->xfer_len - ep->xfer_count;

    if (len > ep->maxpacket)
    {
        len = ep->maxpacket;
    }

    len32b = (len + 3) / 4;
    
    while  (ep->xfer_count < ep->xfer_len &&
            ep->xfer_len != 0)
    {
        /* Write the FIFO */
        len = ep->xfer_len - ep->xfer_count;
        if (pdev->cfg.dma_enable == 1)
        {
            if ((USB_OTG_DEV_DMA_EP_NUM & 0x07) == epnum)
            {
                dma_cntl.d16 = USB_OTG_READ_REG16(&pdev->regs.DMAREGS[0]->DMA_CNTL);
                if(dma_cntl.b.dma_mode == USB_OTG_DMA_MODE1_TRANSFER)
                {
                
                }else{
                    if (len > ep->maxpacket)
                    {
                        len = ep->maxpacket;
                    }
                    pdev->dev.in_ep[epnum].dma_addr = (uint32_t)&pdev->dev.in_ep[epnum].xfer_buff + pdev->dev.in_ep[epnum].xfer_count;
                    USB_OTG_WritePacket (pdev , 
                                         ep->xfer_buff + ep->xfer_count, 
                                         epnum, 
                                         len);
                    ep->xfer_count += len;
                    ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                }
            }
        }
        if ((!pdev->cfg.dma_enable) || ((USB_OTG_DEV_DMA_EP_NUM & 0x07) != epnum))
        {
            if (len > ep->maxpacket)
            {
                len = ep->maxpacket;
            }
            len32b = (len + 3) / 4;
            tx_csrl.d8 = USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRL);
            USB_OTG_WritePacket (pdev , 
                                 ep->xfer_buff + ep->xfer_count, 
                                 epnum, 
                                 len);
            tx_csrl.b.tx_pkt_rdy = 1;
            USB_OTG_WRITE_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRL, tx_csrl.d8);
            ep->xfer_count += len;
        }
    }

    return 1;
}

/**
* @brief  DCD_HandleUsbReset_ISR
*         This interrupt occurs when a USB Reset is detected
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleUsbReset_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_FADDR_TypeDef faddr;
    USB_OTG_INTRUSB_TypeDef intr_usb;
    USB_OTG_INTRRX_TypeDef  intr_rx;
    USB_OTG_INTRTX_TypeDef  intr_tx;
    /* Flush the Tx FIFO */
    USB_OTG_FlushTxFifo(pdev ,  0 );

    /* Reset Device Address */
    faddr.d8 = USB_OTG_READ_REG8( &pdev->regs.COMMREGS->FADDR);
    faddr.b.func_addr = 0;
    USB_OTG_WRITE_REG8( &pdev->regs.COMMREGS->FADDR, faddr.d8);


    /* setup EP0 to receive SETUP packets */
    USB_OTG_EP0_OutStart(pdev);
    if (pdev->cfg.dma_enable)
    {
        /* set Tx FIFO size */
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[0]->TXMAXP, pdev->dev.in_ep[0].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[1]->TXMAXP, pdev->dev.in_ep[1].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[2]->TXMAXP, pdev->dev.in_ep[2].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[3]->TXMAXP, pdev->dev.in_ep[3].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[4]->TXMAXP, pdev->dev.in_ep[4].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[5]->TXMAXP, pdev->dev.in_ep[5].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[6]->TXMAXP, pdev->dev.in_ep[6].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[7]->TXMAXP, pdev->dev.in_ep[7].maxpacket);
        /* set Rx FIFO size */
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[0]->RXMAXP, pdev->dev.out_ep[0].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[1]->RXMAXP, pdev->dev.out_ep[1].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[2]->RXMAXP, pdev->dev.out_ep[2].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[3]->RXMAXP, pdev->dev.out_ep[3].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[4]->RXMAXP, pdev->dev.out_ep[4].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[5]->RXMAXP, pdev->dev.out_ep[5].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[6]->RXMAXP, pdev->dev.out_ep[6].maxpacket);
        USB_OTG_WRITE_REG16(&pdev->regs.CSRREGS[7]->RXMAXP, pdev->dev.out_ep[7].maxpacket);
    }
    /* Clear interrupt */
    intr_usb.d8 = 0;
    USB_OTG_WRITE_REG8 (&pdev->regs.COMMREGS->INTRUSB, intr_usb.d8);
    intr_rx.d16 = 0;
    USB_OTG_WRITE_REG16 (&pdev->regs.COMMREGS->INTRRX, intr_rx.d16);
    intr_tx.d16 = 0;
    USB_OTG_WRITE_REG16 (&pdev->regs.COMMREGS->INTRTX, intr_tx.d16);
    
    /*Reset internal state machine */
    USBD_DCD_INT_fops->Reset(pdev);
    return 1;
}

/**
* @brief  DCD_HandleEnumDone_ISR
*         Read the device status register and set the device speed
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleEnumDone_ISR(USB_OTG_CORE_HANDLE *pdev)
{
//    USB_OTG_GINTSTS_TypeDef  gintsts;
//    USB_OTG_GUSBCFG_TypeDef  gusbcfg;

//    USB_OTG_EP0Activate(pdev);

//    /* Set USB turn-around time based on device speed and PHY interface. */
//    gusbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);

//    /* Full or High speed */
//    if ( USB_OTG_GetDeviceSpeed(pdev) == USB_SPEED_HIGH)
//    {
//        pdev->cfg.speed            = USB_OTG_SPEED_HIGH;
//        pdev->cfg.mps              = USB_OTG_HS_MAX_PACKET_SIZE ;    
//        gusbcfg.b.usbtrdtim = 9;
//    }
//    else
//    {
//        pdev->cfg.speed            = USB_OTG_SPEED_FULL;
//        pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;  
//        gusbcfg.b.usbtrdtim = 5;
//    }

//    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, gusbcfg.d32);

//    /* Clear interrupt */
//    gintsts.d32 = 0;
//    gintsts.b.enumdone = 1;
//    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, gintsts.d32 );
    return 1;
}


/**
* @brief  DCD_IsoINIncomplete_ISR
*         handle the ISO IN incomplete interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
//    USB_OTG_GINTSTS_TypeDef gintsts;  

//    gintsts.d32 = 0;

//    USBD_DCD_INT_fops->IsoINIncomplete (pdev); 

//    /* Clear interrupt */
//    gintsts.b.incomplisoin = 1;
//    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    return 1;
}

/**
* @brief  DCD_IsoOUTIncomplete_ISR
*         handle the ISO OUT incomplete interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
//    USB_OTG_GINTSTS_TypeDef gintsts;  

//    gintsts.d32 = 0;

//    USBD_DCD_INT_fops->IsoOUTIncomplete (pdev); 

//    /* Clear interrupt */
//    gintsts.b.incomplisoout = 1;
//    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1;
}
/**
* @brief  DCD_ReadDevInEP
*         Reads ep flags
* @param  pdev: device instance
* @retval status
*/
static uint16_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    uint16_t v;
    v = ((USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRH) << 8) | \
         USB_OTG_READ_REG8(&pdev->regs.CSRREGS[epnum]->TXCSRL));
    return v;
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

/************************ (C) COPYRIGHT 2014 Megahuntmicro ****END OF FILE****/
