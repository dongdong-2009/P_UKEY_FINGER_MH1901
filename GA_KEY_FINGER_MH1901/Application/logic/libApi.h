/*
 * Functions interface of bin file
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/29/17     For customer call the functions of bin file
 */



#ifndef __FSMAPI_H
#define __FSMAPI_H



#define LIB_PS_OK             0x00
#define LIB_NO_ERROR          0x01     
#define LIB_ADDRESS_OVER      0x02     
#define LIB_DUL_ENROLL_ERROR  0x03     
#define LIB_TIMEOUT           0x04     
#define LIB_ENROLL_ERR        0x05     
#define LIB_GET_IMG_ERR       0x06     
#define LIB_PARAM_ERROR       0x07     


typedef enum
{
	LIB_READ_ID_LIST=0x01,
	LIB_PRE_ENEOLL=0x02,
	LIB_PRE_MATCH=0x03,
	LIB_ENROLL =0x04,
	LIB_MATCH=0x05,
	LIB_DELETE_ID=0x06,
	LIB_IS_FINGERLEAVE=0x07,
	LIB_FRAME_MAX
}_Lib_Cmd;


#define FUNC_TABLE_START	(0x01033000UL)
#define FUNC(i)				(*(unsigned int *)(FUNC_TABLE_START + (i) * 4))


typedef int           (*lib_init_t)(void);
typedef unsigned char (*lib_api_t)(unsigned char arg);
typedef void          (*timer_isr_t)(void);
typedef void          (*debug_print_en_t)(unsigned char en);


#define lib_init			((lib_init_t)FUNC(7))
#define timer0_isr			((timer_isr_t)FUNC(9))
#define lib_debug_print_en	((debug_print_en_t)FUNC(10))
#define lib_api				((lib_api_t)FUNC(8))

#endif
