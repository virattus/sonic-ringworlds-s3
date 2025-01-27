#ifndef __RINGWORLDS_RING_BOUNCE_H__
#define __RINGWORLDS_RING_BOUNCE_H__


#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


void RingBounce_Initialise(void);

//Sets source for generated rings on hit,
//remember to update whenever the player changes characters
//or set to null to stop rings from being generated
void RingBounce_SetSpawnSource(const fix16_vec3_t* sourcePos);

uint8_t RingBounce_GetTotal(void);
void RingBounce_GetPosition(uint8_t index, fix16_vec3_t* dest);
void RingBounce_GetVelocity(uint8_t index, fix16_vec3_t* dest);
uint8_t RingBounce_GetLifetime(uint8_t index);

uint8_t RingBounce_AddRings(uint8_t quantity);

void RingBounce_Update(void);

//These two get done with RingBounce_Update, so they don't need to be called manually, usually
bool RingBounce_UpdateLifetime(uint8_t index, uint8_t totalFreedRings);
void RingBounce_UpdateCollision(uint8_t index);

//Todo: figure out how to handle sorting, geometry transformation, 
//and insertion when both CPUs are writing to the cmdt array
//void RingBounce_DrawRings(vdp1_cmdt_t* cmdt_array, );
void RingBounce_DrawRings(void);

#endif
