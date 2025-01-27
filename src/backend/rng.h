#ifndef __SSEE_RANDOM_NUMBER_GENERATOR_H__
#define __SSEE_RANDOM_NUMBER_GENERATOR_H__


#include <yaul.h>
#include <stdint.h>


extern uint16_t rng_seed;


static inline uint16_t RNG_Initialise(uint16_t value)
{	
	rng_seed = value;
	return value;
}


/*
 * NOTE: smpc_time returns 0 if it's called too soon, so only set the RNG
 * further into the loading sequence, preferrably at the title screen or even after
 * beginning the game
 */
static inline uint16_t RNG_Initialise_RTC(const smpc_time_t* time)
{
	return RNG_Initialise(
		time->year +
		(time->day << 8) +
		time->hours +
		(time->minutes << 8) +
		time->seconds 
	);
}


static inline uint16_t RNG_Generate()
{
	uint16_t bit;
	
	bit = ((rng_seed >> 0) ^ (rng_seed >> 2) ^ (rng_seed >> 3) ^ (rng_seed >> 5)) & 0x0001;
	rng_seed = (rng_seed >> 1) | (bit << 15);
	
	return rng_seed;
}


#endif
