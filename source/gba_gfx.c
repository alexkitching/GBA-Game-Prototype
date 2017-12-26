#include "Intellisense.h"
#include "gba_gfx.h"
#include "sinlut.h"



ObjectAttribute obj_buffer[128] = {};
ObjectAffine *const obj_aff_buffer = (ObjectAffine*)obj_buffer;

void vsync()
{
	while (REG_VCOUNT >= 160);
	while (REG_VCOUNT < 160);
}

u16* vid_page = vid_page_back;

u16* page_flip()
{
	vid_page = (u16*)((u32)vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPLAYCONTROL ^= VIDEO_PAGE;	// update control register	

	return vid_page;
}

//function to return the memory address (pointer) to one of the four tile block locations (0-3)
u16* tile_block_address(u32 a_blockNum)
{
	//each block is 16KB in size
	return (u16*)(VRAM + (a_blockNum * TILE_BLOCK_SIZE));
}
//function to return a pointer to address space for the tile map blocks location (0-31)
u16* tileMap_block_address(u32 a_blockNum)
{
	//each tilemap block is 2KB in size
	return (u16*)(VRAM + (a_blockNum * TILEMAP_BLOCK_SIZE));
}

u16* sprite_tile_block_address(u32 a_tile_number)
{
	//each block is 16KB in size sprites occur on block 4 of tile memory (blocks 0-3 are for bg tiles)
	return (u16*)(VRAM + (4 * TILE_BLOCK_SIZE) + ((a_tile_number & 0x3FF) * sizeof(TILE) ));

}
//Function to set BG Control Register
void setBG_Control_Register(u8 a_regNum, u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize)
{
	vu16 control = (a_priority & BGCNT_PRIORITY_MASK) | ((a_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_mosaic & BGCNT_MOS_MASK) << 6) | ((a_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_bgSize & BGCNT_SIZE_MASK) << 14);
	REG_BGCNT[a_regNum] = control;
}

u16* palette_bg_block_address(u32 a_blockNum)
{
	//the multiply 16 is due to each palette block having 16 entries per row for a total of 256 colours
	return (u16*)(pal_bg_mem + (a_blockNum * 16));
}

u16* palette_sp_block_address(u32 a_blockNum)
{
	//the multiply 16 is due to each palette block having 16 entries per row for a total of 256 colours
	return (u16*)(pal_sp_mem + (a_blockNum * 16));
}
//====================================================================================
//Sprites & OAM
//====================================================================================

//functionality to set attribute 0 properties
u16 SetAttrib0(u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosiac, u8 a_colorMode, u8 a_shape)
{
	u16 attrib0 = (a_y & A0_YMASK) | ((a_objectMode & 0x3) << 8) | ((a_gfxMode & 0x3) << 10) |
		((a_mosiac & 0x1) << 12) | ((a_colorMode & 0x1) << 13) | ((a_shape & 0x3) << 14);
	return attrib0;
}
//functionality to set attribute 1 properties
u16 SetAttrib1(u8 a_x, u8 a_flip, u8 a_size)
{
	u16 attrib1 = (a_x & A1_XMASK) | ((a_flip & A1_FLIPMASK) << 12) | ((a_size & 3) << 14);
	return attrib1;
}

void SetAttrib1AffineIndex(ObjectAttribute* a_object, u8 a_index)
{
	//use 0xC1FF to strip out bits 9 -> D
	a_object->attr1 = (a_object->attr1 & 0xC1FF) | ((a_index & A1_AFF_INDEX_MASK) << 9);
}

//functionality to set attribute 2 properties
u16 SetAttrib2(u16 a_tileIndex, u8 a_priority, u8 a_paletteBank)
{
	u16 attrib2 = (a_tileIndex & 0xFF) | ((a_priority & 3) << 10) | ((a_paletteBank & 0xF) << 12);
	return attrib2;
}
//functionality to set position properties for an object
void SetObjectPosition(ObjectAttribute* a_object, u8 a_x, u8 a_y)
{
	a_object->attr0 = (a_object->attr0 & 0xFF00) | (a_y & 0x00FF);
	a_object->attr1 = (a_object->attr1 & 0xFE00) | (a_x & 0x01FF);
}

void obj_hide(ObjectAttribute* a_obj)
{
	a_obj->attr0 = (a_obj->attr0 & A0_MODE_MASK) | (A0_MODE_HIDE << 8);
}

//! Unhide an object.
void obj_unhide(ObjectAttribute* a_obj, u8 mode)
{
	a_obj->attr0 = (a_obj->attr0 & A0_MODE_MASK) | ((mode& 0x3) << 8);
}

void oam_init(ObjectAttribute* obj, u8 count)
{
	u32 nn = count;
	ObjectAttribute *dst = obj;

	// Hide each object
	while (nn--)
	{
		dst->attr0	= 0;
		dst->attr1	= 0;
		dst->attr2	= 0;
		dst->pad	= 0;
		obj_hide(dst);
		++dst;
	}
	// init oam
	oam_copy(MEM_OAM, obj, count);
}

void oam_copy( ObjectAttribute* a_dst, ObjectAttribute* a_src, u8 a_count)
{

	u32 *dstw = (u32*)a_dst, *srcw = (u32*)a_src;
	while (a_count--)
	{
		*dstw++ = *srcw++;
		*dstw++ = *srcw++;
	}

}

//set the affine object matrix to an identity matrix
void obj_aff_identity(ObjectAffine* a_obj)
{
	a_obj->pa = 0x01001;		a_obj->pb = 0;
	a_obj->pc = 0;			a_obj->pd = 0x0100;
}

//set the rotation and scale of the object 
//uses the sine look up table to retrieve a value for sine & cosine
void obj_aff_rotscale(ObjectAffine* oaff, fixed sx, fixed sy, u16 alpha)
{
	int ss = lu_sin(alpha), cc = lu_cos(alpha);
	//shifts values down 12 to account for fixed point multiply to keep values as .8 fixed
	oaff->pa = cc*sx >> 12;	oaff->pb = -ss*sx >> 12;
	oaff->pc = ss*sy >> 12;	oaff->pd = cc*sy >> 12;
}