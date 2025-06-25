#ifndef __SSEE_ADP68K_REIMP_H__
#define __SSEE_ADP68K_REIMP_H__


#include <stdint.h>


//Channel Actions
#define ADP68K_ACTION_NOP		0x88
#define ADP68K_ACTION_PLAY		0x11
#define ADP68K_ACTION_STOP		0x01

typedef struct
{
	uint8_t action;		//Channel Action
	uint8_t id;			//Sample Index
	uint16_t volume[2]; //0x4000 = 1.0 for volume
	
} ADPCMChannelControl;


typedef struct
{
	uint32_t freq __attribute__((aligned(4)));
	uint16_t volume;	//Output volume, pre-IIR (what?)
	
	uint16_t fb_level;	//Self feedback
	uint16_t mod_level;	//Controls how much the previous operator modulates this operator
	
} SawOperatorControl;


typedef struct
{
	uint16_t square_freq;
	uint16_t square_volume[2];
	
	uint16_t saw_volume[2];
	uint16_t saw_iir[3];
	
	SawOperatorControl saw_operator[4];
	
} PSGControl;


typedef struct
{
	ADPCMChannelControl adpcm[8];
	PSGControl psg;
	
	uint16_t cd_volume[4];

} SoundControlBlock; 


static volatile SoundControlBlock* const adp68k_scblock = (volatile SoundControlBlock*)SCSPVP(0x080);
static volatile uint8_t* const adp68k_sdbase = (volatile uint8_t*)SCSPVP(0x2000);
static volatile uint32_t* const adp68k_effect_table = (volatile uint32_t*)SCSPVP(0x2000);
/*
 * The effect table stores the offsets of each sample in big endian
 * It overlaps with sdbase so that there's not a bunch of wasted space
 */


#endif
