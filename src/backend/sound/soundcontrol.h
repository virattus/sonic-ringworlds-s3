#ifndef __SSEE_SOUND_CONTROL_H__
#define __SSEE_SOUND_CONTROL_H__


#include <stdint.h>


#define ADPCM_MAX_CHANNELS		(8)


void snd_LoadDriver(void);

void snd_LoadSampleData(uint32_t offset, uint32_t data_length, uint8_t* data);

void snd_PlayADPCM(uint8_t slot, uint8_t sndID);

void snd_StopAllADPCM(void);

#endif
