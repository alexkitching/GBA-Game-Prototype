//---------------------------------------------------------------------------------
// This header file outlines the registers for GBA Key Input and provides functionality
// for handling key presses
//
//---------------------------------------------------------------------------------

#include "gba_reg.h"
#include "gba_types.h"

#ifndef __GBA_INPUT_H__
#define __GBA_INPUT_H__
/*
* Define for Hardware Key Input Register
*/
#define REG_KEYINPUT	*(vu16*)(REG_BASE + 0x130)
/*
* Define for Hardware Key Interrupt Control Register
*/
#define REG_KEYCNT		*(vu16*)(REG_BASE + 0x132)

typedef enum KEYS
{
	A = (1 << 0),
	B = (1 << 1),
	SELECT = (1 << 2),
	START = (1 << 3),
	RIGHT = (1 << 4),
	LEFT = (1 << 5),
	UP = (1 << 6),
	DOWN = (1 << 7),
	R = (1 << 8),
	L = (1 << 9),

	KEYIRQ_ENABLE = (1 << 14),	/*!< Enable keypad interrupt */
	KEYIRQ_OR = (0 << 15),		/*!< interrupt logical OR mode */
	KEYIRQ_AND = (1 << 15),		/*!< interrupt logical AND mode */


}KEYS;

#define KEY_MASK 0x03FF
//Vars to hold the current and previous state of the hardware keys
extern u16 __currKeys, __prevKeys;
//Function to test for Hardware Key Depress
__attribute__((always_inline)) inline void PollKeys()
{
	__prevKeys = __currKeys;
	__currKeys = ~REG_KEYINPUT & KEY_MASK;
}

__attribute__((always_inline)) inline u16		currentKeyState() { return __currKeys; }
__attribute__((always_inline)) inline u16		prevKeyState() { return __prevKeys; }
__attribute__((always_inline)) inline u32		keyDown(u32 a_key) { return __currKeys & a_key; }
__attribute__((always_inline)) inline u32		keyUp(u32 a_key) { return ~__currKeys & a_key; }
__attribute__((always_inline)) inline u32		keyHeld(u32 a_key) { return (__currKeys & __prevKeys) & a_key; }
__attribute__((always_inline)) inline u32		keyReleased(u32 a_key) { return (~__currKeys & __prevKeys) & a_key; }
__attribute__((always_inline)) inline u32		keyHit(u32 a_key) { return (__currKeys & ~__prevKeys) & a_key; }
__attribute__((always_inline)) inline u32		keyStateChange(u32 a_key) { return (__currKeys ^ __prevKeys) & a_key; }

typedef enum AXIS
{
	HORIZONTAL = 0,
	VERTICAL,

}AXIS;

__attribute__((always_inline)) inline s32		getAxis(AXIS a_val)
{
	switch (a_val)
	{
	case HORIZONTAL:
		//shift __currKeys down 4 to move the value for RIGHT to the lowest bit & with 1 do the same for LEFT and subtract
		//if right is pressed the equation becomes 1 - 0 = 1, if Left is pressed then 0 - 1 = -1.
		return ((__currKeys >> 4) & 1) - ((__currKeys >> 5) & 1);
	case VERTICAL:
		//This is the same logic as above however uses UP and DOWN.
		return ((__currKeys >> 6) & 1) - ((__currKeys >> 7) & 1);
	default:
		return 0;
	}
}
#endif //__GBA_INPUT_H__