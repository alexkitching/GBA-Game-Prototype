#ifndef __GBA_GFX_H__
#define __GBA_GFX_H__
#include "Intellisense.h"

#include "gba_reg.h"
#include "gba_types.h"
#include "gba_math.h"


//base address pointer for base register
#define REG_DISPLAYCONTROL *((vu32*)(REG_BASE))

//Defines for settng up different video modes
#define VIDEOMODE_0 0x0000  //Sprite Mode 0
#define VIDEOMODE_1 0x0001  //Sprite Mode 1
#define VIDEOMODE_2 0x0002  //Sprite Mode 2
#define VIDEOMODE_3 0x0003	// < Mode 3; 240x160 @16bpp
#define VIDEOMODE_4	0x0004	// < Mode 4; 240x160 @8 bpp
#define VIDEOMODE_5 0x0005  // < Mode 5; 

//Defines for enabling different backgrounds
#define BGMODE_0	0x0100
#define BGMODE_1	0x0200
#define BGMODE_2	0x0400
#define BGMODE_3	0x0800

//vcount is used for testing for vertical blank
#define REG_VCOUNT (*(vu16*)(REG_BASE + 0x06))

extern void vsync();
//=======================================================================
//Functionality for setting up page flipping in bitmap rendering modes
#define VIDEO_PAGE			0x0010	//!< Page indicator

#define VRAM_PAGE_SIZE		0x0A000
#define VRAM_BACK	(VRAM+ VRAM_PAGE_SIZE)

#define vid_mem			((u16*)VRAM)
#define vid_page_front	((u16*)VRAM)
#define vid_page_back	((u16*)VRAM_BACK)

extern u16* vid_page;
extern u16* page_flip();
//=======================================================================

//=======================================================================
//Control Register defines for background tiled rendering
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040

//define a base registry address for the background controllers
//accessing this as REG_BGCNT[0] is controller 0
//accessing this as REG_BGCNT[1] is controller 1 or memory address REG_BGCNT + (sizeof(vu16) * 1) = 0x0400:000A
//accessing this as REG_BGCNT[2] is controller 2 or memory address REG_BGCNT + (sizeof(vu16) * 2) = 0x0400:000C
//accessing this as REG_BGCNT[3] is controller 3 or memory address REG_BGCNT + (sizeof(vu16) * 3) = 0x0400:000E
#define REG_BGCNT ((vu16*)(REG_BASE+0x0008))

//structure to control background offset x & y flags
typedef struct BackgroundOffsets
{
	s16 x, y;
}__attribute__((packed, aligned(4))) BGOffset;
//set up define for REG_BG_OFS accessed as an array to access offset data for bg 0-3 
//REG_BG_OFS[1].x accesses x .y accesses y value
#define REG_BG_OFS  ((BGOffset*)(REG_BASE+0x0010))

#define TILE_BLOCK_SIZE 0x4000
#define TILEMAP_BLOCK_SIZE 0x0800

#define BGCNT_PRIORITY_MASK 0x3
#define BGCNT_TBID_MASK		0x3
#define BGCNT_MOS_MASK		0x1
#define BGCNT_COLOR_MASK	0x1
#define BGCNT_TMID_MASK		0x1F
#define BGCNT_AFW_MASK		0x1
#define BGCNT_SIZE_MASK		0x3

#define BG_REG_SIZE_32x32 0x0
#define BG_REG_SIZE_64x32 0x1
#define BG_REG_SIZE_32x64 0x2
#define BG_REG_SIZE_64x64 0x3

//function to return the memory address (pointer) to one of the four tile block locations (0-3)
extern u16* tile_block_address(u32 a_blockNum);
//function to return a pointer to address space for the tile map blocks location (0-31)
extern u16* tileMap_block_address(u32 a_blockNum);
//function to set a background control register's values
extern void setBG_Control_Register(u8 a_regNum, u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize);
//====================================================================================
//defines for palette locations
//====================================================================================
// --- memmap ---
#define MEM_PALETTE		0x05000000
#define pal_bg_mem		((u16*)MEM_PALETTE)
#define pal_sp_mem		((u16*)(MEM_PALETTE + 0x200))

extern u16* palette_bg_block_address(u32 a_blockNum);
extern u16* palette_sp_block_address(u32 a_blockNum);
//====================================================================================
//defines and functionality for Sprites & OAM
//====================================================================================
//Defines and structures to map tiles in to memory blocks
typedef struct TILE { u32 data[8]; }__attribute__((packed, aligned(4)))TILE, TILE4;
typedef struct TILE8 { u32 data[16]; }__attribute__((packed, aligned(4)))TILE8;

typedef TILE TILEBLOCK[512];
typedef TILE8 TILEBLOCK8[256];

#define tile_mem ((TILEBLOCK*)0x06000000)
#define tile8_mem ((TILEBLOCK8*)0x06000000)

extern u16* sprite_tile_block_address(u32 a_tile_number);

#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040

typedef struct ObjectAttribute {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 pad;
}__attribute__((packed, aligned(4)))ObjectAttribute;

typedef struct ObjectAffine {
	u16 fill0[3];
	s16 pa;
	u16 fill1[3];
	s16 pb;
	u16 fill2[3];
	s16 pc;
	u16 fill3[3];
	s16 pd;

}__attribute__((packed, aligned(4)))ObjectAffine;

extern ObjectAttribute obj_buffer[128];
extern ObjectAffine *const obj_aff_buffer;

#define MEM_OAM ((ObjectAttribute*) 0x07000000)
#define MEM_AFF_OAM ((ObjectAffine*) 0x07000000)

#define A0_YMASK 0xFF
#define A0_MODE_MASK 0xFCFF
//defines for Object mode - affine or regular, or double size affine
#define A0_MODE_REG		0x00
#define A0_MODE_AFF		0x01
#define A0_MODE_HIDE	0x02
#define A0_MODE_AFF_DBL 0x03
//colour mode defines
#define A0_4BPP 0x0
#define A0_8BPP 0x1
//sprite shape
#define A0_SQUARE 0x00
#define A0_WIDE   0x01
#define A0_TALL   0x02
//x position mask
#define A1_XMASK 0x1FF
#define A1_FLIPMASK 0x3
#define A1_HFLIP 1 << 12
#define A1_VFLIP 1 << 13
// Sprite Sizes
#define A1_SIZE_0 0x00 // 8x8
#define A1_SIZE_1 0x01 // 16x16
#define A1_SIZE_2 0x02 // 32x32	
#define A1_SIZE_3 0x03 // 64x64
#define A1_AFF_INDEX_MASK 0x1F

extern u16 SetAttrib0(u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosiac, u8 a_colorMode, u8 a_shape);
extern u16 SetAttrib1(u8 a_x, u8 a_flip, u8 a_size);
extern void SetAttrib1AffineIndex(ObjectAttribute* a_object, u8 a_index);
extern u16 SetAttrib2(u16 a_tileIndex, u8 a_priority, u8 a_paletteBank);
extern void SetObjectPosition(ObjectAttribute* a_object, u8 a_x, u8 a_y);
//! Hide an object.
extern void obj_hide(ObjectAttribute* a_obj);
//! Unhide an object.
extern void obj_unhide(ObjectAttribute* a_obj, u8 a_mode);

extern void oam_init(ObjectAttribute* a_obj, u8 a_count);
extern void oam_copy(ObjectAttribute* a_dst, ObjectAttribute* a_src, u8 a_count);

extern void obj_aff_identity(ObjectAffine* a_obj);
extern void obj_aff_rotscale(ObjectAffine* oaff, fixed sx, fixed sy, u16 alpha);
#endif //__GBA_GFX_H__