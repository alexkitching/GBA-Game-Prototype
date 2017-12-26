/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    particle_sys.c						      /
----------------------------------------------------------/
----- Desc: Defines the Particle Functions	              /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "Intellisense.h"
#include "particle_sys.h"

// Initialises a Particle
void InitParticle(Particle * const a_pP, const Vector a_vPos, const Vector a_vVelocity, const u16 a_uiFrames, const fixed a_fixGravity)
{
	a_pP->vPosition = a_vPos;
	a_pP->vVelocity = a_vVelocity;
	a_pP->fixGravity = a_fixGravity;
	a_pP->uiFrames = a_uiFrames;
	a_pP->uiCurrentFrame = 0;
	a_pP->uiLife = 0;
	a_pP->bAlive = true;
}

// Emits a Particle from an Emitter
void EmitParticle(Emitter * const a_pE, Particle * const a_pP)
{
	a_pP->vPosition = a_pE->vPosition;
	a_pP->vVelocity.x = a_pP->vVelocity.x;
	a_pP->vVelocity.y = a_pP->vVelocity.y;
	a_pP->fixGravity = a_pP->fixGravity;
	a_pP->uiFrames = a_pP->uiFrames;
	a_pP->uiCurrentFrame = 0;
	a_pP->uiLife = 0;
	a_pP->bAlive = true;
}

// Emits a Particle from an Emitter using Specified Parameters
void EmitParticleSpecify(Emitter * const a_pE, Particle * const a_pP, const fixed a_fixPXMin, const fixed a_fixPXMax, const fixed a_fixPYMin, const fixed a_fixPYMax, const Vector a_vPVelocity, const u16 a_uiPFrames, const fixed a_fixPGravity)
{
	if (a_fixPXMin != 0 || a_fixPXMax != 0)
	{
		a_pP->vPosition.x = int2fix(qran_range(fix2int(fixAdd(a_pE->vPosition.x, a_fixPXMin)), fix2int(fixAdd(a_pE->vPosition.x, a_fixPXMax))));
	}
	else
	{
		a_pP->vPosition.x = a_pE->vPosition.x;
	}
	
	if (a_fixPYMin != 0 || a_fixPYMax != 0)
	{
		a_pP->vPosition.y = int2fix(qran_range(fix2int(fixAdd(a_pE->vPosition.y, a_fixPYMin)), fix2int(fixAdd(a_pE->vPosition.y, a_fixPYMax))));
	}
	else
	{
		a_pP->vPosition.y = a_pE->vPosition.y;
	}
	a_pP->vVelocity = a_vPVelocity;
	a_pP->fixGravity = a_fixPGravity;
	a_pP->uiFrames = a_uiPFrames;
	a_pP->uiCurrentFrame = 0;
	a_pP->uiLife = 0;
}

// Updates an Emitted Particle
extern void UpdateEmittedParticle(Particle * const a_pP, Emitter * const a_pE)
{
	// Add Velocity 
	a_pP->vPosition.x = fixAdd(a_pP->vPosition.x, a_pP->vVelocity.x);
	a_pP->vPosition.y = fixAdd(a_pP->vPosition.y, a_pP->vVelocity.y);
	// Apply Gravity
	a_pP->vVelocity.y = fixSub(a_pP->vVelocity.y, a_pP->fixGravity);
	// Increase Lifetime
	a_pP->uiLife += a_pP->uiFrames;

	// Update Animation
	u32 nextFrameTime = (a_pP->uiFrames << 5) - a_pP->uiLife;
	a_pP->uiCurrentFrame = nextFrameTime << 4 >> 9;

	// If Distance from Origin is greater than 40 or Life has expired, reset particle
	if (fixSub(a_pP->vPosition.y, a_pE->vPosition.y) > int2fix(40))
	{
		Vector newVelocity;
		newVelocity.x = int2fix(qran_range(0, 50));
		newVelocity.y = int2fix(200 + qran_range(0, 50));
		EmitParticleSpecify(a_pE, a_pP, -20, -20, 20, 20, newVelocity, 16, -3);
	}
}
