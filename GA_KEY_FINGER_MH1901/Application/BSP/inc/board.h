/*
 * AFS121N Functions and MACROs Define
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/04/17     Include the .h file for the entire board
 */

#ifndef _BOARD_H
#define _BOARD_H

#include <base_defines.h>
#include <mhscpu.h>
#include <mhscpu_timer.h>
#include <timer.h>
#include <mhscpu_flash.h>
#include <mhscpu_uart.h>
#include <uart.h>
#include <mhscpu_exti.h>


#define inline __inline


void hardwareInit(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void init_clock(void);
void init_all_peripheral_devices(void);






#endif /* Endof _BOARD_H */
