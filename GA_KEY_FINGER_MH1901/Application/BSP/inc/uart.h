/*
 * UART0 Operations
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for UART0 Function Declars and MACRO Defines
 */


#ifndef _UART_H
#define _UART_H

#include <base_defines.h>
#include <board.h>

#define USART_REC_LEN  		        256
#define RX_MASK						0x00FF
#define	RX_BUF_F					0x01   
#define RX_BUF_E					0x02   


#define UART_BAUDRATE_115200  (115200UL)
#define UART0_BAUDRATE         UART_BAUDRATE_115200




typedef struct
{
	UINT8	rx_buf[USART_REC_LEN];        
	UINT16	rx_seek;                 
	UINT16	rx_size;                 
	UINT8	rx_stat;                     
}USARTypDef;



void debugPrint(const char *fmt, ...);
void uartInit(U32 baudrate);
S32  uartCheckData(void);
U8   uartReadByte(S32 *ch);
void uartSendBuff(U8 *dat, U32 len);
S32  uartSendBuffEmpty(void);



#endif /* End of _UART_H */
