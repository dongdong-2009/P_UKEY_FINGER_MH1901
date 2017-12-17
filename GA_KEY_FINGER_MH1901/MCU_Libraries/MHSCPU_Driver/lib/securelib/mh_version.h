/**
 * \file version.h
 *
 */

#ifndef __MH_VERSION_H
#define __MH_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
  * @method	mh_get_version
  * @brief	Get the version number.
  * @param	void
  * @retval The constructed version number in the format:MMNNPP00 (Major, Minor, Patch).
  */
uint32_t mh_get_version( void );

#ifdef __cplusplus
}
#endif

#endif /* version.h */
