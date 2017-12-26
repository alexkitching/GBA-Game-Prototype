#include "gba_types.h"

#ifndef __GBA_DMA_H__
#define __GBA_DMA_H__

typedef struct DMA_REC
{
	const void *src;
	void *dest;
	u32 cnt;
} DMA_REC;

#define REG_DMA ((volatile DMA_REC*) 0x040000B0)
#define DMA_SRC_FIXED	0x01000000	//!< Fixed source address
#define DMA_32			0x04000000	//!< Transfer by word
#define DMA_AT_NOW				 0	//!< Start transfer now
#define DMA_ENABLE		0x80000000	//!< Enable DMA
#define DMA_NOW		(DMA_ENABLE  | DMA_AT_NOW)
#define DMA_CPY32	(DMA_NOW | DMA_32)
#define DMA_FILL32	(DMA_NOW | DMA_SRC_FIXED | DMA_32)

// DMA Transfer Macro
#define DMA_TRANSFER(_dest, _src, count, ch, mode) \
do												   \
{											       \
REG_DMA[ch].cnt= 0;								   \
REG_DMA[ch].src= (const void*)(_src);              \
REG_DMA[ch].dest= (void*) (_dest);                 \
REG_DMA[ch].cnt = (count) | (mode);                \
} while(0)

// DMA Copier
__attribute__((always_inline)) inline void dma_cpy(void* a_dest, const void* a_src, u32 a_count, u32 a_channel, u32 a_mode)
{
	REG_DMA[a_channel].cnt = 0; // Shut off any previous transfer
	REG_DMA[a_channel].src = a_src;
	REG_DMA[a_channel].dest = a_dest;
	REG_DMA[a_channel].cnt = a_mode | a_count;
}
// DMA Fill Routine
__attribute__((always_inline)) inline void dma_fill(void* a_dest, volatile u32 a_src, u32 a_count, u32 a_channel, u32 a_mode)
{
	REG_DMA[a_channel].cnt = 0; // shut off any previous transfer
	REG_DMA[a_channel].src = (const void*)&a_src;
	REG_DMA[a_channel].dest = a_dest;
	REG_DMA[a_channel].cnt = a_count | a_mode | DMA_SRC_FIXED;
}
// Word copy using DMA 3
__attribute__((always_inline)) inline void dma3_cpy(void* a_dest, const void* a_src, u32 a_size)
{
	dma_cpy(a_dest, a_src, a_size / 4, 3, DMA_CPY32);
}
// Word fill using DMA 3
__attribute__((always_inline)) inline void dma3_fill(void* a_dest, volatile u32 a_src, u32 a_size)
{
	dma_fill(a_dest, a_src, a_size / 4, 3, DMA_FILL32);
}
#endif // !__GBA_DMA_H__
