/*
 * UART0 Operations
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for UART0 Function Defines
 */


#include <board.h>
#include <stdio.h>
#include <stdarg.h>







/*
 * Init the uart0
 * @baudrate  baudrate of uart0
 */

void uartInit(UINT32 baudrate)
{
	UART_InitTypeDef UART_InitStructure;
	
	GPIO_PinRemapConfig(GPIOA, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Remap_0);
	
	UART_InitStructure.UART_BaudRate = baudrate;
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	UART_InitStructure.UART_Parity = UART_Parity_No;

	UART_Init(UART0, &UART_InitStructure);
	UART_ITConfig(UART0, UART_IT_RX_RECVD, ENABLE);

}













/*
 * Transmit the data
 * @dat, a pointer for external buffer,ready to send 
 * @len,  the length for send from dat
 */
void uartSendBuff(UINT8 *dat, UINT32 len)
{
    UART_TypeDef * UARTx  = UART0;

	while(len--){ 
	    UARTx->OFFSET_0.THR = (*dat++) & 0xFF;
        while (!UART_IsTXEmpty(UARTx));
    }
}


/*
 * See if the UART available to transmit
 * @1,available to transmit.0,unavailable to transmit
 */
INT32  uartSendBuffEmpty(void)
{
    UART_TypeDef * UARTx  = UART0;

    return UART_IsTXEmpty(UARTx);
}


/*
 * Print the log
 * @fmt,the variable parameter
 * @None return value
 */
void debugPrint(const char *fmt, ...)
{
	INT32 ret;
	UINT8 tmp_buf[512];

	va_list args;
	va_start(args, fmt);
	ret = vsprintf((char *)tmp_buf, fmt, args);
	va_end(args);

	uartSendBuff(tmp_buf,ret);
}



void HAL_UART_SendData(unsigned char *data, unsigned short len)
{
	uartSendBuff(data,len);	
}




void HAL_UART_SEND_ASC(unsigned char* pInData,unsigned short len)
{	
	unsigned char temp[4];
	unsigned char m;
	unsigned short i,len1;
	
	len1=2*len;
	for(i=0;i<len;i++)
	{
		m=(pInData[i]>>4)&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[0]=m;
		m=pInData[i]&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[1]=m;
		temp[2]=' ';
		HAL_UART_SendData(temp,3);
	}
	temp[0]=0x0a;
	HAL_UART_SendData(temp,1);
}




void UART_printf(char *fmt,...)
{

	unsigned char temp[256];
	
	va_list arg_ptr;
//	unsigned char *string=pUARTData;
	unsigned char *string=temp;	
	va_start(arg_ptr, fmt);  
	vsprintf((char *)string,fmt,arg_ptr); 
    	HAL_UART_SendData(string,strlen((char *)string));
	va_end(arg_ptr);
}



