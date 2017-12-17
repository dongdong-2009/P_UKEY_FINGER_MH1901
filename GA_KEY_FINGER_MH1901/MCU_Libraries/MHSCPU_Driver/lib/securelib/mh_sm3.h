/* mh_sm3.h - header file for sm3.c
 */

#ifndef __MH_SM3_H
#define __MH_SM3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>


#define MH_RET_SM3_BUSY				(('R'<<24)|('S'<<16)|('3'<<8)|('B'))
#define MH_RET_SM3_SUCCESS			(('R'<<24)|('S'<<16)|('3'<<8)|('S'))

/**
  * @method	mh_sm3
  * @brief	SM3 Hash Check
  * @param	output		:output data buffer
  * @param	input		:input data buffer
  * @param	ibytes		:size of input data
  * @retval 			:SM3_SUCCESS or SM3_BUSY
  */
uint32_t mh_sm3(uint8_t output[32], uint8_t *input, uint64_t ibytes);	

	
#ifdef __cplusplus
}
#endif

#endif
