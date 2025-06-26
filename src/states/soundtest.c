#include "soundtest.h"


#include <yaul.h>
#include <mic3d.h>

#include <backend/sound/soundcontrol.h>

uint32_t samples_size = 0;

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
	dbgio_printf("ADPCM quality test\nA: 1bit B: 2bit C: 4bit\n");

	smpc_peripheral_digital_t digital;
	smpc_peripheral_process();
	smpc_peripheral_digital_port(1, &digital);
	
	if(digital.held.button.a)
	{
		snd_PlayADPCM(0, 0);
	}
	if(digital.held.button.b)
	{
		snd_PlayADPCM(1, 1);
	}
	if(digital.held.button.c)
	{
		snd_PlayADPCM(2, 2);
	}
	
	/*
	if(digital.held.button.x)
	{
		snd_PlayADPCM(3, 3);
	}
	if(digital.pressed.button.y)
	{
		snd_PlayADPCM(4, 4);
	}
	if(digital.pressed.button.z)
	{
		snd_PlayADPCM(5, 5);
	}
	if(digital.pressed.button.l)
	{
		snd_PlayADPCM(6, 6);
	}
	if(digital.pressed.button.r)
	{
		snd_PlayADPCM(7, 7);
	}
	*/
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
