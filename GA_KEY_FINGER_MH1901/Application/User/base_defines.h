/*
 * Base Data Type Defines
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    08/31/17     Create this file for base data type defines
 */

#ifndef _BASE_DEFINES_H
#define _BASE_DEFINES_H

#include <stdint.h>

typedef char            BOOL;     

#ifndef CHAR
typedef signed char CHAR;
#endif

#ifndef INT8
typedef signed char INT8;
#endif

#ifndef INT8U
typedef unsigned char INT8U;
#endif

#ifndef UINT8
typedef unsigned char UINT8;
#endif


#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef INT
typedef signed int INT;
#endif

#ifndef INT32
typedef signed int INT32;
#endif

#ifndef UINT 
typedef unsigned int UINT;
#endif

#ifndef UINT32 
typedef unsigned int UINT32;
#endif

#ifndef UINT16 
typedef unsigned short UINT16;
#endif

#ifndef INT16 
typedef signed short INT16;
#endif

#ifndef U8 
typedef unsigned char	U8;
#endif

#ifndef U16 
typedef unsigned short	U16;
#endif

#ifndef U32 
typedef unsigned int	U32;
#endif


#ifndef S8 
typedef signed char		S8;
#endif

#ifndef S16 
typedef signed short	S16;
#endif

#ifndef S32 
typedef signed int		S32;
#endif



typedef uint32_t	 register_t;






#define            DEF_NULL     (void *)0


/* ----------------- BOOLEAN DEFINES ------------------ */
#define  DEF_FALSE                                         0
#define  DEF_TRUE                                          1

#define  DEF_NO                                            0
#define  DEF_YES                                           1

#define  DEF_DISABLED                                      0
#define  DEF_ENABLED                                       1

#define  DEF_INACTIVE                                      0
#define  DEF_ACTIVE                                        1

#define  DEF_OFF                                           0
#define  DEF_ON                                            1

#define  DEF_CLR                                           0
#define  DEF_SET                                           1

#define  DEF_FAIL                                          0
#define  DEF_OK                                            1


#define  DEF_LOW                                           0
#define  DEF_HIGH                                          1



/* ------------------- BIT DEFINES -------------------- */
#define  DEF_BIT_NONE                                   0x00u

#define  DEF_BIT_00                                     0x01u
#define  DEF_BIT_01                                     0x02u
#define  DEF_BIT_02                                     0x04u
#define  DEF_BIT_03                                     0x08u
#define  DEF_BIT_04                                     0x10u
#define  DEF_BIT_05                                     0x20u
#define  DEF_BIT_06                                     0x40u
#define  DEF_BIT_07                                     0x80u

#define  DEF_BIT_08                                   0x0100u
#define  DEF_BIT_09                                   0x0200u
#define  DEF_BIT_10                                   0x0400u
#define  DEF_BIT_11                                   0x0800u
#define  DEF_BIT_12                                   0x1000u
#define  DEF_BIT_13                                   0x2000u
#define  DEF_BIT_14                                   0x4000u
#define  DEF_BIT_15                                   0x8000u

#define  DEF_BIT_16                               0x00010000u
#define  DEF_BIT_17                               0x00020000u
#define  DEF_BIT_18                               0x00040000u
#define  DEF_BIT_19                               0x00080000u
#define  DEF_BIT_20                               0x00100000u
#define  DEF_BIT_21                               0x00200000u
#define  DEF_BIT_22                               0x00400000u
#define  DEF_BIT_23                               0x00800000u

#define  DEF_BIT_24                               0x01000000u
#define  DEF_BIT_25                               0x02000000u
#define  DEF_BIT_26                               0x04000000u
#define  DEF_BIT_27                               0x08000000u
#define  DEF_BIT_28                               0x10000000u
#define  DEF_BIT_29                               0x20000000u
#define  DEF_BIT_30                               0x40000000u
#define  DEF_BIT_31                               0x80000000u

#define  DEF_BIT(bit)   (1u << (bit))        /* Shift to indicate bit */

/* Byte Mask Definitions */
#define BYTE0_MSK              (0x000000FF)         
#define BYTE1_MSK              (0x0000FF00)         
#define BYTE2_MSK              (0x00FF0000)         
#define BYTE3_MSK              (0xFF000000)         

#define GET_BYTE0(x)    ((x & BYTE0_Msk)      )  /* Extract Byte 0 (Bit  0~ 7) from parameter u32Param */
#define GET_BYTE1(x)    ((x & BYTE1_Msk) >>  8)  /* Extract Byte 1 (Bit  8~15) from parameter u32Param */
#define GET_BYTE2(x)    ((x & BYTE2_Msk) >> 16)  /* Extract Byte 2 (Bit 16~23) from parameter u32Param */
#define GET_BYTE3(x)    ((x & BYTE3_Msk) >> 24)  /* Extract Byte 3 (Bit 24~31) from parameter u32Param */


/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outpw(port,value)     *((volatile register_t *)(port)) = value

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inpw(port)            (*((volatile register_t *)(port)))



#endif /* End of _BASE_DEFINES_H include file */
