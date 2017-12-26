//---------------------------------------------------------------------------------
// This is a small header file used to define types that are used for GBA development
// these types are named according to their signedness and their bit length
// u8 represents an unsigned integer that is 8 bits in size, this would be an unsigned
// char on 0x86 systems. 
// These are defined like this for simplicity and to keep code in appropriate spaces
//---------------------------------------------------------------------------------
#pragma once

#ifndef __GBA_TYPES_H__
#define __GBA_TYPES_H__



#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------------------
// Data	types
//---------------------------------------------------------------------------------
typedef	uint8_t			u8;		/**< Unsigned 8 bit value	*/
typedef	uint16_t		u16;	/**< Unsigned 16 bit value	*/
typedef	uint32_t		u32;	/**< Unsigned 32 bit value	*/
typedef	int8_t			s8;		/**< Signed 8 bit value	*/
typedef	int16_t			s16;	/**< Signed 16 bit value	*/
typedef	int32_t			s32;	/**< Signed 32 bit value	*/

typedef	volatile u8		vu8;	/**< volatile Unsigned 8 bit value	*/
typedef	volatile u16	vu16;	/**< volatile Unigned 16 bit value	*/
typedef	volatile u32	vu32;	/**< volatile Unsigned 32 bit value	*/
typedef	volatile s8		vs8;	/**< volatile Signed 8 bit value	*/
typedef	volatile s16	vs16;	/**< volatile Signed 8 bit value	*/
typedef	volatile s32	vs32;	/**< volatile Signed 8 bit value	*/


#endif //__GBA_TYPES_H__
