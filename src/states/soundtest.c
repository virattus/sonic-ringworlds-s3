#include "soundtest.h"

#include <backend/cd_loader.h>
#include <backend/sound/soundcontrol.h>

#include <yaul.h>
#include <mic3d.h>


uint32_t samples_size = 0;
int32_t currentSample = 0;

#define TOTAL_SAMPLE_TYPES	(4)

bool sample_playback = false;


void LoadSamples(void)
{
	samples_size = CD_LoadFile("SAMPLES.AUD", LWRAM(0));

	snd_LoadSampleData(0, samples_size, LWRAM(0));

}


void SoundTestState_Init(void)
{
	CD_OpenFileList();
	
	snd_LoadDriver();
	LoadSamples();
	
	CD_CloseFileList();	
}


void SoundTestState_Free(void)
{
}


void SoundTestState_Update(void)
{
	dbgio_printf("total samples size: %i\n", samples_size);
	dbgio_printf("\n\nADPCM quality test\nA: 1bit B: 2bit C: 4bit\nUp/Down: Change Target Sample\n");
	dbgio_printf("\n\nCurrent Sample: %d\n", currentSample);
	

	smpc_peripheral_digital_t digital;
	smpc_peripheral_process();
	smpc_peripheral_digital_port(1, &digital);
	
	if(digital.held.button.a)
	{
		snd_PlayADPCM(0, currentSample * 3);
	}
	if(digital.held.button.b)
	{
		snd_PlayADPCM(1, (currentSample * 3) + 1);
	}
	if(digital.held.button.c)
	{
		snd_PlayADPCM(2, (currentSample * 3) + 2);
	}
	if(digital.held.button.up)
	{
		currentSample--;
		if(currentSample < 0)
		{
			currentSample = TOTAL_SAMPLE_TYPES - 1;
		}
	}
	if(digital.held.button.down)
	{
		currentSample++;
		if(currentSample >= TOTAL_SAMPLE_TYPES)
		{
			currentSample = 0;
		}
	}
}



gamestate_t Get_SoundTestState(void)
{
	return (gamestate_t)
	{
		.InitState = SoundTestState_Init,
		.FreeState = SoundTestState_Free,
		.UpdateState = SoundTestState_Update,
	};
}
