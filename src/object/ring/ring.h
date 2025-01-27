#ifndef __RINGWORLDS_RING_H__
#define __RINGWORLDS_RING_H__


#include <yaul.h>
#include <mic3d.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Rings are a special object, they don't take space in the temporary objects array,
 * instead they have their own structure for maintaining their data. Rings can only 
 * collide with Sonic, and loose rings the level geometry.
 */



void Ring_Update(void);


void Ring_SetSpriteData(texture_t* tex);

void Ring_AssignSpriteData(vdp1_cmdt_t* cmdt);
void Ring_ScaleToDepth(vdp1_cmdt_t* cmdt, fix16_t depth_value);


#endif
