#ifndef __GBA_PARTICLE_SYS_H__
#define __GBA_PARTICLE_SYS_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    particle_sys.h    						  /
----------------------------------------------------------/
----- Desc: Declares the Emitter Struct and Particle	  /
-----       Struct and Functions. Particles can be emitted/
-----       from either a defined position or an Emitter. /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba_math.h"

typedef struct Emitter
{
	Vector vPosition;
}Emitter; // 8 Bytes

typedef struct Particle 
{
	Vector vPosition; //position of the particle
	Vector vVelocity;
	fixed fixGravity; // Gravity of the particle
	u32 uiLife;
	u16 uiFrames;
	u8 uiCurrentFrame;
	bool bAlive;
	// Padding
	u8 pad[2];
}Particle; // 32 Bytes

// Forward Declaration of qran
int qran_range(s32 a_min, s32 a_max);

extern void InitParticle(Particle * const a_pP, const Vector a_vPos, const Vector a_vVelocity, const u16 a_uiFrames, const fixed a_fixGravity);

extern void EmitParticle(Emitter * const a_pE, Particle * const a_pP);
extern void EmitParticleSpecify(Emitter * const a_pE, Particle * const a_pP, const fixed a_fixPXMin, const fixed a_fixPXMax, const fixed a_fixPYMin, const fixed a_fixPYMax, const Vector a_vPVelocity, const u16 a_uiPFrames, const fixed a_fixPGravity);

extern void UpdateEmittedParticle(Particle * const a_pP, Emitter * const a_pE);





#endif // !__GBA_PARTICLE_SYS_H__
